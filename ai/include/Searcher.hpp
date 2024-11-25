#pragma once

#include "State.hpp"
#include "Queue.hpp"
#include "ISearchListener.hpp"
#include "IChildFilter.hpp"
#include "AI.hpp"
#include "Tetriminos.hpp"
#include <vector>
#include <memory>

namespace tetris {
    namespace ai {
        class Searcher {
            private:
                static int globalMark;

                // 3D vector of states [height][width][rotation]
                std::vector<std::vector<std::vector<State>>> states;
                Queue queue;
                ISearchListener* searchListener;
                IChildFilter* positionValidator;

            public:
                Searcher(ISearchListener* searchListener, IChildFilter* positionValidator);
                bool search(std::vector<std::vector<int>>& playfield, int tetriminoType, int id);

            private:
                void createStates();

                void lockTetrimino(std::vector<std::vector<int>>& playfield,
                    int tetriminoType, int id, State* state);

                bool addChild(std::vector<std::vector<int>>& playfield,
                    int tetriminoType, int mark, State* state,
                    int x, int y, int rotation);
        };
    }
}

