#pragma once

#include "Point.hpp"
#include <array>

namespace tetris {
    namespace ai {
        class Orientation {
            public:
                std::array<Point, 4> squares;
                int minX;
                int maxX;
                int maxY;

                Orientation();
                ~Orientation();
        };
    }
}