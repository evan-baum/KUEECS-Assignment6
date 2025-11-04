/*
Program Name: EECS 348 Assignment 6
Description: Brute for sodoku puzzle solver refactored into c++
Inputs: 5 files named puzzle[n].txt where [n] is 1-5
Output: 5 files named solution[n].txt where [n] is 1-5, each file has each solved puzzle and the number of solved puzzles
Collaborators: None
Sources: Assignment 2 python code
Author: Evan Baumgartner
Date Created: 11.02.2025
*/

// includes needed modules
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
using namespace std;

// class to store a sodoku puzzle as a vector of int vectors and the methods that are needed
// Author: Evan Baumgartner
class sodokuPuzzle
{
public:
    vector<vector<int>> puzzle; // stores puzzle as a vector of int vectors

    // method to add a value to a sodoku puzzle, if the value is invalid or there is already a value
    // at the given spot an invalid_argument error is thrown
    void addValue(int value, int row, int col)
    {
        if (puzzle[row][col] != 0) // checks if the spot is filled (not 0)
        {
            throw invalid_argument("Spot already filled"); // throws an error
        }

        vector<int> colList;          // creates a vector to store values in a column
        for (auto listedRow : puzzle) // iterates across rows
        {
            colList.push_back(listedRow[col]); // adds values of each column from the given row to colList
        }

        int squareRowFloor = row / 3; // creates a number to compare to the row to determine the square the spot is in
        int squareColFloor = col / 3; // creates a number to compare to the column to determine the square the spot is in

        vector<int> squareList;     // creates a vector to store the values in the 3x3 the spot is in
        for (int i = 0; i < 9; i++) // iterates across rows
        {
            if (i / 3 == squareRowFloor) // checks if i floor 3 is the same as squareRowFloor (they are in the same square rows)
            {
                for (int j = 0; j < 9; j++) // iterates across columns
                {
                    if (j / 3 == squareColFloor) // checks if i floor 3 is the same as squareColFloor (they are in the same square columns)
                    {
                        squareList.push_back(puzzle[i][j]); // adds value to the squareList
                    }
                }
            }
        }

        auto valueInRow = find(puzzle[row].begin(), puzzle[row].end(), value);  // finds value in row if it exists (if not valueInRow will be puzzle[row].end())
        auto valueInCol = find(colList.begin(), colList.end(), value);          // finds value in column if it exists (if not valueInRow will be colList.end())
        auto valueInSquare = find(squareList.begin(), squareList.end(), value); // finds value in square if it exists (if not valueInRow will be squareList.end())

        if (valueInRow != puzzle[row].end()) // checks if value is in row
        {
            throw invalid_argument("Value in Row"); // throws error
        }
        else if (valueInCol != colList.end()) // checks if value is in column
        {
            throw invalid_argument("Vlaue in Column"); // throws error
        }
        else if (valueInSquare != squareList.end()) // checks if value is in square
        {
            throw invalid_argument("Value in Square"); // throws error
        }
        else
        {
            puzzle[row][col] = value; // adds value to given spot
        }
        return;
    }

    // method to remove a value from a given spot
    void removeValue(int row, int col)
    {
        puzzle[row][col] = 0; // sets spot's value to 0
        return;
    }

    // value to check if the puzzle is solved, returns boolean
    bool isSolved()
    {
        for (auto row : puzzle) // iterates across rows
        {
            for (auto value : row) // iterates across values in row
            {
                if (value == 0) // checks if the value is 0 (puzzle not solved)
                {
                    return false;
                }
            }
        }

        return true;
    }

    // method to get the 2d vector of the puzzle
    vector<vector<int>> getPuzzle()
    {
        return puzzle; // returns puzzle
    }

    // method to print the puzzle to the console
    void print()
    {
        for (auto row : puzzle) // iterates across rows
        {
            for (auto value : row) // iterates across values in rows
            {
                cout << value << " "; // adds value and a space
            }
            cout << endl; // adds new line to format square
        }

        return;
    }

    // method to output the puzzle to a file, takes in a pointer to an ofstream
    void filePrint(ofstream *filePtr)
    {
        for (auto row : puzzle) // iterates across rows
        {
            for (auto value : row) // iterates across values in rows
            {
                *filePtr << value << " "; // adds value and space to dereferenced file stream
            }
            *filePtr << endl; // adds new line to format square
        }

        return;
    }
};

// recursive function to solve an input sodokuPuzzle, takes in a vector of sodokuPuzzles
// Author: Evan Baumgartner
vector<sodokuPuzzle> recPuzzleSolve(sodokuPuzzle sodoku, vector<sodokuPuzzle> solvedPuzzles)
{
    vector<vector<int>> puzzle = sodoku.getPuzzle(); // gets puzzle from the input sodoku so getPuzzle isnt continually called

    int row, col;               // declares row and col to store first empty space
    bool found = false;         // boolean for if an empty space has been found
    for (int i = 0; i < 9; i++) // iterates across rows
    {
        for (int j = 0; j < 9; j++) // iterates across columns
        {
            if (puzzle[i][j] == 0) // checks if the value at i,j is a 0 (empty)
            {
                row = i;      // sets row to i
                col = j;      // sets col to j
                found = true; // marks an empty spot has been found
                break;        // breaks from column loop
            }
        }
        if (found) // if an empty space has been found
        {
            break; // break from row loop
        }
    }

    if (found) // if an empty space has been found
    {
        for (int value = 1; value < 10; value++) // iterate nums 1-9
        {
            try // try block to handle errors of adding values
            {
                sodoku.addValue(value, row, col); // adds value to the given row and col
                if (sodoku.isSolved())            // checks if the puzzle has been solved
                {
                    solvedPuzzles.push_back(sodoku); // adds sodoku
                    sodoku.removeValue(row, col);    // removes value from row and col for future iteration
                    return solvedPuzzles;            // returns solved puzzles with solved puzzle added
                }
                else
                {
                    solvedPuzzles = recPuzzleSolve(sodoku, solvedPuzzles); // iterates with new value added
                    sodoku.removeValue(row, col);                          // removes value from row and col for future iteration
                }
            }
            catch (invalid_argument e) // catches invalid_argument errors (when the value is invalid)
            {
                continue; // continues through value iteration without recursion
            }
            catch (...) // any other value
            {
                cout << "error"; // outputs error to console
            }
        }
    }

    return solvedPuzzles; // returns solvedPuzzlesn vector
}

// facade function to call recursion to solve sodoku puzzles
vector<sodokuPuzzle> puzzleSolve(sodokuPuzzle sodoku)
{
    vector<sodokuPuzzle> solvedList;           // creates empty vector of sodokuPuzzle objects to input into recursive function
    return recPuzzleSolve(sodoku, solvedList); // returns output of recursive solving function
}

// main function to take in all puzzles and output to solution files
// Author: Evan Baumgartner
int main()
{
    for (int i = 1; i < 6; i++) // iterates across 5 puzzle files
    {
        string inFileName = "puzzle" + to_string(i) + ".txt";    // creates the input file name from the iteration
        string outFileName = "solution" + to_string(i) + ".txt"; // creates the output file name from the iteration

        ifstream inputFile(inFileName);   // gets input file stream
        ofstream outputFile(outFileName); // gets output file stream

        string line;         // creates a line to read each line of the file
        sodokuPuzzle sodoku; // declares a sodokuPuzzle variable as sodoku to store the input to be solved

        if (inputFile.is_open()) // checks is the file can be opened or is available
        {
            while (getline(inputFile, line)) // loops across lines of the file
            {
                if (!line.empty()) // checks that the line is not empty
                {
                    vector<int> puzzleLine; // creates a vector of integers to take in a line of the sodoku puzzle

                    for (auto character : line) // iterates across all characters in the line
                    {
                        if (character != ' ') // checks that the character is not a space
                        {
                            if (character != '_') // if the character is not an underline
                            {
                                puzzleLine.push_back(character - '0'); // add character casted to int to line
                            }
                            else
                                puzzleLine.push_back(0); // if it's an underscore a 0 is added to the line
                        }
                    }
                    sodoku.puzzle.push_back(puzzleLine); // adds line to the sodoku to create the puzzle
                }
            }
        }
        vector<sodokuPuzzle> solvedPuzzles = puzzleSolve(sodoku); // uses puzzleSolve

        for (auto solvedPuzzle : solvedPuzzles) // iterates acrossed solved puzzles
        {
            solvedPuzzle.filePrint(&outputFile); // prints all sodoku puzzles
            outputFile << endl;                  // adds new line to output
        }

        outputFile << "Solutions found: " << solvedPuzzles.size(); // amount of solutions found
    }

    return 0;
}