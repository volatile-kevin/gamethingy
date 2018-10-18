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
	int alive = 0;
	int position = row*boardColSize+col;

	if(row-1>=0){
		if(col-1>=0 && board[position-boardColSize-1]==1)
			alive++;
		if(board[position-boardColSize]==1)
			alive++;
		if(col+1 < boardColSize && board[position-boardColSize+1]==1)
			alive++;
	}

	if(col-1>=0 && board[position-1]==1)
		alive++;
	if(col+1 < boardColSize && board[position+1]==1)
		alive++;

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
	int temp[boardColSize*boardRowSize];
	int lol;
	int size = boardColSize*boardRowSize;
	for(lol = 0; lol<size; lol++){
		temp[lol]=board[lol];
	}
	int i;
	for(i = 0; i < size; i++){
		if(temp[i]==1){
			if(countLiveNeighbor(temp,boardRowSize,boardColSize,i/boardColSize,i%boardColSize)==2 || countLiveNeighbor(temp,boardRowSize,boardColSize,i/boardColSize,i%boardColSize)==3)
				board[i]= 1;
			else
				board[i]= 0;
		}else{
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
	int size = boardColSize*boardRowSize;
	int flag = 1;
	int i;
	for(i=0;i<size;i++){
		if(board[i]==0 && countLiveNeighbor(board,boardRowSize,boardColSize,i/boardColSize,i%boardColSize)==3)
			flag = 0;
		if(board[i]==1 && !(countLiveNeighbor(board,boardRowSize,boardColSize,i/boardColSize,i%boardColSize)==2 || countLiveNeighbor(board,boardRowSize,boardColSize,i/boardColSize,i%boardColSize)==3))
			flag = 0;
	}
	return flag;
}
