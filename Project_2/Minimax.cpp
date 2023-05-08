//
// Created by snewman on 3/9/2023.
//

#include "Minimax.h"

/*
 * Function to initiate the alpha-beta search on the game board.
 * Args: Board, the current game board
 *       int, the number of the player who is MAX
 * Returns: int, the column that the player should play in
 */
int Minimax::alpha_beta(Board *game_board, int MAX, int depth) {
    tuple<int, int> ret = max_value(game_board, -inf, inf, MAX, depth);
    cout << "Expected Utility: " << get<0>(ret) << endl;
    return get<1>(ret);
}

/*
 * Function to calculate the maximum value that can be gained from a state
 * Args: Board, the game board to be tested
 *       int, the alpha value, highest we have seen
 *       int, the beta value, lowest we have seen
 *       int, the number of the player who is being evaluated for
 * Returns: tuple<int, int>, the tuple consisting of (utility value, action)
 */
tuple<int, int> Minimax::max_value(Board *game_board, int alpha, int beta, int MAX, int depth) {
    // No moves can be made
    if (!game_board->can_play() || depth <= 0) return make_tuple(game_board->evaluate(MAX), -1);

    int v = -inf;
    int move = -1;
    vector<int> actions = game_board->find_actions();

    for (int a : actions) {
        tuple<int, int> ret2;
        // Generate duplicate board with new move
        Board board2;
        game_board->dup_board(&board2);
        board2.one_move(a + 1, MAX);
        // Evaluate new board's performance
        ret2 = min_value(&board2, alpha, beta, 3 - MAX, depth - 1);
        if (get<0>(ret2) > v) {
            v = get<0>(ret2);
            move = a;
            alpha = max(alpha, v);
        }
        if (v >= beta) return make_tuple(v, move);
    }
    return make_tuple(v, move);
}

/*
 * Function to calculate the minimum value that can be gained from a state
 * Args: Board, the game board to be tested
 *       int, the alpha value, highest we have seen
 *       int, the beta value, lowest we have seen
 *       int, the number of the player who is being evaluated for
 * Returns: tuple<int, int>, the tuple consisting of (utility value, action)
 */
tuple<int, int> Minimax::min_value(Board *game_board, int alpha, int beta, int MIN, int depth) {
    // No moves can be made
    if (!game_board->can_play() || depth <= 0) return make_tuple(game_board->evaluate(3 - MIN), -1);

    int v = inf;
    int move = -1;
    vector<int> actions = game_board->find_actions();

    for (int a : actions) {
        tuple<int, int> ret2;
        // Generate duplicate board with new move
        Board board2;
        game_board->dup_board(&board2);
        board2.one_move(a + 1, MIN);
        // Evaluate new board's performance
        ret2 = max_value(&board2, alpha, beta, 3 - MIN, depth - 1);
        if (get<0>(ret2) < v) {
            v = get<0>(ret2);
            move = a;
            beta = min(beta, v);
        }
        if (v <= beta) return make_tuple(v, move);
    }
    return make_tuple(v, move);
}
