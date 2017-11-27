#include <SoftwareSerial.h> //enables XBee connection pins to transmit and receive serial data
SoftwareSerial XBee(2, 3); // connect XBee and Arduino
//Pin Assignments for Input Values
int backLeft = 4; 
int leftLaser = 5;
int fwdRight = 6;
int upLaser = 7;
int fwdLeft = 8;
int smallLaser = 9;
int rightLaser = 10;
int backRight = 11;
int downLaser = 12;
int bigLaser = 13;
void setup() {
  // set pin modes of input pins to input
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
//start serial data exchange
XBee.begin(9600);
Serial.begin(9600);
}

void loop() {

  
  //test code to figure out which pin connected to what
  /*Serial.println(digitalRead(4));  //back left car     0
  Serial.println(digitalRead(5)); //left laser         1
  Serial.println(digitalRead(6)); //forward right car  2
  Serial.println(digitalRead(7)); //up laser           3
  Serial.println(digitalRead(8)); // forward left car  4
  Serial.println(digitalRead(9)); //targeting          5
  Serial.println(digitalRead(10)); // right laser      6
  Serial.println(digitalRead(11)); // back right car   7
  Serial.println(digitalRead(12)); // down laser       8*/
  //delay(1000);

  //left side logic control
  int leftDigit; //initiates variable
  if(digitalRead(4) == HIGH) //switch flipped one way
  {
      leftDigit = 1; //backwards
  }
  else if(digitalRead(8) == HIGH)// switch flipped other way
  {
    leftDigit = 2; //forwards
  }
  Else // switch not flipped
  {
    leftDigit = 0; // no movement
  }

  //right side
  int rightDigit; //initiates variable
  if(digitalRead(11) == HIGH) //switch one way
  {
      rightDigit = 1; //backwards
  }
  else if(digitalRead(6) == HIGH)//switch other way
  {
    rightDigit = 2; //forwards
  }
  else//switch off
  {
    rightDigit = 0; //no movement
  }

  //left-right laser
  int xDigit; //initiates variable
  if(digitalRead(5) == HIGH) //switch flipped one way
  {
    xDigit = 1; //left
  }
  else if(digitalRead(10) == HIGH) //switch flipped other way
  {
    xDigit = 2; //right
  }
  else //switch off
  {
    xDigit = 0; //no movement
  }

  //up-down laser
  int yDigit; //initiates variable
  if(digitalRead(7) == HIGH) // switch flipped one way
  {
      yDigit = 2; //up
  }
  else if(digitalRead(12) == HIGH) // switch flipped other way
  {
    yDigit = 1; //down
  }
  else //switch off
  {
    yDigit = 0; //no movement
  }

  //laser settings
  int lzrDigit; //initiates variable
  if(digitalRead(9) == HIGH) //button depressed
  {
      lzrDigit = 1; //targeting on
      if(digitalRead(13) ==HIGH)
      {
        lzrDigit = 0; //big laser on
      }
  }
  else
  {
    lzrDigit = 2; //both off
  }
  Serial.println(lzrDigit);
//packages command variables for transmission in base 3
 int command = (81*leftDigit) + (27*rightDigit) + (9*xDigit) + (3*yDigit) + (lzrDigit);
  Serial.println(command);
  if(command != 0) //sends command only if not zero for efficiency
  {
    XBee.write(command);
  }

  
  delay(50);
  
}

