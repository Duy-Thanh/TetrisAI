#include <PlayfieldEvaluation.hpp>

namespace tetris {
    namespace ai {
        PlayfieldEvaluation::PlayfieldEvaluation() :
            holes(0),
            columnTransitions(0),
            rowTransitions(0),
            wells(0) {}

        PlayfieldEvaluation::~PlayfieldEvaluation() = default;
    }
}