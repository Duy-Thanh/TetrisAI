#pragma once

#include <vector>

namespace tetris {
    namespace ai {
        class IChildFilter {
            public:
                virtual ~IChildFilter() = default;

                virtual bool validate(std::vector<std::vector<int>>& playfield, int tetriminoType, 
                    int x, int y, int rotation) = 0;
        };
    }
}