#include <iostream>
#include <limits>

#include "Board.h"
#include "Minimax.h"

using namespace std;

/*
 * Function to print an error informing the user of the valid formats for the
 * execution call. Allows for more detail with type.
 * Args: string, describes the issue that caused the error
 * Returns: int, -1 denoting an error
 */
int format_error(const string& type) {
    cerr << "Problem with " << type << endl;
    cerr << "Please use one of the following formats:" << endl;
    cerr << ".\\maxconnect4 interactive [input file] <computer-next / human-next> [depth]" << endl;
    cerr << ".\\maxconnect4 one-move [input file] [output file] [depth]" << endl;
    return(-1);
}

/*
 * Function to allow the human player to take their turn.
 * Args: Board*, the game board
 */
void human_turn(Board *game_board) {
    int selection;

    do {
        cout << "Please select a column to play a piece in." << endl;
        cout << "Remember that you are number " << game_board->hum_num << "." << endl;
        cin >> selection;
        while (!cin.good()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please use a valid integer." << endl;
            cin >> selection;
        }
    } while (game_board->one_move(selection, game_board->hum_num) != 0);

    cout << "Human chose " << selection << endl;
    if (game_board->write_board("human.txt", game_board->comp_num) != 0)
        cerr << "Problem writing to 'human.txt'. Please verify." << endl;
}

/*
 * Function to allow the computer player to take their turn, using minimax
 * Args: Board*, the game board
 *       Minimax*, the algorithm to use
 *       int, the maximum depth to search for an optimal solution
 *       string&, the file to write to
 */
void computer_turn(Board *game_board, Minimax *minimax, int depth, const string& out_filename) {
    int selection = minimax->alpha_beta(game_board, game_board->comp_num, depth);
    cout << "Computer chose " << selection + 1 << endl;

    game_board->one_move(selection + 1, game_board->comp_num);

    if (game_board->write_board(out_filename, game_board->hum_num) != 0)
        cerr << "Problem writing to 'computer.txt'. Please verify." << endl;
}

/*
 * Function to handle the interactive mode of the game
 * Args: string, name of the input file
 *       string, who's turn is next, either computer-next or human-next
 *       int, depth that the program will look ahead
 * Returns: int, status of the execution
 */
int interactive_mode(const string& in_filename, string next, int depth) {
    // Verify next player's turn
    if (next != "computer-next" && next != "human-next")
        return format_error("next player's turn");

    // Verify the max depth
    if (depth < 1 || depth > 42) {
        cerr << "Invalid depth: " << depth << ". Please use an int between 1 and 42" << endl;
        return(-1);
    }

    // Create the game board
    Board game_board = *new Board();
    int next_turn = game_board.read_board(in_filename, next); // Next player

    if (next_turn == -1) {
        cerr << "Something went wrong reading the board." << endl;
        return(-1);
    }

    // Verify that the game board is correct
    if (game_board.verify_board() != 0) {
        cerr << "Please ensure that the game board follows the expected format." << endl;
        return(-1);
    }

    Minimax minimax;

    // Swap through turns
    while (game_board.can_play()) {
        game_board.draw_board();
        if (next == "computer-next") {
            cout << "Computer's Turn:" << endl;
            computer_turn(&game_board, &minimax, depth, "computer.txt");
            next = "human-next";
        } else {
            cout << "Human's Turn:" << endl;
            human_turn(&game_board);
            next = "computer-next";
        }
    }

    cout << "GAME OVER." << endl;
    game_board.draw_board();

    return(0);
}

/*
 * Function to handle the one-move mode of the game
 * Args: string, name of the input file
 *       string, name of the output file
 *       int, depth that the program will look ahead
 * Returns: int, status of the execution
 */
int one_move_mode(const string& in_filename, const string& out_filename, int depth) {
    // Verify the max depth
    if (depth < 1 || depth > 42) {
        cerr << "Invalid depth: " << depth << ". Please use an int between 1 and 42" << endl;
        return(-1);
    }

    // Create the game board
    Board game_board = *new Board();
    int next_turn = game_board.read_board(in_filename, "computer-next"); // Next player

    if (next_turn == -1) {
        cerr << "Something went wrong reading the board." << endl;
        return(-1);
    }

    // Verify that the game board is correct
    if (game_board.verify_board() != 0) {
        cerr << "Please ensure that the game board follows the expected format." << endl;
        return(-1);
    }

    game_board.draw_board();
    if (!game_board.can_play()) return(0);

    Minimax minimax;

    computer_turn(&game_board, &minimax, depth, out_filename);

    game_board.draw_board();

    return(0);
}

/*
 * Main for the program to use minimax to play Max Connect 4
 * Args: string, mode, either interactive or one-move
 *       string, name of the input file
 *       string, either next turn (interactive mode) or output filename (one-move mode)
 *       int, the max depth that the program will look ahead
 * Returns: int, the status of the execution
 *
 * Note: For a blank board, a depth of 16 can finish in under 30s. Any higher value will take
 *       significantly longer, and should not be used.
 */
int main(int argc, char *argv[]) {
    // Verify number of arguments
    if (argc != 5)
        return format_error("number of arguments");

    // Store relevant information
    string mode = argv[1];
    string input_filename = argv[2];
    string next_out_filename = argv[3];
    int depth = atoi(argv[4]);

    // Run the appropriate program
    if (mode == "interactive")
        return interactive_mode(input_filename, next_out_filename, depth);
    else if (mode == "one-move")
        return one_move_mode(input_filename, next_out_filename, depth);
    else
        return format_error("mode");
}
