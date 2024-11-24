#pragma once

namespace tetris {
    namespace ai {
        class Point {
            public:
                int x;
                int y;

                Point();
                Point(int x, int y);
                ~Point();
        };
    }
}