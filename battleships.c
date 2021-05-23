#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define BUF_SIZE 200
#define MAX_STRING_SIZE 50
#define BOARD_SIZE 8
#define NUM_SHIPS 5
#define X_OFFSET 49
#define Y_OFFSET 65

char shipNameArray[5][MAX_STRING_SIZE] = { "Patrol Boat", "Submarine", "Destroyer", "Battleship", "Carrier"};

int playerOneLives = 5;
int playerTwoLives = 5;

struct Ship {
    int shipIndex;
    char shipX[NUM_SHIPS];
    char shipY[NUM_SHIPS];
    int health;
};


void initialiseShips(char map[BOARD_SIZE][BOARD_SIZE], struct Ship* shipsPointer){

    int lengthOfShip = 5;

    int shipIndex = 5;

    while(shipIndex >= 1){

        // This should print out the current ship we would like ot access
        printf("%s: ", shipNameArray[shipIndex-1]);

        char buf[BUF_SIZE];

        // Initialise variables
        int x = 0;
        int y = 0;
        char orientation = 0;

        // Use fgets instead of scanf
        fgets(buf, sizeof(buf), stdin);

        y = (int)buf[0] - Y_OFFSET;
        x = buf[2] - X_OFFSET;
        orientation = buf[4];

        // printf("y=%d x=%d orientation=%c\n", y, x, orientation);


        // Check if coordinates are valid
        if(y < 0 || y > 7 || x < 0 || x > 7){
            // Coordinates are out of bounds
            printf("Invalid ship configuration\n");
            continue;
        } else if (orientation != 'H' && orientation != 'V'){
            // Invalid Orientation
            printf("Invalid ship configuration\n");
            continue;
        } else if (x + lengthOfShip > 8 && orientation == 'H'){
            // Ship out of bounds on vertical axis
            // printf("Length Of Ship %d\n", x + lengthOfShip);
            printf("Invalid ship configuration\n");
            continue;
        } else if (y + lengthOfShip > 8 && orientation == 'V'){
            // Ship out of bounds on horizontal axis
            // printf("Length Of Ship %d\n", y + lengthOfShip);
            printf("Invalid ship configuration\n");
            continue;
        }

        // Validation checks complete

        // Begin storing coordinate of ship into player's board

        // Subtracting 65 from x coordinates to get into the 0-7 range
        // Subtracting 1 from y to change range from 1-8 to 0-7



        if(orientation == 'V'){
            // Ship is vertical
            bool shouldContinue = false;
            // First check if all the coordinates are empty
            for(int i = 0; i < lengthOfShip; i++){
                if(map[y + i][x] == '*'){
                    // This slot already taken
                    printf("Invalid ship configuration\n");
                    shouldContinue = true;
                    break;
                }
            }

            if(shouldContinue){
                // Break out of while loop
                continue;
            }

            // Initiliase the shipArray
            for(int i = 0; i < 5; i ++){

                (shipsPointer+shipIndex)->shipX[i] = -1;
                (shipsPointer+shipIndex)->shipY[i] = -1;

                // shipArray[shipIndex].shipX[i] = -1;
                // shipArray[shipIndex].shipY[i] = -1;
            }

            (shipsPointer+shipIndex)->shipIndex = shipIndex;
            (shipsPointer+shipIndex)->health = lengthOfShip;
            // printf("Ship's health initialised to: %d\n", (shipsPointer+shipIndex)->health);

            // shipArray[shipIndex].shipIndex = shipIndex;
            // shipArray[shipIndex].health = lengthOfShip;

            for(int i = 0; i < lengthOfShip; i++){
                // printf("'*' stored at y = %d, x = %d \n", y-65+i, x-1);
                map[y + i][x] = '*';
                // shipArray[shipIndex].shipX[i] = x;
                // shipArray[shipIndex].shipY[i] = y + i;

                (shipsPointer+shipIndex)->shipX[i] = x;
                (shipsPointer+shipIndex)->shipY[i] = y + i;

            }
        } else {
            // Ship is horizontal
            bool shouldContinue = false;

            // First check if all the coordinates are empty
            for(int i = 0; i < lengthOfShip; i++){
                if(map[y][x + i] == '*'){
                    // This slot already taken
                    printf("Invalid ship configuration\n");
                    shouldContinue = true;
                    // Break out of for loop
                    break;
                }
            }

            if(shouldContinue){
                // Break out of while loop
                continue;
            }

            // Initiliase the shipArray
            for(int i = 0; i < 5; i ++){
                (shipsPointer+shipIndex)->shipX[i] = -1;
                (shipsPointer+shipIndex)->shipY[i] = -1;
                
                // shipArray[shipIndex].shipX[i] = -1;
                // shipArray[shipIndex].shipY[i] = -1;
            }

            (shipsPointer+shipIndex)->shipIndex = shipIndex;
            (shipsPointer+shipIndex)->health = lengthOfShip;
            // printf("Ship's health initialised to: %d\n", (shipsPointer+shipIndex)->health);

            for(int i = 0; i < lengthOfShip; i++){
                // printf("'*' stored at y = %d, x = %d \n", y-65, x-1+i);
                map[y][x + i] = '*';
                // shipArray[shipIndex].shipX[i] = x + i;
                // shipArray[shipIndex].shipY[i] = y;

                (shipsPointer+shipIndex)->shipX[i] = x + i;
                (shipsPointer+shipIndex)->shipY[i] = y;
            }
        }


        // After the ship has been appended into our array

        // If we're placing our patrol boat, do not decrease lengthOfShip
 
        if(shipIndex == 2){
            shipIndex--;
            continue;
        }
        
        // printf("Decrementing Index and Length\n");
        shipIndex--;
        lengthOfShip--;

    }


    // Reaches here if all ships have been stored

    printf("\n");


}

int strike(int y, int x, char map[BOARD_SIZE][BOARD_SIZE], char shotsTaken[BOARD_SIZE][BOARD_SIZE], struct Ship* enemyShipPointer){

    // First make sure coordinates are valid
    // printf("y = %d, x = %d\n", y, x);

    if(y < 0 || y > 7 || x < 0 || x > 7){
        // printf("y = %d, x = %d\n", y, x);
        printf("Invalid coordinates\n");
        return 1;
    }

    // TODO: Check if we have already fired at this location
    if(shotsTaken[y][x] == 'X' || shotsTaken[y][x] == 'M'){
        // printf("Character at x,y = %c\n", shotsTaken[y][x]);
        printf("You have already fired at this location\n");
        // Returns 1 if the player needs to re-enter a new coordinates
        return 1;
    }
    
    // Check given location in the map whether an '*' resides there
    if(map[y][x] == '*' || map[y][x] == '&' || map[y][x] == 1){
        // Ship at this location
        // Check which ship this is
        // printf("Found Asterix\n");
        for(int shipIndex = 1; shipIndex < 6; shipIndex++){
            // For each ship
            // struct Ship currentShip = enemyShipArray[shipIndex];
            for(int i = 0; i < 5; i++){
                // Fix 42 to 0
                if((enemyShipPointer+shipIndex)->shipY[i] == 42){
                    (enemyShipPointer+shipIndex)->shipY[i] = 0;
                }
                // printf("Looking for (%d, %d), currently at (%d, %d)\n", y, x, enemyShipArray[shipIndex].shipY[i], enemyShipArray[shipIndex].shipX[i]);
                if((enemyShipPointer+shipIndex)->shipX[i] == x && (enemyShipPointer+shipIndex)->shipY[i] == y){
                    // printf("Found Relevant ship\n");
                    // Found the relevant ship
                    // Mark the location on shots taken
                    shotsTaken[y][x] = 'X';
                    // Decrement ship health
                    // printf("Health before decrement: %d\n", enemyShipArray[shipIndex].health);
                    (enemyShipPointer+shipIndex)->health--;   
                    // printf("Ship Index %d -  new health %d\n", shipIndex, enemyShipArray[shipIndex].health);
                    // Check ship health
                    // printf("ShipHealth = %d\n", (enemyShipPointer+shipIndex)->health);
                    if((enemyShipPointer+shipIndex)->health < 1){
                        // Ship has sunken
                        printf("\nWe have sunk your %s!\n", shipNameArray[shipIndex-1]);
                        return 2;
                    } else {
                        printf("\nWe have hit the target!\n");
                        return 0;
                    }
                }

                

            }
        }
        // printf("Found an asterix but no corresponding ship\n");
    } else {
        // printf("map[y][x] == %d\n", map[y][x]);
        printf("\nYou have missed!\n");
        // Put a 0 into shotsTakenBoard
        shotsTaken[y][x] = 'M';
        return 0;
    }

    // Reached the bottom without returning in other places
    // printf("Reached bottom without returning\n");
    return -1;

}

void showShotsTaken(char shotsTaken[BOARD_SIZE][BOARD_SIZE]){

    printf("\n");
    char rowIndex = 'A';
    // Print top row of numbers
    printf("  1 2 3 4 5 6 7 8\n");

    for(int i = 0; i < BOARD_SIZE; i++){
        printf("%c", rowIndex+i);
        printf("|");
        for(int j = 0; j < BOARD_SIZE; j++){

            if(shotsTaken[i][j] == 'M'){
                printf("0");
            } else if (shotsTaken[i][j] == 'X'){
                printf("X");
            } else {
                printf(" ");
            }

            printf("|");

        }

        printf("\n");
    }

    printf("\n");


}

void beginGame(char playerOneMap[BOARD_SIZE][BOARD_SIZE], char playerTwoMap[BOARD_SIZE][BOARD_SIZE], struct Ship* shipsPointerOne, struct Ship* shipsPointerTwo){

    char buf[BUF_SIZE];
    // Toggle to true when win condition is met
    bool gameFinished = false; 
    // Toggle this variable for player 2
    bool playerOneTurn = true;
    // The shots taken board for each player
    char shotsTakenOne[BOARD_SIZE][BOARD_SIZE] = {{0}};
    char shotsTakenTwo[BOARD_SIZE][BOARD_SIZE] = {{0}};

    // Print out all ships healths
    for(int i = 1; i < NUM_SHIPS+1; i++){


        if( (shipsPointerOne+i)->health == 42 ){
           (shipsPointerOne+i)->health = 5; 
        }

        // printf("ShipOne %d, health %d\n",i, (shipsPointerOne+i)->health );
        // printf("ShipTwo %d, health %d\n",i, (shipsPointerTwo+i)->health );
    }

    // Need a while loop which checks for commands
    while(fgets(buf, sizeof(buf), stdin) && !gameFinished){

        // Check which command has been called
        // Check for 'fire', 'shots', or 'exit' from input
        // Anything else is Unrecognised command

        int fire = strncmp(buf, "fire", 4);    
        int shots = strncmp(buf, "shots", 5);
        int exit = strncmp(buf, "exit", 4);

        // printf("fire=%d shots=%d exit=%d\n", fire, shots, exit);

        if(fire == 0){
            // printf("Inside Fire call\n");
            // Call fire function on the coordinates given
            // Against the enemy player's board
            // printf("y = %c, x = %c\n", buf[5], buf[7]);
            int y = (int)buf[5] - Y_OFFSET;
            int x = buf[7] - X_OFFSET;
            // printf("Char after coordinates: %d\n", buf[8]);

            if(buf[8] != 10){
                printf("Invalid coordinates\n");
                continue;
            }
            
            if(playerOneTurn){
                // Player One's turn
                // Pass relevant maps to the strike function
                // We pass in the other player's shipArray to check if a boat has sunken
                
                int strikeOp = strike(y, x, playerOneMap, shotsTakenOne, shipsPointerTwo);
                if(strikeOp == 1){
                    // Player fired at a location they've previously fired at
                    // Prompt again without swapping to the other player's turn
                    continue;;
                } else if (strikeOp == 2){
                    // We just sunk a ship
                    // Decrement the number of ships
                    playerTwoLives--;
                    // If its below 1, we sank all the ships
                    if(playerTwoLives < 1){
                        // Playe 1 Won
                        printf("\nPlayer 1 wins!\n");
                        gameFinished = true;
                    } else {
                        printf("\nPlayer 2's turn\n\n");
                        playerOneTurn = !playerOneTurn;
                    }


                } else if (strikeOp == 0){
                    // Successful strike / miss
                    playerOneTurn = !playerOneTurn;
                    // printf("New value of playerOneTurn %d = \n", playerOneTurn);
                    printf("\nPlayer 2's turn\n\n");
                }
            } else {
                // Player Two's turn
                int strikeOp = strike(y, x, playerTwoMap, shotsTakenTwo, shipsPointerOne);
                if(strikeOp == 1){
                    // Player fired at a location they've previously fired at
                    // Prompt again without swapping to the other player's turn
                    continue;;
                } else if (strikeOp == 2){
                    // We just sunk a ship
                    // Decrement the number of ships
                    playerOneLives--;
                    // If its below 1, we sank all the ships
                    if(playerOneLives < 1){
                        // Playe 1 Won
                        printf("\nPlayer 2 wins!\n");
                        gameFinished = true;
                    } else {
                        printf("\nPlayer 1's turn\n\n");
                        playerOneTurn = !playerOneTurn;
                    }



                } else if (strikeOp == 0){
                    // Successful strike, swap turns
                    playerOneTurn = !playerOneTurn;
                    // printf("New value of playerOneTurn %d = \n", playerOneTurn);
                    // Next players Turn
                    printf("\nPlayer 1's turn\n\n");
                }
            }

        } else if (shots == 0){
            // printf("Inside shots call\n");
            if(playerOneTurn){
                // Show Player One's shots taken
                // printf("Showing Player One's shots\n");
                showShotsTaken(shotsTakenOne);

            } else {
                // printf("Showing Player Two's shots\n");
                showShotsTaken(shotsTakenTwo);

            }


        } else if (exit == 0){
            // printf("Inside exit call\n");
            // Break out of loop and finish game
            gameFinished = true;
            break;
        } else {
            printf("Unrecognised Command\n");
        }


    }


}





int main(){

    // Initialise 2D arrays where the players will store the coordinates of their ships
    char playerOneMap[BOARD_SIZE][BOARD_SIZE] = {{0}};
    char playerTwoMap[BOARD_SIZE][BOARD_SIZE] = {{0}};

    // Create array of structs to store the coordinates and health of the boats for both players
    struct Ship playerOneShips[NUM_SHIPS] = {0};
    struct Ship playerTwoShips[NUM_SHIPS] = {0};

    struct Ship* shipsPointerOne = &playerOneShips;
    struct Ship* shipsPointerTwo = &playerTwoShips;
 
    printf("Player 1, please set up your ships (y, x, orientation)\n");
    
    initialiseShips(playerOneMap, shipsPointerOne);

    printf("\n");

    printf("Player 2, please set up your ships (y, x, orientation)\n");

    initialiseShips(playerTwoMap, shipsPointerTwo);
    
    // Both boats have been set up
    printf("All boats set up, Player 1 can now strike.\n\n");

    // Quick test on if the boats are placed correctly

    // for(int i = 0; i < BOARD_SIZE; i++){
    //     printf("\n");
    //     printf("A");
    //     for(int j = 0; j < BOARD_SIZE; j++){
    //         printf("%c", playerOneMap[i][j]);
    //         if(playerOneMap[i][j] != '*'){
    //             printf(" ");
    //         }
    //     }
    // }

    // The game begins here!
    beginGame(playerOneMap, playerTwoMap, shipsPointerOne, shipsPointerTwo);

}
