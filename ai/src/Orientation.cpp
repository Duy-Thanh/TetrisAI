#include <Orientation.hpp>

namespace tetris {
    namespace ai {
        Orientation::Orientation() :
            squares(),
            minX(0),
            maxX(0),
            maxY(0) {}

        Orientation::~Orientation() = default;
    }
}