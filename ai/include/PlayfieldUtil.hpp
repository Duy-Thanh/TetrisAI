#pragma once

#include "AI.hpp"
#include "State.hpp"
#include "Tetriminos.hpp"
#include "PlayfieldEvaluation.hpp"
#include <vector>
#include <memory>

namespace tetris {
    namespace ai {
        class PlayfieldUtil {
            private:
                std::vector<std::vector<int>> spareRows;
                std::vector<int> columnsDepths;
                int spareIndex;
            
            public:
                PlayfieldUtil();

                std::vector<std::vector<int>> createPlayfield();
                void lockTetrimino(std::vector<std::vector<int>>& playfield, int tetriminoType, const State& state);
                void evaluatePlayfield(std::vector<std::vector<int>>& playfield, PlayfieldEvaluation& evaluation);
                int clearRows(std::vector<std::vector<int>>& playfield, int tetriminoY);
                void restoreRows(std::vector<std::vector<int>>& playfield, int rows);

            private:
                void clearRow(std::vector<std::vector<int>>& playfield, int y);
                void restoreRow(std::vector<std::vector<int>>& playfield);
        };
    }
}