#include <Queue.hpp>

namespace tetris {
    namespace ai {
        Queue::Queue() : head(nullptr), tail(nullptr) {}

        Queue::~Queue() {
            head = nullptr;
            tail = nullptr;
        }

        void Queue::enqueue(State* state) {
            if (head == nullptr) {
                head = state;
                tail = state;
            } else {
                tail->next = state;
                tail = state;
            }

            state->next = nullptr;
        }

        State* Queue::dequeue() {
            State* state = head;

            if (head != nullptr) {
                if (head == tail) {
                    head = nullptr;
                    tail = nullptr;
                } else {
                    head = head->next;
                }
            }

            return state;
        }

        bool Queue::isEmpty() const {
            return head == nullptr;
        }

        bool Queue::isNotEmpty() const {
            return head != nullptr;
        }
    }
}