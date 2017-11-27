#include <SoftwareSerial.h>  //enables XBee and Arduino to connect
#include <Servo.h>  //includes servo library
SoftwareSerial XBee(2, 3); // Connect XBee and Arduino

//variable initiation
//command read-in values
int leftSide;
int rightSide;
int lzrX;
int lzrY;
int lzrTog;
//pin assignments
int targetPin = 4;
int left1 = 5;
int left2 = 6;
int right1 = 7;
int right2 = 8;
int baseServoPin = 9;
int upperServoPin = 10;
//servo starting positions
int upperServoPos = 90;
int baseServoPos = 90;
//boolean values to help with code logic and continuity of control
boolean targetLaser = false;
boolean fwdLeft = false;
boolean fwdRight = false;
boolean backLeft = false;
boolean backRight = false;

//servos
Servo upperServo;
Servo baseServo;

void setup() {
  // begin serial communication
 Serial.begin(9600);
XBee.begin(9600);
//set output pin modes
pinMode(targetPin, OUTPUT);
pinMode(left1, OUTPUT);
pinMode(left2, OUTPUT);
pinMode(right1, OUTPUT);
pinMode(right2, OUTPUT);
//start with lasers off
digitalWrite(targetPin, LOW);
//attach and initiate servos
upperServo.attach(upperServoPin);
upperServo.write(upperServoPos);
baseServo.attach(baseServoPin);
baseServo.write(baseServoPos);
delay(30);

}

void loop() {
  
  // reset command value
int command = 0;
if (XBee.available()) //if it is receiving values 
{
  command = XBee.read();  //the command is what it receives
  Serial.println(command);  //for debugging 
}
if(command == 0) //if no command received 
  {
//don’t move or do anything
  digitalWrite(right1, LOW); 
  digitalWrite(right2, LOW);
  digitalWrite(left1, LOW);
  digitalWrite(left2, LOW);
  //digitalWrite(targetPin, LOW);
  //delay(10);
  }
  else{
    Serial.println(command); //for debugging
//parse the data using mod 3
    leftSide = (command - (command % 81)) / 81;
    Serial.println(leftSide);
    command = command % 81;
    rightSide = (command - (command % 27)) / 27;
    Serial.println(rightSide);
    command = command % 27;
    lzrX = (command - (command % 9)) / 9;
    Serial.println(lzrX);
    command = command % 9;
    lzrY = (command - (command % 3)) / 3;
    Serial.println(lzrY);
    command = command % 3;
    lzrTog = command;
    Serial.println(lzrTog);
//LEFT SIDE CAR CONTROL
    if(leftSide == 1)
    {
      backLeft = true;
      fwdLeft = false;
      //move left side car backwards
    }
    else if(leftSide == 2)
    {
      backLeft = false;
      fwdLeft = true;
      //move left side car forwards
    }
    else
    {
      backLeft = false;
      fwdLeft = false;
//don’t move
    }
//RIGHT SIDE CAR CONTROL
    if(rightSide == 1)
    {
      backRight = true;
      fwdLeft = false;
      //move right side car backwards
    }
    else if(rightSide == 2)
    {
      backRight = false;
      fwdRight = true;
      //move right side car forwards
    }
    else
    {
      backRight = false;
      fwdRight = false;
//don’t move
    }
//LEFT-RIGHT ARRAY CONTROL
    if(lzrX == 1)
    {
      lzrLeft(); //move lower servo one way
    }
    else if(lzrX == 2)
    {
      lzrRight(); // move lower servo the other way
    }
//UP-DOWN ARRAY CONTROL

    if(lzrY == 1)
    {
      lzrUp(); //move upper servo one way
    }
    else if(lzrY == 2)
    {
      lzrDown(); //move upper servo the other way
    }
    
//LASER CONTROL
    if(lzrTog == 1)
    {
      targetLaser = true; //targeting laser on
    }
    else if(lzrTog == 2)
    {
      targetLaser = false; //targeting laser off
    }
    else
    {
      //big laser code would go here but we did not have it in the demo for safety reasons
      
    }
    if(targetLaser)
    {
      digitalWrite(targetPin, HIGH); //laser on
    }
    else{
      digitalWrite(targetPin, LOW); //laser off
    }
    //left side
    if(backLeft) //left side moves backwards
{
      digitalWrite(left1, HIGH); 
      digitalWrite(left2, HIGH);
    }
    else if(fwdLeft) //left side moves forwards
    {
      digitalWrite(left1, HIGH);
      digitalWrite(left2, LOW);
    }
    else //left side does not move
    {
      digitalWrite(left1, LOW);
      digitalWrite(left2, LOW);
    }
    //right side
    if(backRight) //right side moves backwards
{
      digitalWrite(right1, HIGH);
      digitalWrite(right2, HIGH);
    }
    else if(fwdRight) //right side moves forwards
    {
      digitalWrite(right1, HIGH);
      digitalWrite(right2, LOW);
    }
    else //right side does not move
    {
      digitalWrite(right1, LOW);
      digitalWrite(right2, LOW);
    }
  }
delay(10);
}
//helper methods
void lzrRight() //increments lower servo movement in one direction
{
  if(baseServoPos > 30) //cut off so no wires are tangled
      {
//move the servo one degree
        baseServoPos = baseServoPos - 1;
      baseServo.write(baseServoPos);
      delay(30);
      }
}
void lzrLeft() //increments lower servo movement in the other direction
{
  if(baseServoPos < 150) //cut off so no wires are tangled
      {
//move the servo one degree
        baseServoPos = baseServoPos + 1;
      baseServo.write(baseServoPos);
      delay(30);
      }
}
void lzrDown() //moves upper servo in one direction
{
   if(upperServoPos > 60) //cut off so laser cannot be aimed at car
      {
        upperServoPos = upperServoPos - 1;
      upperServo.write(upperServoPos);
      delay(30);
      }
}
void lzrUp() //moves upper servo in the other direction
{
  if(upperServoPos < 120) //cut off so laser cannot be aimed at people’s eyes for demo version
      {
        upperServoPos = upperServoPos + 1;
      upperServo.write(upperServoPos);
      delay(30);
      }
}

