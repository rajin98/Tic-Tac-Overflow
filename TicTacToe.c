#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>

char board[3][3] = {{'#', '#', '#'},{'#', '#', '#'},{'#', '#', '#'}};

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
    char c[2]; int p = 0;
    printf("Player Move\nEnter an integer between 1-9:\n");
    while (p < 1 || p > 9){
        gets(c);
        int p = atoi(c);
        if(p > 0 && p < 10) {
            if(spaceAvailable(p) == 0) {
                p = 0;
                printf("The space is filled. Choose another space.\nEnter an integer between 1-9:\n");
            }
        }
        else printf("Invalid Input. Enter an integer between 1-9:\n");
    }
    return atoi(c);
}

int aiInput(){

}

int spaceAvailable(int p) {
    if(board[(p-1)/3][(p-1)%3] == '#') return 1;
    printf("reached");
    return 0;
}

void updateBoard(int playerID, int p) {
    board[(p-1)/3][(p-1)%3] = (playerID) ? 'O' : 'X';
}

int checkFinsihed() {
    int i, j, lines[8] = {0}, filled = 1, isWon = 0;
    char c;
    memset(lines, 1, sizeof(lines));
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++){
            if(board[i][0] != board[i][j]) lines[i] = 0;
            if(board[0][i] != board[j][i]) lines[3+i] = 0;
            if(board[i][j] == '#') filled = 0;
        }
        if(board[0][0] != board[i][i]) lines[6] = 0;
        if(board[2][0] != board[2-i][i]) lines[7] = 0;
    }

    for(i = 0; i < 8; i++) {
        if(lines[i] == 1) {
            if(i < 3) c = board[i][0];
            else if (i < 6) c = board[0][i];
            else c = board[i][i];
            if(c != '#'){
                validateWin(i);
                return 1;
            }
        }
    }

    if(filled) printf("\nIt appears that the Champion is stil unbeaten. Better luck next time.");

    return filled;
}

void validateWin(char c) {
    if(c == 'X') playerWins();
    else aiWins();
}

void aiWins(){
    printf("\nYou are not yet worthy to join the CoolKidzClub.");
}

void playerWins(){
    printf("\nWow, you actually won. Adding you to the CoolKidzClub...\n");
    FILE *fp;
    char line[64];
    const char * name = getUserName();
    int alreadyIn = 0, len;
    fp = fopen("CoolKidzClub.txt", "a+");
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)){
        len = strlen(line);
        if(line[len-1] == '\n') line[len-1] = 0;
        if(strcmp(name, line) != 0) {
            printf("You are already in the CoolKidzClub.");
            alreadyIn = 1;
            break;
        }
    }
    if(!alreadyIn) fprintf(fp, "%s", name);
    printf("Done.");
    fclose(fp);
}

const char * getUserName() {
    uid_t uid = geteuid();  
    struct passwd *pw = getpwuid(uid);
    if (pw) return pw->pw_name;
    return "";
}

int main() {
    short isFinished = 0, playerID = 0;
    printf("Can you beat the unbeatable TicTacToe Champion and join the \"CoolKidzClub\"?\n\nPlayers going first have a higher chance of winning. So, make your move.\n");
    printBoard();
    while (!isFinished){
        updateBoard(playerID, (playerID) ? playerInput() : playerInput());
        printBoard();
        playerID = (playerID ^ 1) & 1;
        isFinished = checkFinsihed();
    }
    return 0;
}
