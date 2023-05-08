Samuel Newman
10850376

Programmed with C++
Compiled and Ran with Windows 10

The code uses two classes, along with a main file containing several necessary functions.
Main File:
Contains the logic for reading the input and initiating the correct move.
Contains one function each for one-move and interactive mode which control player-side logic.
Contains several helper functions to control a single turn for the computer and human,
and to control error output.
Board Class:
Contains functions for reading from an input file and writing to an output file.
Contains functions for in-game playing, such as playing a single move, scoring the current
board, and verifying that the board is formatted correctly.
Contains the actively used board itself.
Contains logic for drawing the board for the user.
Minimax Class:
Contains three functions: one to start the minimax algorithm, a max-function, and a min-function.

Compilation:
In the Project_2 directory, run the following command. This uses g++ to compile the C++ code.

g++ -o maxconnect4 main.cpp Board.h Board.cpp Minimax.h Minimax.cpp

Running:
Once the code has been compiled to the maxconnect4 executable, run one of
the following commands in the same directory as the executable.

.\maxconnect4 one-move [input_file] [output_file] [depth]
or
.\maxconnect4 interactive [input_file] [human-next / computer-next] [depth]

I recommend using a depth of 16 or less. At depth 16, an empty board takes about 30 seconds
for the computer to make a move on.

There is one file for each test. All of the tests, including how to run them and what they
should output, are detailed in tests.txt.
