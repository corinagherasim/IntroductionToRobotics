const int inputr0 = A0;
const int inputg1 = A1;
const int inputb2 = A2;
const int ledR = 9;
const int ledG = 10;
const int ledB = 11;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(inputr0, INPUT);
  pinMode(inputg1, INPUT);
  pinMode(inputb2, INPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
}
// the loop routine runs over and over again forever:
void loop() {
  analogWrite(ledR, map(analogRead(inputr0), 0, 1023, 0, 255));
  analogWrite(ledG, map(analogRead(inputg1), 0, 1023, 0, 255));
  analogWrite(ledB, map(analogRead(inputb2), 0, 1023, 0, 255));
}
