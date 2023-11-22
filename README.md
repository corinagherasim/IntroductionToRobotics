# Introduction To Robotics (2023-2024)

Introduction to Robotics laboratory homeworks, taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework includes requirements, implementation details, code and image file and other things depending on the case. They will have a description written down below:

# Homework #1: Controlling an RGB Led </strong>

### Components Used:

* 1x RGB Led
* 3x Potentiometers
* 1x Arduino Uno
* 1x Half breadboard
* 3x 220&#8486; rezistors
* many wires

### Technical Task:
Use a separate potentiometer for controlling each color of the RGB LED:Red,Green, andBlue.  This control must leveragedigital electronics.  Specifically,you  need  to  read  the  potentiometer’s  value  with  Arduino  and  then  write  amapped value to the LED pins.

### Wokwi Implementation: <a href= "https://wokwi.com/projects/379391396363897857"> Click here </a>

### Youtube link presenting the homework: <a href= "https://youtube.com/shorts/TKrzJHekS78?si=Onj5sHFCqPesFDtI"> Click here </a>

### Photos:
<p float = "left">
<image src = "https://github.com/corinagherasim/IntroductionToRobotics/assets/94368761/94d7bab1-aed2-4021-971a-0569af257e3d" width="49%" height="500">
<image src = "https://github.com/corinagherasim/IntroductionToRobotics/assets/94368761/605db853-76f4-482d-bd08-2b4b7a755bf4" width="49%" height="500">
</p>

# Homework #2: Elevator simulator wannabe </strong>

### Components Used:

* 4x Leds
* 1x Arduino Uno
* 1x Half breadboard
* 3x Buttons
* 1x Buzzer
* 3x 220&#8486; rezistors
* many wires

### Technical Task:
Design  a  control  system  that  simulates  a  3-floor  elevator  using  the  Arduinoplatform.  Here are the specific requirements:
* LED Indicators: Each of the 3 LEDs should represent one of the 3 floors.The LED corresponding to the current floor should light up.  Additionally,another LED should represent the elevator’s operational state.  It should blink when the elevator is moving and remain static when stationary.
* Buttons: Implement 3 buttons that represent the call buttons from the3 floors.  When pressed, the elevator should simulate movement towardsthe floor after a short interval (2-3 seconds).
* Buzzer: The buzzer should sound briefly during the following scenarios:
  –Elevator arriving at the desired floor (something resembling a ”cling”).
  –Elevator  doors  closing  and  movement  (pro  tip:  split  them  into  2different sounds
* State Change & Timers: If the elevator is already at the desired floor,pressing the button for that floor should have no effect.  Otherwise, aftera button press, the elevator should ”wait for the doors to close” and then”move”  to  the  corresponding  floor.   If  the  elevator  is  in  movement,  itshould either do nothing or it should stack its decision (get to the firstprogrammed floor,  open the doors,  wait, close them and then go to thenext desired floor).
* Debounce: Remember to implement debounce for the buttons to avoidunintentional repeated button presses.
  
### Wokwi Implementation: <a href= "https://wokwi.com/projects/380141359615915009"> Click here </a>

### Youtube link presenting the homework: <a href= "https://youtube.com/shorts/oiq6_3mm_4U?si=GGP5qF4VtxzpSHbG"> Click here </a>

### Photos:
<p float = "left">
<image src = "https://github.com/corinagherasim/IntroductionToRobotics/assets/94368761/b38acdc8-ab6c-49d4-8af7-53e992c682b7" width="49%" height="500">
<image src = "https://github.com/corinagherasim/IntroductionToRobotics/assets/94368761/c7eb517b-4318-493b-bff0-fa7e81927b0b" width="49%" height="500">
</p>

# Homework #3: 7 segment display drawing </strong>

### Components Used:

* 1x 7 segment display
* 1x Joystick
* 1x Arduino Uno
* 1x Half breadboard
* 8x 220&#8486; rezistors
* many wires

### Technical Task:
You will use the joystick to control the position ofthe segment and ”draw” on the display.  The movement between segmentsshould be natural, meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”.
The  initial  position  should  be  on  the  DP.  The  currentposition always blinks (irrespective of the fact that the segment is on oroff).  Use the joystick to move from one position to neighbors (see table forcorresponding movement).  Short pressing the button toggles the segmentstate  from  ON  to  OFF  or  from  OFF  to  ON.  Long  pressing  the  buttonresets the entire display by turning all the segments OFF and moving thecurrent position to the decimal point.

### The moves for each segment:
<p float = "left">
<image src = "https://github.com/corinagherasim/IntroductionToRobotics/assets/94368761/b64e3250-a3ff-4ae3-849b-a10e6f659414" width="69%" height="500">
<image src = "https://github.com/corinagherasim/IntroductionToRobotics/assets/94368761/3c6fa0c9-a268-4961-8cfd-13e29018494c" width="29%" height="500">
</p>

### Youtube link presenting the homework: <a href= "https://youtube.com/shorts/HsakrfMFFGg?si=RIqajCSwfyVJIkH9"> Click here </a>

### Photos:
<p float = "left">
<image src = "https://github.com/corinagherasim/IntroductionToRobotics/assets/94368761/cf49ff0c-7686-44d8-838a-ff1c919c1ca5" width="49%" height="500">
<image src = "https://github.com/corinagherasim/IntroductionToRobotics/assets/94368761/a03f1342-75a6-4686-9b86-9bb5232f78b6" width="49%" height="500">
</p>

# Homework #4: Stopwatch Timer </strong>

### Components Used:

* 1x 4-digit 7 segment display
* 1x Arduino Uno
* 1x Half breadboard
* 3x Buttons
* 1x Led
* 1x Shift Register: 74HC595
* 8x 220&#8486; rezistors
* many wires

### Technical Task:
Using the 4 digit 7 segment display and 3 buttons,you should implement a stopwatch timer that counts in 10ths of a second and has a save lap functionality (similar to most basic stopwatch functions on most phones).The starting value of the 4 digit 7 segment display should be ”000.0”.  Your buttons should have the following functionalities:
* Button 1:  Start / pause;
* Button  2:   Reset  (if  in  pause  mode).   Reset  saved  laps  (if  in  lapviewing mode);
* Button 3:  Save lap (if in counting mode), cycle through last savedlaps (up to 4 laps).

### Youtube link presenting the homework: <a href= "https://youtube.com/shorts/pwVVdbKgEe4?si=tifyem3schArTnf6"> Click here </a>

### Photos:
<p float = "left">
<image src = "https://github.com/corinagherasim/IntroductionToRobotics/assets/94368761/8eea63f3-3377-4143-b24a-5e0d6b578273" width="49%" height="500">
<image src = "https://github.com/corinagherasim/IntroductionToRobotics/assets/94368761/8e2fb165-4863-4622-a1d3-569e7ea12055" width="49%" height="500">
</p>

# Homework #5: Pseudo-Smart Environment Monitor and Logger </strong>

### Components Used:

* 1x LDR (Light-Dependent Resistor) aka Photocell aka Photoresistor aka LightSensor
* 1x Ultrasonic Sensor (HC-SR04)
* 1x Arduino Uno
* 1x Half breadboard
* 1x RGB Led
* 2x 5k&#8486; rezistors
* many wires

### Technical Task:
Develop a ”Smart Environment Monitor and Logger” using Arduino.  This sys-tem will utilize various sensors to gather environmental data, log this data intoEEPROM, and provide both visual feedback via an RGB LED and user interac-tion through a Serial Menu.  The project focuses on integrating sensor readings,memory management, Serial Communication and the general objective of build-ing a menu.

### Menu Structure:
1. Sensor Settings// Go to submenu
   <ul>
   <li>1.1 Sensors Sampling Interval.  Here you should be prompted fora value between 1 and 10 seconds.  Use this value as a sampling ratefor the sensors.  You can read a separate value for each or have thesame for both.
   <li>1.2 Ultrasonic Alert Threshold.  Here you should be promptedfor a threshold value for the ultrasonic sensor.  You can decide if thatis the min or max value (you can signal that something is too close).When sensor value exceeds the threshold value,  an alert should begiven.  This can be in the form of a message.  If the LED is set toAutomatic Mode (see section 4.2), it should also turn red if any ofthe sensors are outside the value.
   <li>1.3 LDR Alert Threshold.  Here you should be prompted for athreshold  value  for  the  LDR  sensor.   You  can  decide  if  that  is  themin or max value (for example, it could signal that night is coming).When sensor value exceeds the threshold value,  an alert should begiven.  This can be in the form of a message.  If the LED is set toAutomatic Mode (see section 4.2), it should also turn red if any ofthe sensors are outside the value.
   <li>1.4 Back// Return to main menu
   </ul>
2. Reset Logger Data. Should  print  a  message,  promting  if  you  toconfirm to delete all  data.  Something like ”are you sure?”,  followed by the submenu with YES or NO. You can reset both sensor data at the same time, or you can do it individually.  Your choice.  Individually makes more sense, but I’m trying to simplify the homework.
    <ul>
   <li>2.1 Yes.
   <li>2.2 No.
   </ul>
3. System Status// Check current status and health
   <ul>
   <li>3.1 Current Sensor Readings.Continuously print sensor readingsat the set sampling rate, from all sensors.  Make sure you have a wayto exit this (such as pressing a specific key) and inform the user ofthis method through a message.
   <li>3.2 Current Sensor Settings.Displays  the  sampling  rate  andthreshold value for all sensors.
   <li>3.3 Display Logged Data.Displays last 10 sensor readings for allsensors.  (or be creative and do it another way).
   <li>3.4 Back.Return to Main menu.
   </ul>
4. RGB LED Control// Go to submenu
   <ul>
   <li>4.1 Manual Color Control.  Set the RGB colors manually.  You decide how to input them, either by making an option for each channel, or by putting a string etc.  If you expect a specific format, makesure to inform the user.
   <li>4.2 LED: Toggle Automatic ON/OFF.  If  automatic  mode  is ON, then the led color should be GREEN when all sensors value donot exceed threshold values (aka no alert) and RED when there is analert (aka ANY sensor value exceeds the threshold). When automaticmode is OFF, then the LED should use the last saved RGB values.
   <li>4.3 Back// Return to main menu
   </ul>

### Youtube link presenting the homework: <a href= ""> Click here </a>

### Photos:
<p float = "left">
<image src = "https://github.com/corinagherasim/IntroductionToRobotics/assets/94368761/973946c8-def2-415f-a62c-843bbee5d309" width="49%" height="500">
<image src = "https://github.com/corinagherasim/IntroductionToRobotics/assets/94368761/936e9209-781c-44fc-832f-75eefae2bbb6" width="49%" height="500">
</p>
