/*Jose Lopez - joseal2
*Partners: Daniel Ao - dao3, Kevin Hu - kwh2
*This program updates a gameboard based on Conway's game of life.
*The three functions are called by main.c and provide the status of the board
*and what should be update/changed. The board is represented by an array that
*acts as a 2 dimensional gameboard, with 1's representing live cells and 0's
*representing dead ones.
*/



/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){

	int alive = 0; //start with the number of alive neighbors at 0
	int position = row*boardColSize+col; //mark the position of element in 1D array

	//If this is not the top row, check the three elements above
	if(row-1>=0){
		if(col-1>=0 && board[position-boardColSize-1]==1)
			alive++;
		if(board[position-boardColSize]==1)
			alive++;
		if(col+1 < boardColSize && board[position-boardColSize+1]==1)
			alive++;
	}

	//Check the left and right elements
	if(col-1>=0 && board[position-1]==1)
		alive++;
	if(col+1 < boardColSize && board[position+1]==1)
		alive++;

	//If element is not in bottom row, check the three elements below
	if(row+1<boardRowSize){
		if(col-1>=0 && board[position+boardColSize-1]==1)
			alive++;
		if(board[position+boardColSize]==1)
			alive++;
		if(col+1 < boardColSize && board[position+boardColSize+1]==1)
			alive++;
	}

	return alive;

}
/*
 * Update the game board to the next step.
 * Input:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
	//Create a temporary board to read the past status of the elements
	int temp[boardColSize*boardRowSize];

	int lol; //silly counter for the for first for loop
	int size = boardColSize*boardRowSize; //get the size of the array in 1D

	//copy current board status into temp[]
	for(lol = 0; lol<size; lol++){
		temp[lol]=board[lol];
	}

	//Update the board based on how many alive cells surround each element
	int i;
	for(i = 0; i < size; i++){
		//If element is alive and has 2 or 3 live neighbors, keep it alive
		if(temp[i]==1){
			if(countLiveNeighbor(temp,boardRowSize,boardColSize,i/boardColSize,i%boardColSize)==2 ||
				 countLiveNeighbor(temp,boardRowSize,boardColSize,i/boardColSize,i%boardColSize)==3)
				board[i]= 1;
			else
				board[i]= 0;
		}
		//If element is dead and has 3 live neighbors, bring it back to life
		else{
			if(countLiveNeighbor(temp,boardRowSize,boardColSize,i/boardColSize,i%boardColSize)==3)
				board[i]= 1;
			else
				board[i]= 0;
		}

	}
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */
int aliveStable(int* board, int boardRowSize, int boardColSize){
	int size = boardColSize*boardRowSize; //size of board in 1D
	int flag = 1; //flag gets set to 0 if the board needs to change
	int i;
	//Parse through the game board
	for(i=0;i<size;i++){
		//If element is dead but has three live neighbors, gameboard must be updated
		if(board[i]==0 && countLiveNeighbor(board,boardRowSize,boardColSize,i/boardColSize,i%boardColSize)==3)
			flag = 0;
		//If element is alive and doesn't have 2 or 3 live neighbors, the gameboard must be updated
		if(board[i]==1 && !(countLiveNeighbor(board,boardRowSize,boardColSize,i/boardColSize,i%boardColSize)==2 ||
												countLiveNeighbor(board,boardRowSize,boardColSize,i/boardColSize,i%boardColSize)==3))
			flag = 0;
	}
	return flag;
}
