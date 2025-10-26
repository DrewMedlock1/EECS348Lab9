// Lab9 matrix operations code by Drew Medlock

#include <climits>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <limits>
using namespace std;

// function for printing a matrix
void print_matrix(vector<vector<int>> matrix, int largest_width){
    int size = matrix.size();
    // uses a for each loop to get the values out of matrix for printing
    // uses const as we aren't writing in the values, and the specifies that it is a reference to avoid copying
    for (const vector<int>& row : matrix){
        cout << "\t";
        for (const int& num : row){ // prints each value
            // cout << num << " ";
            printf("%*d ", largest_width, num); // this print will look good for any sized numbers, however from the example file, they might only be 2 digits long
            }
        cout << "\n";
    }
}

int sum_main_diag(vector<vector<int>> matrix){ // gets the some across the  main diagonal
    int sum = 0;
    for (int i = 0; i < matrix.size(); i++){
        sum += matrix[i][i];
    }
    return sum;
}

int sum_second_diag(vector<vector<int>> matrix){ // gets the sum across the second diagonal top right to bottom left
    int sum = 0;
    int j = matrix.size()-1;
    for (int i = 0; i < matrix.size(); i++){
        sum += matrix[i][j];
        j--;
    }
    return sum;
}

void swap_matrix_row(vector<vector<int>>& matrix, int index1, int index2){ // swaps the rows of a matrix
    if (index1 < 0 or index2 < 0 or index1 > matrix.size()-1 or index2 > matrix.size()-1){ // checks that the indices are in bounds
        cerr << "The indices provided are outside the range of the matrix.\n";
        cerr << matrix.size()-1 << " Index1" << index1 << " Index2: " << index2;
        throw runtime_error("Provided indices outside of range 0 < i < matrix.size - 1"); // throws an error if they aren't
    }
    matrix[index1].swap(matrix[index2]); // uses swap to perform an efficienet swap of the vectors
}

void swap_matrix_column(vector<vector<int>>& matrix, int column1, int column2){ // swaps the columns of a matrix
    if (column1 < 0 or column2 < 0 or column1 > matrix.size()-1 or column2 > matrix.size()-1){
        cerr << "The indices provided are outside the range of the matrix.\n";
        throw runtime_error("Provided indices outside of range 0 < i < matrix.size - 1");
    }
    for (int i = 0; i < matrix.size(); i++){
        swap(matrix[i][column1],(matrix[i][column2])); // swaps each individual value 1 by 1
    }
}

void update_matrix(vector<vector<int>>& matrix, int row, int column, int value){ // updates a value given a position
    if (row < 0 or column < 0 or row > matrix.size()-1 or column > matrix.size()-1){
        cerr << "The indices provided are outside the range of the matrix.\n";
        throw runtime_error("Provided indices outside of range 0 < i < matrix.size - 1");
    }
    matrix[row][column] = value; // changes the value of the matrix at given position
}


struct matrices { // struct for holding some data of the 2 matrices and the max_width of their numbers, as well as some methods
    // these are 2 dimensional arrays using vectors for memory management, where a vector holds vectors of type ints
    vector<vector<int>> matrix1;
    vector<vector<int>> matrix2;
    int max_width; // this is going to be used for better formatting in printing the matrix

    // method to print out the matrices
    void print_matrices(){ // this calls the print_matrix function to print each matrix
        cout << "Matrix 1:\n";
        print_matrix(matrix1, max_width);
        cout << "Matrix 2:\n";
        print_matrix(matrix2, max_width);
    }

    vector<vector<int>> add_matrices(){ // this returns a vector that is the result of adding the two matrices together
        vector<vector<int>> output_matrix(matrix1.size(),vector<int>()); // makes a new matrix for outputting to of the size of previous matrices
        for (int i = 0; i < matrix1.size(); i ++){
            for (int j = 0; j < matrix1.size(); j ++){
                output_matrix[i].push_back(matrix1[i][j]+matrix2[i][j]);
            }
        }
        return output_matrix;
    }

    vector<vector<int>> multiply_matrices(){ // this returns the result of the matrix multiplication between the matrices
        vector<vector<int>> output_matrix(matrix1.size(),vector<int>()); // still the same size because they are square matrices
        int value; // holder for addition between multiplication of elements
        for (int i = 0; i < matrix1.size(); i++){
            for (int j = 0; j < matrix1.size(); j++){
                value = 0;
                for (int k = 0; k < matrix1.size(); k++){
                    value = value + matrix1[i][k] * matrix2[k][j];
                    
                }
                output_matrix[i].push_back(value);
            }
        }
        return output_matrix;
    }

};

// this function for reading in the matrices assume the input file is well formatted based on instructions
// it will NOT handle incorrect file formatting well
matrices read_file(string file_name){ // given a file_name it reads it and returns 2 matrices
    ifstream input_file(file_name);
    if (input_file){
        cout << "Opened file: " << file_name << "\n";
        int size;
        int value; // integer for storing values being read in
        int max_width = 0; // value that will check max value in both matrices for formatting print
        input_file >> size; // extracts the first integer which is size
        if (size < 1){ // checks the size
            cerr << "Size listed in file was less than 1, then minimum size\n";
        } else { 
            vector<vector<int>> matrix1(size, vector<int>());
            vector<vector<int>> matrix2(size, vector<int>());
            for (int i = 0; i < size; i++){ // handles the first matrix
                for (int j = 0; j < size; j++){
                    input_file >> value;
                    matrix1[i].push_back(value); // pushes each value found to the vector
                    int value_width = to_string(value).length();
                    max_width = max(max_width, value_width); // this will find the max_width of each value for printing
                }
            }
            for (int i = 0; i < size; i++){ // handles the second matrix
                for (int j = 0; j < size; j++){
                    input_file >> value;
                    matrix2[i].push_back(value);
                    int value_width = to_string(value).length();
                    max_width = max(max_width, value_width);
                }
            }
            matrices m1m2;
            m1m2.matrix1 = matrix1;
            m1m2.matrix2 = matrix2;
            m1m2.max_width = max_width;
            return m1m2;
        } // end of if else
        input_file.close();
    } else { // if input file was a logical 0
        cerr << "Couldn't read file: " << file_name << "\n";
        throw runtime_error("File Not Found");
    } // end of if else
}

// gets a valid integer input given a lower and upper range
int int_get_valid_input(int lower_range, int upper_range){
    int output_int;
    int valid_input = 0;
    while(!valid_input){
        cin >> output_int; // the call to this function should be proceeded by a prompt for user input
        if (cin.fail() or output_int < lower_range or output_int > upper_range){ // checks for a failure when reading
            cout << "Invalid input. \n";
            cin.clear(); // clears the failure bit
            cout << "Enter an integer between " << lower_range << "-" << upper_range << ": ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // this will ignore all the previous input to essentially clear it and stop at a newline
        } else {
            valid_input = 1;
        }
    }
    return output_int;
}

int main(){
    string file_name;
    cout << "Enter the filename: ";
    getline(cin, file_name); // uses getline in case filename has spaces
    try { // code runs in a try block
        matrices m1m2 = read_file(file_name); // reads in file
        cout << "Got: \n";
        m1m2.print_matrices(); // prints it
        char input = ' '; // an initial char
        while (input != 'q'){ // while user hasn't said to quit, keep running
            cout << "Operations:\n" << "1: Add the matrices\n" << "2: Multiply the matrices\n" << "3: Sum the diagonal elements\n" << "4: Swap matrix rows\n"
            << "5: Swap matrix columns\n" << "6: Change element\n" << "q: Quit the program\n";
            cout << "Enter operation: ";
            cin >> input; // get which operation the user wants
            switch (input){
                case '1': // case for adding matrices
                    print_matrix(m1m2.add_matrices(), m1m2.max_width+1);
                    break;
                case '2': // case for multiplying matrices
                    print_matrix(m1m2.multiply_matrices(), m1m2.max_width*2);
                    break;
                case '3': // case for summing diagonals
                    cout << "Choose which matrix to sum (1/2): ";
                    char matrix;
                    cin >> matrix;
                    while (matrix != '1' and matrix != '2'){
                        cout << "Enter only '1' or '2':\n";
                        cin >> matrix;
                    }
                    cout << "Choose a diagonal, main or secondary (m/s): ";
                    char diagonal;
                    cin >> diagonal;
                    while (diagonal != 'm' and diagonal != 's'){
                        cout << "Enter only 'm' or 's':\n";
                        cin >> diagonal;
                    }
                    if (matrix == '1'){
                        if (diagonal == 'm'){cout << "Sum of main diagonal of matrix 1: " << sum_main_diag(m1m2.matrix1) << '\n';} 
                        else { cout << "Sum of second diagonal of matrix 1: " << sum_second_diag(m1m2.matrix1) << '\n';}
                    } else {
                        if (diagonal == 'm'){cout << "Sum of main diagonal of matrix 2: " << sum_main_diag(m1m2.matrix2) << '\n';} 
                        else { cout << "Sum of second diagonal of matrix 2: " << sum_second_diag(m1m2.matrix2) << '\n';}
                    }
                    break;
                case '4': // case for swapping rows
                    cout << "Choose which matrix to change (1/2): ";
                    char row_matrix;
                    cin >> row_matrix;
                    while (row_matrix != '1' and row_matrix != '2'){
                        cout << "Enter only '1' or '2':\n";
                        cin >> row_matrix;
                    }
                    int row_index1;
                    int row_index2;
                    cout << "Enter the first row to swap: ";
                    row_index1 = int_get_valid_input(0, m1m2.matrix1.size() - 1);
                    cout << "Enter the second row to swap: ";
                    row_index2 = int_get_valid_input(0, m1m2.matrix1.size() - 1);
                    if (row_matrix == '1'){
                        swap_matrix_row(m1m2.matrix1, row_index1, row_index2);
                        print_matrix(m1m2.matrix1, m1m2.max_width);
                    } else {
                        swap_matrix_row(m1m2.matrix2, row_index1, row_index2);
                        print_matrix(m1m2.matrix2, m1m2.max_width);
                    }
                    break;
                case '5': // case for swapping columns
                    cout << "Choose which matrix to change (1/2): ";
                    char column_matrix;
                    cin >> column_matrix;
                    while (column_matrix != '1' and column_matrix != '2'){
                        cout << "Enter only '1' or '2':\n";
                        cin >> column_matrix;
                    }
                    int column_index1;
                    int column_index2;
                    cout << "Enter the first column to swap: ";
                    column_index1 = int_get_valid_input(0, m1m2.matrix1.size() - 1);
                    cout << "Enter the second column to swap: ";
                    column_index2 = int_get_valid_input(0, m1m2.matrix1.size() - 1);
                    if (column_matrix == '1'){
                        swap_matrix_column(m1m2.matrix1, column_index1, column_index2);
                        print_matrix(m1m2.matrix1, m1m2.max_width);
                    } else {
                        swap_matrix_column(m1m2.matrix2, column_index1, column_index2);
                        print_matrix(m1m2.matrix2, m1m2.max_width);
                    }
                    break;
                case '6': // case for updating values
                    cout << "Choose which matrix to change (1/2): ";
                    char matrix_to_update;
                    cin >> matrix_to_update;
                    while (matrix_to_update != '1' and matrix_to_update != '2'){
                        cout << "Enter only '1' or '2':\n";
                        cin >> matrix_to_update;
                    }
                    cout << "Enter a row: ";
                    int update_row; 
                    update_row = int_get_valid_input(0, m1m2.matrix1.size() -1);
                    cout << "Enter a column: ";
                    int update_column;
                    update_column = int_get_valid_input(0, m1m2.matrix1.size() -1);
                    cout << "Enter a value: ";
                    int update_value;
                    update_value = int_get_valid_input(INT_MIN, INT_MAX);
                    if (matrix_to_update == '1'){
                        update_matrix(m1m2.matrix1, update_row, update_column, update_value);
                        print_matrix(m1m2.matrix1, m1m2.max_width);
                    } else {
                        update_matrix(m1m2.matrix2, update_row, update_column, update_value);
                        print_matrix(m1m2.matrix2, m1m2.max_width);
                    }
                    break;
                case 'q': // case for user quitting
                    break;
                default: // default if the input was not one of these chars
                    cout << "Only enter values 1-6 or q";
                    break;
            }
        }
    } catch (runtime_error) { // if there is an error it exits, such as not being able to read a file
        cout << "Exiting program...";
    }

    return 0;
}