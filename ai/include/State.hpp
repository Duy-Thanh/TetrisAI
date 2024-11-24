#pragma once

namespace tetris {
    namespace ai {
        class State {
            public:
                int x;
                int y;
                int rotation;
                int visited;
                State* parent;
                State* next;

                State(int x, int y, int rotation);
                ~State();
        };
    }
}