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


