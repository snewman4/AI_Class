//
// Created by snewman on 3/9/2023.
//

#ifndef PROJECT_2_MINIMAX_H
#define PROJECT_2_MINIMAX_H

#include <tuple>

#include "Board.h"

using namespace std;

class Minimax {
public:
    int inf = 10000; // Used to simulate infinity, essentially just a large number that score can't reach

    int alpha_beta(Board *game_board, int player_num, int depth);
    tuple<int, int> max_value(Board *game_board, int alpha, int beta, int player_num, int depth);
    tuple<int, int> min_value(Board *game_board, int alpha, int beta, int player_num, int depth);
};


#endif //PROJECT_2_MINIMAX_H
