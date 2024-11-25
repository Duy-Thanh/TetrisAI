#include "AI.hpp"
#include "PlayfieldUtil.hpp"
#include "Searcher.hpp"
#include <algorithm>

namespace tetris {
    namespace ai {
        const std::array<double, 6> AI::WEIGHTS = {
            1.0,
            12.885008263218383,
            15.842707182438396,
            26.89449650779595,
            27.616914062397015,
            30.18511071927904
        };

        AI::AI(IChildFilter* positionValidator) 
            : playfieldUtil(std::make_unique<PlayfieldUtil>())
            , searchListener(std::make_unique<SearchListenerImpl>(this)) {
            searchers.reserve(TETRIMINOS_SEARCHED);
            for(int i = 0; i < TETRIMINOS_SEARCHED; i++) {
                searchers.push_back(std::make_unique<Searcher>(searchListener.get(), positionValidator));
            }
        }

        AI::~AI() = default;

        double AI::computeFitness() {
            return WEIGHTS[0] * totalRows
                 + WEIGHTS[1] * totalDropHeight
                 + WEIGHTS[2] * evaluation.wells
                 + WEIGHTS[3] * evaluation.holes
                 + WEIGHTS[4] * evaluation.columnTransitions
                 + WEIGHTS[5] * evaluation.rowTransitions;
        }

        State* AI::search(std::vector<std::vector<int>>& playfield, const std::vector<int>& indices) {
            tetriminoIndices = indices;
            bestResult = nullptr;
            bestFitness = std::numeric_limits<double>::max();

            searchers[0]->search(playfield, tetriminoIndices[0], 0);

            return bestResult;
        }

        std::vector<State*> AI::buildStatesList(State* state) {
            std::vector<State*> states;
            State* s = state;
            
            // Count states
            while(s != nullptr) {
                states.push_back(s);
                s = s->predecessor;
            }
            
            // Reverse to get correct order
            std::reverse(states.begin(), states.end());
            return states;
        }

        void AI::SearchListenerImpl::handleResult(std::vector<std::vector<int>>& playfield, 
            int tetriminoType, int id, State state) {
            
            if (id == 0) {
                ai->result0 = &state;
            }

            const auto& orientation = Tetriminos::ORIENTATIONS[tetriminoType][state.rotation];
            int rows = ai->playfieldUtil->clearRows(playfield, state.y);  // Note the -> instead of .
            int originalTotalRows = ai->totalRows;
            int originalTotalDropHeight = ai->totalDropHeight;
            
            ai->totalRows += rows;
            ai->totalDropHeight += orientation.maxY - state.y;

            int nextID = id + 1;

            if (nextID == ai->tetriminoIndices.size()) {
                ai->playfieldUtil->evaluatePlayfield(playfield, ai->evaluation);  // Note the -> instead of .

                double fitness = ai->computeFitness();
                if (fitness < ai->bestFitness) {
                    ai->bestFitness = fitness;
                    ai->bestResult = ai->result0;
                }
            } else {
                ai->searchers[nextID]->search(playfield, ai->tetriminoIndices[nextID], nextID);
            }

            ai->totalDropHeight = originalTotalDropHeight;
            ai->totalRows = originalTotalRows;
            ai->playfieldUtil->restoreRows(playfield, rows);  // Note the -> instead of .
        }
    }
}