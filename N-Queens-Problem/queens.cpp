//Clare DuVal
//Lab 08 - N Queens
//CPSC 2121 001

#include <algorithm>
#include <iostream>

using namespace std;

//Global Variables
int num = 0;
//Global arrays are set in respect
//to the max amount of n
int rows [16]; 
int cols [16];
int diag1 [2*16-1];
int diag2 [2*16-1];

//Check row
void check_row(int r, int n) {

  //Base case
  if (r == n) {
    num++;
    return;
  }
 
  //Temp variable to check for half of the board only
  int temp;
  if (r == 0) temp = n/2;  
  else temp = n;
 
  //Permute the column
  for (int col = 0; col < temp; col++) {

    //Only check the permutation if the col, row, and diags are not taken
    if ((cols[col] == 0) && (diag1[r-col+n-1] == 0) && (diag2[r+col] == 0)) {

      //Temporary col, row, and diags
      cols[col] = 1;
      diag1[r-col+n-1] = 1;
      diag2[r+col] = 1;

      //Check the permutation
      check_row(r+1, n);

      //Reset the arrays
      cols[col] = 0;
      diag1[r-col+n-1] = 0;
      diag2[r+col] = 0;
    }
  } 
}

//Main
int main(int argc, char * argv[]) {

  //Check input arguments
  if (argc != 2) {
    cout << "Incorrect amount of arguments." << endl;
    cout << "Please enter ./main + number of queens." << endl;
    return 0;
  }

  //cin >> argv[1];

  if (atoi(argv[1]) > 16) {
   cout << atoi(argv[1]) << endl;

    cout << "Invalid number of queens." << endl;
    cout << "Please enter a number <= 16." << endl;
    return 0;
  }

  //Given the input of n queens, make the arrays
  int n = atoi(argv[1]);
  rows[n];
  cols[n];
  diag1[2*n-1];
  diag2[2*n-1];
  
  //Check for each position n in each column
  check_row(0, n);
  num = num*2;

  //Odd number of queens special case for doubling the amount of solutions
  if (n%2 == 1) {
    cols[n/2] = 1;
    diag1[0-n/2+n-1] = 1;
    diag2[0+n/2] = 1;

    //Check odd row
    check_row(1, n);
  }

  //Output the amount of solutions
  cout << "There are " << num << " solutions for " << n << " queens." << endl; 
  return 0;

}
