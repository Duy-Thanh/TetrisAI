#pragma once

#include "AI.hpp"
#include "Tetriminos.hpp"
#include "PlayfieldEvaluation.hpp"
#include <vector>
#include <memory>

namespace tetris {
    namespace ai {
        class PlayfieldUtil {
            private:
                std::vector<std::vector<int>> spareRows;
                std::vector<int> columnsDepth;
                int spareIndex;
            
            public:
                PlayfieldUtil();
        };
    }
}