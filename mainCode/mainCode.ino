//Libraries
#include <QTRSensors.h>

QTRSensors qtr; 


//Pinnumbers
const int grabberPin = 2;                    // Servo for grabbing the pion
const int servoPin = 9;                      // Servo for the ultra sonic distance sensor
const int leftMotorPin1 = 3;                 // Left motor backwards
const int triggerPin = 4;                    // Ultra sonic distance sensor trigger
const int leftMotorPin2 = 5;                 // Left motor forwards
const int rightMotorPin2 = 6;                // Right motor forwards
const int echoPin = 7;                       // Ultra sonic distance sensor echo
const int rightMotorPin1 = 11;               // Right motor backwards
const int rightRotationPin = 10;             // Right wheel rotation sensor
const int leftRotationPin = 12;              // Left wheel rotation sensor
const int lineSensorOuterRight = A0;
const int lineSensorFarRight = A1;
const int lineSensorRight = A2;
const int lineSensorInnerRight = A3;
const int lineSensorInnerLeft = A4;
const int lineSensorLeft = A5;
const int lineSensorFarLeft = A6;
const int lineSensorOuterLeft = A7;



//Variables
bool grabberIsClosed = false;
int counterRight = 0;                     // Counts the amount of pulses detected by the rotation sensor
int counterLeft = 0;
int rotationStateRight;                   // Saves the rotation state
int rotationStateLeft;
int rotationLastStateRight;               // Remembers the last rotation state
int rotationLastStateLeft;
int left = 255;                           // Speed of the left motor
int right = 255;                          // Speed of the right motor
unsigned long duration;                   // Time detected by the ultra sonic distance sensor
double distance;                          // Distance in centimetres
const double wheelCircumference = 20.41;  // Circumference of the wheel in centimetres
const double pulseDistance = 0.51;        // The amount of centimetres the battlebot will drive forward in 1 pulse
const int rotation = 40;                  // The amount of pulses for the wheel to reach one rotation
boolean checking = false;
boolean turnedLeft = false;
boolean turnedRight = false;
boolean turnedAround = false;
boolean started = false;
uint16_t sensorValues[8];                 // An array to store the current values of the sensors in
bool isOnLine;                            // Variable to keep track of whether the robot is on the line or not
uint16_t position;                        // Variable to store the position of the line relative to the sensors
bool wasOnLines = false;                  // Calibration line counting variables
long lastAllLines = 0;                    // Variable for keeping the last time the robot was on a black line




//Setup
void setup() {
  servoFront();
  pinMode(servoPin, OUTPUT);
  pinMode(triggerPin, OUTPUT);    
  pinMode(echoPin, INPUT);           
  Serial.begin(9600);                     // Open serial communication                          
  pinMode(rightMotorPin1, OUTPUT);    
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);     
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightRotationPin, INPUT);   
  pinMode(leftRotationPin, INPUT);   
  pinMode(lineSensorOuterRight, INPUT);
  pinMode(lineSensorFarRight, INPUT);
  pinMode(lineSensorRight, INPUT);
  pinMode(lineSensorInnerRight, INPUT);
  pinMode(lineSensorInnerLeft, INPUT);
  pinMode(lineSensorLeft, INPUT);
  pinMode(lineSensorFarLeft, INPUT);
  pinMode(lineSensorOuterLeft, INPUT);
  grabberOpen();
  calibrateSensor();
}



//Loop
void loop() {
  hugRightWall();
}



//Functions
void detectWall() {                     // This function activates the ultra sonic distance sensor and it calculates the distance of the object 
  digitalWrite(triggerPin, LOW);        // in front of it in centimetres
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH); 
  distance = (duration * 0.034)/2;
  Serial.println(distance);
  delay(10);
}

void forwardOneSquare() {               // This function makes the battlebot drive on square forward in the maze
  while(counterLeft < 1) {
    readRotationLeft();
    calibrateDrive();
  }
  counterLeft = 0;
  while(counterLeft < 52) {
    readRotationLeft();
    driveForward();
  }
}

void forwardHalfSquare() {               // This function makes the battlebot drive on square forward in the maze
  while(counterLeft < 1) {
    readRotationLeft();
    calibrateDrive();
  }
  counterLeft = 0;
  while(counterLeft < 29) {
    readRotationLeft();
    driveStraightForward();
  }
}

void forwardInMaze() {               // This function makes the battlebot drive on square forward in the maze
  while(counterLeft < 1) {
    readRotationLeft();
    calibrateDrive();
  }
  counterLeft = 0;
  while(counterLeft < 20) {
    readRotationLeft();
    driveStraightForward();
  }
}

void startPosition() {                  // This function makes the battlebot drive drive to the right position in the square
  while(counterLeft < 18) {
    readRotationLeft();
    driveForward();
    started = true;
  }
}

void driveForward() {                   // This function activates both motors and will make the battlebot drive forward
  left = 237;
  right = 231;
  analogWrite(leftMotorPin2, left);
  digitalWrite(leftMotorPin1, LOW);
  analogWrite(rightMotorPin2, right);
  digitalWrite(rightMotorPin1, LOW);  
}

void driveStraightForward() {                   // This function activates both motors and will make the battlebot drive forward
  left = 236;
  right = 231;
  analogWrite(leftMotorPin2, left);
  digitalWrite(leftMotorPin1, LOW);
  analogWrite(rightMotorPin2, right);
  digitalWrite(rightMotorPin1, LOW);  
}

void driveBackward() {                   // This function activates both motors and will make the battlebot drive backward
  left = 237;
  right = 232;
  analogWrite(leftMotorPin1, left);
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(rightMotorPin1, right);
  digitalWrite(rightMotorPin2, LOW);   
}

void brake() {                          // This function deactivates both motors and will make the battlebot stop driving for a short time
  left = 0;
  right = 0;
  analogWrite(leftMotorPin2, LOW);
  digitalWrite(leftMotorPin1, LOW);
  analogWrite(rightMotorPin2, LOW);
  digitalWrite(rightMotorPin1, LOW);
  delay(500); 
}

void turnRight() {                      // This function will make the battlebot make a 90 degree right turn
  counterLeft = 0;
  while(counterLeft < 1){
    readRotationLeft();
    left = 255;
    right = 255;
    analogWrite(leftMotorPin2, right);
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
    digitalWrite(rightMotorPin1, LOW);
  }
  counterLeft = 0;
  while(counterLeft < 32) {
    readRotationLeft();
    forwardsLeft();
  }
  brake();
  counterLeft = 0;
  counterRight = 0;
  forwardHalfSquare();
  counterLeft = 0;
}

void driveInMaze() {                       // This function will make the battlebot make a 90 degree left turn
  counterRight = 0;
  while(counterRight < 1){
    readRotationRight();
    left = 255;
    right = 255;
    digitalWrite(leftMotorPin2, LOW);
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(rightMotorPin1, LOW);
    analogWrite(rightMotorPin2, right);
  }
  counterRight = 0;
  while(counterRight < 32) {
    readRotationRight();
    forwardsRight();
  }
  brake();
  counterLeft = 0;
  counterRight = 0;
  forwardInMaze();
}

void turnLeft() {                       // This function will make the battlebot make a 90 degree left turn
  counterRight = 0;
  while(counterRight < 1){
    readRotationRight();
    left = 255;
    right = 255;
    digitalWrite(leftMotorPin2, LOW);
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(rightMotorPin1, LOW);
    analogWrite(rightMotorPin2, right);
  }
  counterRight = 0;
  while(counterRight < 32) {
    readRotationRight();
    forwardsRight();
  }
  brake();
  counterLeft = 0;
  counterRight = 0;
  forwardHalfSquare();
}

void calibrateDrive() {
  left = 255;
  right = 255;
  analogWrite(leftMotorPin2, left);
  digitalWrite(leftMotorPin1, LOW);
  analogWrite(rightMotorPin2, right);
  digitalWrite(rightMotorPin1, LOW); 
}

void backwardsLeft() {                 // This function will make the left wheel turn backward
  left = 200;
  right = 200;
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(leftMotorPin1, left);
  digitalWrite(rightMotorPin2, LOW);
  digitalWrite(rightMotorPin1, LOW);
}

void forwardsLeft() {                   // This function will make the left wheel turn forward
  left = 200;
  right = 200;
  analogWrite(leftMotorPin2, right);
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(rightMotorPin2, LOW);
  digitalWrite(rightMotorPin1, LOW);
}

void backwardsRight() {                 // This function will make the right wheel turn backward
  left = 200;
  right = 200;
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(leftMotorPin1, LOW);
  analogWrite(rightMotorPin1, right);
  digitalWrite(rightMotorPin2, LOW);
}

void forwardsRight() {                  // This function will make the right wheel turn forward
  left = 200;
  right = 200;
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(rightMotorPin1, LOW);
  analogWrite(rightMotorPin2, right);
}

void turnAround() {                 // This function will make the battlebot make a 180 degree turn to the left
  double distanceLeft = 0;
  double distanceRight = 0;
  servoLeft();
  delay(300);
  detectWall();
  distanceLeft = distance;
  servoRight();
  delay(300);
  detectWall();
  distanceRight = distance;
  if (distanceLeft < distanceRight){
    counterLeft = 0;
    counterRight = 0;
    while(counterRight < 33) {
      readRotationRight();
      backwardsRight();
    }
    while(counterLeft < 33) {
      readRotationLeft();
      forwardsLeft();
    }
    brake();
    counterRight = 0;
    counterLeft = 0;
    servoFront();
    brake();
    while(counterRight < 8) {
      readRotationRight();
      driveForward();
    }
    counterRight = 0;
    counterLeft = 0; 
  }
  else {
    counterLeft = 0;
    counterRight = 0;
    while(counterLeft < 33) {
      readRotationLeft();
      backwardsLeft();
    }
    while(counterRight < 33) {
      readRotationRight();
      forwardsRight();
    }
    brake();
    counterRight = 0;
    counterLeft = 0;
    brake(); 
    servoFront();
    while(counterRight < 8) {
      readRotationRight();
      driveForward();
    }
    counterRight = 0;
    counterLeft = 0;  
  }
}

void readRotationRight() {              // This function will read the pulses of the right rotation sensor
  rotationStateRight = digitalRead(rightRotationPin);
  if(rotationStateRight != rotationLastStateRight) {
      counterRight++;   
  }
  rotationLastStateRight = rotationStateRight;
  Serial.println(counterRight);
}

void readRotationLeft() {               // This function will read the pulses of the left rotation sensor
  rotationStateLeft = digitalRead(leftRotationPin);
  if(rotationStateLeft != rotationLastStateLeft) {
      counterLeft++;   
  }
  rotationLastStateLeft = rotationStateLeft;
  Serial.println(counterLeft);
}

void servoLeft() {                   // This function will make the servo turn to the left 
  // A pulse each 20ms
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(2600); // Duration of the pusle in microseconds
  digitalWrite(servoPin, LOW);
  delayMicroseconds(18550); // 20ms - duration of the pusle
  // Pulses duration: 600 - 0deg; 1650 - 90deg; 2600 - 180deg
}

void servoFront() {                  // This function will make the servo turn to the front
  // A pulse each 20ms  
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(1650); // Duration of the pusle in microseconds
  digitalWrite(servoPin, LOW);
  delayMicroseconds(18550); // 20ms - duration of the pusle
  // Pulses duration: 600 - 0deg; 1650 - 90deg; 2600 - 180deg
}

void servoRight() {                   // This function will make the servo turn to the right
  // A pulse each 20ms
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(600); // Duration of the pusle in microseconds
  digitalWrite(servoPin, LOW);
  delayMicroseconds(18550); // 20ms - duration of the pusle
  // Pulses duration: 600 - 0deg; 1650 - 90deg; 2600 - 180deg
}

void hugRightWall() {
  if(checking == false) {                                     // Check if the function is already running
    counterLeft = 0;                                          // Reset the left pulse count
    counterRight = 0;                                         // Reset the right pulse count
    checking = true;                                          
    brake();    
    servoRight();                                              // Position the servo to look to the right
    delay(500);
    detectWall();                                             // Activates the ultra sonic distance sensor and checks if there is a wall
    if(distance < 20) {                                       // If a wall is detected on the right side, position the servo to look to the front
      servoFront();
      delay(500);
      detectWall();
      if(distance < 15) {                                     // If a wall is detected on the front side, position the servo to the left
        servoLeft();
        delay(500);
        detectWall(); 
        if(distance < 20) {                                   // If a wall is detected on the left side, the battlebot will turn around
          servoFront();
          delay(500);
          counterLeft = 0;
          counterRight = 0;
          turnAround();
          brake();
          checking = false;
        }
        else {                                                // If no path is detected on the sides, turn to the left
          servoFront();
          turnLeft();
          brake();
          checking = false;
        }
      }
      else {                                                 // If a path is detected on the right side, drive one square forward
        servoFront();
        delay(500);
        forwardOneSquare();
        brake();
        checking = false;
      }
    }
    else {                                                  // If a path is detected on the left side, turn to the right
      servoFront();
      delay(200);
      turnRight();
      brake();
      checking = false;
    }
  }
  else {
    checking = false;
  }
}

//closes the grabber
void grabberClosed() {
  digitalWrite(grabberPin, HIGH);
  delayMicroseconds(1025); // Duration of the pusle in microseconds
  digitalWrite(grabberPin, LOW);
  delayMicroseconds(18550); // 20ms - duration of the pusle
}

// opens the grabber
void grabberOpen() {
  digitalWrite(grabberPin, HIGH);
  delayMicroseconds(1650); // Duration of the pusle in microseconds
  digitalWrite(grabberPin, LOW);
  delayMicroseconds(18550); // 20ms - duration of the pusle
}

void updateLineData()
{
  position = qtr.readLineBlack(sensorValues);
  bool isCurrentlyOnLine = false;
  for (int i = 0; i < 8; i++)
  {
    if (sensorValues[i] > qtr.calibrationOn.maximum[i] - 100)
    {
      isCurrentlyOnLine = true;
      break;
    }
  }
  isOnLine = isCurrentlyOnLine;
}

bool isAllOnLine()
{
  updateLineData();
  bool allSensors = true;
  for (int i = 0; i < 8; i++)
  {
    if (sensorValues[i] < qtr.calibrationOn.maximum[i] - 100)
    {
      allSensors = false;
      break;
    }
  }
  return allSensors;
}

void calibrateSensor()
{
  // First open the grapper, then drive forward over the calibration lines, then proceed to pick up the pawn and rotate towards the course
  grabberOpen(); 
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){lineSensorOuterLeft, lineSensorFarLeft, lineSensorLeft, lineSensorInnerLeft, lineSensorInnerRight, lineSensorRight, lineSensorFarRight, lineSensorOuterRight}, 8);
  forward(253,255);
  delay(30);
  forward(115,110);
  while (true)
  {
    qtr.calibrate();
    if (isAllOnLine())
    {
      if (wasOnLines && lastAllLines > millis() - 2000)
      {
        while (true)
        {
          qtr.calibrate();
          if (!isAllOnLine())
          {
            delay(250);
            break;
          }
        }
        break;
      }
      else
      {
        wasOnLines = true;
        lastAllLines = millis();
      }
    }
    else
    {
        wasOnLines = false;
        lastAllLines = 0;
    }
  }
  brake();
  delay(100);
  grabberClosed();
  driveInMaze();
}

void forward(int left, int right) {                   // This function activates both motors and will make the battlebot drive forward
  analogWrite(leftMotorPin2, left);
  digitalWrite(leftMotorPin1, LOW);
  analogWrite(rightMotorPin2, right);
  digitalWrite(rightMotorPin1, LOW);   
}
