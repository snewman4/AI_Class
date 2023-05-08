// Compile w/ > g++ -o find_route main.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>

#include "TreeNode.h"

using namespace std;

/*
 * Function to split a line of 3 into thirds
 * Args: string*, line to split
 * Returns: string[3], list of split strings
 */
vector<string> split_line(string line) {
    vector<string> ret_list = {};
    // While still ' '
    int split = line.find(' ');
    while (split != string::npos) {
        string word = line.substr(0, split);
        ret_list.push_back(word);
        line = line.substr(split + 1);
        split = line.find(' ');
    }

    ret_list.push_back(line); // Add final word

    return ret_list;
}

/*
 * Function to get unique cities from a file
 * Args: string*, pointer to name of file
 * Returns: vector<string>, list of unique cities
 *
 * NOTE : You could make an argument for the use of a set,
 *        as opposed to a vector. However, the implementation
 *        of the adjacency matrix relies on indexes, which
 *        are not supported by sets.
 */
vector<string> get_unique_cities(string *filename) {
    ifstream infile(*filename);
    string line;
    vector<string> cities = {};
    // Read in each line
    if (infile.good()) {
        while (getline(infile, line)) {
            // Look for END OF INPUT
            if (line.compare("END OF INPUT") == 0) {
                infile.close();
                return cities;
            }
            vector<string> split = split_line(line);
            // Identify unique cities
            string city1 = split[0];
            string city2 = split[1];
            // Add unique cities
            if (find(cities.begin(), cities.end(), city1) == cities.end())
                cities.push_back(city1);
            if (find(cities.begin(), cities.end(), city2) == cities.end())
                cities.push_back(city2);
        }
        infile.close();
        cerr << "No END OF INPUT found, function still succeeded" << endl;
        return cities;
    }
    // Failure
    cerr << "Unable to open file " << filename << endl;
    return {};
}

/*
 * Overload of the comparator between two tree nodes, this
 * compares their _path_cost values.
 * Args: TreeNode&, first TreeNode to check
 *       TreeNode&, second TreeNode to check
 * Returns: bool, True if t1->_path_cost > t2->_path_cost,
 *          else false
 */
bool operator>(TreeNode t1, TreeNode t2) {
    return t1.get_path_cost() > t2.get_path_cost();
}

/*
 * Function to print out the path from the origin to the destination
 * Args: TreeNode*, node representing the destination
 * Returns: void
 */
void print_output(TreeNode *dest_node, vector<string> city_names) {
    // No path exists
    if (dest_node == nullptr) {
        cout << "distance: infinity" << endl;
        cout << "route:" << endl;
        cout << "none" << endl;
        return;
    }

    cout << "distance: " << dest_node->get_path_cost() << " km" << endl;
    cout << "route:" << endl;

    // The origin and destinations were the same
    if (dest_node->get_parent() == nullptr) {
        cout << city_names[dest_node->get_state()] << " to " << city_names[dest_node->get_state()] << ", 0 km" << endl;
        return;
    }

    // Structures to store path
    vector<int> cities;
    vector<int> distances;

    while (dest_node != nullptr) {
        cities.push_back(dest_node->get_state());
        distances.push_back(dest_node->get_path_cost());
        dest_node = dest_node->get_parent();
    }

    for (int i = cities.size() - 1; i >= 1; i--)
        cout << city_names[cities[i]] << " to " << city_names[cities[i-1]] << ", " << distances[i-1] - distances[i] << " km" << endl;
}

/*
 * Function to perform uniform cost search on an input file, and print
 * out the optimal path to get to the destination.
 * Args: string, the path to the input file
 *       string, the name of the origin node
 *       string, the name of the destination node
 * Returns: int, the status of the execution
 */
int uniform_cost_search(string filename, const string& origin, const string& dest) {
    // Verify file
    ifstream infile(filename);
    string line;
    if (!infile.good()) {
        cerr << "Unable to find file: " << filename << ". Please try again." << endl;
        return -1;
    }

    // Get list of unique cities
    vector<string> cities = get_unique_cities(&filename);

    // Verify that origin and destination exist
    if (find(cities.begin(), cities.end(), origin) == cities.end()) {
        cerr << origin << " was not found in " << filename << "; please verify." << endl;
        return -1;
    }
    if (find(cities.begin(), cities.end(), dest) == cities.end()) {
        cerr << dest << " was not found in " << filename << "; please verify." << endl;
        return -1;
    }

    // Create adjacency matrix
    int num_cities = cities.size();
    int adj_mat[num_cities][num_cities]; // Square matrix, one row / col per city
    for (int i = 0; i < num_cities; i++) {
        for (int j = 0; j < num_cities; j++) {
            if (i == j) adj_mat[i][j] = 0;
            else adj_mat[i][j] = -1;
        }
    }

    // Read in contents of file, store
    int origin_index;
    int dest_index;

    while (getline(infile, line)) {
        // Look for END OF INPUT
        if (line == "END OF INPUT")
            break;
        // Define distances in adjacency matrix
        vector<string> split = split_line(line);
        int city1 = find(cities.begin(), cities.end(), split[0]) - cities.begin();
        int city2 = find(cities.begin(), cities.end(), split[1]) - cities.begin();
        int dist = stoi(split[2]);

        // Store the index of origin and destination
        // TODO : (OPTIONAL) Try to make this better
        if (split[0] == origin)
            origin_index = city1;
        else if (split[1] == origin)
            origin_index = city2;
        if (split[0] == dest)
            dest_index = city1;
        else if (split[1] == dest)
            dest_index = city2;

        // Distance from city1 to city2 is the same as city2 to city1
        adj_mat[city1][city2] = dist;
        adj_mat[city2][city1] = dist;
    }

    infile.close();

    priority_queue<TreeNode, vector<TreeNode>, greater<>> frontier; // Frontier for new nodes, min-heap

    // Add origin to frontier
    TreeNode origin_node = TreeNode(origin_index, 0, nullptr);
    TreeNode *parent;
    frontier.push(origin_node);

    TreeNode *dest_node = nullptr; // Will be used to store goal node

    // Loop through the frontier
    while (!frontier.empty()) {
        // Get the node with the minimum path cost to it
        TreeNode curr_node = frontier.top();
        // Allocate new memory, because frontier.pop() will delete curr_node
        parent = new TreeNode(curr_node.get_state(), curr_node.get_path_cost(), curr_node.get_parent());
        frontier.pop();
        int curr_state = curr_node.get_state();

        // If this is the destination, then we found optimal path
        if (curr_state == dest_index) {
            dest_node = &curr_node;
            break;
        }

        // If depth > num_cities, this is looping, drop it
        if (curr_node.get_depth() > num_cities)
            continue;

        // Find all adjacencies from curr_node
        for (int i = 0; i < num_cities; i++) {
            // Don't track if no adjacency, or itself
            if (adj_mat[curr_state][i] <= 0)
                continue;
            // Add expanded node to frontier
            TreeNode exp_node = TreeNode(i, curr_node.get_path_cost() + adj_mat[curr_state][i], parent);
            frontier.push(exp_node);
        }
    }

    print_output(dest_node, cities);
    return 0;
}

/*
 * Main function of the program, finds shortest path from origin to goal
 * Args: string, name of the input file
 *       string, name of the origin city
 *       string, name of the destination city
 * Returns: int, status of completion
 */
int main(int argc, char *argv[]) {
    // Verify correct number of arguments
    if (argc != 4) {
        cerr << "Please use format .\\find_route <input_file> <origin_city> <destination_city>" << endl;
        return -1;
    }

    return uniform_cost_search(argv[1], argv[2], argv[3]);
}
