#include "PlayfieldUtil.hpp"
#include <algorithm>

namespace tetris {
    namespace ai {
        PlayfieldUtil::PlayfieldUtil() : spareIndex(0) {
            // Initialize spare rows
            spareRows.resize(8 * AI::TETRIMINOS_SEARCHED, std::vector<int>(AI::PLAYFIELD_WIDTH + 1, Tetriminos::NONE));

            columnsDepths.resize(AI::PLAYFIELD_WIDTH);
        }
    }
}

