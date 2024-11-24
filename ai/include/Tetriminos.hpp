#pragma once

#include "Orientation.hpp"
#include "AI.hpp"
#include <vector>
#include <array>
#include <climits>
#include <algorithm>

namespace tetris {
    namespace ai {
        class Tetriminos {
            public:
                static const int NONE = -1;
                static const int T = 0;
                static const int J = 1;
                static const int Z = 2;
                static const int O = 3;
                static const int S = 4;
                static const int L = 5;
                static const int I = 6;

                // 4D array for patterns: [pieceType][orientation][block][coordinate]
                static const std::array<std::array<std::array<std::array<int, 2>, 4>, 4>, 7> PATTERNS;

                static std::vector<std::vector<Orientation>> ORIENTATIONS;

                // Delete constructors to prevent instantiation
                Tetriminos() = delete;
                ~Tetriminos() = delete;

                static void Initialize();
            private:
                static const std::array<int, 7> ROTATION_COUNTS;
        };

        const std::array<std::array<std::array<std::array<int, 2>, 4>, 4>, 7> Tetriminos::PATTERNS = {{
            {{
                {{ {{ -1,  0 }}, {{  0,  0 }}, {{  1,  0 }}, {{  0,  1 }} }},
                {{ {{  0, -1 }}, {{ -1,  0 }}, {{  0,  0 }}, {{  0,  1 }} }},
                {{ {{ -1,  0 }}, {{  0,  0 }}, {{  1,  0 }}, {{  0, -1 }} }},
                {{ {{  0, -1 }}, {{  0,  0 }}, {{  1,  0 }}, {{  0,  1 }} }}
            }},
            {{
                {{ {{ -1,  0 }}, {{  0,  0 }}, {{  1,  0 }}, {{  1,  1 }} }},
                {{ {{  0, -1 }}, {{  0,  0 }}, {{ -1,  1 }}, {{  0,  1 }} }},
                {{ {{ -1, -1 }}, {{ -1,  0 }}, {{  0,  0 }}, {{  1,  0 }} }},
                {{ {{  0, -1 }}, {{  1, -1 }}, {{  0,  0 }}, {{  0,  1 }} }}
            }},
            {{
                {{ {{ -1,  0 }}, {{  0,  0 }}, {{  0,  1 }}, {{  1,  1 }} }},
                {{ {{  1, -1 }}, {{  0,  0 }}, {{  1,  0 }}, {{  0,  1 }} }}
            }},
            {{
                {{ {{ -1,  0 }}, {{  0,  0 }}, {{ -1,  1 }}, {{  0,  1 }} }}
            }},
            {{
                {{ {{  0,  0 }}, {{  1,  0 }}, {{ -1,  1 }}, {{  0,  1 }} }},
                {{ {{  0, -1 }}, {{  0,  0 }}, {{  1,  0 }}, {{  1,  1 }} }}
            }},
            {{
                {{ {{ -1,  0 }}, {{  0,  0 }}, {{  1,  0 }}, {{ -1,  1 }} }},
                {{ {{ -1, -1 }}, {{  0, -1 }}, {{  0,  0 }}, {{  0,  1 }} }},
                {{ {{  1, -1 }}, {{ -1,  0 }}, {{  0,  0 }}, {{  1,  0 }} }},
                {{ {{  0, -1 }}, {{  0,  0 }}, {{  0,  1 }}, {{  1,  1 }} }}
            }},
            {{
                {{ {{ -2,  0 }}, {{ -1,  0 }}, {{  0,  0 }}, {{  1,  0 }} }},
                {{ {{  0, -2 }}, {{  0, -1 }}, {{  0,  0 }}, {{  0,  1 }} }}
            }}
        }};

        std::vector<std::vector<Orientation>> Tetriminos::ORIENTATIONS;
    }
}
