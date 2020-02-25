#include "game.h"

game * make_game()
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure)
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    int rows = 25;
    int cols = 25;
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //Set row, column, and scores of the new game structure to values passed by user
    mygame->rows = rows;
    mygame->cols = cols;
    mygame->score = 0;

    int i; //Set each cell in the game to "-1"
    for (i = 0;i < rows*cols;i++)
    {
      mygame->cells[i] = -1;
    }

    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any
	memory previously allocated is not lost in this function.
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

  (*_cur_game_ptr)->rows = new_rows;
  (*_cur_game_ptr)->cols = new_cols;
  (*_cur_game_ptr)->score = 0;

  int i; //Set each cell in the game to "-1"
  for (i = 0;i < new_rows*new_cols;i++)
  {
    (*_cur_game_ptr)->cells[i] = -1;
  }

	return;
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    int position = row*(cur_game->cols) + col; //multiply rows by # of columns and add cols
    return &(cur_game->cells[position]); //return pointer to cell
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can
   not merge twice in one turn. If sliding the tiles up does not cause any
   cell to change value, w is an invalid move and return 0. Otherwise, return 1.
*/
{
    int N,M,i,move, likeFound, notLikeFound; //column, row, and counter for for loops
    move = 0; //variable that states wether a move has been made (move = 1)
    likeFound = 0; //variable to flag when a like cell is found
    notLikeFound = 0; //variable to flag when a cell that is different is found

    for (N=0;N < cur_game->cols;N++)
    {
      for (M=0;M < cur_game->rows;M++)
      {
        if (*(get_cell(cur_game,M,N)) != -1) //if you find a block that isn't empty
        {
          //First check for like cells below
          i = M + 1;
          //Check all rows below for a like cell or an unlike, occupied cell
          while (i < cur_game->rows && !likeFound && !notLikeFound)
          {
            //If an unlike,occupied cell is found, set the flag
            if (*(get_cell(cur_game,i,N)) != -1 &&
                *(get_cell(cur_game,i,N)) != *(get_cell(cur_game,M,N)))
              notLikeFound = 1;

            //If one is found, combine the cells
            else if (*(get_cell(cur_game,M,N)) == *(get_cell(cur_game,i,N)))
            {
              //Combine the two cells
              *(get_cell(cur_game,M,N)) = *(get_cell(cur_game,M,N)) + *(get_cell(cur_game,i,N));
              //Increment score
              cur_game->score += *(get_cell(cur_game,M,N));

              //Make the lower cell == -1
              *(get_cell(cur_game,i,N)) = -1;
              move = 1;
              likeFound = 1;
            }
            i++; //Keep on checking boi
          }
          i = 0; //Reset i, likeFound, and notLikeFound values
          likeFound = 0;
          notLikeFound = 0;

          //Now check the ones above it
          for (i=0; i < M; i++)
          {
            if (*(get_cell(cur_game,i,N)) == -1) //If it's empty above, slide up
            {
              *(get_cell(cur_game,i,N)) = *(get_cell(cur_game,M,N));
              *(get_cell(cur_game,M,N)) = -1;
              move = 1;
            }
          }
        }
      }
    }

    return move;
};

int move_a(game * cur_game) //slide left
{
  int N,M,i,move, likeFound, notLikeFound; //column, row, and counter for for loops
  move = 0;
  likeFound = 0; //variable to flag when a like cell is found
  notLikeFound = 0; //variable to flag when a cell that is different is found

  for (M=0;M < cur_game->rows;M++)
  {
    for (N=0;N < cur_game->cols;N++)
    {
      if (*(get_cell(cur_game,M,N)) != -1) //if you find a block that isn't empty
      {
        //First check for like cells to the right
        i = N + 1;
        //Check all rows to right for a like cell or an unlike, occupied cell
        while (i < cur_game->cols && !likeFound && !notLikeFound)
        {
          //If an unlike,occupied cell is found, set the flag
          if (*(get_cell(cur_game,M,i)) != -1 &&
              *(get_cell(cur_game,M,i)) != *(get_cell(cur_game,M,N)))
            notLikeFound = 1;

          //If one is found, combine the cells
          else if (*(get_cell(cur_game,M,N)) == *(get_cell(cur_game,M,i)))
          {
            //Combine the two cells
            *(get_cell(cur_game,M,N)) = *(get_cell(cur_game,M,N)) + *(get_cell(cur_game,M,i));
            //Increment score
            cur_game->score += *(get_cell(cur_game,M,N));

            //Make the right cell == -1
            *(get_cell(cur_game,M,i)) = -1;
            move = 1;
            likeFound = 1;
          }
          i++; //Keep on checking boi
        }
        i = 0; //Reset i, likeFound, and notLikeFound values
        likeFound = 0;
        notLikeFound = 0;

        //Now check the ones to the left of it
        for (i=0; i < N; i++)
        {
          if (*(get_cell(cur_game,M,i)) == -1) //If it's empty above, slide left
          {
            *(get_cell(cur_game,M,i)) = *(get_cell(cur_game,M,N));
            *(get_cell(cur_game,M,N)) = -1;
            move = 1;
          }
        }
      }
    }
  }

    return move;
};

int move_s(game * cur_game) //slide down
{
  int N,M,i,move, likeFound, notLikeFound; //column, row, and counter for for loops
  move = 0;
  likeFound = 0; //variable to flag when a like cell is found
  notLikeFound = 0; //variable to flag when a cell that is different is found

  for (N=0; N<cur_game->cols; N++)
  {
    for (M = cur_game->rows-1; M>=0; M--)
    {
      if (*(get_cell(cur_game,M,N)) != -1) //if you find a block that isn't empty
      {
        //First check for like cells above
        i = M-1;
        //Check all rows above for a like cell or an unlike,occupied cell
        while (i >= 0 && !likeFound && !notLikeFound)
        {
          //If an unlike,occupied cell is found, set the flag
          if (*(get_cell(cur_game,i,N)) != -1 &&
              *(get_cell(cur_game,i,N)) != *(get_cell(cur_game,M,N)))
            notLikeFound = 1;

          //If one is found, combine the cells
          if (*(get_cell(cur_game,M,N)) == *(get_cell(cur_game,i,N)))
          {
            //Combine the two cells
            *(get_cell(cur_game,M,N)) = *(get_cell(cur_game,M,N)) + *(get_cell(cur_game,i,N));
            //Increment score
            cur_game->score += *(get_cell(cur_game,M,N));

            //Make the higher cell == -1
            *(get_cell(cur_game,i,N)) = -1;
            move = 1;
            likeFound = 1;
          }
          i--; //Keep on checking boi
        }
        i = 0; //Reset i, likeFound, and notLikeFound values
        likeFound = 0;
        notLikeFound = 0;

        //Now check the ones below it
        for (i=cur_game->rows-1; i > M; i--)
        {
          if (*(get_cell(cur_game,i,N)) == -1) //If it's empty above, slide up
          {
            *(get_cell(cur_game,i,N)) = *(get_cell(cur_game,M,N));
            *(get_cell(cur_game,M,N)) = -1;
            move = 1;
          }
        }
      }
    }
  }

    return move;
};

int move_d(game * cur_game) //slide to the right
{
  int N,M,i,move, likeFound, notLikeFound; //column, row, and counter for for loops
  move = 0;
  likeFound = 0; //variable to flag when a like cell is found
  notLikeFound = 0; //variable to flag when a cell that is different is found

  for (M = cur_game->rows-1; M>=0; M--)
  {
    for (N = cur_game->cols-1; N>=0; N--)
    {
      if (*(get_cell(cur_game,M,N)) != -1) //if you find a block that isn't empty
      {
        //First check for like cells to the left
        i = N-1;
        //Check all rows to left for a like cell or an unlike,occupied cell
        while (i >= 0 && !likeFound && !notLikeFound)
        {
          //If an unlike,occupied cell is found, set the flag
          if (*(get_cell(cur_game,M,i)) != -1 &&
              *(get_cell(cur_game,M,i)) != *(get_cell(cur_game,M,N)))
            notLikeFound = 1;

          //If one is found, combine the cells
          if (*(get_cell(cur_game,M,N)) == *(get_cell(cur_game,M,i)))
          {
            //Combine the two cells
            *(get_cell(cur_game,M,N)) = *(get_cell(cur_game,M,N)) + *(get_cell(cur_game,M,i));
            //Increment score
            cur_game->score += *(get_cell(cur_game,M,N));

            //Make the left cell == -1
            *(get_cell(cur_game,M,i)) = -1;
            move = 1;
            likeFound = 1;
          }
          i--; //Keep on checking boi
        }
        i = 0; //Reset i, likeFound, and notLikeFound values
        likeFound = 0;
        notLikeFound = 0;

        //Now check the ones to the right
        for (i=cur_game->cols-1; i > N; i--)
        {
          if (*(get_cell(cur_game,M,i)) == -1) //If it's empty above, slide right
          {
            *(get_cell(cur_game,M,i)) = *(get_cell(cur_game,M,N));
            *(get_cell(cur_game,M,N)) = -1;
            move = 1;
          }
        }
      }
    }
  }
    return move;
};

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
  int M,N;
  for (N=0; N<cur_game->cols; N++)
  {
    for (M=0; M<cur_game->rows; M++)
    {
      if (*(get_cell(cur_game,M,N)) == -1) //Check for empty spaces
        return 1;
      //Check for like cells above
      else if (M-1 >= 0 &&
              *(get_cell(cur_game,M,N)) == *(get_cell(cur_game,M-1,N)))
        return 1;
      //Check for like cells below
      else if (M+1 < cur_game->rows &&
              *(get_cell(cur_game,M,N)) == *(get_cell(cur_game,M+1,N)))
        return 1;
      //Check for like cells to left
      else if (N-1 >= 0 &&
              *(get_cell(cur_game,M,N)) == *(get_cell(cur_game,M,N-1)))
        return 1;
      //Check for like cells to right
      else if (N+1 < cur_game->cols &&
              *(get_cell(cur_game,M,N)) == *(get_cell(cur_game,M,N+1)))
        return 1;

    }
  }

  return 0;
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{

	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    if (cell_ptr == NULL){
        printf("Bad Cell Pointer.\n");
        exit(0);
    }


	//check for an empty cell
	int emptycheck = 0;
	int i;

	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}

    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game)
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;

	printf("\n\n\nscore:%d\n",cur_game->score);


	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char


    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  ");
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }

	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char

    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;

    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}

		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		}

		remake_game(&cur_game,rows,cols);

		move_success = 1;

    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }




    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game);
		 print_game(cur_game);
    }

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
