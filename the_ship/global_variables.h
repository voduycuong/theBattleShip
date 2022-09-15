volatile int player[2] = {0, 0};                // Row - Col
//volatile char gameMap[8][8] = {'\0'};
char gameMap[8][8] = {'0','0','0','0','0','0','0','0',
                      '0','1','1','0','0','0','0','0',
                      '0','0','0','0','0','1','1','0',
                      '0','0','0','0','0','0','0','0',
                      '0','0','1','1','0','0','0','0',
                      '0','0','0','0','0','0','0','0',
                      '1','1','0','0','1','1','0','0',
                      '0','0','0','0','0','0','0','0'};
volatile int row, col;
volatile int received = 0;
volatile char playerMap[8][8] = {'\0'};
volatile int shots = 16;                        // Shots left
volatile int numberOfSunk = 0;
volatile int numberOfHit = 0;
volatile char firstDigitOfShots, secondDigitOfShots;
bool orientation = true;
