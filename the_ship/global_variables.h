volatile int player[2] = {0, 0};        // Row - Col
volatile char gameMap[8][8];
volatile int row, col;
volatile byte shots = 16;               // Shots left
volatile byte numberOfSunk = 0;
byte numberOfHit = 0; 
volatile char firstDigitOfShots, secondDigitOfShots;
bool orientation = true;                // true - col, false - row
