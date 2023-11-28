#include "LedControl.h"

const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;
byte swState = LOW;
int xValue = 0;
int yValue = 0;

const int buttonPin = 4;
bool buttonState = false;
bool lastButtonState = false;

const int buzzerPin = 8;  
const int ledPin = 7;

const int minThresholdCenter = 350;
const int maxThresholdCenter = 700;
const int minThresholdMargin = 200;
const int maxThresholdMargin = 900;

bool joystickMove = true;
bool lastJoystickMove = false;
bool currentJoystickMove = false;

unsigned long lastDebounceTimeJoy = 0;
unsigned long debounceDelayJoy = 200;

unsigned long lastDebounceTimeButton = 0;
unsigned long debounceDelayButton = 50;

const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
byte matrixBrightness = 2;

int playerRow = 0;
int playerCol = 0;
int bombRow = 0;
int bombCol = 0;

bool walls[matrixSize][matrixSize];

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

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);
  generateRandomMap();
}

void loop() {
  readJoystickValues();
  joystickMoveMatrix();
  bomb();
}

void readJoystickValues() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
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
    generateRandomMap();
    resetPlayer();
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
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    lastDebounceTimeButton = millis();
  }

  if (millis() - lastDebounceTimeButton > debounceDelayButton) {
    if (buttonState == LOW && !lastButtonState) { 
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
    tone(buzzerPin, 400, 100);
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
        digitalWrite(ledPin, LOW);
      }
    } else {
      if (millis() - lastBlinkTimeBomb > blinkIntervalBomb) {
        isBombLedOn = !isBombLedOn;
        lc.setLed(0, bombRow, bombCol, isBombLedOn);
        lastBlinkTimeBomb = millis();
      }
    }
  }

  lastButtonState = buttonState;
}

//generates wall on maximum 75% of the map
//the walls are put in an array
void generateRandomMap() {
  lc.clearDisplay(0);

  int wallPercentage = 75;

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
