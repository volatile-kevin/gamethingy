/*Kevin Hu
* Partners: Joe Lopez - Joseal2
*            Daniel Ao - dao3
* This program does sparse matrix operations, as well as loading matrices from a file
* it creates a linkedlist of all the non-zero elements in the matrix
*/
#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>

sp_tuples * load_tuples(char* input_file)
{
  //printf("hi\n");
  FILE* fp = fopen(input_file, "r");
  //printf("lo\n");

  sp_tuples* tuple = malloc(sizeof(sp_tuples));
  //printf("bi\n");


  fscanf(fp,"%d %d ",&tuple->m,&tuple->n);
  tuple->nz = 0;
  tuple->tuples_head = NULL;

  double c = 1;
  int row = 0;
  int column = 0;
  int placed = 0;
  int i;

  sp_tuples_node* temp = NULL; //temporary node to be compared to node we are going to insert
  sp_tuples_node* previous = NULL; //the node we previously looked at

  while (fscanf(fp,"%d %d %lf ",&row,&column,&c) != EOF)
  {
    if (c != 0)
    { //If the entry is a non-zero, create a new node and fill it with the values
      sp_tuples_node* node = malloc(sizeof(sp_tuples_node));
      //printf("yo\n");
      node->row = row;
      node->col = column;
      node->value = c;
      node->next = NULL; //We don't know the node it will point to yet

      placed = 0; //Reset placed flag
      previous = NULL; //Reset previous value

      if (tuple->tuples_head != NULL) //If the head is pointing to a node
      {
        temp = tuple->tuples_head; //Currently looking at the first node

        for (i = 0; i<tuple->nz && !placed; i++) //Check each current node while the new one hasn't been placed
        {
          if (node->row < temp->row) // If the new node is the lowest row value
          {
            node->next = temp; //Place it before the temp
            placed = 1;
          }
          else if (node->row == temp->row) //If they are equal in row value
          {
            while (!placed)
            {
              if (node->col < temp->col)
              {
                node->next = temp;      //if the node column is less, put it before temp
                previous->next = node; //point the previous value to it
                placed = 1;
              }
              else if (node->col == temp->col) //If they have the same column, put node's value in temp and free it
              {
                temp->value = node->value;
                placed = 1;
                free(node);
              }
              if (!placed && temp->next != NULL && temp->next->row == temp->row) //If it hasn't been placed and there are nodes with equal rows to check, move onto the next
              {
                previous = temp;
                temp = temp->next;
              }
              else //There are no more equal rows to check, place it after temp
              {
                node->next = temp->next;
                temp->next = node;
                previous = temp; //Need to make sure we don't overwrite temp
                placed = 1;
              }
            }//end while loop
          }//end checks
          else if (temp->next == NULL) //If the next node is NULL, the new node becomes the last
          {
            temp->next = node;
            node->next = NULL;
            previous = temp; //edited
            placed = 1;
          }
          if (!placed) // If it still hasn't been placed, move on to the Daniel Ao -next node to check
          {
            previous = temp; //Make the temporary node the old one
            temp = temp->next; //Increment the node to be compared
          }
          else if (previous == NULL) //If the node is now the first element, set the tuple head to point to it
            tuple->tuples_head = node;
          else
            previous->next = node;
        }//end for loop
      }
      else //If tuple has no head, set the node as the new head
      {
          tuple->tuples_head = node;
          node->next = NULL;
          placed = 1;
      }
    if (placed) //Increase the number of nodes in tuple if the node was placed in the list
      tuple->nz++;
  } else{ //If value is zero we delete it from the linked list
      int i;
      previous = NULL;
      temp = tuple->tuples_head;
      for(i=0; i<tuple->nz; i++){
        if(row==temp->row && column == temp->col){
          if(previous!=NULL)
          previous->next = temp->next;
          free(temp);
          tuple->nz--;
        }
        previous = temp;
        temp = temp->next;
      }
    }
}
  fclose(fp);
  return tuple;
}


/*
*Returns value at a given tuple's row and column
*/
double gv_tuples(sp_tuples * mat_t,int row,int col)
{
    if(mat_t->tuples_head != NULL){
      sp_tuples_node* pointer = mat_t->tuples_head;

      if (row <= mat_t->m && col <= mat_t->n){ //loop through and look for the row/colomn combo
        while(pointer!=NULL){
          if (pointer->row == row && pointer->col == col)
            return pointer->value;
          pointer = pointer->next;
        }
      }
    }
    return 0;
}


/*
* Handles all cases where we need to
*1. Delete a certain node
*2. Insert a certain node
*3. Alter a node
*4. Do something to the beginning of the list
*/
void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
  sp_tuples_node* current = mat_t->tuples_head;
  sp_tuples_node* insert = NULL;
  sp_tuples_node* destroy = NULL;
  while (current != NULL)
  {
    if (current->row == row && current->col == col){
      if (value != 0){ //if row and col already exist, replace value
        current->value = value;
        return;
      }
      else{
        if (destroy == NULL){
          mat_t->tuples_head = current->next;
        }
        else{
          destroy->next = current->next;
        }
        mat_t->nz--;
        free(current);
        return;
      }
    }
    if (current->row < row || (current->row == row && current->col < col)){
      insert = current;
    }
    destroy = current;
    current = current->next;
  }

  if (!value){
    return;
  }
  else{
    sp_tuples_node* node = malloc(sizeof(sp_tuples_node));
    node->row = row;
    node->col = col;
    node->value = value;
    if (insert == NULL){
      node->next = mat_t->tuples_head;
      mat_t->tuples_head = node;
    }
    else{
      node->next = insert->next;
      insert->next = node;
    }
    mat_t->nz++;
    return;
  }
  return;
}


//Saves the tuples into an output file with row and columns of matrix on top
//Then lists all contents of the matrix that aren't zero
void save_tuples(char * file_name, sp_tuples * mat_t)
{
  FILE* fp = fopen(file_name, "w");
  int i;
  fprintf(fp, "%d %d\n", mat_t->m, mat_t->n);
  sp_tuples_node* temp = mat_t->tuples_head; //temp for printing
  for(i=0; i<mat_t->nz;i++){
    fprintf(fp, "%d %d %lf\n", temp->row, temp->col, temp->value);
    if(temp->next!=NULL)
      temp=temp->next;
  }
  fclose(fp);
  //printf("wow\n");
  return;
}


//Add two tuples together using matrix addition
sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){
  if (matA->m != matB->m || matA->n != matB->n)
    return NULL;

  sp_tuples* sum = malloc(sizeof(sp_tuples));
  sum->m=matA->m;
  sum->n=matA->n;
  sum->nz=0;
  sp_tuples_node* tempA = matA->tuples_head; //temp for adding
  sp_tuples_node* tempB = matB->tuples_head;
  //sp_tuples_node* temp1 = NULL; //temp for adding
  while (tempA != NULL && tempB != NULL)
  {
      //if A's row comes before B's row or they have the same row but A's column is earlier
      if (tempA->row < tempB->row || (tempA->row == tempB->row && tempA->col < tempB->col))
      {
        set_tuples(sum,tempA->row,tempA->col,tempA->value);
        tempA = tempA->next;
      }
      //If they have the same row and column
      else if (tempA->row == tempB->row && tempA->col == tempB->col)
      {
        set_tuples(sum,tempA->row,tempB->col,tempA->value + tempB->value);
        tempA = tempA->next;
        tempB = tempB->next;
      }
      else
      {
        set_tuples(sum, tempB->row,tempB->col,tempB->value);
        tempB = tempB->next;
      }
  }
  while (tempA != NULL)
  {
    set_tuples(sum, tempA->row,tempA->col,tempA->value);
    tempA = tempA->next;
  }
  while (tempB != NULL)
  {
    set_tuples(sum, tempB->row,tempB->col,tempB->value);
    tempB = tempB->next;
  }
  return sum;
}

/*
INPUT: matrix A with size mAxnA, and with matrix B size mBxnB       (make sure that inputs are of valid size: n1 == m2)
INITIATE matrix C with the size mAxnB and nz = 0
FOR every non-zero entry in A
    iA = row of current entry in A; jA = column of current entry in A
    FOR every non-zero element in B with row iB = jA                (every element in B where the row is equal to the column of the element in A)
        C_(iA,jB) = C_(iA,jB) + A_(iA,jA) * B_(iB,jB);              (accumulate value into C_(iA,jB))
*/
sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){
  sp_tuples* product = malloc(sizeof(sp_tuples));
  if(matA->n == matB->m){
    product->m = matA->m;
    product->n = matB->n;
    product->nz = 0;
    product->tuples_head = NULL;
    double sum;
    sp_tuples_node* left = matA->tuples_head; //temp for adding
    sp_tuples_node* right = matB->tuples_head; //temp for adding
    while(left != NULL){
      right = matB->tuples_head;
      while (right != NULL){
        if (left->col == right->row){
          sum = gv_tuples(product,left->row,right->col) + left->value * right->value;
          set_tuples(product,left->row,right->col,sum);
        }
        right = right->next;
      }
      left = left->next;
    }
    return product;
  }
  return NULL;

}

//Free all memory we used
void destroy_tuples(sp_tuples * mat_t){
  int i;
  sp_tuples_node* curr = mat_t->tuples_head;
  sp_tuples_node* prev = NULL;
  for(i=0;i<mat_t->nz;i++){
    prev = curr;
    curr = curr->next;
    free(prev);
  }
  free(mat_t);
  return;
}
