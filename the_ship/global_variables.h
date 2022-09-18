volatile int player[2] = {0, 0};                // Row - Col
volatile char gameMap[8][8] = {'\0'};
//char gameMap[8][8] = {'0','0','0','0','0','0','0','0',
//                      '0','1','1','0','0','0','0','0',
//                      '0','0','0','0','0','1','1','0',
//                      '0','0','0','0','0','0','0','0',
//                      '0','0','1','1','0','0','0','0',
//                      '0','0','0','0','0','0','0','0',
//                      '1','1','0','0','1','1','0','0',
//                      '0','0','0','0','0','0','0','0'};
volatile int row, col;
volatile int received = 0;
volatile char playerMap[8][8] = {'\0'};
volatile byte shots = 16;                        // Shots left
volatile byte numberOfSunk = 0;
byte numberOfHit = 0;
volatile char firstDigitOfShots, secondDigitOfShots;
bool orientation = true;
