const int floorLeds[3] = {9, 10, 11}; 
const int buzzerPin = 8;             
const int callButtons[3] = {2, 3, 4};  
const int operationalLed = 13;        

int currentFloor = 0;
int targetFloor = -1;
bool doorsOpen = false;

bool buttonState[3] = {false, false, false};
bool lastButtonState[3] = {false, false, false};
unsigned long lastDebounceTime[3] = {0, 0, 0};
unsigned long debounceDelay = 50;

unsigned long lastTransitionTime = 0;
unsigned long floorTransitionDuration = 5000;
unsigned long operationalLedBlinkInterval = 500; 
bool operationalLedState = LOW;
unsigned long lastOperationalLedToggle = 0;

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(floorLeds[i], OUTPUT);
    pinMode(callButtons[i], INPUT_PULLUP);
  }
  pinMode(buzzerPin, OUTPUT);
  pinMode(operationalLed, OUTPUT);

  // Initially set the elevator at the first floor
  digitalWrite(floorLeds[0], HIGH);
}

void loop() {
  for (int i = 0; i < 3; i++) {
    buttonState[i] = digitalRead(callButtons[i]);

    if (buttonState[i] != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }

    if (millis() - lastDebounceTime[i] > debounceDelay) {
      if (buttonState[i] == LOW && i != currentFloor) {
        targetFloor = i;
        soundBuzzer(2);
      }
    }

    lastButtonState[i] = buttonState[i];
  }

  if (currentFloor != targetFloor) {
    if (millis() - lastTransitionTime > floorTransitionDuration) {
      lastTransitionTime = millis();
      if (currentFloor < targetFloor) {
        currentFloor++;
        updateFloorIndicator(currentFloor);
        operationalLedState = LOW;
      } else {
        currentFloor--;
        updateFloorIndicator(currentFloor);
        operationalLedState = LOW;
      }
      soundBuzzer(2);
    }
  } else {
    if (!doorsOpen) {
      doorsOpen = true;
      soundBuzzer(1); 
      operationalLedState = HIGH;
    }
  }

  // Toggle the operational LED based on the interval
  if (currentFloor != targetFloor && millis() - lastOperationalLedToggle > operationalLedBlinkInterval) {
    operationalLedState = !operationalLedState;
    digitalWrite(operationalLed, operationalLedState);
    lastOperationalLedToggle = millis();
  } else if (currentFloor == targetFloor) {
    digitalWrite(operationalLed, HIGH);
  }
}

void updateFloorIndicator(int floor) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(floorLeds[i], (i == floor) ? HIGH : LOW);
  }
}

void soundBuzzer(int soundType) {
  if (soundType == 1) {
    tone(buzzerPin, 400, 100);
  } else if (soundType == 2) {
    tone(buzzerPin, 800, 100);
  }
}
