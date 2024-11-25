#include <Tetriminos.hpp>

namespace tetris {
    namespace ai {
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
        
        void Tetriminos::Initialize() {
            ORIENTATIONS.resize(PATTERNS.size());

            for (size_t i = 0; i < PATTERNS.size(); i++) {
                std::vector<Orientation> tetriminos;

                for (size_t j = 0; j < PATTERNS[i].size(); j++) {
                    if (PATTERNS[i][j][0][0] == 0 && PATTERNS[i][j][0][1] == 0 && j > 0) break;

                    Orientation tetrimino;
                    int minX = INT_MAX;
                    int maxX = INT_MIN;
                    int maxY = INT_MIN;

                    for (size_t k = 0; k < 4; k++) {
                        tetrimino.squares[k].x = PATTERNS[i][j][k][0];
                        tetrimino.squares[k].y = PATTERNS[i][j][k][1];

                        minX = std::min(minX, PATTERNS[i][j][k][0]);
                        maxX = std::max(maxX, PATTERNS[i][j][k][0]);
                        maxY = std::max(maxY, PATTERNS[i][j][k][1]);
                    }

                    tetrimino.minX = -minX;
                    tetrimino.maxX = AI::PLAYFIELD_WIDTH - maxX - 1;
                    tetrimino.maxY = AI::PLAYFIELD_HEIGHT - maxY - 1;

                    tetriminos.push_back(tetrimino);
                }

                ORIENTATIONS[i] = tetriminos;
            }
        }
    }
}