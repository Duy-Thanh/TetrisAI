#include <State.hpp>

namespace tetris {
    namespace ai {
        State::State(int x, int y, int rotation) :
            x(x), y(y), rotation(rotation) {}

        State::~State() = default;
    }
}
