//
// Created by snewman on 2/23/2023.
//

#ifndef PROJECT_2_BOARD_H
#define PROJECT_2_BOARD_H

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Board {
public:
    int board[6][7]{};
    int score = 0;
    int comp_num = -1;
    int hum_num = -1;

    void dup_board(Board *new_board);
    int read_board(const string& in_filename, const string& next);
    int write_board(const string& out_filename, int next);
    int verify_board();
    void draw_board();
    vector<int> find_actions();
    bool can_play();
    int one_move(int column, int player);
    int evaluate(int player);
};


#endif //PROJECT_2_BOARD_H
