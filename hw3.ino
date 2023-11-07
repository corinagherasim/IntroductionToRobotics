// Declare all the segments pins
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

const int segSize = 8;
int index = 0;

bool commonAnode = false;

const int pinSW = 2;  // digital pin connected to switch output
const int pinX = A0;  // A0 - analog pin connected to X output
const int pinY = A1;  // A1 - analog pin connected to Y output

int xValue = 0;
int yValue = 0;
int swValue;

byte swState = HIGH;
byte lastSwState = HIGH;

byte onState = HIGH;
byte dpState = LOW;

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte segmentsStates[segSize] = {
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};

int currentSegment = 7;
byte currentSegmentState = LOW;

int state = 1;

unsigned long lastDebounceTimeSw = 0;
unsigned long debounceDelaySw = 100;
unsigned long lastDebounceTimeJoy = 0;
unsigned long debounceDelayJoy = 200;
unsigned long longPress = 300;

const int minThresholdCenter = 350;
const int maxThresholdCenter = 700;
const int minThresholdMargin = 200;
const int maxThresholdMargin = 900;

bool joystickMove = true;
bool lastJoystickMove = false;
bool currentJoystickMove = false;

const int nrDirections = 4;
int up = 0;
int down = 1;
int right = 2;
int left = 3;
int move;

unsigned long previousBlink = 0;
const long intervalBlink = 300;

byte segmentsMovements[segSize][nrDirections] = {
  //U  D  L  R
  { 0, 6, 5, 1 },  // a  0
  { 0, 6, 5, 1 },  // b  1
  { 6, 3, 4, 7 },  // c  2
  { 6, 3, 4, 2 },  // d  3
  { 6, 3, 4, 2 },  // e  4
  { 0, 6, 5, 1 },  // f  5
  { 0, 3, 6, 6 },  // g  6
  { 7, 7, 2, 7 }   // dp 7
};

void setup() {
  // Initialize all the pins
  pinMode(pinSW, INPUT_PULLUP);
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  if (commonAnode == true) {
    onState = !onState;
  }
  Serial.begin(9600);
}

void loop() {
  
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swValue = digitalRead(pinSW);
  
  for (int i = 0; i < segSize; i++) {
    if (i == currentSegment) {
      continue;
    }

    digitalWrite(segments[i], segmentsStates[i]);
  }

  if (state == 1) {
    blink();
    joystickMoveState1();
  } else {
    segmentsStates[currentSegment] = !segmentsStates[currentSegment];
    digitalWrite(segments[currentSegment], segmentsStates[currentSegment]);
    state = 1;
  }

  press();
    
}

//if the joystick didn't move is fitted between the central thresholds
//if the joystick moved its values are bigger or smaller than the margin thresholds
void joystickMoveState1() {
  if ((minThresholdCenter <= xValue && xValue <= maxThresholdCenter) and (minThresholdCenter <= yValue && yValue <= maxThresholdCenter)) {
    joystickMove = false;
  }

  if (xValue < minThresholdMargin && joystickMove == false) {
    move = left;
    joystickMove = true;
  }

  if (xValue > maxThresholdMargin && joystickMove == false) {
    move = right;
    joystickMove = true;
  }

  if (yValue < minThresholdMargin && joystickMove == false) {
    move = down;
    joystickMove = true;
  }

  if (yValue > maxThresholdMargin && joystickMove == false) {
    move = up;
    joystickMove = true;
  }

  if (lastJoystickMove != joystickMove) {
    lastDebounceTimeJoy = millis();
  }

  if (millis() - lastDebounceTimeJoy > debounceDelayJoy) {
    if (joystickMove != currentJoystickMove) {
      currentJoystickMove = joystickMove;
      
      if (currentJoystickMove == true) {
        currentSegment = segmentsMovements[currentSegment][move];
      }
    }
  }

  lastJoystickMove = joystickMove;
}

void blink() {
  if (millis() - previousBlink >= intervalBlink) {
    previousBlink = millis();
    digitalWrite(segments[currentSegment], currentSegmentState);
    currentSegmentState = !currentSegmentState;
  }
}

//checks if its a long press or not
//if it is not then it changes to state 2 which in loop makes the segment the different from before (from LOW to HIGH or from HIGH to LOW)
//if it is then resets
void press() {
  if (swValue == LOW and lastSwState == HIGH) {
    lastDebounceTimeSw = millis();
  }

  if (millis() - lastDebounceTimeSw > debounceDelaySw) {
    if (swValue != swState) {
      swState = swValue;

      if (swState == HIGH) {
        unsigned long pressDuration = millis() - lastDebounceTimeSw;

        if (pressDuration < longPress) { 
          if (state == 1) {
            state = 2;
          } else if (state == 2) {
            state = 1;
          }
        } 
        else {
          if (state == 1) {
            reset();
          }
        }
      }
    }
  }
  
  lastSwState = swValue;
}

//makes all the segments LOW
void reset() {
  for (int i = 0; i < segSize; i++) {
    segmentsStates[i] = LOW;
  }

  currentSegment = 7;
}
