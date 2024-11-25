#include <Searcher.hpp>

namespace tetris {
    namespace ai {
        int Searcher::globalMark = 1;

        Searcher::Searcher(ISearchListener* searchListener, IChildFilter* positionValidator)
            : searchListener(searchListener), positionValidator(positionValidator) {
                createStates();
            }

        void Searcher::createStates() {
            states.resize(AI::PLAYFIELD_HEIGHT);

            for (int y = 0; y < AI::PLAYFIELD_HEIGHT; y++) {
                states[y].resize(AI::PLAYFIELD_WIDTH);

                for (int x = 0; x < AI::PLAYFIELD_WIDTH; x++) {
                    states[y][x].resize(4);

                    for (int rotation = 0; rotation < 4; rotation++) {
                        states[y][x][rotation] = State(x, y, rotation);
                    }
                }
            }
        }

        void Searcher::lockTetrimino(std::vector<std::vector<int>>& playfield, 
            int tetriminoType, int id, State* state) {
            
            const auto& squares = Tetriminos::ORIENTATIONS[tetriminoType][state->rotation].squares;

            // Lock the tetrimino
            for (int i = 0; i < 4; i++) {
                const auto& square = squares[i];

                int y = state->y + square.y;

                if (y >= 0) {
                    playfield[y][state->x + square.x] = Tetriminos::NONE;
                    playfield[y][AI::PLAYFIELD_WIDTH]++;
                }
            }

            searchListener->handleResult(playfield, tetriminoType, id, *state);

            // Unlock the tetrimino
            for (int i = 0; i < 4; i++) {
                const auto& square = squares[i];

                int y = state->y + square.y;

                if (y >= 0) {
                    playfield[y][state->x + square.x] = Tetriminos::NONE;
                    playfield[y][AI::PLAYFIELD_WIDTH]--;
                }
            }
        }

        bool Searcher::addChild(std::vector<std::vector<int>>& playfield, 
            int tetriminoType, int mark, State* state, 
            int x, int y, int rotation) {
            
            const auto& orientation = Tetriminos::ORIENTATIONS[tetriminoType][rotation];

            if (x < orientation.minX || x > orientation.maxX || y > orientation.maxY) {
                return false;
            }

            State& childNode = states[y][x][rotation];
            if (childNode.visited == mark) {
                return true;
            }

            const auto& squares = orientation.squares;

            for (int i = 0; i < 4; i++) {
                const auto& square = squares[i];

                int playfieldY = y + square.y;

                if (playfieldY >= 0 && playfield[playfieldY][x + square.x] != Tetriminos::NONE) {
                    return false;
                }
            }

            if (positionValidator && !positionValidator->validate(playfield, tetriminoType, x, y, rotation)) {
                return true;
            }

            childNode.visited = mark;
            childNode.predecessor = state;

            queue.enqueue(&childNode);

            return true;
        }

        bool Searcher::search(std::vector<std::vector<int>>& playfield, int tetriminoType, int id) {
            const auto orientationCount = static_cast<int>(Tetriminos::ORIENTATIONS[tetriminoType].size());
            int maxRotation = orientationCount - 1;

            int mark = globalMark++;

            if (!addChild(playfield, tetriminoType, mark, nullptr, 5, 0, 0)) {
                return false;
            }

            while (queue.isNotEmpty()) {
                State* state = queue.dequeue();

                if (maxRotation != 0) {
                    addChild(playfield, tetriminoType, mark, state, state->x, state->y,
                        state->rotation == 0 ? maxRotation : state->rotation - 1);

                    if (maxRotation != 1) {
                        addChild(playfield, tetriminoType, mark, state, state->x, state->y,
                            state->rotation == maxRotation ? 0 : state->rotation + 1);
                    }
                }

                addChild(playfield, tetriminoType, mark, state, state->x - 1, state->y, state->rotation);
                addChild(playfield, tetriminoType, mark, state, state->x + 1, state->y, state->rotation);

                if (addChild(playfield, tetriminoType, mark, state, state->x, state->y + 1, state->rotation)){
                    lockTetrimino(playfield, tetriminoType, id, state);
                }
            }

            return true;
        }
    }
}

