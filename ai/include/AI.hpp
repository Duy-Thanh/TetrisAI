#pragma once

#include "PlayfieldEvaluation.hpp"
#include "Searcher.hpp"  // Move this include to AI.cpp
#include "ISearchListener.hpp"
#include "IChildFilter.hpp"
#include <array>
#include <memory>
#include <vector>
#include <limits>

namespace tetris {
    namespace ai {
        // Forward declarations
        class PlayfieldUtil;
        class Searcher;

        class AI {
        public:
            static const int PLAYFIELD_WIDTH = 10;
            static const int PLAYFIELD_HEIGHT = 20;
            static const int TETRIMINOS_SEARCHED = 2;

            explicit AI(IChildFilter* positionValidator = nullptr);
            ~AI();  // Need to define this since we have forward-declared types

            State* search(std::vector<std::vector<int>>& playfield, const std::vector<int>& tetriminoIndices);
            std::vector<State*> buildStatesList(State* state);

        private:
            static const std::array<double, 6> WEIGHTS;
            
            std::vector<std::unique_ptr<Searcher>> searchers;
            std::vector<int> tetriminoIndices;
            std::unique_ptr<PlayfieldUtil> playfieldUtil;  // Change to pointer
            PlayfieldEvaluation evaluation;
            
            int totalRows{0};
            int totalDropHeight{0};
            double bestFitness{0.0};
            State* bestResult{nullptr};
            State* result0{nullptr};

            double computeFitness();

            class SearchListenerImpl : public ISearchListener {
            public:
                explicit SearchListenerImpl(AI* ai) : ai(ai) {}
                
                void handleResult(std::vector<std::vector<int>>& playfield, 
                    int tetriminoType, int id, State state) override;
            
            private:
                AI* ai;
            };

            std::unique_ptr<SearchListenerImpl> searchListener;
        };
    }
}

