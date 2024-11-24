#pragma once

namespace tetris {
    namespace ai {
        class IChildFilter {
            virtual ~IChildFilter() = default;

            virtual bool validate(int** playfield, int tetriminoType, 
                int x, int y, int rotation) = 0;
        };
    }
}