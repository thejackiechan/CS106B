// By Jackie Chan with help from David Morales and Tessera Chin

// The Game of Life allows the user to simulate the life cycle of bacteria (following a specific set of rules) by first inputting a
// text file name containing a bacterial colony grid. The main function first sets up the console by displaying the rules, creating
// the initial grid, and then animating the life cycle frame by frame. The user is given various options such as choosing the initial
// grid, if they want to wrap, and how they want to animate the simulation.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
using namespace std;

void display_text();
void create_grid(int &rows, int &columns, Grid<char> &matrix);
void edit_grid(Grid<char> &matrix, string str, int rows, int columns);
void print_grid(Grid<char> matrix);
void animate(Grid<char> matrix, string & str, int rows, int columns);
void wrap(string &str);
void count_neighbors(Grid<char> temp, string str, int r, int c, int rows, int columns, int &count);

int main() {
    int rows;
    int columns;
    Grid<char> matrix;
    string str;

    display_text();
    create_grid(rows, columns, matrix);
    wrap(str);
    print_grid(matrix);
    animate(matrix, str, rows, columns);
    cout << "Have a nice Life!" << endl;
    return 0;
}

// Displays the introduction and rules to the Game of Life.

void display_text(){
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << "" << endl;
}

// Reads the text file and stores the grid into matrix

void create_grid(int &rows, int &columns, Grid<char> &matrix){
    ifstream stream;
    string s;
    s = promptUserForFile(stream, "Grid input file name? ");
    getLine(stream, s);
    rows = stringToInteger(s);
    getLine(stream, s);
    columns = stringToInteger(s);
    matrix.resize(rows, columns);

    for(int r = 0; r < matrix.numRows(); r++) {
        getLine(stream, s);
        for(int c = 0; c < matrix.numCols(); c++) {
            matrix[r][c] = s[c];
        }
    }
    stream.close();
}

// Iterates through each position of the grid, calling count_neighbors and modifying each position with either 'X' or '-'.

void edit_grid(Grid<char> &matrix, string str, int rows, int columns){
    Grid<char> temp = matrix;
    int count = 0;
    for(int r = 0; r < temp.numRows(); r++){
        for(int c = 0; c < temp.numCols(); c++){
            count_neighbors(temp, str, r, c, rows, columns, count);

            if(count < 2){
                matrix[r][c] = '-';
            }else if(count == 3){
                matrix[r][c] = 'X';
            }else if(count > 3){
                matrix[r][c] = '-';
            }count = 0;
        }
    }
}

// Checks each adjacent space and increments count for each cell. Method of checking neighbors depends on the string str (indicating
//wrap or not) passed in.

void count_neighbors(Grid<char> temp, string str, int r, int c, int rows, int columns, int &count){
    int row_position = 0;
    int column_position = 0;
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if(i != 0 || j != 0){ //false only if both are = 0
                if(str == "y"){ //y for wrap
                    row_position = (r + i + rows) % rows;
                    column_position = (c + j + columns) % columns;
                    if(temp[row_position][column_position] == 'X'){
                        count++;
                    }
                }else if(temp.inBounds(r+i, c+j) == true){  //for no wrap
                    if(temp[r+i][c+j] == 'X'){
                        count++;
                    }
                }
            }
        }
    }
}

// Prints out the matrix by iterating through each position.

void print_grid(Grid<char> matrix){
    for(int r = 0; r < matrix.numRows(); r++) {
        for(int c = 0; c < matrix.numCols(); c++) {
            cout << matrix[r][c];
        }cout << endl;
    }
}

// Takes in user input (animate, tick, or quit) and updates the console accordingly. If animate is selected, the console is cleared
// and the matrix is subsequently updated for the number of frames inputted by user. If tick is selected, the next frame is printed out.

void animate(Grid<char> matrix, string &str, int rows, int columns){
    int a;
    string s;
    while(s!= "q"){
        s = toLowerCase(getLine("a)nimate, t)ick, q)uit? "));
        if(s != "a" && s != "t" && s!= "q"){
            cout << "Invalid choice; please try again." << endl;
        }else if(s == "t"){
            edit_grid(matrix, str, rows, columns);
            print_grid(matrix);
        }else if(s == "a"){
            a = getInteger("How many frames? ");
            for(int i = 0; i < a; i++){
                pause(50);
                clearConsole();
                edit_grid(matrix, str, rows, columns);
                print_grid(matrix);
            }
        }
    }
}

// Prompts user if they want to wrap or not. Prompts user again if invalid input is entered.

void wrap(string &str){
    while(str != "y" && str != "n"){
        str = toLowerCase(getLine("Should the simulation wrap around the grid (y/n)? "));
        if(str != "y" && str != "n"){
            cout << "Invalid choice; please try again." << endl;
        }
    }
}
