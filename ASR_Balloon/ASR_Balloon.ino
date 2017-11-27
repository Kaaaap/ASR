#include <SD.h>
#include <Arduino.h>
//#include <Wire.h>
//#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define KEY 23746
#define BEGIN "THREESIXNINE"
#define END "TWOFOURSIX"


#define SEALEVELPRESSURE_HPA (1013.25)

#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(50, 51); // RX, TX (TX not used)
//HardwareSerial gpsSerial = Serial3;
const int sentenceSize = 200;

char sentence[sentenceSize];

//Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);
int SPEED_OF_SOUND = 1;
int OZONE = 2;
int PRESSURE = 3;
int HUMIDITY = 4;
int SOLAR1 = 5;
int SOLAR2 = 6;
int GEIGER = 0;
int TEMP_1 = 8;
int TEMP_2 = 9;
int TEMP_3 = 10;
int TEMP_4 = 11;
int ALTITUDE = 12;
int CUT_DOWN = 9;
boolean cutDown = false;

String data;

int altitudeInFeet = 0;

double hPaPressure;
int start;
int sensorValue;
int in = 4;
int counter = 0;
boolean previousLow = true;
long startTimer = 0;
long currentTime = 0;
long TIME_IN_SECONDS = 0;


void setup() {

  gpsSerial.begin(9600);
  Serial.begin(9600);
  Serial.println(F("BME280 test"));
  Serial1.begin(9600);
  Serial2.begin(9600);
  int timeInMillis = 0;
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  pinMode(CUT_DOWN, OUTPUT);
  Serial1.print("");
  delay(500);
  Serial1.print("Time,InteriorTemp,InteriorHPA,BMEAltitude,BMEHumidity,");
  Serial1.print("Miller,Ozone,Humidity,Temp1,Temp2,Temp3,Temp4,");
  Serial1.println("OtherAltitude,Solar1,Solar2,Geiger,CutDown,Radio");
  delay(500);
}

void loop() {


  data = "";
  TIME_IN_SECONDS = millis() / 1000;
  data = data + (millis() / 1000);
  data = data + ",";
  //Serial1.print(TIME_IN_SECONDS);
  //Serial1.print(",");

  Serial.print("Time in Seconds = ");
  Serial.println(millis() / 1000);

  data = data + bme.readTemperature();
  data = data + ",";
  //Serial1.print(bme.readTemperature());
  //Serial1.print(",");

  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");


  Serial.print("Pressure = ");

  data = data + bme.readPressure() / 100.0F;
  data = data + ",";
  //Serial1.print(bme.readPressure() / 100.0F);
  //Serial1.print(",");

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");
  hPaPressure = bme.readPressure() / 100.0F;
  data = data + bme.readAltitude(SEALEVELPRESSURE_HPA);
  data = data + ",";
  //Serial1.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  //Serial1.print(",");

  

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  altitudeInFeet = bme.readAltitude(SEALEVELPRESSURE_HPA) * 3.28;
  altitudeInFeet = altitudeInFeet + 200;
  Serial.println(" m");
  Serial.println("Altitude in Feet = ");
  Serial.println(altitudeInFeet);

  data = data + bme.readHumidity();
  data = data + ",";
  //Serial1.print(bme.readHumidity());
  //Serial1.print(",");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");



  int soundReadIn = analogRead(SPEED_OF_SOUND);
  double doubleReadIn = soundReadIn;
  doubleReadIn = doubleReadIn * 5 / 1023;
  doubleReadIn = doubleReadIn / 0.0098;

  data = data + doubleReadIn;
  data = data + ",";
  //Serial1.print(doubleReadIn);

  Serial.println("Speed of sound stuff=");
  Serial.println(doubleReadIn);

  int ozoneReadIn = analogRead(OZONE);
  double ozoneRead = ozoneReadIn;
  ozoneRead = ozoneRead * 0.00488;

  data = data + ozoneRead;
  data = data + ",";
  //Serial1.print(",");
  //Serial1.print(ozoneRead);
  Serial.println("OzoneStuff:");
  Serial.println(ozoneRead);

  int humidityReadIn = analogRead(HUMIDITY);
  double humidityRead = humidityReadIn;

  data = data + humidityRead;
  data = data + ",";

  //Serial1.print(",");
  //Serial1.print(humidityRead);
  Serial.println("Other Humidity Stuff");
  Serial.println(humidityRead);



  double temp1 = analogRead(TEMP_1); //White A
  double temp2 = analogRead(TEMP_2); //lauren's temperature, C
  double temp3 = analogRead(TEMP_3); //silver D
  double temp4 = analogRead(TEMP_4); //black one, E
  Serial.println("Temps");
  Serial.println(temp1);
  Serial.println(temp2);
  Serial.println(temp3);
  Serial.println(temp4);

  data = data + temp1;
  data = data + ",";
  data = data + temp2;
  data = data + ",";
  data = data + temp3;
  data = data + ",";
  data = data + temp4;
  data = data + ",";
  //Serial1.print(",");
  //Serial1.print(temp1);
  //Serial1.print(",");
  //Serial1.print(temp2);

  //Serial1.print(",");
  //Serial1.print(temp3);
  //Serial1.print(",");
  //Serial1.print(temp4);

  //Serial1.print(",");
  double altRead = analogRead(ALTITUDE);
  altRead = altRead * 2.541;
  altRead = altRead - 255.5;
  data = data + altRead;
  data = data + ",";
  //Serial1.print(altRead);
  Serial.print("Altitude = ");
  Serial.println(altRead);

  double solar1Read = analogRead(SOLAR1);
  double solar2Read = analogRead(SOLAR2);
  Serial.println("Solar stuff");
  Serial.println(solar1Read);
  Serial.println(solar2Read);
  data = data + solar1Read;
  data = data + ",";
  data = data + solar2Read;
  data = data + ",";

  //Serial1.print(",");
  startTimer = millis();
  while (currentTime - startTimer < 58999)  ////58999
  {
    sensorValue = analogRead(in);
    if (sensorValue == HIGH && previousLow)
    {
      counter++;
      previousLow = false;
    }
    else if (sensorValue == LOW)
    {
      previousLow = true;
    }
    currentTime = millis();
  }
  int measurement = counter;
  data = data + measurement;
  //Serial1.print(measurement);
  Serial.println("Geiger Counter");
  Serial.println(measurement);
  counter = 0;

  data = data + ",";
  data = data + cutDown;
  data = data + ",";

  int i = 0;
  bool done = false;
  while (!done)
  {
//    Serial.println(gpsSerial.available());
    if (gpsSerial.available())
    {
//      Serial.println("GPS");
      char ch = gpsSerial.read();
      if (ch != '\n' && i < sentenceSize)
      {
        sentence[i] = ch;
        i++;
      }
      else
      {
        sentence[i] = '\0';
        i = 0;
        String sentenceString(sentence);
        String dataType = String("$GPGGA");
        if (find_text(dataType, sentenceString) != -1)
        {
          Serial.print("GPS Data=");
          Serial.println(sentence);
          data = data + sentence;
          done = true;
        }
      }
    }
  }





  TIME_IN_SECONDS = TIME_IN_SECONDS + 60;


  Serial1.println(data);
  Serial.println(data);


  int checkSum = 0;
  for (int x = 0; x < data.length(); x ++)
  {
    checkSum += (int)data[x];
  }
  checkSum %= KEY;

  for (int x = 0; x < 200; x ++)
  {
    digitalWrite(3, HIGH);
    delay(10);
    Serial2.print(BEGIN);
    Serial2.print('|');
    Serial2.print(data);
    Serial2.print('|');
    Serial2.print(checkSum);
    Serial2.print('|');
    Serial2.println(END);
    delay(10);
    digitalWrite(3, LOW);
  }

  hPaPressure = bme.readPressure() / 100.0F;
  if (hPaPressure <= 16.5816 && TIME_IN_SECONDS > 4200 && cutDown == false)
  {
    digitalWrite(CUT_DOWN, HIGH);
    delay(15000);
    digitalWrite(CUT_DOWN, LOW);
    TIME_IN_SECONDS = TIME_IN_SECONDS + 15;
    cutDown = true;
  }


  //Serial1.println();

}

int find_text(String& needle, String& haystack)
{
  int notfoundpos = -1;
  for (int i = 0; i <= haystack.length() - needle.length(); i++)
  {
    if (haystack.substring(i, needle.length() + i).equals(needle))
    {
      return i;
    }
  }
  return notfoundpos;
}


