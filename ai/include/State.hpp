#pragma once

#include <vector>

namespace tetris {
    namespace ai {
        class State {
            public:
                int x;
                int y;
                int rotation;
                int visited;
                State* predecessor;
                State* next;

                State() : x(0), y(0), rotation(0), visited(0), predecessor(nullptr), next(nullptr) {}
                State(int x, int y, int rotation);
                ~State();
        };
    }
}