// Define connections to the shift register
const int latchPin = 11;
const int clockPin = 10;
const int dataPin = 12;

const int pinLed = 8;
byte ledState = LOW;

// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

// Define pins for buttons
const int pinButtonStart = 2;
const int pinButtonReset = 3;
const int pinButtonSave = 13;

// To check if a button is pressed
byte buttonStateStart = LOW;
byte buttonStateReset = LOW;
byte buttonStateSave = LOW;

byte prevbuttonStateStart = LOW;
byte prevbuttonStateReset = LOW;
byte prevbuttonStateSave = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;

// Store the digits in an array for easy access
int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = 4;

// Define the number of unique encodings (0-9, A-F for hexadecimal)
const int encodingsNumber = 16;
// Define byte encodings for the hexadecimal characters 0-F
byte byteEncodings[encodingsNumber] = {
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};

// Variables for controlling the display update timing
unsigned long lastIncrement = 0;
unsigned long delayCount = 50; // Delay between updates (milliseconds)
unsigned long number = 0; // The number being displayed

// Add variables to indicate whether the timer is paused or not and when is it paused
bool timerPaused = true;
unsigned long pausedTime = 0;

void setup() {
  // Initialize the pins connected to the shift register as inputs
  pinMode(pinButtonStart, INPUT_PULLUP);
  pinMode(pinButtonReset, INPUT_PULLUP);
  pinMode(pinButtonSave, INPUT_PULLUP);
  pinMode(pinLed, INPUT_PULLUP);

  // Initialize the pins connected to the shift register as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(pinLed, OUTPUT);

  // Initialize digit control pins and set them to LOW (off)
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  // Begin serial communication for debugging purposes
  Serial.begin(9600);
}

void loop() {
  buttonStateStart = digitalRead(pinButtonStart);
  buttonStateReset = digitalRead(pinButtonReset);
  buttonStateSave = digitalRead(pinButtonSave);
  digitalWrite(pinLed, ledState);

  if (buttonStateStart != prevbuttonStateStart && prevbuttonStateStart != LOW) {
    // Toggle the timerPaused when the start button is pressed
    timerPaused = !timerPaused;

    // If the timer is paused the led is off and the paused time becomes 000.0 if it resets or the time we pressed pause
    if (timerPaused) {
      ledState = LOW;
      // If the timer is paused, store the current time
      pausedTime = number;
      
    }
  }
  prevbuttonStateStart = buttonStateStart;

  if (buttonStateReset != prevbuttonStateReset && prevbuttonStateReset != LOW) {
    ledState = LOW;
    // Reset both the timer and paused time to 0 when the reset button is pressed
    number = 0;
    pausedTime = 0;
    timerPaused = true;
  }
  prevbuttonStateReset = buttonStateReset;

  if (!timerPaused) {
    // If the timer is not paused the led is on
    ledState = HIGH;
    // DONE: Check if the current time is greater than 'lastIncrement' plus 'delayCount'
    if (millis() - lastIncrement > delayCount) {
      // DONE: Increment the number and reset 'lastIncrement'
      number++;
      // DONE: Ensure that 'number' wraps around after reaching 9999 to start over from 0
      number %= 10000; // Wrap around after 9999
      lastIncrement = millis();
    }
  }

  // DONE: Display the number on the 7-segment display using multiplexing
  writeNumber(number);
}

void writeReg(int digit) {
    // Prepare to shift data by setting the latch pin low
    digitalWrite(latchPin, LOW);
    // Shift out the byte representing the current digit to the shift register
    shiftOut(dataPin, clockPin, MSBFIRST, digit);
    // Latch the data onto the output pins by setting the latch pin high
    digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
    // Turn off all digit control pins to avoid ghosting
    for (int i = 0; i < displayCount; i++) {
      digitalWrite(displayDigits[i], HIGH);
    }
    // Turn on the current digit control pin
    digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number) {
  // DONE: Initialize necessary variables for tracking the current number and digit position
  int currentNumber;
  if (timerPaused) {
    currentNumber = pausedTime;
  } else {
    currentNumber = number;
  }
  int displayDigit = 3; // DONE: Start with the least significant digit
  int lastDigit = 0;

  // DONE: Loop through each digit of the current number
  while (currentNumber != 0) {
    // DONE: Extract the last digit of the current number
    lastDigit = currentNumber % 10;
    // DONE: Activate the current digit on the display
    activateDisplay(displayDigit);
    // DONE: Output the byte encoding for the last digit to the display
    // DONE: Checking if it's the second display digit to add the .
    if (displayDigit == 2)
      writeReg(byteEncodings[lastDigit] + 1);
    else
      writeReg(byteEncodings[lastDigit]);
    // DONE: Implement a delay if needed for multiplexing visibility
    delay(0); // DONE: A delay can be increased to visualize multiplexing
    // DONE: Move to the next digit
    displayDigit--;
    // DONE: Update 'currentNumber' by removing the last digit
    currentNumber /= 10;
    // DONE: Clear the display to prevent ghosting between digit activations
    writeReg(B00000000); // DONE: Clear the register to avoid ghosting
  }

  // DONE: fill the rest of the display with 0
  while (displayDigit >= 0) {
    // DONE: Activate the current digit on the display
    activateDisplay(displayDigit);
    // DONE: Output the byte encoding for 0 to the display
    if (displayDigit == 2)
      writeReg(byteEncodings[0] + 1);
    else
      writeReg(byteEncodings[0]);
    // DONE: Move to the next digit
    displayDigit--;
    // DONE: Clear the display to prevent ghosting between digit activations
    writeReg(B00000000);
  }
}
