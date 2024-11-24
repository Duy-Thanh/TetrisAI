#pragma once

#include "State.hpp"

namespace tetris {
    namespace ai {
        class ISearchListener {
            virtual ~ISearchListener() = default;

            virtual void handleResult(int** playfield, int tetriminotype,
                int id, State state) = 0;
        };
    }
}