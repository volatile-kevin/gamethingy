#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
/* Kevin Hu - kwh2
*  Partners: Daniel Ao - dao3, Jose Lopez - joseal2
*  This Program solves a maze using recursive backtracking.
*  First we have to read a maze from a file then input it into
*  the maze structure and allocate enough memory for it. We then
*  solve the maze.
*/

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    //Set file pointer to start of file
    FILE* fp = fopen(fileName, "r");

    //Create new maze structure
    maze_t* maze = (maze_t*)malloc(sizeof(maze_t));

    //Scan in height and width of maze and set them in maze structure
    fscanf(fp,"%d %d ",&(maze->height),&(maze->width));

    //Allocate memory for all pointers in cells
    maze->cells = (char**)malloc(sizeof(char*) * maze->height);
    int n;
    for (n=0;n<=maze->height;n++)
      maze->cells[n] = malloc(sizeof(char)*maze->width);

    int i,j; //variables for rows and columns of maze
    char c; //character to detect the newline

    //Parse through file, adding the maze elements into maze->cells array
    for (i=0;i<maze->height;i++)
    {
      for (j = 0;j<=maze->width;j++)
      {
        //Check for '\n' we don't want those in the array
        fscanf(fp,"%c",&c);
        if (c != '\n')
        {
          //Add non-newline char to array
          maze->cells[i][j] = c;

          //If it's the start character, set the start elements in maze
          if (maze->cells[i][j] == START)
          {
            maze->startRow = i;
            maze->startColumn = j;
          }

          //If it's the end character, set the end elements in maze
          else if (maze->cells[i][j] == END)
          {
            maze->endRow = i;
            maze->endColumn = j;
          }
        }
      }
    }
    fclose(fp); //close file

    return maze;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    free(maze); //Free the allocated memory
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
  //Print the maze out, element by element
  int i,j;
  for (i=0;i<=maze->height;i++)
  {
    for (j = 0;j<=maze->width;j++)
    {
      printf("%c",maze->cells[i][j]);
    }
    printf("\n"); //print newline after each row
  }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */
int solveMazeDFS(maze_t * maze, int col, int row)
{
    //Check if col is invalid
    if (col > maze->width || col < 0)
      return 0;

    //Check if row is invalid
    else if (row >= maze->height || row < 0)
      return 0;

    //Check if we are looking at a wall or a path already taken
    else if (maze->cells[row][col] == WALL || maze->cells[row][col] == PATH)
      return 0;

    //If none of these conditions are true and we reached the end, replace the START and return true
    else if (maze->endRow == row && maze->endColumn == col)
    {
      maze->cells[maze->startRow][maze->startColumn] = START;
      return 1;
    }

    //Set the element to a path
    maze->cells[row][col] = PATH;

    if (solveMazeDFS(maze,col,row-1)==1) //solve up
      return 1;
    else if (solveMazeDFS(maze,col,row+1)==1) //solve down
      return 1;
    else if (solveMazeDFS(maze,col-1,row)==1) //solve left
      return 1;
    else if (solveMazeDFS(maze,col+1,row)==1) //solve left
      return 1;

    //Mark the cell as visited
    maze->cells[row][col] = VISITED;

    return 0;
}
