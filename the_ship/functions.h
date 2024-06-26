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
            if(gameMap[i][j] == 'x' && gameMap[i][j+1] == 'x')
            {
                gameMap[i][j] = 'h';
                gameMap[i][j+1] = 'h';
                numberOfSunk++;
            }
}

/* 
 *  Checking if user hit a ship, hit the same position twice, or missed
 *  Blink LED indicator accordingly:
 *      - Hit -> blink fast twice
 *      - Missed -> blink fast once
 *      - Shot same position twice -> blink slowly once
*/
void hit()
{
    int row = player[0]; int col = player[1];

    // Shot same position twice
    if(gameMap[row][col] == 'x' || gameMap[row][col] == 'm' || gameMap[row][col] == 'h') 
    {
        PORTB |= (1 << 5);
        _delay_ms(1000);
        PORTB &= ~(1 << 5);
    }
    else if(gameMap[row][col] == '1') {     // Hit
        gameMap[row][col] = 'x';
        numberOfHit++;

        for (int i = 0; i < 4; i++)
        {
            PORTB ^= (1 << 5);
            _delay_ms(100);
        }
    } else if(gameMap[row][col] == '0') {   // Missed
        gameMap[row][col] = 'm';
        PORTB |= (1 << 5);
        _delay_ms(100);
        PORTB &= ~(1 << 5);
    }
}

/*
 * Check if game over
 */
bool game_over()
{
    if(shots == 0) {            // Run out of shots -> true
        return true;
    }

    if(numberOfSunk == 5) {     // Shot down all 5 ships -> true
        return true;
    }

    for(int i = 0; i < 8; i++){         // Ships left -> false
        for(int j = 0; j < 8; j++){
            if(gameMap[i][j] == '1')
                return false;
        }
    }
    return false;
}

/*
 * Reset game elements
 */
void replay(){
    for(int i = 0; i < 8; i++){         // reset gameMap
        for(int j = 0; j < 8; j++){
            if(gameMap[i][j] == 'x' || gameMap[i][j] == 'h')
                gameMap[i][j] = '1';
            if(gameMap[i][j] == 'm')
                gameMap[i][j] = '0';
        }
    }
    shots = 16;                         // reset shots
    player[0] = 0; player[1] = 0;       //reset player pos
    numberOfSunk = 0; numberOfHit = 0;  // reset sunk, shot
}

/*
 * Blink all digits three times to indicate game over
 */
void blink_game_over(){
    for (int i = 0; i < 3; i++)
    {
        on_digit(1); on_digit(2); on_digit(3); on_digit(4); on_digit(5);
        show_number(8);
        _delay_ms(100);
        off_digit(1); off_digit(2); off_digit(3); off_digit(4); off_digit(5);
        _delay_ms(100);
    }
}

/*
 * Get coordinate and show on 7 segment LED
 */
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
