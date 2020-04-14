#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>

char board[3][3] = {{'#', '#', '#'},{'#', '#', '#'},{'#', '#', '#'}};
const char * username;

void validateWin(char c);
void printBoard();
int playerInput();
int aiInput();
int spaceAvailable(int p);
void updateBoard(int playerID, int p);
int checkFinished();
void aiWins();
void playerWins();
const char * getUserName();

void printBoard(){
    int i, j;
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            printf("%c",board[i][j]);
            if(j < 2) printf(" | ");
        }
        if(i < 2) printf("\n--+---+--\n");
    }
    printf("\n");
}

int playerInput() {
    char c[8]; int p = 0, s;
    printf("Player Move\nEnter an integer between 1-9:\n");
    while (p < 1 || p > 9){
        gets(c);
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

int aiInput(){

}

int spaceAvailable(int p){
    if(board[(p-1)/3][(p-1)%3] == '#') return 1;
    return 0;
}

void updateBoard(int playerID, int p){
    board[(p-1)/3][(p-1)%3] = (playerID) ? 'O' : 'X';
}

int checkFinsihed() {
    int i, j, lines[8]={0}, filled = 1;
    char c;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++){
            if(board[i][0] != board[i][j]) lines[i] = 1;
            if(board[0][i] != board[j][i]) lines[i+3] = 1;
            if(board[i][j] == '#') filled = 0;
        }
        if(board[0][0] != board[i][i]) lines[6] = 1;
        if(board[2][0] != board[2-i][i]) lines[7] = 1;
    }

    for(i = 0; i < 8; i++) {
        if(lines[i] == 0) {
            if(i < 3) c = board[i][0];
            else if (i < 6) c = board[0][i-3];
            else c = board[1][1];
            if(c != '#'){
                validateWin(c);
                return 1;
            }
        }
    }

    if(filled == 1) printf("\nIt appears that the Champion is stil unbeaten. Better luck next time.\n");

    return filled;
}

void validateWin(char c) {
    if(c == 'X') playerWins();
    else aiWins();
}

void aiWins(){
    printf("\nYou are not yet worthy to join the CoolKidzClub.\n");
}

void playerWins(){
    char line[64];
    FILE * fp;
    int alreadyIn = 0, len;
    printf("\nWow, you actually won. Adding you to the CoolKidzClub...\n");
    fp = fopen("/home/kali/CoolKidzClub.txt", "a+");
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)){
        len = strlen(line);
        if(line[len-1] == '\n') line[len-1] = 0;
        if(strcmp(username, line) == 0) {
            printf("You are already in the CoolKidzClub.\n");
            alreadyIn = 1;
            break;
        }
    }
    if(!alreadyIn) fprintf(fp, "%s\n", username);
    printf("Done.\n");
    fclose(fp);
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
    printf("Can you beat the unbeatable TicTacToe Champion and join the \"CoolKidzClub\"?\n\nPlayers going first have a higher chance of winning. So, make your move.\n");
    printBoard();
    while (isFinished==0){	
        updateBoard(playerID, (playerID) ? playerInput() : playerInput());
        printBoard();
        playerID = (playerID ^ 1) & 1;
        isFinished = checkFinsihed();
    }
    return 0;
}
