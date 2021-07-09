#include <stdio.h>
#include <ctype.h>
#define size 6

        /* Function calling*/
void display(char board[][size]);

int valid_moves(char board[][size], int moves[][size], char player);

void make_move(char board[][size], int row, int col, char player);

void computer_move(char board[][size], int moves[][size], char player);

int best_move(char board[][size], int moves[][size], char player);

int get_score(char board[][size], char player);

void main()
{
  char board [size][size] = { 0 };
  int moves[size][size] = { 0 };
  int row = 0;
  int col = 0;
  int no_of_games = 0;
  int no_of_moves = 0;
  int invalid_moves = 0;
  int comp_score = 0;
  int user_score = 0;
  char y = 0;
  int x = 0;
  char again = 0;
  int player = 0;

  printf("\nHere User is White and Computer is Black.\n");
  printf("\nPress Enter to start.\n");
  scanf("%c", &again);
   do
   {
     player = ++no_of_games % 2;
     no_of_moves = 4;                /* Starts with four counters */

     /* Blank all the board squares */
     for(row = 0; row < size; row++)
       for(col = 0; col < size; col++)
         board[row][col] = ' ';

     /* Place the initial four counters in the center */
     board[size/2 - 1][size/2 - 1] = board[size/2][size/2] = 'w';
     board[size/2 - 1][size/2] = board[size/2][size/2 - 1] = 'b';

     /*play loop */
     do
     {
       display(board);
       if(player++ % 2)
       { /* player's turn      */
         if(valid_moves(board, moves, 'w'))
         {
           /* Read player moves until a valid move is entered */
           for(;;)
           {
             fflush(stdin);   /* returns zero  */
             printf("Please enter your move (row column): ");
             scanf("%d%c", &x, &y);              /* Read input        */
             y = tolower(y) - 'a';         /* Convert to column index */
             x--;                          /* Convert to row index    */
             if( x>=0 && y>=0 && x<size && y<size && moves[x][y])
             {
               make_move(board, x, y, 'w');
               no_of_moves++;              /* Increment move count */
               break;
             }
             else
               printf("Invalid move, try again.\n");
           }
         }
         else                     /* No valid moves */
           if(++invalid_moves<2)
           {
             fflush(stdin);
             printf("\nYou have to pass, press return");
             scanf("%c", &again);
           }
           else
             printf("\nNeither of us can go, so the game is over.\n");
       }
       else
       { /* It is the computer's turn                    */
         if(valid_moves(board, moves, 'b')) /* Check for valid moves */
         {
           invalid_moves = 0;               /* Reset invalid count   */
           computer_move(board, moves, 'b');
           no_of_moves++;                   /* Increment move count  */
         }
         else
         {
           if(++invalid_moves<2)
             printf("\nI have to pass, your go\n"); /* No valid move */
           else
             printf("\nNeither of us can go, so the game is over.\n");
         }
       }
     }while(no_of_moves < size*size && invalid_moves<2);

     /* Game is over */
     display(board);  /* Show final board */

     /* Get final scores and display them */
     comp_score = user_score = 0;
     for(row = 0; row < size; row++)
       for(col = 0; col < size; col++)
       {
         comp_score += board[row][col] == 'b';
         user_score += board[row][col] == 'w';
       }
     printf("The final score is:\n");
     printf("Computer %d\n    User %d\n\n", comp_score, user_score);

     fflush(stdin);               /* Flush the input buffer */
     printf("Do you want to play again (y/n): ");
     scanf("%c", &again);         /* Get y or n             */
   }while(tolower(again) == 'y'); /* Go again on y          */

}
  /*game board */
void display(char board[][size])
{
   int row  = 0;
   int col = 0;
   char col_label = 'a';

   printf("\n ");
   for(col = 0 ; col<size ;col++)
     printf("   %c", col_label+col); /* Display the top line */
   printf("\n");                     /* End the top line     */

   /* Display the intermediate rows */
   for(row = 0; row < size; row++)
   {
     printf("  +");
     for(col = 0; col<size; col++)
       printf("---+");
     printf("\n%2d|",row + 1);

     for(col = 0; col<size; col++)
       printf(" %c |", board[row][col]);  /* Display counters in row */
     printf("\n");
   }

   printf("  +");                  /* Start the bottom line   */
   for(col = 0 ; col<size ;col++)
     printf("---+");               /* Display the bottom line */
   printf("\n");                   /* End the bottom  line    */
}

 /*move calculations */
int valid_moves(char board[][size], int moves[][size], char player)
{
   int rowdelta = 0;
   int coldelta = 0;
   int row = 0;
   int col = 0;
   int x = 0;
   int y = 0;
   int no_of_moves = 0;

   /* Set the opponent            */
   char opponent = (player == 'w')? 'b' : 'w';

   /* Initialize moves array to zero */
   for(row = 0; row < size; row++)
     for(col = 0; col < size; col++)
       moves[row][col] = 0;

   /* Find squares for valid moves.                           */
   for(row = 0; row < size; row++)
     for(col = 0; col < size; col++)
     {
       if(board[row][col] != ' ')   /* a blank square?  */
         continue;                  /* No - so on to the next */

       /* Check all the squares around the blank square  */
       /* for the opponents counter                      */
       for(rowdelta = -1; rowdelta <= 1; rowdelta++)
         for(coldelta = -1; coldelta <= 1; coldelta++)
         {
           /* Don't check outside the array, or the current square */
           if(row + rowdelta < 0 || row + rowdelta >= size ||
              col + coldelta < 0 || col + coldelta >= size ||
                                       (rowdelta==0 && coldelta==0))
             continue;

           /* Now check the square */
           if(board[row + rowdelta][col + coldelta] == opponent)
           {
             /* If we find the opponent, move in the delta direction  */
             /* over opponent counters searching for a player counter */
             x = row + rowdelta;                /* Move to          */
             y = col + coldelta;                /* opponent square  */

             /* Look for a player square in the delta direction */
             for(;;)
             {
               x += rowdelta;                  /* Go to next square */
               y += coldelta;                  /* in delta direction*/

               /* If we move outside the array, give up */
               if(x < 0 || x >= size || y < 0 || y >= size)
                 break;

               /* If we find a blank square, give up */
               if(board[x][y] == ' ')
                 break;

               if(board[x][y] == player)
               {
                 moves[row][col] = 1;   /* Mark as valid */
                 no_of_moves++;         /* Increase valid moves count */
                 break;                 /* Go check another square    */
               }
             }
           }
         }
     }
   return no_of_moves;
}

void computer_move(char board[][size], int moves[][size], char player)
{
   int row = 0;
   int col = 0;
   int best_row = 0;
   int best_col = 0;
   int i = 0;
   int j = 0;
   int new_score = 0;
   int score = 100;
   char temp_board[size][size];
   int temp_moves[size][size];
   char opponent = (player == 'w')? 'b' : 'w'; /* Identify opponent */

   /* Go through all valid moves */
   for(row = 0; row < size; row++)
     for(col = 0; col < size; col++)
     {
       if(moves[row][col] == 0)
         continue;

       /* First make copies of the board and moves arrays */
       for(i = 0; i < size; i++)
         for(j = 0; j < size; j++)
           temp_board[i][j] = board[i][j];

       /* Now make this move on the temporary board */
       make_move(temp_board, row, col, player);

       /* find valid moves for the opponent after this move */
       valid_moves(temp_board, temp_moves, opponent);

       /* Now find the score for the opponents best move */
       new_score = best_move(temp_board, temp_moves, opponent);

       if(new_score<score)    /* Is it worse?           */
       {                      /* Yes, so save this move */
         score = new_score;   /* Record new lowest opponent score */
         best_row = row;  /* Record best move row             */
         best_col = col;  /* and column                       */
       }
     }

   /* Make the best move */
   make_move(board, best_row, best_col, player);
}

int get_score(char board[][size], char player)
{
   int score = 0;      /* Score for current position */
   int row = 0;
   int col = 0;
   char opponent = player == 'w' ? 'b' : 'w';

   /* Check all board squares */
   for(row = 0; row < size; row++)
     for(col = 0; col < size; col++)
   {
     score -= board[row][col] == opponent; /* Decrement for opponent */
     score += board[row][col] == player;   /* Increment for player   */
   }
   return score;
}

int best_move(char board[][size], int moves[][size], char player)
{
   int row = 0;
   int col = 0;
   int i = 0;
   int j = 0;

   char opponent = player=='w'?'b':'w';

   char new_board[size][size] = { 0 };
   int score = 0;
   int new_score = 0;

   /* Check all valid moves to find the best */
   for(row = 0 ; row<size ; row++)
     for(col = 0 ; col<size ; col++)
     {
       if(!moves[row][col])             /* Not a valid move?      */
         continue;                      /* Go to the next         */

       /* Copy the board */
       for(i = 0 ; i<size ; i++)
         for(j = 0 ; j<size ; j++)
           new_board[i][j] = board[i][j];

       /* Make move on the board copy */
       make_move(new_board, row, col, player);

       /* Get score for move */
       new_score = get_score(new_board, player);

       if(score<new_score)         /* Is it better?               */
               score = new_score;  /* Yes, save it as best score  */
     }
   return score;
}

void make_move(char board[][size], int row, int col, char player)
{
   int rowdelta = 0;
   int coldelta = 0;
   int x = 0;                          /* for searching    */
   int y = 0;                          /* for searching */
   char opponent = (player == 'w')? 'b' : 'w';

   board[row][col] = player;           /* Place the player counter   */

   /* Check all the squares around this square */
   for(rowdelta = -1; rowdelta <= 1; rowdelta++)
     for(coldelta = -1; coldelta <= 1; coldelta++)
     {

       if(row + rowdelta < 0 || row + rowdelta >= size ||
          col + coldelta < 0 || col + coldelta >= size ||
                               (rowdelta==0 && coldelta== 0))
         continue;

       /* check the square */
       if(board[row + rowdelta][col + coldelta] == opponent)
       {
         /* If we find the opponent, search in the same direction */
         x = row + rowdelta;        /* Move to opponent */
         y = col + coldelta;        /* square           */

         for(;;)
         {
           x += rowdelta;           /* Move to the      */
           y += coldelta;           /* next square      */

           /* If we are off the board give up */
           if(x < 0 || x >= size || y < 0 || y >= size)
             break;

           /* If the square is blank give up */
           if(board[x][y] == ' ')
             break;

           /* If we find the player counter, go backwards from here */
           /* changing all the opponents counters to player         */
           if(board[x][y] == player)
           {
             while(board[x-=rowdelta][y-=coldelta]==opponent) /* Opponent? */
               board[x][y] = player;    /* change it */
             break;                     /* done    */
           }
         }
       }
     }
}
