#include "sudoku.h"
// Daniel Ao, Kevin Hu
// dao3, kwh2
// This program uses functions to check if a new value can be entered into a box in the sudoku array
// The recursive function solve_sudoku calls itself and is_val_valid to solve the array
//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
// This function checks to see if the value in the current box has already existed in the entire array.
// This is done by parsing through rows and checking if the current value already exists
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  int j;
  for(j=0; j<9; j++){
    if(val==sudoku[i][j])
    return 1;
  }

  return 0;
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
// This function checks to see if the value in the current box has already existed in the entire array.
// This is done by parsing through columns and checking if the current value already exists
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
  int i;
  for(i=0; i<9; i++){
    if(val==sudoku[i][j])
    return 1;
  }
  return 0;
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
// This function checks to see if the value in the current box has already existed in its respective 3x3 box
// This is done by parsing through each value in the respective box and checking if the current value already exists
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO

    int zonerow,zonecol;
    zonerow = j/3;
    zonecol = i/3;
    int tempi,tempj;
    for(tempi=zonecol*3;tempi<zonecol*3+3;tempi++){
      for(tempj=zonerow*3;tempj<zonerow*3+3;tempj++){
        if(val==sudoku[tempi][tempj])
          return 1;
      }
    }

  return 0;
  // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
// This function uses is_val_in_row, is_val_in_col, and is_val_in_3x3 to check all cases to see if the value can be put into the box
// This function returns true if and only if none of the past three functions return true
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);

  // BEG TODO
  if(is_val_in_col(val, j, sudoku)==1 || is_val_in_row(val, i, sudoku)==1 || is_val_in_3x3_zone(val, i, j, sudoku)==1)
    return 0;

  return 1;
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
// This recursive function calls itself and is_val_valid to solve the sudoku puzzle
// The flag loop in the beginning is used to stop the function when it runs into an empty cell
int solve_sudoku(int sudoku[9][9]) {
  // BEG TODO.

  int flag = 1;
  int stop = 0;
  int i,j,val,x,y;
  for(x=0;x<9 && flag==1;x++){
    for(y=0;y<9 && flag==1;y++){
      i=x;
      j=y;
      if(sudoku[x][y]==0){
        flag=0;
      }
    }
  }
  if(flag == 1){
    return 1;
  }
  for(val = 1; val <=9; val++){
    if(is_val_valid(val,i,j,sudoku)==1){
      sudoku[i][j]=val;
      if(solve_sudoku(sudoku)==1){
        return 1;
      }
      sudoku[i][j]=0;
    }
  }
  return 0;
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}
