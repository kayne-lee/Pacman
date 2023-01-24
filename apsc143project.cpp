//include all the libraries and "colours.h" to use colour
#include <stdio.h>
#include "colours.h"
#include <stdlib.h>

#define PACMAN 'P'
#define GHOST 'G'
#define WALL 'W'
#define DOT '.'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'

// Below are the required functions for this program, but you will need to create your own as well.
// Make sure to specify the required parameters, if any.

//declare global variables
int a, b, c, d, e, f;
char **map, **map1;

//First create a function to read, and print the text file that we were given. The text file was altered from the orginal to allow a surround wall
char **readMap(char **array) {
    FILE *fp;
    fp = fopen("map.txt", "r");

    //This statement will let us know if the code has issue finding the text file
    if (fp == NULL) {
        printf("File not found.\n");
        exit(0);

    //This code is the part that outlines the text file.
    } else {
        array = (char**)malloc(140 * sizeof(char*));
        for (int i = 0; i < 9; ++i) {
            array[i] = (char *) malloc(9 * sizeof(char));
        }
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                fscanf(fp, "%c  ", &array[i][j]);
            }
        }

        return array;
    }
}

//This map will be the one visible to the user during the game.
//Since this map is the one visible to the user, it is the map that it's colours are altered.
//This is done to closer resemble the game of pac man
void printMap(char** array) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (array[i][j] == WALL) {
                colourChange(BLUE);
                printf("%c  ", array[i][j]);
            }
            if (array[i][j] == PACMAN) {
                colourChange(YELLOW);
                printf("%c  ", array[i][j]);
            }
            if (array[i][j] == GHOST) {
                colourChange(PINK);
                printf("%c  ", array[i][j]);
            }
            if (array[i][j] == DOT) {
                colourChange(WHITE);
                printf("%c  ", array[i][j]);
            }
            if (array[i][j] == EMPTY) {
                colourChange(WHITE);
                printf("%c  ", array[i][j]);
            }
        }
        printf("\n");
        }

}

//Now a function must be created focused around the objective of pac man, the dots.
char **dotMap(char ** array) {
    FILE *fp;
    fp = fopen("map.txt", "r");
    if (fp == NULL) {
        printf("File not found.\n");
        exit(0);
    } else {
        array = (char**)malloc(140 * sizeof(char*));
        for (int i = 0; i < 9; ++i) {
            array[i] = (char *) malloc(31 * sizeof(char));
        }
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                fscanf(fp, "%c  ", &array[i][j]);
            }
        }

        //This section of code clears a dot when pacman goes over it.
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if(array[i][j] == PACMAN) {
                    array[i][j] = EMPTY;
                }
                //This makes sure the ghost don't clear the dots/
                if(array[i][j] == GHOST) {
                    array[i][j] = DOT;
                }
            }
        }
        return array;
    }

}
//The wincheck function checks to see if there are any dots remaining
//If there are no dots remaining the user has won the game
int winCheck() {
    int counter = 0;
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (map[i][j] == DOT) {
                counter++;
            }
        }
    }
    if(counter == 0) {
        return 1;
    }
    return 0;
}

//In pacman a user loses if the ghost touches them.
//This function will alert the user when a ghost has touched them,telling them they've lost.
int loseCheck() {
    if (map[a][b] == map[c][d] || map[a][b] == map[e][f]) {
        map[a][b] = GHOST;
        return 1;
    } else {
        return 0;
    }
}

//This is a short function that initializes the wall
//This will make sure pac man and the ghost can't go through the wall.
//It does this by coming up in the respective pac man and ghost functions.
int isWall(int a, int b) {
    if (a < 0 || a > 8 || b < 0 || b > 8) {
        return 0;
    } else if (map[a][b] == WALL) {
        return 0;
    } else {
        return 1;
    }
}

//This function controls the ghost
//When a ghost sees pacman it should follow him.
//If it doesn't see pacman it should have random movement.
//This specific function accounts for the random movement of both ghost.
void ghost (char &direction, int &c, int &d) {
    if (direction == RIGHT && isWall(c, d + 1)) {
        map[c][d] = map1[c][d];
        d++;
        map[c][d] = GHOST;
    } else if (direction == LEFT && isWall(c, d - 1)) {
        map[c][d] = map1[c][d];
        d--;
        map[c][d] = GHOST;
    } else if (direction == DOWN && isWall(c + 1, d)) {
        map[c][d] = map1[c][d];
        c++;
        map[c][d] = GHOST;
    } else if (direction == UP && isWall(c - 1, d)) {
        map[c][d] = map1[c][d];
        c--;
        map[c][d] = GHOST;
    } else if (direction == DOWN && !isWall(c + 1, d)) {
        direction = RIGHT;
        ghost(direction, c, d);
    } else if (direction == RIGHT && !isWall(c, d + 1)) {
        direction = UP;
        ghost(direction, c, d);
    } else if (direction == UP && !isWall(c - 1, d)) {
        direction = LEFT;
        ghost(direction, c, d);
    } else if (direction == LEFT && !isWall(c, d - 1)) {
        direction = DOWN;
        ghost(direction, c, d);
    }
}

//This function accounts for the movement of pacman.
//Notice how the wall function is called several times throughout this function, as well as some important constants
void pacman (char direction, char &direction1, char &direction2, int &a, int &b) {

        //For the right direction.
        if (direction == RIGHT && isWall(a, b + 1)) {
            map[a][b] = EMPTY;
            map1[a][b] = EMPTY;
            b++;
            map[a][b] = PACMAN;
            ghost(direction1, c, d);
            ghost(direction2, e, f);
            printMap(map);

        //For the upwards direction.
        } else if (direction == UP && isWall(a - 1, b)) {
            map[a][b] = EMPTY;
            map1[a][b] = EMPTY;
            a--;
            map[a][b] = PACMAN;
            ghost(direction1, c, d);
            ghost(direction2, e, f);
            printMap(map);

        //For the downwards direction.
        } else if (direction == DOWN && isWall(a + 1, b)) {
            map[a][b] = EMPTY;
            map1[a][b] = EMPTY;
            a++;
            map[a][b] = PACMAN;
            ghost(direction1, c, d);
            ghost(direction2, e, f);
            printMap(map);

        //For the left direction
        } else if (direction == LEFT && isWall(a, b - 1)) {
            map[a][b] = EMPTY;
            map1[a][b] = EMPTY;
            b--;
            map[a][b] = PACMAN;
            ghost(direction1, c, d);
            ghost(direction2, e, f);
            printMap(map);
        }
}

//The ghost change functions are what allows the ghost to chase down pacman.
//There are two of these functions, one for each ghost.
void ghostChange (char &direction, int c, int d) {
    int wallCounter = 0;
    if(a == c) {
        if(b < d) {
            for (int i = b; i < d; ++i) {
                if(map[a][i] == WALL){
                    wallCounter++;
                }
            }
        } else if(b > d) {
            for (int i = d; i < b; ++i) {
                if(map[a][i] == WALL){
                    wallCounter++;
                }
            }
        }
        if(wallCounter == 0) {
            if (b < d) {
                direction = LEFT;
            }
            if (b > d) {
                direction = RIGHT;
            }
        }
    } else if(b == d) {
        int wallCounter = 0;
        if(a < c) {
            for (int i = a; i < c; ++i) {
                if(map[i][b] == WALL){
                    wallCounter++;
                }
            }
        } else if(a > c) {
            for (int i = c; i < a; ++i) {
                if(map[i][b] == WALL){
                    wallCounter++;
                }
            }
        }
        if(wallCounter == 0) {
            if (a < c) {
                direction = UP;
            }
            if (a > c) {
                direction = DOWN;
            }
        }
    }
}

//Now that all the functions have been created, we can move on to the main function
//The main function will not require a lot of coding as most of the coding is done outside the main function.
int main () {

    //Now in the main function we can print the map for the user.
    map = readMap(map);
    map1 = dotMap(map1);
    char direction1 = DOWN;
    char direction2 = UP;
    printMap(map);
    printf("\n");
    a = 4, b = 4;
    c = 0, d = 0;
    e = 8, f = 8;


    //Now finally this takes the input from the user, and converts it into a direction for pacman.
    //Since on mac, unfortunately we could not use getch
    while (1) {
        colourChange(WHITE);
        char direction;
        scanf("%c", &direction);
        ghostChange(direction1, c, d);
        ghostChange(direction2, e, f);
        pacman(direction, direction1, direction2, a, b);

        //Activates the message after a win, in white text.
        if(winCheck()) {
            colourChange(WHITE);
            printf("Congratulations! You win! Press any key to exit the game");
            exit(0);
        }

        //Activates the message after a loss, in white text.
        if (loseCheck()) {
            colourChange(WHITE);
            printf("Sorry, you lose. Press any key to exit the game");
            exit(0);
        }
    }

}