#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define game mode constants
#define GM_END -1
#define GM_MAINMENU 0
#define GM_ENTERGAME 1
#define GM_NEXTROUND 2
#define GM_TEST 3
#define GM_PLAYING 4
#define GM_VICTORY 5
#define GM_TIE 6

// Print paper contents
void printPaper(char *paper)
{
    // Print paper contents with ASCII lines
    printf("%c\xBA%c\xBA%c\n",paper[0],paper[1],paper[2]);
    printf("\xCD\xCE\xCD\xCE\xCD\n");
    printf("%c\xBA%c\xBA%c\n",paper[3],paper[4],paper[5]);
    printf("\xCD\xCE\xCD\xCE\xCD\n");
    printf("%c\xBA%c\xBA%c\n",paper[6],paper[7],paper[8]);
}

// Check if the last player won
int checkVictory(int last, char *paper)
{
    // Horizontal checking where the last input was made
    if(paper[last/3*3]==paper[last/3*3+1])
    {
        if(paper[last/3*3]==paper[last/3*3+2])
        {
            return 1;
        }
    }
    // Vertical checking where the last input was made
    if(paper[last%3]==paper[last%3+3])
    {
        if(paper[last%3]==paper[last%3+6])
        {
            return 1;
        }
    }
    // Cross checking if the last entry was on even spots
    if(last%2==0)
    {
        // NW-SE checking
        if(paper[4]==paper[last])
        {
            if(paper[0]==paper[8])
            {
                if(paper[0]==paper[4])
                {
                    return 1;
                }
            }
        }
        // SW-NE checking
        if(paper[4]==paper[last])
        {
            if(paper[2]==paper[4])
            {
                if(paper[2]==paper[6])
                {
                    return 1;
                }
            }
        }
    }
    // Nobody has won yet
    return 0;
}

// Check if enter was pressed
int ifEnter(char ch)
{
    // Check if this character is carriage return or line feed
    if(ch=='\r'||ch=='\n')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int main()
{
    // Declaration and definition of variables
    int gamemode=0; // Gamemode redirects the game loop
    int player1score=0; // Score of player 1
    int player2score=0; // Score of player 2
    int turn=0; // Which turn it is now
    int ostarts=0; // Does O start the game
    char paper[9+1]="         "; // Paper array
    unsigned char c; // Inputted character from the keyboard
    // Enter game loop
    do{
        // Check game mode
        switch(gamemode)
        {
        case GM_MAINMENU: // Main menu
            player1score=0; // Reset players' score
            player2score=0;
            system("cls"); // Clear screen
            // Print mainmenu text
            printf("Welcome to XO4DOS!\nCreated by: Zvonimir Martinic (a.k.a. Zvoc47)\nPress ENTER to play or ESC to quit. (Press T to test).\n");
            while(1) // Enter choice
            {
                c=tolower(getch()); // Wait for keyboard input
                if(ifEnter(c)) // If Enter was pressed
                {
                    gamemode=GM_ENTERGAME; // Enter game
                    break; // Break from the choice loop
                }
                else if(c=='t') // If T was pressed
                {
                    gamemode=GM_TEST; // Enter keyboard test
                    break; // Break from the choice loop
                }
                else if(c=='\x1B') // If Escape was pressed
                {
                    gamemode=GM_END; // End the game
                    break; // Break from the choice loop
                }
            }
            break; // End of game mode
        case GM_ENTERGAME: // Starting the game
            strcpy(paper,"         "); // Initialize the game paper
            turn=0; // Reset turns
            gamemode=GM_PLAYING; // Set game mode to playing
            system("cls"); // Clear screen
            printf("Who wants to start first? (X/O) (ESC to cancel)\n");
            while(1) // Enter choice
            {
                c=tolower(getch()); // Wait for keyboard input
                if(c=='x') // If X was pressed
                {
                    ostarts=0; // O doesn't start the game
                    break; // Break from the choice loop
                }
                else if(c=='o') // If O was pressed
                {
                    ostarts=1; // O starts the game
                    break; // Break from the choice loop
                }
                else if(c=='\x1B') // If Escape was pressed
                {
                    gamemode=GM_MAINMENU; // Go to the main menu
                    break; // Break from the choice loop
                }
            }
            break; // End of game mode
        case GM_NEXTROUND: // Setting up the next round
            strcpy(paper,"         "); // Initialize the game paper
            turn=0; // Reset turns
            ostarts=(ostarts==1)?0:1; // Toggle who starts the turn now
            gamemode=GM_PLAYING; // Set game mode to playing
            break; // End of game mode
        case GM_TEST: // Keyboard test
            system("cls"); // Clear screen
            printf("Test started. Press ENTER to end test.\n");
            do // Loop for game inputs
            {
                c=getch(); // Wait for keyboard input
                printf("You pressed 0x%02X. Press ENTER to end test.\n",c); // Print hexadecimal code of the pressed key
            }
            while(!ifEnter(c)); // End loop if Enter was pressed
            gamemode=GM_MAINMENU; // Go to main menu
            break; // End of game mode
        case GM_PLAYING: // This is where the game is played
            if(turn>=9) // If 9 turns have passed which have filled all of the paper,...
            {
                gamemode=GM_TIE; // It means it's a tie
                break; // Break from the game mode
            }
            system("cls"); // Clear screen
            printf("Turn %d. SCORE: Player 1=%d | Player 2=%d.\n",turn,player1score,player2score); // Print turn and score
            printPaper(paper); // Print paper contents
            printf("It's %c's turn. Press 1-9. (Press ESC to end game)",((turn+ostarts)%2==0)?'X':'O'); // Print whose turn it is
            while(1) // This loop is needed in case an occupied spot is chosen so that goto isn't used
            {
                while(1) // Enter choice
                {
                    c=getch(); // Wait for keyboard input
                    if(c>='0'&&c<='9') // If 0-9 was selected
                    {
                        break; // Break from the choice loop
                    }
                    else if(c=='\x1B') // If Espace was pressed
                    {
                        gamemode=GM_MAINMENU; // Go to main menu
                        break; // Break from the choice loop
                    }
                }
                if(gamemode==GM_PLAYING) // If main menu game mode wasn't went to
                {
                    if(paper[c-49]==' ') // If the spot is unoccupied
                    {
                        paper[c-49]=((turn+ostarts)%2==0)?'X':'O'; // Write X or O into it, depending on the current player
                        if(checkVictory(c-49,paper)) // Check if the current player won
                        {
                            gamemode=GM_VICTORY; // Set to victory game mode
                            break; // Break from the choice loop
                        }
                        turn++; // Increment turn
                        break; // Break from the choice loop
                    }
                }
                else // If main menu was went to
                {
                    break; // Break from the choice loop
                }
            }
            break; // End of game mode
        case GM_VICTORY: // This is when someone wins
            if((turn+ostarts)%2==0) // Determine winner (true: Player 1; false: Player 2)
            {
                player1score++; // Increment player 1's score
            }
            else
            {
                player2score++; // Increment player 2's score
            }
            system("cls"); // Clear screen
            printf("Turn %d.\n",turn); // Print the current turn
            printPaper(paper); // Print paper contents
            // Print winner and the scores
            printf("And the winner is %c!\nPlayer 1=%d, player 2=%d.\nWanna play again? (Y/N)",((turn+ostarts)%2==0)?'X':'O',player1score,player2score);
            while(1) // Enter choice
            {
                c=tolower(getch()); // Wait for keyboard input
                if(c=='y') // If yes was chosen
                {
                    gamemode=GM_NEXTROUND; // Go to next round
                    break; // Break from the choice loop
                }
                else if(c=='n') // If no was chosen
                {
                    gamemode=GM_MAINMENU; // Go to the main menu
                    break; // Break from the choice loop
                }
            }
            break; //End of game mode
        case GM_TIE: // This is when it's a tie
            system("cls"); // Clear screen
            printf("Turn %d.\n",turn); // Print the current turn
            printPaper(paper); // Print paper contents
            printf("It's a tie! Wanna play again? (Y/N)\n");
            while(1) // Enter choice
            {
                c=tolower(getch()); // Wait for keyboard input
                if(c=='y') // If yes was chosen
                {
                    gamemode=GM_NEXTROUND; // Go to next round
                    break; // Break from the choice loop
                }
                else if(c=='n') // If no was chosen
                {
                    gamemode=GM_MAINMENU; // Go to the main menu
                    break; // Break from the choice loop
                }
            }
            break; // End of game mode
        }
    }while(gamemode!=GM_END); // Loop the game loop until the end game mode is chosen
    printf("ThEnD!\n"); // Print a nice shortening of the end
    return 0;
}
