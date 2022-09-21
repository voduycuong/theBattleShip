#ifndef F_CPU
#define F_CPU 16000000UL    // Set 16 MHz clock speed
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <SevSeg.h>
#include "functions.h"

#define DELAY_TIME 200
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE ((((F_CPU / 16) + (USART_BAUDRATE / 2)) / (USART_BAUDRATE )) - 1)

int main()
{
    // OUTPUT -----------------------------------------
        // FEDCBA
    DDRD |= (1 << DDD7)|(1 << DDD6)|(1 << DDD5)|(1 << DDD4)|(1 << DDD3);
    DDRC |= (1 << 4)|(1 << 5);
        // D5-D1
    DDRB |= (1 << DDB4)|(1 << DDB3)|(1 << DDB2)|(1 << DDB1)|(1 << DDB0);
        // HIT LED
    DDRB |= (1 << DDB5);

    // INPUT ------------------------------------------
        // DOWN/UP/XY Button 
    DDRC &= ~((1 << 2)|(1 << 1)|(1 << 0));
        // SHOOT Button - INT0
    DDRD &= ~(1 << 2);
    EICRA |= (1 << ISC01);          // Set INT0 trigger on falling-edge
    EIMSK |= (1 << INT0);           // Turns on INT0

    // UART --------------------------------------------
        // Setup the Baud Rate
    UBRR0H = (BAUD_PRESCALE >> 8);  // Load upper 8-bits of the baud rate value into the high byte of the UBRR0H register
    UBRR0L = BAUD_PRESCALE ;        // Load lower 8-bits of the baud rate value into the low byte of the UBRR0L register
    
        // Configure data format for transmission
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);     // Use 8-bit character sizes
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);       // Turn on the transmission and reception circuitry
    
        // Setup for interrupts
    UCSR0B |= (1 << RXCIE0);        // Enable RX Complete Interrupt
    
    // Timer 1 for scanning rate of the LED compound
    DDRB |= (1 << 5);               // Set LED as output
    TCCR1B |= (1 << WGM12);         // Turn on the CTC mode for Timer 1
    TCCR1B |= (1 << CS11);          // Set up Timer 1 with the prescaler of 8
    OCR1A = 1999;                   // Set CTC compare value to 0.001s at 16 MHz AVR clock , with a prescaler of 8
    TIMSK1 = (1 << OCIE1A);         // Enable Output Compare A Match Interrupt
    sei();                          // Enable the Global Interrupt Bit
    
    // Cloning Game Map
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            playerMap[i][j] = gameMap[i][j];

    bool active = true;
    
    while(1)
    {           
        check_ships();
        firstDigitOfShots = shots / 10;
        secondDigitOfShots = shots % 10;
                
        // UP Button - PORTC1
        if(!(PINC & (1 << 1)) && active) 
        {
            _delay_ms(200);
            move_up(orientation);
        }
        
        // DOWN Button - PORTC2
        if(!(PINC & (1 << 2)) && active) 
        {
            _delay_ms(200);
            move_down(orientation);
        }
        
        // X/Y Button - PORTC0
        if(!(PINC & (1 << 0)) && active) 
        {
            _delay_ms(200);
            orientation = !orientation;
        }

        // Replay Button - PORTC3
        if(!(PINC & (1 << 3)))
        {
            interrupts();
            _delay_ms(200);
            replay();
            if(!active){
                active = true;
            }
        }

        if(active && game_over()){
            active = false;
            noInterrupts();
            blink_game_over();
        }
    }
}

// LED COMPOUND SCANNING --------------------------------------------------------------------------------
volatile char digitCount = 1, msCount = 0;
ISR(TIMER1_COMPA_vect)
{
    sei();
    msCount++;
    if(msCount == 5) msCount = 0;
    off_digit(1);
    off_digit(2);
    off_digit(3);
    off_digit(4);
    off_digit(5);
    if(msCount < 4)
    {
        on_digit(digitCount);
        if(digitCount == 1) show_number(numberOfSunk);
        else if(digitCount == 2) show_number(numberOfHit);
        else if(digitCount == 3) show_number(firstDigitOfShots);
        else if(digitCount == 4) show_number(secondDigitOfShots);
        else if(digitCount == 5) show_coordinate(orientation);
        digitCount++;
        if(digitCount > 5) digitCount = 1;
    }
}

// SHOOT - PORTD2 -----------------------------------------------------------------------------------------
ISR(INT0_vect)
{
    shots--;
    hit();
    PORTB &= ~(1 << 5);
    _delay_ms(200);
}

ISR(USART_RX_vect)
{
    char ReceivedByte;          // Variable to store the data (1 byte) read from the register
    ReceivedByte = UDR0;        // Read the received byte value
    
    if(ReceivedByte == '0' || ReceivedByte == '1')
    {
        gameMap[col][row] = ReceivedByte;
        UDR0 = gameMap[col][row];
        col++;
    }
    
    else if(ReceivedByte == '\n')
    {
        row++;
        col = 0;
    }
}
