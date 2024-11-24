#pragma once

namespace tetris {
    namespace ai {
        class PlayfieldEvaluation {
            public:
                int holes;
                int columnTransitions;
                int rowTransitions;
                int wells;

                PlayfieldEvaluation();
                ~PlayfieldEvaluation();
        };
    }
}