Samuel Newman
10850376

Programmed with C++
Compiled and Ran with Windows 10

The code uses one class, along with a main file containing several necessary functions.
Main File:
Contains the logic for reading the input and initiating the search.
Contains the logic for initializing the map from the provided input file.
Contains the logic for uniform search, along with several helper functions.
TreeNode Class:
Contains individual information about a single tree node, including the state, depth,
path cost, and parent.

Compilation:
In the Project_1 directory, run the following command. This uses g++ to compile the C++ code.

g++ -o find_route .\main.cpp .\TreeNode.cpp .\TreeNode.h

Running:
Once the code has been compiled to the find_route executable, run the
following command in the same directory as the executable.

.\find_route.exe [input_file] [origin_city] [destination_city]

All of the tests, including how to run them and their expected output, are detailed
in the file tests.txt.