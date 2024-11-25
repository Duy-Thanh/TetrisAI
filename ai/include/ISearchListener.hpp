#pragma once

#include "State.hpp"

namespace tetris {
    namespace ai {
        class ISearchListener {
            public:
                virtual ~ISearchListener() = default;

                virtual void handleResult(std::vector<std::vector<int>>& playfield, int tetriminoType,
                    int id, State state) = 0;
        };
    }
}