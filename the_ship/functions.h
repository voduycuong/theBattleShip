#include "global_variables.h"

void move_up(bool dir);
void move_down(bool dir);
void check_ships();
void hit();
bool game_over();
void replay();
void blink_game_over();
void show_coordinate(bool dir);
void show_number(int number);
void on_digit(int digit);
void off_digit(int digit);

// MOVE PLAYER -------------------------------------------------------------
void move_up(bool dir) 
{
    if(dir)                             // x direction
    {      
        player[1]++;
        if (player[1] > 7) player[1] = 0;
    } 
    else                                // y direction
    {                                   
        player[0]++;
        if (player[0] > 7) player[0] = 0;
    }
}

void move_down(bool dir)
{
    if(dir)                             // x direction
    {       
        player[1]--;
        if (player[1] < 0) player[1] = 7;
    }
    else                                // y direction
    {                                   
        player[0]--;
        if (player[0] < 0) player[0] = 7;
    }
}

// ===================== SHIPS ===================================
void check_ships()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) 
            if(playerMap[i][j] == 'x' && playerMap[i][j+1] == 'x')
            {
                playerMap[i][j] = 'h';
                playerMap[i][j+1] = 'h';
                numberOfSunk++;
            }
}

/* 
 *  return false if missed, return true if hit
*/
void hit()
{
    int row = player[0]; int col = player[1];
    int count = 0;

    //check if hit same position twice
    if(playerMap[row][col] == 'x' || playerMap[row][col] == 'm' || playerMap[row][col] == 'h') {
        PORTB |= (1 << 5);
        while(count < 50) {
            if(count == 50) PORTB &= ~(1 << 5);
            if (TIFR2 & (1 << OCF2A)) {
                count ++;
                TIFR2 |= (1 << OCF2A);
            }
        }
    } else if(playerMap[row][col] == '1') {
        playerMap[row][col] = 'x';
        numberOfHit++;
        // blink fast twice
        while(count < 200) {
            if((count%50) == 0) {
                PORTB ^= (1 << PORTB5);
            }

            if (TIFR2 & (1 << OCF2A)){  // Check if Compare Match
                count++; 
                TIFR2 |= (1 << OCF2A);  // Clear the flag
            }
        }
    } else if(playerMap[row][col] == '0') {
        playerMap[row][col] = 'm';
    }
}

//done, i think?
bool game_over()
{
    if(shots == 0) {
        return true;
    }

    if(numberOfSunk == 5) {
        return true;
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(playerMap[i][j] == '1')
                return false;
        }
    }
    return false;
}

void replay(){
    for(int i = 0; i < 8; i++){ // reset playerMap
        for(int j = 0; j < 8; j++){
            playerMap[i][j] = gameMap[i][j];
        }
    }
    shots = 16; // reset shots
    player[0] = 0; player[1] = 0; //reset player pos
    numberOfSunk = 0; numberOfHit = 0; // reset sunk, shot
}

void blink_game_over(){
    int count = 0; 
    boolean state = true; //true to turn on, false to turn off
    
    for(int i = 0; i < 5; i++){
        if(count == 25){
            if(state){
                on_digit(1); on_digit(2); on_digit(3); on_digit(4); on_digit(5);
                PORTB |= (1 << 5);
                count = 0;
            } else {
                off_digit(1); off_digit(2); off_digit(3); off_digit(4); off_digit(5);
                PORTB &= ~(1 << 5);
                count = 0;
            }
        }

        if (TIFR2 & (1 << OCF2A))   // Check if Compare Match
        {  
            count++; 
            state = !state;
            TIFR2 |= (1 << OCF2A);  // Clear the flag
        }
    }
}

void show_coordinate(bool dir)
{
    short coor = 0;
    if(!dir) coor = player[0];  //row
    else coor = player[1];      //col
    show_number(coor);
}

// TURN ON NUMBERS -------------------------------------------------------------------------------------
void show_number(int number)
{
    switch(number)
    {
        case 0:
            PORTD &= ~((1 << 6)|(1 << 5)|(1 << 4)|(1 << 3));
            PORTD |= (1 << 7);
            PORTC &= ~((1 << 5)|(1 << 4));
            break;  
        case 1:
            PORTD &= ~((1 << 3));
            PORTD |= (1 << 7)|(1 << 6)|(1 << 5)|(1 << 4);
            PORTC |= (1 << 4);
            PORTC &= ~(1 << 5);
            break;
        case 2:
            PORTD &= ~((1 << 7)|(1 << 5)|(1 << 4));
            PORTD |= (1 << 6)|(1 << 3);
            PORTC &= ~((1 << 5)|(1 << 4));
            break;
        case 3:
            PORTD &= ~((1 << 7)|(1 << 4)|(1 << 3));
            PORTD |= (1 << 6)|(1 << 5);
            PORTC &= ~((1 << 5)|(1 << 4));
            break;
        case 4:
            PORTD &= ~((1 << 7)|(1 << 6)|(1 << 3));
            PORTC &= ~(1 << 5);
            PORTD |= (1 << 5)|(1 << 4);
            PORTC |= (1 << 4);
            break;
        case 5:
            PORTD &= ~((1 << 7)|(1 << 6)|(1 << 4)|(1 << 3));
            PORTD |= (1 << 5);
            PORTC |= (1 << 5);
            PORTC &= ~(1 << 4);
            break;
        case 6:
            PORTD &= ~((1 << 7)|(1 << 6)|(1 << 5)|(1 << 4)|(1 << 3));
            PORTC &= ~(1 << 4);
            PORTC |= (1 << 5);
            break;
        case 7:
            PORTD &= ~(1 << 3);
            PORTD |= (1 << 7)|(1 << 6)|(1 << 5)|(1 << 4);
            PORTC &= ~((1 << 4)|(1 << 5));
            break;
        case 8:
            PORTD &= ~((1 << 7)|(1 << 6)|(1 << 5)|(1 << 4)|(1 << 3));
            PORTC &= ~((1 << 4)|(1 << 5));
            break;
        case 9:
            PORTD &= ~((1 << 7)|(1 << 6)|(1 << 4)|(1 << 3));
            PORTD |= (1 << 5);
            PORTC &= ~((1 << 4)|(1 << 5));
            break;
        case 10:
            PORTD |= (1 << 7)|(1 << 6)|(1 << 5)|(1 << 4)|(1 << 3);
            PORTC |= (1 << 4)|(1 << 5);
            break;
    }
}

// TURN DIGIT ON/OFF --------------------------------------------------------------------------
void on_digit(int digit)
{
    switch(digit)
    {
        case 1:
        PORTB |= (1 << 0);break;
        case 2:
        PORTB |= (1 << 1);break;
        case 3:
        PORTB |= (1 << 2);break;
        case 4:
        PORTB |= (1 << 3);break;
        case 5:
        PORTB |= (1 << 4);break;
    }
}

void off_digit(int digit)
{
    switch(digit)
    {
        case 1: 
        PORTB &= ~(1 << 0);break;
        case 2: 
        PORTB &= ~(1 << 1);break;
        case 3: 
        PORTB &= ~(1 << 2);break;
        case 4: 
        PORTB &= ~(1 << 3);break;
        case 5: 
        PORTB &= ~(1 << 4);break;
    }
}
