#include <LedControl.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>


const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const byte v0 = 3;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int lcdWidth = 16;

//all of these below are from hw6
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;

byte swState = LOW;
byte lastSwState = LOW;

int xValue = 0;
int yValue = 0;

const int minThresholdCenter = 350;
const int maxThresholdCenter = 700;
const int minThresholdMargin = 200;
const int maxThresholdMargin = 900;

const int ledPin = 13;

bool joystickMove = true;
bool lastJoystickMove = false;
bool currentJoystickMove = false;

unsigned long lastDebounceTimeJoy = 0;
unsigned long debounceDelayJoy = 200;

unsigned long lastDebounceTimeButton = 0;
unsigned long debounceDelayButton = 50;

int playerRow = 0;
int playerCol = 0;
int bombRow = 0;
int bombCol = 0;

unsigned long lastBlinkTimePlayer = 0;
unsigned long blinkIntervalPlayer = 500;
bool isPlayerLedOn = true;

unsigned long lastBlinkTimeBomb = 0;
unsigned long blinkIntervalBomb = 100;
bool isBombLedOn = true;
unsigned long bombStartTime = 0;
unsigned long bombDelay = 3000;
unsigned long elapsedTime;
bool bombActive = false;

bool gameInProgress = false;
int n = 0;
int lives = 3;

const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

bool walls[matrixSize][matrixSize];

const int lcdBrightnessAddr = 0;       // EEPROM address for storing LCD brightness
const int matrixBrightnessAddr = 1;    // EEPROM address for storing matrix brightness

//inspired by surdubob Github
enum menuStates {
  START_GAME, SETTINGS, ABOUT
} menuState;

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

byte arrow[8] = {
  0b00000000,
  0b00000000,
  0b00000100,
  0b00000010,
  0b00111111,
  0b00000010,
  0b00000100,
  0b00000000
};

void setup() {
  Serial.begin(9600);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(ledPin, INPUT);
  displayMaze();
  analogWrite(v0, 100);
  lcd.begin(16, 2);
  centerTextOnLcd(F("Welcome to"), 0);
  centerTextOnLcd(F("Bomberman"), 1);
  delay(3000);
  lcd.clear();

  lc.shutdown(0,false);
  lc.setIntensity(0, 8);

  lcd.createChar(0, heart);//used for lives
  lcd.createChar(1, arrow);//used for menu
  
  menuState = START_GAME;
}

void loop() {
  readJoystickValues();
  //if the game is not in progress it shows the menu but if it is it uses the functions for the game and it shows on the lcd the lives i have left to play (maximum 3 and when at 0 the game stops)
  if (!gameInProgress) {
    joystickMoveMenu();
    checkButtonPress();
  } else {
    lcd.clear();
    int startColumn = (lcdWidth - lives) / 2;
    for (int i = 0; i<lives; i++){
      centerTextOnLcd(F("Lives:"), 0);
      lcd.setCursor(startColumn + i, 1);
      lcd.write((byte)0);
    }
    delay(100);
    joystickMoveMatrix();
    bomb();
  } 
}

void readJoystickValues() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swState = digitalRead(pinSW);
}

//printing main menu depending on the menu state which can be changed with the joystick
void mainMenu(int menuState){
  displayMaze();
  lcd.clear();
  switch(menuState){
    case START_GAME:{
      lcd.setCursor(1, 0);
      lcd.write(byte(1));
      centerTextOnLcd(F("Start game"), 0);
      break;
    }
    
    case SETTINGS:{
      lcd.setCursor(1, 0);
      lcd.write(byte(1));
      centerTextOnLcd(F("Settings"), 0);
      break;
    }
    
    case ABOUT:{
      lcd.setCursor(1, 0);
      lcd.write(byte(1));
      centerTextOnLcd(F("About"), 0);
      break;
    }
  }
}

//checks if the button is pressed on the menu and if it is i put what it's supposed to do
void checkButtonPress() {
  if (swState != lastSwState) {
    if (swState == LOW && (millis() - lastDebounceTimeButton > debounceDelayButton)) {
      switch (menuState) {
        case START_GAME:
          Serial.println("Button pressed in START_GAME state");
          lcd.setCursor(1, 0);
          lcd.write(byte(1));
          centerTextOnLcd(F("Game starting..."), 0);
          delay(2000);
          generateRandomMap();
          resetPlayer();
          readJoystickValues();
          joystickMoveMatrix();
          bomb();
          gameInProgress = true;
          break;
        case SETTINGS:
          displayMaze();
          Serial.println("Button pressed in SETTINGS state");
          adjustBrightness();
          break;
        case ABOUT:
          displayMaze();
          Serial.println("Button pressed in ABOUT state");
          lcd.setCursor(1, 0);
          lcd.write(byte(1));      
          displayScrollingText(F("Game: Bomberman made by Corina Gherasim"), 0);
          displayScrollingText(F("GitHub: corinagherasim"), 1);
          break;
      }
      lastDebounceTimeButton = millis();
    }
    lastSwState = swState;
  }
}


//moving the menu with the joystick
void joystickMoveMenu() {
  if ((minThresholdCenter <= xValue && xValue <= maxThresholdCenter) && (minThresholdCenter <= yValue && yValue <= maxThresholdCenter)) {
    joystickMove = true;
  }

  if (xValue > maxThresholdMargin && joystickMove) {
    if(menuState == START_GAME){
      menuState = SETTINGS;
    }
    else if(menuState == SETTINGS){
      menuState = ABOUT;
    }
    joystickMove = false;  
  }

  if (xValue < minThresholdMargin && joystickMove) {
    if(menuState == ABOUT){
      menuState = SETTINGS;
    }
    else if(menuState == SETTINGS){
      menuState = START_GAME;
    }
    joystickMove = false;  
  }

  mainMenu(menuState);

  if (lastJoystickMove != joystickMove) {
    lastDebounceTimeJoy = millis();
  }

  if (millis() - lastDebounceTimeJoy > debounceDelayJoy) {
    if (joystickMove != currentJoystickMove) {
      currentJoystickMove = joystickMove;
    }
  }

  lastJoystickMove = joystickMove;
  delay(50);
}

//adjust brightness on lcd and matrix and i use it when i press the menu in settings
void adjustBrightness() {
  lcd.clear();
  centerTextOnLcd(F("Adjust Brightness"), 0);
  centerTextOnLcd(F("LCD"), 1);

  // LCD brightness adjustment
  int lcdBrightness = readBrightnessFromSerial();
  analogWrite(v0, lcdBrightness);

  centerTextOnLcd(F("Matrix"), 1);
  // Matrix brightness adjustment
  int matrixBrightness = readBrightnessFromSerial();
  lc.setIntensity(0, matrixBrightness / 16); // Map analogRead range (0-255) to LedControl intensity range (0-15)

  // Save brightness values to EEPROM
  EEPROM.write(lcdBrightnessAddr, lcdBrightness);
  EEPROM.write(matrixBrightnessAddr, matrixBrightness);

  lcd.clear();
  Serial.println("Brightness Saved");
  centerTextOnLcd(F("Brightness Saved"), 0);
  delay(2000);
  lc.clearDisplay(0); // Clear the matrix display after saving brightness
}

int readBrightnessFromSerial() {
  while (!Serial.available()); 
  int brightness = Serial.parseInt(); 
  brightness = constrain(brightness, 0, 255); 
  return brightness;
}

//taken from surdubob Github
void centerTextOnLcd(String text, short line) {
  short l = text.length();
  short spaces = (lcdWidth - l) / 2;
  lcd.setCursor(spaces, line);
  lcd.print(text);
}

void displayScrollingText(String text, short line) {
  short l = text.length();
  short spaces = (lcdWidth - l) / 2;
  for (short i = 0; i <= l; ++i) {
    lcd.setCursor(spaces, line);
    lcd.print(text.substring(i));
    delay(450); 
    lcd.clear();
  }
}

//copied from homework #3 and changed so that it fits this homework
//apart from checking the movement of the joystick it also checks if the position you want to move the player to is in the walls array or not
void joystickMoveMatrix() {
  if ((minThresholdCenter <= xValue && xValue <= maxThresholdCenter) && (minThresholdCenter <= yValue && yValue <= maxThresholdCenter)) {
    joystickMove = false;
  }

  if (xValue > maxThresholdMargin && joystickMove == false && playerCol > 0 && !walls[playerRow][playerCol - 1]) {
  movePlayer(playerRow, playerCol, playerRow, playerCol - 1);
  }
  
  if (xValue < minThresholdMargin && joystickMove == false && playerCol < matrixSize - 1 && !walls[playerRow][playerCol + 1]) {
    movePlayer(playerRow, playerCol, playerRow, playerCol + 1);
  }
  
  if (yValue < minThresholdMargin && joystickMove == false && playerRow < matrixSize - 1 && !walls[playerRow + 1][playerCol]) {
    movePlayer(playerRow, playerCol, playerRow + 1, playerCol);
  }
  
  if (yValue > maxThresholdMargin && joystickMove == false && playerRow > 0 && !walls[playerRow - 1][playerCol]) {
    movePlayer(playerRow, playerCol, playerRow - 1, playerCol);
  }

  if (lastJoystickMove != joystickMove) {
    lastDebounceTimeJoy = millis();
  }

  if (millis() - lastDebounceTimeJoy > debounceDelayJoy) {
    if (joystickMove != currentJoystickMove) {
      currentJoystickMove = joystickMove;
    }

    if (currentJoystickMove == true) {
      lc.setLed(0, playerRow, playerCol, true);
    }
  }

  if (!currentJoystickMove) {
    // Blink the player LED when not moving
    if (millis() - lastBlinkTimePlayer > blinkIntervalPlayer) {
      isPlayerLedOn = !isPlayerLedOn;
      lc.setLed(0, playerRow, playerCol, isPlayerLedOn);
      lastBlinkTimePlayer = millis();
    }
  }

  if (playerRow == matrixSize - 1 && playerCol == matrixSize - 1) {
    n+=1;
    if(n == 2){
      displayCup();
      lcd.clear();
      centerTextOnLcd(F("Congrats!"), 0);
      delay(3000);
      gameInProgress = false;
      n = 0;
    }
    else{
      generateRandomMap();
      resetPlayer(); 
    }
    
  }

  lastJoystickMove = joystickMove;
}

//change the position of player
void movePlayer(int currentRow, int currentCol, int newRow, int newCol) {
  lc.setLed(0, currentRow, currentCol, false);
  lc.setLed(0, newRow, newCol, true);
  playerRow = newRow;
  playerCol = newCol;
  joystickMove = true;
}

void resetPlayer() {
  lc.setLed(0, 0, 0, true);
  playerRow = 0;
  playerCol = 0;
}

//when i press the button it makes a bomb on the position of the player that destroys the wall from above, below, left and right
//there is a buzzer that is on and the bomb is blinking for 3 seconds before exploding
//the player should move from the spot of the bomb before exploding or the game will restart
//there is also a led that warns the player that is in the spot of the bomb
void bomb() {
  // Read joystick press state
  swState = digitalRead(pinSW);

  if (swState != lastSwState) {
    lastDebounceTimeButton = millis();
  }

  if (millis() - lastDebounceTimeButton > debounceDelayButton) {
    if (swState == LOW && !lastSwState) { 
      if (!bombActive) {
        bombStartTime = millis();
        bombActive = true;
        bombRow = playerRow; 
        bombCol = playerCol;
        isBombLedOn = true;
      }
    }
  }

  if (bombActive) {
    if (
        (playerRow == bombRow && playerCol == bombCol) ||
        (playerRow == bombRow - 1 && playerCol == bombCol) ||
        (playerRow == bombRow + 1 && playerCol == bombCol) ||
        (playerRow == bombRow && playerCol == bombCol - 1) ||
        (playerRow == bombRow && playerCol == bombCol + 1)
         ) {
        digitalWrite(ledPin, HIGH);
      } else{
        digitalWrite(ledPin, LOW);
      }
    elapsedTime = millis() - bombStartTime;
    if (elapsedTime >= bombDelay) {
      if (bombRow > 0) {
        walls[bombRow - 1][bombCol] = false; // up
        lc.setLed(0, bombRow - 1, bombCol, false);
      }
      if (bombRow < matrixSize - 1) {
        walls[bombRow + 1][bombCol] = false; // down
        lc.setLed(0, bombRow + 1, bombCol, false);
      }
      if (bombCol > 0) {
        walls[bombRow][bombCol - 1] = false; // left
        lc.setLed(0, bombRow, bombCol - 1, false);
      }
      if (bombCol < matrixSize - 1) {
        walls[bombRow][bombCol + 1] = false; // right
        lc.setLed(0, bombRow, bombCol + 1, false);
      }

      // Turn off bomb LED after exploding
      lc.setLed(0, bombRow, bombCol, false);
      bombActive = false;

      if (
        (playerRow == bombRow && playerCol == bombCol) ||
        (playerRow == bombRow - 1 && playerCol == bombCol) ||
        (playerRow == bombRow + 1 && playerCol == bombCol) ||
        (playerRow == bombRow && playerCol == bombCol - 1) ||
        (playerRow == bombRow && playerCol == bombCol + 1)
         ) {
        // restart game
        generateRandomMap();
        resetPlayer();
        n = 0;
        lives-=1;
        digitalWrite(ledPin, LOW);
        if(lives == 0){
          lcd.clear();
          displayX();
          centerTextOnLcd(F("Game over!"), 0);
          delay(3000);
          gameInProgress = false;
          lives = 3;
        }
      }
    } else {
      if (millis() - lastBlinkTimeBomb > blinkIntervalBomb) {
        isBombLedOn = !isBombLedOn;
        lc.setLed(0, bombRow, bombCol, isBombLedOn);
        lastBlinkTimeBomb = millis();
      }
    }
  }

  lastSwState = swState;
}

//generates wall on maximum 75% of the map
//the walls are put in an array
void generateRandomMap() {
  lc.clearDisplay(0);

  int wallPercentage = 65;

  for (int i = 0; i < matrixSize; ++i) {
    for (int j = 0; j < matrixSize; ++j) {
      int randomNumber = random(100);

      if (i == 0 && j == 0) {
        lc.setLed(0, i, j, true);
      } else if (i == 1 && j == 0) {
        lc.setLed(0, i, j, false);
        walls[i][j] = false;
      } else if (i == 0 && j == 1) {
        lc.setLed(0, i, j, false);
        walls[i][j] = false;
      } else if (i == 1 && j == 1) {
        lc.setLed(0, i, j, false);
        walls[i][j] = false;
      } else {
        walls[i][j] = randomNumber < wallPercentage;
        if (walls[i][j]) {
          lc.setLed(0, i, j, true);
        }
      }
    }
  }
}

int getUserInput() {
  while (!Serial.available()); // Wait for user input
  return Serial.parseInt();    // Read user input as an integer
}

void displayCup() {
  byte cup[8] = {
    B00111100,
    B00011000,
    B00011000,
    B00111100,
    B11111111,
    B10111101,
    B11111111,
    B00111100

  };

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      lc.setLed(0, row, col, bitRead(cup[row], 7 - col));
    }
  }
}

void displayX() {
  byte cup[8] = {
    B11111111,
    B11000011,
    B10100101,
    B10011001,
    B10011001,
    B10100101,
    B11000011,
    B11111111

  };

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      lc.setLed(0, row, col, bitRead(cup[row], 7 - col));
    }
  }
}

void displayMaze() {
  byte cup[8] = {
    B01010101,
    B10101010,
    B01010101,
    B10101010,
    B01010101,
    B10101010,
    B01010101,
    B10101010
  };

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      lc.setLed(0, row, col, bitRead(cup[row], 7 - col));
    }
  }
}
