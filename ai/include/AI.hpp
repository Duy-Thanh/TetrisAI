#pragma once

#include "PlayfieldEvaluation.hpp"
#include <array>
#include <memory>

namespace tetris {
    namespace ai {
        class AI {
            public:
                static const int PLAYFIELD_WIDTH = 10;
                static const int PLAYFIELD_HEIGHT = 20;
                static const int TETRIMINOS_SEARCHED = 2;

            private:
                static const std::array<double, 6> WEIGHTS;
        };
    }
}

