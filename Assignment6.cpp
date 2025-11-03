#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
using namespace std;

class sodokuPuzzle
{
public:
    vector<vector<int>> puzzle;

    void addValue(int value, int row, int col)
    {
        if (puzzle[row][col] != 0)
        {
            throw invalid_argument("Spot already filled");
        }

        vector<int> colList;
        for (auto listedRow : puzzle)
        {
            colList.push_back(listedRow[col]);
        }

        int squareRowFloor = row / 3;
        int squareColFloor = col / 3;

        vector<int> squareList;

        for (int i = 0; i < 9; i++)
        {
            if (i / 3 == squareRowFloor)
            {
                for (int j = 0; j < 9; j++)
                {
                    if (j / 3 == squareColFloor)
                    {
                        squareList.push_back(puzzle[i][j]);
                    }
                }
            }
        }

        auto valueInRow = find(puzzle[row].begin(), puzzle[row].end(), value);
        auto valueInCol = find(colList.begin(), colList.end(), value);
        auto valueInSquare = find(squareList.begin(), squareList.end(), value);

        if (valueInRow != puzzle[row].end())
        {
            throw invalid_argument("Value in Row");
        }
        else if (valueInCol != colList.end())
        {
            throw invalid_argument("Vlaue in Column");
        }
        if (valueInSquare != squareList.end())
        {
            throw invalid_argument("Value in Square");
        }
        else
        {
            puzzle[row][col] = value;
        }
        return;
    }

    void removeValue(int row, int col)
    {
        puzzle[row][col] = 0;
        return;
    }

    bool isSolved()
    {
        for (auto row : puzzle)
        {
            for (auto value : row)
            {
                if (value == 0)
                {
                    return false;
                }
            }
        }

        return true;
    }

    vector<vector<int>> getPuzzle()
    {
        return puzzle;
    }

    void print()
    {
        for (auto row : puzzle)
        {
            for (auto value : row)
            {
                cout << value << " ";
            }
            cout << endl;
        }

        return;
    }
};

vector<sodokuPuzzle> recPuzzleSolve(sodokuPuzzle sodoku, vector<sodokuPuzzle> solvedPuzzles)
{
    vector<vector<int>> puzzle = sodoku.getPuzzle();

    int row = 0, col = 0;
    bool found = false;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (puzzle[i][j] == 0)
            {
                row = i;
                col = j;
                found = true;
                break;
            }
        }
        if (found)
        {
            break;
        }
    }

    for (int value = 1; value < 10; value++)
    {
        try
        {
            sodoku.addValue(value, row, col);
            if (sodoku.isSolved())
            {
                solvedPuzzles.push_back(sodoku);
                sodoku.removeValue(row, col);
                return solvedPuzzles;
            }
            else
            {
                solvedPuzzles = recPuzzleSolve(sodoku, solvedPuzzles);
                sodoku.removeValue(row, col);
            }
        }
        catch (invalid_argument e)
        {
            continue;
        }
        catch (...)
        {
            cout << "error";
        }
    }

    return solvedPuzzles;
}

vector<sodokuPuzzle> puzzleSolve(sodokuPuzzle sodoku)
{
    vector<sodokuPuzzle> solvedList;
    return recPuzzleSolve(sodoku, solvedList);
}

int main()
{
    for (int i = 1; i < 6; i++)
    {
        char puzzleNum = i + '0';
        string inFileName = "puzzle" + puzzleNum;
        inFileName += ".txt";

        string outFileName = "solution" + puzzleNum;
        inFileName += ".txt";

        ifstream stringFile(inFileName); // gets file stream
        string line;                        // creates a line to read each line of the file
        sodokuPuzzle sodoku;

        if (stringFile.is_open()) // checks is the file can be opened or is available
        {
            while (getline(stringFile, line)) // loops across lines of the file
            {
                if (!line.empty())
                {
                    vector<int> puzzleLine;
                    for (auto character : line)
                    {
                        if (character != ' ')
                        {
                            if (character != '_')
                            {
                                puzzleLine.push_back(character - '0');
                            }
                            else
                                puzzleLine.push_back(0);
                        }
                    }
                    sodoku.puzzle.push_back(puzzleLine);
                }
            }
        }
        vector<sodokuPuzzle> solvedPuzzles = puzzleSolve(sodoku);

        cout << solvedPuzzles.size();

        for (auto solvedPuzzle : solvedPuzzles)
        {
            solvedPuzzle.print();
        }
    }

    return 0;
}