#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>

char board[9] = {'#', '#', '#','#', '#', '#','#', '#', '#'};
const char * username;

char hasWon();
void printBoard();
int playerInput();
int aiInput();
int spaceAvailable(int p);
void updateBoard(int playerID, int p);
int checkFinished();
const char * getUserName();
int minimax(char player);
void winResponse(char c);

void printBoard(){
    int i, j;
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            printf("%c",board[i*3+j]);
            if(j < 2) printf(" | ");
        }
        if(i < 2) printf("\n--+---+--\n");
    }
    printf("\n");
}

int playerInput() {
    char c[6], d[2]; int p = 0, s;
    printf("Player Move\nEnter an integer between 1-9: ");
    while (p < 1 || p > 9) {
        gets(c);
        printf("Are you sure you want to place your token at position ");
        printf(c); printf("? [Y/N]\n");
        scanf("%c",&d);

        if(c != 'Y' || c != 'y') continue;

        p = atoi(c);
        if(p > 0 && p < 10) {
	    s = spaceAvailable(p);
            if(s == 0) {
                p = 0;
                printf("The space is filled. Choose another space.\nEnter an integer between 1-9:\n");
            }
        } else printf("Invalid Input. Enter an integer between 1-9:\n");
    }

    return p;
}

int aiInput() {
    int move = -1, score = -2, i;
    for(i = 0; i < 9; i++) {
        if(spaceAvailable(i+1)) {
            board[i] = 'O';
            int tempScore = -minimax('X');
            board[i] = '#';
            if(tempScore > score) {
                score = tempScore;
                move = i;
            }
        }
    }
    printf("AI input: %d\n", move+1);
    return move+1;
}


int minimax(char player) {
    char c = hasWon();
    if (c == '#') return 0;
    if (c) return ((c == 'O') ? 1 : -1)*((player == 'O') ? 1 : -1);

    int move = -1, score = -2, i;
    for(i = 0; i < 9; i++) {
        if(spaceAvailable(i+1)) {
            board[i] = player;
            int thisScore = -minimax((player == 'O') ? 'X' : 'O');
            if(thisScore > score) {
                score = thisScore;
                move = i;
            }
            board[i] = '#';
        }
    }
    if(move == -1) return 0;
    return score;
}

int spaceAvailable(int p) {
    if(board[p-1] == '#') return 1;
    return 0;
}

void updateBoard(int playerID, int p){
    board[p-1] = (playerID) ? 'O' : 'X';
}

char hasWon() {
    int i, j, lines[8]={0}, filled = 1;
    char c;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            if(board[i*3] != board[i*3+j]) lines[i] = 1;
            if(board[i] != board[j*3+i]) lines[i+3] = 1;
            if(board[i*3+j] == '#') filled = 0;
        }
        if(board[0] != board[i*3+i]) lines[6] = 1;
        if(board[2*3] != board[(2-i)*3+i]) lines[7] = 1;
    }

    for(i = 0; i < 8; i++) {
        if(lines[i] == 0) {
            if(i < 3) c = board[i*3];
            else if (i < 6) c = board[i-3];
            else c = board[4];
            if(c != '#') {
                return c;
            }
        }
    }

    if(filled) return '#';
    return 0;
}

int checkFinsihed() {
    char c = hasWon();

    winResponse(char c);
    return (c != 0);
}

void winResponse(char c) {
    if(c == 'O') printf("\nYou are not yet worthy to join the CoolerKidzClub.\n");
    else if (c == 'X') {
        char line[64];
        FILE * fp;
        int alreadyIn = 0, len;
        printf("\nWow, you actually won. Adding you to the CoolerKidzClub...\n");
        fp = fopen("/home/kali/CoolerKidzClub.txt", "a+");
        fgets(line, sizeof(line), fp);
        while (fgets(line, sizeof(line), fp)){
            len = strlen(line);
            if(line[len-1] == '\n') line[len-1] = 0;
            if(strcmp(username, line) == 0) {
                printf("You are already in the CoolerKidzClub.\n");
                alreadyIn = 1;
                break;
            }
        }
        if(!alreadyIn) fprintf(fp, "%s\n", username);
        printf("Done.\n");
        fclose(fp);
    }
    else if (c == '#') printf("\nIt appears that the Champion is stil unbeaten. Better luck next time.\n");
}

const char * getUserName(){
    uid_t uid = getuid();  
    struct passwd *pw = getpwuid(uid);
    if (pw) return pw->pw_name;
    return "";
}

int main() {
    int isFinished = 0, playerID = 0;
    username = getUserName();
    printf("Hey %s,\n", username);
    printf("Can you beat the unbeatable TicTacToe Champion and join the \"CoolerKidzClub\"?\n\nMake sure to thik twice before making a move this time.\n");
    printBoard();
    while (isFinished == 0){	
        updateBoard(playerID, (playerID) ? aiInput() : playerInput());
        printBoard();
        playerID = (playerID ^ 1) & 1;
        isFinished = checkFinsihed();
    }
    return 0;
}
