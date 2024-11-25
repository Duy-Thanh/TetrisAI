#include "PlayfieldUtil.hpp"
#include <algorithm>

namespace tetris {
    namespace ai {
        PlayfieldUtil::PlayfieldUtil() : spareIndex(0) {
            // Initialize spare rows
            spareRows.resize(8 * AI::TETRIMINOS_SEARCHED, std::vector<int>(AI::PLAYFIELD_WIDTH + 1, Tetriminos::NONE));

            columnDepths.resize(AI::PLAYFIELD_WIDTH);
        }

        std::vector<std::vector<int>> PlayfieldUtil::createPlayfield() {
            return std::vector<std::vector<int>>(AI::PLAYFIELD_HEIGHT, std::vector<int>(AI::PLAYFIELD_WIDTH + 1, Tetriminos::NONE));
        }

        void PlayfieldUtil::lockTetrimino(std::vector<std::vector<int>>& playfield, int tetriminoType, const State& state) {
            const auto& squares = Tetriminos::ORIENTATIONS[tetriminoType][state.rotation].squares;

            // Lock the tetrimino
            for (int i = 0; i < 4; i++) {
                const auto& square = squares[i];
                int y = state.y + square.y;
                if (y >= 0) {
                    playfield[y][state.x + square.x] = tetriminoType;
                    playfield[y][AI::PLAYFIELD_WIDTH]++;
                }
            }

            int startRow = std::max(1, state.y - 2);
            int endRow = std::min(AI::PLAYFIELD_HEIGHT - 1, state.y + 1);

            for (int y = startRow; y <= endRow; y++) {
                if (playfield[y][AI::PLAYFIELD_WIDTH] == AI::PLAYFIELD_WIDTH) {
                    auto clearedRow = playfield[y];

                    for (int i = y; i > 0; i--) {
                        playfield[i] = playfield[i - 1];
                    }

                    std::fill(clearedRow.begin(), clearedRow.begin() + AI::PLAYFIELD_WIDTH, Tetriminos::NONE);
                    clearedRow[AI::PLAYFIELD_WIDTH] = 0;
                    playfield[0] = clearedRow;
                }
            }
        }

        void PlayfieldUtil::evaluatePlayfield(std::vector<std::vector<int>>& playfield, PlayfieldEvaluation& evaluation) {
            // Calculate column depths
            for (int x = 0; x < AI::PLAYFIELD_WIDTH; x++) {
                columnDepths[x] = AI::PLAYFIELD_HEIGHT - 1;

                for (int y = 0; y < AI::PLAYFIELD_HEIGHT; y++) {
                    if (playfield[y][x] != Tetriminos::NONE) {
                        columnDepths[x] = y;
                        break;
                    }
                }
            }

            // Calculate wells
            evaluation.wells = 0;
            for (int x = 0; x < AI::PLAYFIELD_WIDTH; x++) {
                int minY = 0;

                if (x == 0) {
                    minY = columnDepths[x];
                } else if (x == AI::PLAYFIELD_WIDTH - 1) {
                    minY = columnDepths[AI::PLAYFIELD_WIDTH - 2];
                } else {
                    minY = std::min(columnDepths[x - 1], columnDepths[x + 1]);
                }

                for (int y = columnDepths[x]; y >= minY; y--) {
                    if ((x == 0 || playfield[y][x - 1] != Tetriminos::NONE) &&
                        (x == AI::PLAYFIELD_WIDTH - 1
                            || playfield[y][x + 1] != Tetriminos::NONE)) {
                        evaluation.wells++;
                    }
                }
            }

            // Calculate holes and column transitions
            evaluation.holes = 0;
            evaluation.columnTransitions = 0;

            for (int x = 0; x < AI::PLAYFIELD_WIDTH; x++) {
                bool solid = true;

                for (int y = columnDepths[x] + 1; y < AI::PLAYFIELD_HEIGHT; y++) {
                    if (playfield[y][x] == Tetriminos::NONE) {
                        if (playfield[y - 1][x] != Tetriminos::NONE) {
                            evaluation.holes++;
                        }

                        if (solid) {
                            solid = false;
                            evaluation.columnTransitions++;
                        }
                    } else if (!solid) {
                        solid = true;
                        evaluation.columnTransitions++;
                    }
                }
            }

            // Calculate row transitions
            evaluation.rowTransitions = 0;
            for (int y = 0; y < AI::PLAYFIELD_HEIGHT; y++) {
                bool solidFound = false;
                bool solid = true;
                int transitions = 0;

                for (int x = 0; x <= AI::PLAYFIELD_WIDTH; x++) {
                    if (x == AI::PLAYFIELD_WIDTH) {
                        if (!solid) {
                            transitions++;
                        }
                    } else {
                        if (playfield[y][x] == Tetriminos::NONE) {
                            if (solid) {
                                solid = false;
                                transitions++;
                            }
                        } else {
                            solidFound = true;
                            if (!solid) {
                                solid = true;
                                transitions++;
                            }
                        }
                    }
                }

                if (solidFound) {
                    evaluation.rowTransitions += transitions;
                }
            }
        }

        int PlayfieldUtil::clearRows(std::vector<std::vector<int>>& playfield, int tetriminoY) {
            int rows = 0;
            int startRow = std::max(1, tetriminoY - 2);
            int endRow = std::min(AI::PLAYFIELD_HEIGHT - 1, tetriminoY + 1);

            for (int y = startRow; y <= endRow; y++) {
                if (playfield[y][AI::PLAYFIELD_WIDTH] == AI::PLAYFIELD_WIDTH) {
                    rows++;
                    clearRow(playfield, y);
                }
            }

            return rows;
        }

        void PlayfieldUtil::clearRow(std::vector<std::vector<int>>& playfield, int y) {
            auto clearedRow = playfield[y];
            clearedRow[AI::PLAYFIELD_WIDTH] = y;

            for (int i = y; i > 0; i--) {
                playfield[i] = playfield[i - 1];
            }

            playfield[0] = spareRows[spareIndex];
            playfield[0][AI::PLAYFIELD_WIDTH] = 0;

            spareRows[spareIndex++] = clearedRow;
        }

        void PlayfieldUtil::restoreRow(std::vector<std::vector<int>>& playfield) {
            auto restoredRow = spareRows[--spareIndex];
            int y = restoredRow[AI::PLAYFIELD_WIDTH];

            spareRows[spareIndex] = playfield[0];

            for (int i = 0; i < y; i++) {
                playfield[i] = playfield[i + 1];
            }
            
            restoredRow[AI::PLAYFIELD_WIDTH] = AI::PLAYFIELD_WIDTH;
            playfield[y] = restoredRow;
        }

        void PlayfieldUtil::restoreRows(std::vector<std::vector<int>>& playfield, int rows) {
            for (int i = 0; i < rows; i++) {
                restoreRow(playfield);
            }
        }
    }
}

