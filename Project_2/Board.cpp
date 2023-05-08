//
// Created by snewman on 2/23/2023.
//

#include "Board.h"

/*
 * Function to duplicate a game board.
 * Args: Board, the board that will become the cloned board
 */
void Board::dup_board(Board *new_board) {
    new_board->score = this->score;
    new_board->comp_num = this->comp_num;
    new_board->hum_num = this->hum_num;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            new_board->board[i][j] = this->board[i][j];
        }
    }
}

/*
 * Function to read the board into the input file
 * Args: string&, name of the input file
 * Returns: int, either -1 if failure, or number of next player
 */
int Board::read_board(const string& in_filename, const string& next) {
    // Open the input file
    ifstream infile(in_filename);
    if (!infile.good()) {
        cerr << "Unable to open input file " << in_filename << ". Please try again." << endl;
        return(-1);
    }

    // Create the game board
    int next_num = -1; // Next player
    int curr_row = 0; // Which row is being added to
    string line;

    while (getline(infile, line)) {
        if (line.size() == 1) {
            // Final character of file
            next_num = stoi(line);
            break;
        } else {
            // Row of game board
            if (line.size() != 7) {
                cerr << "Incorrect row length in input file. Please verify." << endl;
                return(-1);
            }
            for (int i = 0; i < 7; i++)
                board[curr_row][i] = (int) (line[i] - '0');
            curr_row++;
        }
    }

    if (next_num != 1 && next_num != 2) {
        cerr << "The input file should specify either 1 or 2 as the next player. Please verify." << endl;
        return(-1);
    }

    // Close the input and output files
    infile.close();

    // Store which number corresponds to which player
    if (next == "computer-next") {
        comp_num = next_num;
        hum_num = 3 - next_num; // Assigns opposite of 1 or 2
    } else {
        comp_num = 3 - next_num;
        hum_num = next_num;
    }

    score = evaluate(comp_num);
    return next_num;
}

/*
 * Function to write the game board to an output file.
 * Args: string&, the name of the file to write the board to
 */
int Board::write_board(const string& out_filename, int next) {
    ofstream outfile(out_filename);
    if (!outfile.good()) {
        cerr << "Unable to open output file " << out_filename << ". Please try again." << endl;
        return(-1);
    }

    // Write each element to the board
    for (auto & row : board) {
        for (int col : row) {
            outfile << col;
        }
        outfile << endl;
    }
    outfile << next << endl; // Write next turn
    return(0);
}

/*
 * Function to ensure that the board is formatted correctly
 * Returns: int, the status of the function
 */
int Board::verify_board() {
    int top[7] = {6, 6, 6, 6, 6, 6, 6}; // Used to track the top of each column
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            int el = board[row][col];
            if (el > 2 || el < 0) {
                // Check character validity
                cerr << "This is not a valid character for the board: " << el << endl;
                return(-1);
            }
            if (el == 0 && top[col] < row) {
                // Check floating pieces
                cerr << "Column " << col + 1 << " has a floating piece." << endl;
                return(-1);
            }
            if (el != 0 && top[col] > row) {
                // Update top of each column
                top[col] = row;
            }
        }
    }
    return(0);
}

/*
 * Function to draw the game board from a table of the board
 * Returns: void
 */
void Board::draw_board() {
    cout << "   1   2   3   4   5   6   7 " << endl;
    cout << "  ___________________________" << endl;
    for (auto & i : board) {
        cout << " | ";
        for (int j : i) {
            if (j == 0) cout << "  | ";
            else cout << j << " | ";
        }
        cout << endl;
    }

    cout << "Score: " << score << endl << endl;
}

/*
 * Function to calculate what actions can be taken for the board
 * Returns: vector<int>, a list of columns that can be played in
 */
vector<int> Board::find_actions() {
    vector<int> actions;

    for (int i = 0; i < 7; i++) {
        if (board[0][i] == 0)
            actions.push_back(i);
    }

    return actions;
}

/*
 * Function to verify if the board is filled or not
 * Returns: bool, true if you can play, else false
 */
bool Board::can_play() {
    for (int i = 0; i < 7; i++) {
        if (board[0][i] == 0) return true;
    }
    return false;
}

/*
 * Function to drop a piece down a column
 * Args: int, the column to drop a piece in
 *       int, the player that played the piece
 * Returns: int, -1 if invalid selection, 0 otherwise
 */
int Board::one_move(int column, int player) {
    column -= 1; // Convert to 0 indexing
    // Verify column is valid
    if (column < 0 || column > 6) {
        cerr << "Please select a column in the range from 1 to 7, inclusive." << endl;
        return(-1);
    }

    // Verify player is valid
    if (player != 1 && player != 2) {
        cerr << "Not a valid player selection, please contact your coder." << endl;
        return(-1);
    }

    // Verify column isn't full
    if (board[0][column] != 0) {
        cout << "Column " << column + 1 << " is full. Please select another column." << endl;
        return(-1);
    }

    // Identify where the dropped piece will fall to
    for (int i = 0; i < 6; i++) {
        if (board[i][column] != 0) {
            // Add piece to next lowest slot
            board[i - 1][column] = player;
            break;
        } else if (i == 5) {
            // Empty column
            board[i][column] = player;
        }
    }

    score = evaluate(comp_num);
    return(0);
}

/*
 * Function to calculate the utility value of the board in terms of the
 * specified player. For example, if Player 1 is specified, a positive
 * value will mean that Player 1 has the advantage, and a negative value
 * will mean that Player 2 has the advantage.
 *
 * Each row, column, or diagonal of four pieces is worth two points.
 *
 * Args: int, the player who should have positive associated values
 * Returns: int, the cumulative sum of the board's utility value
 */
int Board::evaluate(int player) {
    int one_score = 0; // Score of the first player
    int two_score = 0; // Score of the second player

    // Check Row
    for (auto & row : board) {
        for (int i = 0; i < 4; i++) {
            // Check four in a row from current index
            if (row[i] == row[i+1] && row[i] == row[i+2] && row[i] == row[i+3]) {
                // Assign points. Only one four in a row possible per row
                if (row[i] == 1) {
                    one_score++;
                    break;
                } else if (row[i] == 2) {
                    two_score++;
                    break;
                }
            }
        }
    }

    // Check Column
    for (int col = 0; col < 7; col++) {
        for (int row = 0; row < 3; row++) {
            // Check four in a row from current index
            if (board[row][col] == board[row+1][col] && board[row][col] == board[row+2][col] && board[row][col] == board[row+3][col]) {
                // Assign points. Only one four in a row possible per column
                if (board[row][col] == 1) {
                    one_score++;
                    break;
                } else if (board[row][col] == 2) {
                    two_score++;
                    break;
                }
            }
        }
    }

    // Check ascending diagonal
    // Stores the possible ascending diagonal locations
    // Each row corresponds to one of the possible diagonals
    // The first column is the bottom left row, second column is the bottom left column, third column is number of possible diagonals
    int asc_diag[6][3] = {
            {3, 0, 1},
            {4, 0, 2},
            {5, 0, 3},
            {5, 1, 3},
            {5, 2, 2},
            {5, 3, 1}
    };

    for (auto & diag : asc_diag) {
        for (int i = 0; i < diag[2]; i++) {
            int row = diag[0] - i;
            int col = diag[1] + i;
            // Check four in a row for current diagonal
            if (board[row][col] == board[row-1][col+1] && board[row][col] == board[row-2][col+2] && board[row][col] == board[row-3][col+3]) {
                // Assign points. Only one four in a row possible per diagonal
                if (board[row][col] == 1) {
                    one_score++;
                    break;
                } else if (board[row][col] == 2) {
                    two_score++;
                    break;
                }
            }
        }
    }

    // Check descending diagonal
    // Stores the possible descending diagonal locations
    // Each row corresponds to one of the possible diagonals
    // The first column is the top left row, second column is the top left column, third column is the number of possible diagonals
    int des_diag[6][3] = {
            {2, 0, 1},
            {1, 0, 2},
            {0, 0, 3},
            {0, 1, 3},
            {0, 2, 2},
            {0, 3, 1}
    };

    for (auto & diag : des_diag) {
        for (int i = 0; i < diag[2]; i++) {
            int row = diag[0] + i;
            int col = diag[1] + i;
            // Check four in a row for current diagonal
            if (board[row][col] == board[row+1][col+1] && board[row][col] == board[row+2][col+2] && board[row][col] == board[row+3][col+3]) {
                // Assign points. Only one four in a row possible per diagonal
                if (board[row][col] == 1) {
                    one_score++;
                    break;
                } else if (board[row][col] == 2) {
                    two_score++;
                    break;
                }
            }
        }
    }

    // Return total value of board
    if (player == 1) return one_score - two_score;
    return two_score - one_score;
}
