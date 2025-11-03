#include <iostream>
#include<vector>
#include <algorithm>
using namespace std;

class sodokuPuzzle {
    private:
        vector<vector<int>> puzzle;
    public:

    void addValue(int value, int row, int col) {
        if (puzzle[row][col] != 0) {
            throw invalid_argument("Spot already filled");
        }

        vector<int> colList;
        for(auto listedRow: puzzle) {
            colList.push_back(listedRow[col]);
        }

        int squareRowFloor = row / 3;
        int squareColFloor = col / 3;

        vector<int> squareList;

        for(int i = 0; i < 9; i++) {
            if (i == squareRowFloor) {
                for(int j = 0; j < 9; j++) {
                    if(j == squareColFloor) {
                        squareList.push_back(puzzle[i][j]);
                    }
                }
            }
        }

        auto valueInRow = find(puzzle[row].begin(), puzzle[row].end(), value);
        auto valueInCol = find(colList.begin(), colList.end(), value);
        auto valueInSquare = find(squareList.begin(), squareList.end(), value);

        if (valueInRow != puzzle[row].end() || valueInCol != colList.end() || valueInSquare != squareList.end()) {
            throw invalid_argument("InvalidValue");
        }
        else {
            puzzle[row][col] = value;
        }
        return;
    }

    void removeValue(int row, int col) {
        puzzle[row][col] = 0;
        return;
    }

    bool isSolved() {
        for(auto row: puzzle) {
            for(auto value: row) {
                if (value == 0) {
                    return false;
                }
            }
        }

        return true;
    }
};

void main() {
    
    return;
}