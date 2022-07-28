#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Part 1: Functions for setting up the game

void initializeBoard(char board[][26],int n){

    for(int i = 0;i<n;i++){
        for(int j = 0;j<n;j++){
            // Temp Change To Match AI
            if(i==(n/2)-1 && j==(n/2)-1){
                board[i][j]='B';
            }
            else if(i==(n/2) && j==(n/2)-1){
                board[i][j]='W';
            }
            else if(i==(n/2)-1 && j==(n/2)){
                board[i][j]='W';
            }
            else if(i==(n/2) && j==(n/2)){
                board[i][j]='B';
            }
            else{
                board[i][j]='U';
            }
        }
    }  
}

void printBoard(char board[][26], int n) {
    // Column Number
    printf("  ");
    for(int i = 0;i<n;i++){
        printf("%c ",'a'+i);
    }
    printf("\n");
    for(int i = 0;i<n;i++){
        // Row Number
        printf("%c ",'a'+i);
        for(int j = 0;j<n;j++){
            printf("%c ",board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool positionInBounds(int n, int row, int col) {
    if (col<n && 0<=col && 0<=row && row<n)
        return true;
    else
        return false;
}

char oppositeColour(char colour){
    if(colour == 'W')
        return 'B';
    else if(colour == 'B')
        return 'W';
    return 'U';
}

void gameOverHelper(char computer, int Bcount, int Wcount){
    if(computer=='B'){
        printf("MiniMax - %c: %d Pieces\n",computer,Bcount);
        printf("Other Program/Opponent - %c: %d Pieces\n",oppositeColour(computer),Wcount);
    }
    else if(computer=='W'){
        printf("MiniMax - %c: %d Pieces\n",computer,Wcount);
        printf("Other Program/Opponent - %c: %d Pieces\n",oppositeColour(computer),Bcount);
    }
}

bool gameOver(char board[][26],int n, bool invalid_player_move,char computer, bool notificationOn, bool noMovesLeft){
    // Will have to find a way to disable calling game over helper
    if (invalid_player_move==true){
        printf("%c player wins.",computer);
        return true;
    }
    int Ucount=0;
    int Bcount = 0;
    int Wcount = 0;
    for(int row=0;row<n;row++){
        for(int col = 0;col<n;col++){
            if(board[row][col]=='U')
                Ucount++;
            else if(board[row][col]=='B')
                Bcount++;
            else if(board[row][col]=='W')
                Wcount++;
        }
    }
    // Case for no moves left
    if(noMovesLeft==true){
        Ucount=0;
    }
    if(Ucount>0){
        return false;
    }
    else if(Ucount==0 && Bcount>Wcount){
        // Indicate who is who
        if(notificationOn==true){
            printf("\nB player wins!!\n");
            gameOverHelper(computer,Bcount,Wcount); 
        }   
        return true;
    }
    else if(Ucount==0 && Wcount>Bcount){
        // Indicate who is who
        if(notificationOn==true){
            printf("\nW player wins!!\n");
            gameOverHelper(computer,Bcount,Wcount);
        }
        return true;
    }
    else if(Ucount == 0 && Wcount==Bcount){
        if(notificationOn==true){
            printf("\nDraw!!\n");
            // Indicate who is who
            gameOverHelper(computer,Bcount,Wcount);    
        }
        return true;
    }
    // Indicate who is who
    
    return false;
}

// Part 2: Functions for checking legality

bool checkLegalInDirection(char board[][26], int n, int row, int col,char colour, int deltaRow, int deltaCol){
    //Check if initial position is legal
    if(positionInBounds(n,row,col)==false)
        return false;
    //Check if first piece is opposite colour
    if(board[row+deltaRow][col+deltaCol]==oppositeColour(colour)){
        // Checks if pieces after first piece are valid
        for(int i = 2;positionInBounds(n,row+i*deltaRow,col+i*deltaCol);i++){
            if(board[row+i*deltaRow][col+i*deltaCol]==colour)
                return true;
            else if(board[row+i*deltaRow][col+i*deltaCol]=='U')
                return false;
        }
    }
    return false;
}

void flipInDirection(char board[][26], int row, int col,char colour, int deltaRow, int deltaCol){
    board[row][col]=colour;
    for(int i=1;board[row+i*deltaRow][col+i*deltaCol]==oppositeColour(colour);i++){
        board[row+i*deltaRow][col+i*deltaCol]=colour;
    }
}

int CheckLegalAndScore(char board[][26],int n,int row,int col,char colour){
    // NOTE: When calling this function make sure to add the condition if the spot
    // is occupied by U, otherwise it will glitch out
    int positionScore=0;
    for(int deltaRow = -1;deltaRow<=1;deltaRow++){
        for(int deltaCol = -1;deltaCol<=1;deltaCol++){
            
            // Variables defined inside loop
            int directionScore=0;
            bool directionValid = false;

            //Check if first piece is opposite colour
            if(board[row+deltaRow][col+deltaCol]==oppositeColour(colour)){
                // Check legal in direction process
                if(positionInBounds(n,row+deltaRow,col+deltaCol)==false)
                    break;
                // Checks if pieces after first piece are valid
                directionScore++;
                for(int i = 2;positionInBounds(n,row+i*deltaRow,col+i*deltaCol);i++){
                    if(board[row+i*deltaRow][col+i*deltaCol]==colour){
                        directionValid = true;
                        break;
                    }
                    else if(board[row+i*deltaRow][col+i*deltaCol]==oppositeColour(colour))
                        directionScore++;
                    else if(board[row+i*deltaRow][col+i*deltaCol]=='U'){
                        directionValid=false;
                        break;
                    }
                }
            }
            if(directionValid==true){
                positionScore=positionScore+directionScore;
            }
        }    
    }
    if(positionScore!=0)
        return positionScore+1;
    else
        return positionScore;
}

void PlacePiece(char board[][26],int n,int row,int col,char colour){
    for(int rowDir=-1;rowDir<=1;rowDir++){
        for(int colDir=-1;colDir<=1;colDir++){
            if (checkLegalInDirection(board,n,row,col,colour,rowDir,colDir)==true){
                flipInDirection(board,row,col,colour,rowDir,colDir);
            }
        }
    }
}

int isThereValidMove(char board[][26],int n,char colour){
    int doesColourHaveMoves = 0;
    for(int i = 0;i<n;i++){
        for(int j = 0;j<n;j++){
            if(board[i][j]=='U'){
                doesColourHaveMoves += CheckLegalAndScore(board,n,i,j,colour);
            }
        }
    }
    return doesColourHaveMoves;
}

// Part 3: Function for player input

bool userMove(char board[][26],int n,char colour){
    int highScore=0;
    for(int row = 0;row<n;row++){
        for(int col = 0;col<n;col++){
            if(board[row][col]=='U'){
                int positionScore = CheckLegalAndScore(board,n,row,col,colour);
                if(positionScore>highScore){
                    highScore=positionScore;
                } 
            }
            
        }
    }
    if(highScore==0){
        printf("%c player has no valid move.\n",colour);
        return false;
    }
    printf("Enter move for colour %c (RowCol): ",colour);
    char input[3];
    int row;
    int col;
    scanf("%s",input);
    row = input[0]-'a';
    col = input[1]-'a';

    if(board[row][col]!='U'){
        printf("Invalid move.\n");
        return true;
    }

    bool valid_move = false;
    for(int deltaRow = -1;deltaRow<=1;deltaRow++){
        for(int deltaCol = -1;deltaCol<=1;deltaCol++){
            if(checkLegalInDirection(board, n, row, col,colour, deltaRow, deltaCol)==true){
                board[row][col]=colour;
                valid_move = true;
            }
        }
    }
    PlacePiece(board,n,row,col,colour);

    if(valid_move==true){
        board[row][col]=colour;
        return false;
    }
    if(valid_move==false){
        printf("Invalid move.\n");
        return true;
    }

    return false;
}

// Part 4: Scoring system for my AI (Will have to implement new scoring system)

int edgeScore(int n, int row, int col){
    // Corner Piece
    if(((row == n-1)||(row == 0)) && ((col == n-1)||(col==0))){
        return 2;
    }
    // Edge Piece
    else if((row == n-1)||(row == 0) || (col == n-1)||(col==0)){
        return 1;
    }
    return 0;
}

int dangerZone(int n, int row, int col){
    if((row==1||row==n-2)&&(col==1||col==n-2))
        return 2;
    else if(row==1||row==n-2||col==1||col==n-2)
        return 1;
    return 0;
}

int innerRing(int n, int row, int col){
    if( (row==(n/2)-2||row==(n/2)+1) && (col==(n/2)-2||col==(n/2)+1) )
        return 2;
    else if( row==(n/2)-2||row==(n/2)+1 || col==(n/2)-2||col==(n/2)+1 )
        return 1;
    return 0;
}

int opponentRange(char board[][26],char turn,int n,int row,int col){
    
    // Pre move
    int preMovePoints = 0;
    int temp = 0;
    for(int row = 0;row<n;row++){
        for(int col = 0;col<n;col++){
            if(board[row][col]=='U'){
                temp = CheckLegalAndScore(board,n,row,col,oppositeColour(turn));
                preMovePoints += temp;
            }
        }
    }
    
    // Make a copy of board
    char copyBoard[26][26];
    for(int i=0;i<n;i++){
        for(int j = 0;j<n;j++){
            copyBoard[i][j]=board[i][j];
        }
    }
    // Place piece for current turn at position in copy
    PlacePiece(copyBoard,n,row,col,turn);
    // Use opponent range measure 
    int postMovePoints=0;
    temp=0;
    for(int row = 0;row<n;row++){
        for(int col = 0;col<n;col++){
            if(copyBoard[row][col]=='U'){
                temp = CheckLegalAndScore(copyBoard,n,row,col,oppositeColour(turn));
                postMovePoints = postMovePoints + temp +0.5*edgeScore(n,row,col);
            }
        }
    }
    return preMovePoints - postMovePoints;
}

float instScore(char board[][26],char turn,int n){
    float scorekeep=0;
    for(int row = 0;row<n;row++){
        for(int col = 0;col<n;col++){
            if(board[row][col]==turn){
                int cornerScore = edgeScore(n,row,col);
                int dangerZoneScore = dangerZone(n,row,col);
                int innerSpots = innerRing(n,row,col);
                //scorekeep += 1 + (0.75*cornerScore)-(0.75*dangerZoneScore)+(innerSpots/5.0); //ORIGINAL DON'T DELETE
                scorekeep += 1 + (1*cornerScore)-(0.75*dangerZoneScore)+(innerSpots/5.0);
            }
        }
    }
    return scorekeep;
}

float heurScore(char board[][26],char turn,int n){
    float playerScore = instScore(board,turn,n);
    float opponentScore = instScore(board,oppositeColour(turn),n);
    return playerScore - opponentScore;
}

// Part 5: Use Mini-Max Algorithm

float makeMoveMK2(char board[][26], int n, char turn, int *row, int *col) {
    float highScore=0;
    int rowIndex=0;
    int colIndex=0;
    for(int row = 0;row<n;row++){
        for(int col = 0; col<n;col++){
            if(board[row][col]=='U'){
                float positionScore;
                // Check for number of flips
                int flipScore = CheckLegalAndScore(board,n,row,col,turn);
                // Add points if corner or edge case
                int cornerScore = edgeScore(n,row,col);
                // Check opponent moves 
                int opponentMovesEliminated = opponentRange(board,turn,n,row,col);
                // Check danger zone
                int dangerZoneScore = dangerZone(n,row,col);
                // Desirable inner ring
                int innerSpots = innerRing(n,row,col);

                // AI Calculation For Score
                positionScore = (2*flipScore) + (0.75*cornerScore)-(0.75*dangerZoneScore)+(innerSpots/5.0)+(opponentMovesEliminated/20.0);// /30.0
                // Algorithm for returning high score
                if(positionScore>highScore){
                    highScore=positionScore;
                    //highScore = flipScore;
                    rowIndex = row;
                    colIndex = col;
                }
            }
        }
    }

    // End of function
    *row = rowIndex;
    *col = colIndex;
    return highScore;
}

float miniMaxValue(char board[][26],int depth,char originalTurn, char currentTurn, int n, float alpha, float beta){
    // Base Case
    if(depth==0||gameOver(board,n,false,currentTurn,false,false)==true){
        float value = heurScore(board,originalTurn,n);
        return value;
    }

    // Maximizing Scenario
    if(currentTurn == originalTurn){
        float maxEva = -9999.0;
        float alphaTrack = alpha;
        // Is the check legal and score messing it up? There should be an if statement before this in the event of no moves
        if(isThereValidMove(board,n,currentTurn)==0){
            // Do recursive step
            float eva = miniMaxValue(board,depth-1,originalTurn,oppositeColour(currentTurn),n,alphaTrack,beta);
            if(eva>maxEva){
                maxEva = eva;
            }
        }
        else{
            for(int row=0;row<n;row++){
                for(int col=0;col<n;col++){
                    if(board[row][col]=='U'&&CheckLegalAndScore(board,n,row,col,currentTurn)!=0){
                        // Make copy of board
                        char tempBoard[26][26];
                        memcpy(tempBoard,board,26*26*sizeof(char));
                        PlacePiece(tempBoard,n,row,col,currentTurn);
                        // Do recursive step
                        float eva = miniMaxValue(tempBoard,depth-1,originalTurn,oppositeColour(currentTurn),n,alphaTrack,beta);
                        if(eva>maxEva){
                            maxEva = eva;
                        }
                        if(eva>alphaTrack){
                            alphaTrack=eva;
                        }
                        if(beta<=alphaTrack){
                            break;
                        }
                    }
                }
                if(beta<=alphaTrack){
                    break;
                }
            }  
        }
        return maxEva;
    }

    // Minimizing Scenario
    if(currentTurn == oppositeColour(originalTurn)){
        //printf("Testing Minimizing Case\n");
        float minEva = 9999.0;
        float betaTrack = beta;
        if(isThereValidMove(board,n,currentTurn)==0){
            // Do recursive step
            float eva = miniMaxValue(board,depth-1,originalTurn,oppositeColour(currentTurn),n,alpha,betaTrack);
            if(eva<minEva){
                minEva = eva;
            }
        }
        else{
            for(int row=0;row<n;row++){
                for(int col=0;col<n;col++){
                    if(board[row][col]=='U'&&CheckLegalAndScore(board,n,row,col,currentTurn)!=0){
                        // Make copy of board
                        char tempBoard[26][26];
                        memcpy(tempBoard,board,26*26*sizeof(char));
                        PlacePiece(tempBoard,n,row,col,currentTurn);
                        // Do recursive step
                        float eva = miniMaxValue(tempBoard,depth-1,originalTurn,oppositeColour(currentTurn),n,alpha,betaTrack);
                        if(eva<minEva){
                            minEva = eva;
                        }
                        if(eva<betaTrack){
                            betaTrack=eva;
                        }
                        if(betaTrack<=alpha){
                            break;
                        }
                    }
                }
                if(betaTrack<=alpha){
                    break;
                }
            }
        }
        return minEva;
    }
    return 0;
}

float miniMaxAI(char board[][26],int depth,char originalTurn, char currentTurn, int *x,int *y,int n){
    // Base Case
    if(depth==0||gameOver(board,n,false,currentTurn,false,false)==true){
        *x=*x;
        *y=*y;
        float value = heurScore(board,originalTurn,n);
        return value;
    }

    // Maximizing Scenario
    if(currentTurn == originalTurn){
        float maxEva = -9999.0;
        int xindex,yindex;
        for(int row=0;row<n;row++){
            for(int col=0;col<n;col++){
                if(board[row][col]=='U'&&CheckLegalAndScore(board,n,row,col,currentTurn)!=0){
                    // Make copy of board
                    char tempBoard[26][26];
                    memcpy(tempBoard,board,26*26*sizeof(char));
                    if(isThereValidMove(tempBoard,n,currentTurn)!=0){
                        PlacePiece(tempBoard,n,row,col,currentTurn);
                    }  
                    // Do recursive step
                    float eva = miniMaxValue(tempBoard,depth-1,originalTurn,oppositeColour(currentTurn),n,-9999.0,9999.0);
                    if(eva>maxEva){
                        maxEva = eva;
                        xindex = row;
                        yindex = col;
                    }
                }
            }
        }
        *x = xindex, *y = yindex;
        return maxEva;
    }

    return 0;
}

// Part 6: Function for running the game

void runGame(char DANISH,int depth){
    // Temporarily blocking print statements
    char board[26][26];
    int n = 8;
    initializeBoard(board,n);

    char computer = DANISH;
    char player = oppositeColour(computer);
    printf("Original Board\n");
    printBoard(board,n);

    bool game_over = false;
    char turn = 'B';
    bool invalid_player_move = false;

    while(game_over==false){

        // Should have condition here that checks if both players don't have a move anymore, then ends the game
        int doesComputerHaveMoves = isThereValidMove(board,n,computer);
        int doesPlayerHaveMoves = isThereValidMove(board,n,player);
        bool noMovesInGame = false;
        if(turn == computer){
            // Computer makes a move
            int row,col;
            // Check if there is valid move
            float score;
            score=miniMaxAI(board,depth,computer,computer,&row,&col,n);
            if(doesComputerHaveMoves!=0){
                PlacePiece(board,n,row,col,computer);
                printf("MiniMax places %c at %c%c. SCORE: %lf\n",turn,'a'+row,'a'+col,score);
                printBoard(board,n); 
            }
            else{
                printf("Minimax has no valid move.\n");
            }
            
        }
        // Where is the invalid player move?
        else if(turn == player){
            // Human makes a move
            invalid_player_move = userMove(board,n,player);
            if (invalid_player_move==false){
                printBoard(board,n);
            }
        }
        if(doesComputerHaveMoves==0&&doesPlayerHaveMoves==0){
            noMovesInGame = true;
        }
        game_over=gameOver(board,n,invalid_player_move,computer,true,noMovesInGame);
        turn = oppositeColour(turn);
    }
}

// The main function

int main(void){
    printf("Computer plays (B/W): ");
    char input[2];
    scanf("%s",input);
    char computer = input[0];

    printf("\nThere are three difficulties the computer plays at:\n 1 - Easy \n 2 - Medium \n 3 - Hard\n\n");
    int difficulty;
    do{
        printf("Enter your difficulty: ");
        scanf("%d",&difficulty);
    }while(difficulty>3 || difficulty<1);

    printf("\nYou chose %d/3 as your difficulty and the computer plays as %c.\n\n",difficulty,computer);

    int depth;
    if(difficulty == 1) depth = 3;
    else if(difficulty == 2) depth = 4;
    else if(difficulty==3) depth = 5;

    runGame(computer,depth);
}
