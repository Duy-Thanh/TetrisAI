#include <Point.hpp>

namespace tetris {
    namespace ai {
        Point::Point() : x(0), y(0) {}
        
        Point::Point(int x, int y) : x(x), y(y) {}

        Point::~Point() = default;
    }
}