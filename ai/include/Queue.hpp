#pragma once

#include "State.hpp"

namespace tetris {
    namespace ai {
        class Queue {
            private:
                State* head;
                State* tail;
            
            public:
                Queue();
                ~Queue();

                void enqueue(State* state);
                State* dequeue();
                bool isEmpty() const;
                bool isNotEmpty() const;
        };
    }
}
