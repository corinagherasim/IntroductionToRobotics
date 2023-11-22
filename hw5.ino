#include <EEPROM.h>

int selectedOption = 0; // Variable to store the user's selected option
int userInput = 0;
int confirmation = 0;

//pins for RGB led
const int ledR = 9;
const int ledG = 10;
const int ledB = 11;

//rgb led values
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

//value for choice from automatic on/off
int choiceAutomatic = 0;

//pins for distance sensor
const int trigPin = 5;
const int echoPin = 6;

//values used for distance sensor
long duration = 0;
int distance = 0;

//pin and value for photocell
int photocellPin = A0;
int photocellValue;
int averageReading; 
int totalReadings;  

unsigned long startTime;
unsigned long endTime;
unsigned long samplingDuration;

void setup() {
  Serial.begin(9600);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}


void loop() {
  printMainMenu();
  userInput = getUserInput();
  handleMainMenu(userInput);
}


void printMainMenu() {
  Serial.println("Main Menu:");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");
}

void printSensorSettingsMenu() {
  Serial.println("1. Sensor Settings:");
  Serial.println("  1. Sensors Sampling Interval");
  Serial.println("  2. Ultrasonic Alert Threshold");
  Serial.println("  3. LDR Alert Threshold");
  Serial.println("  4. Back");
}

void printResetLoggerDataMenu() {
  Serial.println("2. Reset Logger Data:");
  Serial.println("  1. Yes");
  Serial.println("  2. No");
  Serial.println("  3. Back");
}

void printSystemStatusMenu() {
  Serial.println("3. System Status:");
  Serial.println("  1. Current Sensor Readings");
  Serial.println("  2. Current Sensor Settings");
  Serial.println("  3. Display Logged Data");
  Serial.println("  4. Back");
}

void printRGBLEDControlMenu() {
  Serial.println("4. RGB LED Control:");
  Serial.println("  1. Manual Color Control");
  Serial.println("  2. LED: Toggle Automatic ON/OFF");
  Serial.println("  3. Back");
}

void handleMainMenu(int option){
  switch (option) {
    case 1:
      // User selected Sensor Settings
      selectedOption = 0;
      while (selectedOption != 4) {
        printSensorSettingsMenu();
        selectedOption = getUserInput();
        handleSensorSettingsOption(selectedOption);
      }
      break;
    case 2:
      // User selected Reset Logger Data
      selectedOption = 0;
      while (selectedOption != 3) {
        printResetLoggerDataMenu();
        selectedOption = getUserInput();
        handleResetLoggerDataOption(selectedOption);
      }
      break;
    case 3:
      // User selected System Status
      selectedOption = 0;
      while (selectedOption != 4) {
        printSystemStatusMenu();
        selectedOption = getUserInput();
        handleSystemStatusOption(selectedOption);
      }
      break;
    case 4:
      // User selected RGB LED Control
      selectedOption = 0;
      while (selectedOption != 3) {
        printRGBLEDControlMenu();
        selectedOption = getUserInput();
        handleRGBLEDControlOption(selectedOption);
      }
      break;
    default:
      Serial.println("Invalid option. Please try again.");
      break;
  } 
}

void handleSensorSettingsOption(int option) {
  switch (option) {
    case 1:
        // Implement Sensors Sampling Interval functionality
      Serial.println("Sensors Sampling Interval option selected");
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      // Sets the trigPin on HIGH state for 10 microseconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      // Sound wave reflects from the obstacle, so to calculate the distance we
      // consider half of the distance traveled.
      distance = duration * 0.034 / 2;
      // Prints the distance on the Serial Monitor
      Serial.print("Distance: ");
      Serial.println(distance);
      break;


    // I tried to take samples for a longer duration of time but its not working
    //  digitalWrite(trigPin, LOW);
    //  delayMicroseconds(2);
    //
    //  digitalWrite(trigPin, HIGH);
    //  delayMicroseconds(10);
    //  digitalWrite(trigPin, LOW);
    //
    //  startTime = millis(); // Record the start time
    //  samplingDuration = 10000; // Set the sampling duration to 10 seconds
    //
    //  while (millis() - startTime < samplingDuration) {
    //    duration = pulseIn(echoPin, HIGH, 30000); // Increase timeout (in microseconds) if needed
    //    distance = duration * 0.034 / 2;
    //
    //    // Only print valid distances
    //    if (distance != 0) {
    //      Serial.print("Distance: ");
    //      Serial.println(distance);
    //    }

    
    case 2:
      // Implement Ultrasonic Alert Threshold functionality
      Serial.println("Ultrasonic Alert Threshold option selected");
      if (choiceAutomatic == 1) {
          if (distance < 10) {
            analogWrite(ledR, 255);
            analogWrite(ledG, 0);
            startTime = millis(); // Record the start time

            while (millis() - startTime < 2000) {
              // Wait for 2 seconds without blocking the loop
            }
            analogWrite(ledR, 0);
            analogWrite(ledG, 255);
          } else {
            analogWrite(ledR, 0);
            analogWrite(ledG, 255);
          }
      }else{
        Serial.print("Automatic mode is OFF. Cannot update LEDs.");
      }
      break;
    case 3:
      // Implement LDR Alert Threshold functionality
      Serial.println("LDR Alert Threshold option selected");
      photocellValue= analogRead(photocellPin);  
 
      totalReadings = 0;
        for (int i = 0; i < 10; ++i) {
          photocellValue = analogRead(photocellPin);
          Serial.print("Analog reading ");
          Serial.print(i + 1);
          Serial.print(" = ");
          Serial.println(photocellValue);

          totalReadings += photocellValue;
          delay(100); // Introduce a short delay between readings
        }

        averageReading = totalReadings / 5;
        
      if (choiceAutomatic == 1) {
          if (averageReading < 100) {
            analogWrite(ledR, 255);
            analogWrite(ledG, 0);
            startTime = millis(); // Record the start time

            while (millis() - startTime < 2000) {
              // Wait for 2 seconds without blocking the loop
            }
            analogWrite(ledR, 0);
            analogWrite(ledG, 255);
          } else {
            analogWrite(ledR, 0);
            analogWrite(ledG, 255);
          }
      }else{
        Serial.print("Automatic mode is OFF. Cannot update LEDs.");
      }
      
      break;
    case 4:
      // Back to Main Menu
      break;
    default:
      Serial.println("Invalid option. Please try again.");
  }
}

void handleResetLoggerDataOption(int option) {
  switch (option) {
    case 1:
      // Implement Yes functionality
      Serial.println("Are you sure you want to reset?");
      Serial.println("1. Yes");
      Serial.println("2. No");

      confirmation = getUserInput();

      if (confirmation == 1) {
        Serial.println("Resetting...");
        redValue = 0;
        greenValue = 0;
        blueValue = 0;
        analogWrite(ledR, redValue);
        analogWrite(ledG, greenValue);
        analogWrite(ledB, blueValue);
        choiceAutomatic = 0;
        averageReading = 0;
        Serial.println("Reset completed.");

      } else {
        Serial.println("Reset canceled.");
      }
      break;

    case 2:
      // Implement No functionality
      Serial.println("No option selected");
      break;
    case 3:
      // Back to Main Menu
      break;
    default:
      Serial.println("Invalid option. Please try again.");
  }
}

void handleSystemStatusOption(int option) {
  switch (option) {
    case 1:
      // Implement Current Sensor Readings functionality
      Serial.println("Current Sensor Readings option selected");
      break;
    case 2:
      // Implement Current Sensor Settings functionality
      Serial.println("Current Sensor Settings option selected");
      break;
    case 3:
      // Implement Display Logged Data functionality
      Serial.println("Display Logged Data option selected");
      break;
    case 4:
      // Back to Main Menu
      break;
    default:
      Serial.println("Invalid option. Please try again.");
  }
}

void handleRGBLEDControlOption(int option) {
  switch (option) {
    case 1:
      // Implement Manual Color Control functionality
      Serial.println("Manual Color Control option selected");
      choiceAutomatic = 0;
      Serial.println("Choose a value from 0 to 255 for red:");
      redValue = getUserInput();
      if (redValue > 255) {
        redValue = 255;
      }
      Serial.print(redValue);
      Serial.print('\n');

      Serial.println("Choose a value from 0 to 255 for green:");
      greenValue = getUserInput();
      if (greenValue > 255) {
        greenValue = 255;
      }
      Serial.print(greenValue);
      Serial.print('\n');

      Serial.println("Choose a value from 0 to 255 for blue:");
      blueValue = getUserInput();
      if (blueValue > 255) {
        blueValue = 255;
      }
      Serial.print(blueValue);
      Serial.print('\n');

      analogWrite(ledR, redValue);
      analogWrite(ledG, greenValue);
      analogWrite(ledB, blueValue);
      break;
    case 2:
      // Implement LED: Toggle Automatic ON/OFF functionality
      Serial.println("LED: Toggle Automatic ON/OFF option selected");
      Serial.println("Choose 0 FOR OFF and 1 for ON:");
      choiceAutomatic = getUserInput();
      if (choiceAutomatic == 0) {
        Serial.println("OFF");
        analogWrite(ledR, redValue);
        analogWrite(ledG, greenValue);
        analogWrite(ledB, blueValue);
      } else if (choiceAutomatic == 1) {
        Serial.println("ON");
        analogWrite(ledR, 0);
        analogWrite(ledB, 0);
        analogWrite(ledG, 255);
      } else {
        Serial.println("Invalid option. Please choose 0 or 1");
      }
      break;
    case 3:
      // Back to Main Menu
      break;
    default:
      Serial.println("Invalid option. Please try again.");
  }
}

// Simulate user input (replace this with your actual input method)
int getUserInput() {
  while (!Serial.available()); // Wait for user input
  return Serial.parseInt();    // Read user input as an integer
}
