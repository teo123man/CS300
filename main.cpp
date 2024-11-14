#include <iostream>
#include <fstream>
#include "stack.h"
#include <string>
using namespace std;

struct State 
{
    int row;
    int col;
};

int PmtForN()
{
    int N;
    cout << "Enter the value of N for the N-Queens problem: ";
    cin >> N;
    return N;
}

bool attackControl(int row, int col, int N, bool colCheck[], bool diagFCheck[], bool diagSCheck[])
{
    //Controls if there are any threats to the queen + does row check
    return col < N && colCheck[col] || diagFCheck[row + col] || diagSCheck[N - 1 + col - row];
} 

void removeQueen(State& currentState, int N, bool colCheck[], bool diagFCheck[], bool diagSCheck[], int positions[]) {
    colCheck[currentState.col] = false;
    diagFCheck[currentState.row + currentState.col] = false;
    diagSCheck[N - 1 + currentState.col - currentState.row] = false;
    positions[currentState.row] = -1;
}

int main() 
{
    int N = PmtForN();

    if(N <= 0) 
    {
        cout << "Invalid input. Please enter a positive integer" << endl;
        return 0;
    }

    int* positions = new int[N];

    for(int i = 0; i < N; i++) 
    {
        positions[i] = -1;
    }

    // To check column and the two diagonals is enough, don't need to check row.
    // Instead of a n*n 2d matrix to check the board,
    // I implemented 3 1d arrays to check the columns and the two diagonals.
    // This reduces the space complexity from O(n^2) to O(n).
    bool* colCheck = new bool[N];
    bool* diagFCheck = new bool[2 * N - 1];
    bool* diagSCheck = new bool[2 * N - 1];

    // Initialize the arrays
    for(int i = 0; i < N; i++) 
    {
        colCheck[i] = false;
    }

    for(int i = 0; i < 2 * N - 1; i++) 
    {
        diagFCheck[i] = false;
        diagSCheck[i] = false;
    }
    
    Stack<State> stack; 
    
    ofstream outFile;
    string fileName = to_string(N) + "queens_solutions.txt";
    outFile.open(fileName);

    int solutionCount = 0;

    // initial state
    State currentState;
    currentState.row = 0;
    currentState.col = -1; // Will be 0 right after being incremented
    // currentState.col = 0; 

    bool done = false; 
    while (!done)
    {
        currentState.col += 1; // needed because of the -1

        while (attackControl(currentState.row, currentState.col, N, colCheck, diagFCheck, diagSCheck))
        {
            currentState.col += 1;
        }

        if (currentState.col < N)
        {
            //placing queen
            positions[currentState.row] = currentState.col;
            colCheck[currentState.col] = true;
            diagFCheck[currentState.row + currentState.col] = true;
            diagSCheck[N - 1 + currentState.col - currentState.row] = true;

            if (currentState.row == N - 1) 
            {
                solutionCount++;
                outFile << "Solution " << solutionCount << ": [";
                for (int i = 0; i < N; i++)
                {
                    outFile << positions[i];
                    if (i != N - 1) outFile << ", ";
                }
                outFile << "]" << endl;

                //Backtracking
                removeQueen(currentState, N, colCheck, diagFCheck, diagSCheck, positions);
                while (currentState.col >= N - 1)
                {
                    if (stack.isEmpty())
                    {
                        done = true;
                        break;
                    }
                    currentState = stack.top();
                    stack.pop();
                    removeQueen(currentState, N, colCheck, diagFCheck, diagSCheck, positions);
                }
                if (done)
                {
                    break;
                }
            }
            else
            {
                //Push current state and move to next row
                stack.push(currentState);
                currentState.row += 1;
                currentState.col = -1;
            }
        }
        else
        {
            if (stack.isEmpty())
            {
                break; 
            }
            currentState = stack.top();
            stack.pop(); //backtracking
            removeQueen(currentState, N, colCheck, diagFCheck, diagSCheck, positions);
        }
    }

    outFile.close();

    //Opening the file again to put the total solutions at the top
    ifstream inFile(fileName);
    string fileContent;
    string line;
    while(getline(inFile, line)) 
    {
        fileContent += line + "\n";
    }
    inFile.close();

    // Write total solutions - stream
    outFile.open(fileName);
    outFile << "Total solutions for N=" << N << ": " << solutionCount << endl;
    outFile << fileContent;
    outFile.close();

    cout << "Solutions have been saved to '" << fileName << "'" << endl;

    // Deleting leftover memory
    delete[] positions;
    delete[] colCheck;
    delete[] diagFCheck;
    delete[] diagSCheck;

    return 0;
}
