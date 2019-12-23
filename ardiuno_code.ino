

// include the library code for LCD shield:
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
// include code for temperature and humidity
#include <dht.h>

//Setup Temperature and humidity
#define dht_dpin 2 //chanel for temperature sensor
dht DHT;

//Setup LCD
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

//ports for input devices
int const smallPM1 = 13;
int const largePM1 = 12;
int const smallPM2 = 5;
int const largePM2 = 4;
int const mq2port = A0;
int const NO2port = A2;
int const mq9port = A1;
int const O3port = A3;
int const mq9powerPort = 3;

// constants for use with gas sensors
float const mq2seriesResistor = 47000;
float const NO2seriesResistor = 22000;
float const mq9seriesResistor = 10000;
float const O3seriesResistor = 22000;

// variables for use in running particulate matter calculations
long const sampleRate = 20;
long measurementCount = 0;
long smallPM1Count = 0;
long largePM1Count = 0;
long smallPM2Count = 0;
long largePM2Count = 0;
long priorSampleTime = 0;
double smallPM1percentRunning;
double largePM1percentRunning;
double smallPM2percentRunning;
double largePM2percentRunning;

//variables for gas sensor calcuations
float mq2resistance;
float NO2resistance;
float mq9resistance;
float O3resistance;

//variables for temperature and humidity
int temperature;
int humidity;

//miscellaneous variables
int mq9power = 300;
int displayNumber = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode (smallPM1, INPUT);
  pinMode (largePM1, INPUT);
  pinMode (smallPM2, INPUT);
  pinMode (largePM2, INPUT);
  pinMode (mq2port, INPUT);
  pinMode (NO2port, INPUT);
  pinMode (mq9port, INPUT);
  pinMode (O3port, INPUT);
  pinMode (mq9power, OUTPUT);
  analogWrite (mq9powerPort, mq9power);
  lcd.setBacklight(WHITE);
  delay(1000);
}

void loop() {

  samplePMDetectors();         //sample particulate detectors for 2 seconds and update running Averages
  readResistances();           //calculateResistancesFromInputs
  readTemperatureAndHumidity();    //aquire temperature and humidity data takes about 25ms

  timestampSerial();            //print time
  printRunningPMDataToSerial(); //print percentages to Serial
  printGasDataToSerial();       //print gas sensor data to Serial
  printTempAndHumidityToSerial();//print temperature and humidity data
  Serial.println();
  Serial.println();

  displayLCD();    //display data on LCD
}

void samplePMDetectors() {
  for (int i = 0; i < 100; i++) {
    while (millis() - priorSampleTime < sampleRate) {
    }
    priorSampleTime = millis();
    measurementCount += 1;
    if (digitalRead(smallPM1) == 0) {
      smallPM1Count += 1;
    }
    if (digitalRead(largePM1) == 0) {
      largePM1Count += 1;
    }
    if (digitalRead(smallPM2) == 0) {
      smallPM2Count += 1;
    }
    if (digitalRead(largePM2) == 0) {
      largePM2Count += 1;
    }
  }
  //calculate running PM percentages
  smallPM1percentRunning = 100.0 * smallPM1Count / measurementCount;
  largePM1percentRunning = 100.0 * largePM1Count / measurementCount;
  smallPM2percentRunning = 100.0 * smallPM2Count / measurementCount;
  largePM2percentRunning = 100.0 * largePM2Count / measurementCount;
}

void readResistances() {
  //read gas sensor data
  int mq2rawInput = analogRead(mq2port);
  int NO2rawInput = analogRead(NO2port);
  int mq9rawInput = analogRead(mq9port);
  int O3rawInput = analogRead(O3port);
  //calculate resistances
  mq2resistance = mq2seriesResistor * ((1023.0 / mq2rawInput) - 1.0);
  NO2resistance = NO2seriesResistor * ((1023.0 / NO2rawInput) - 1.0);
  mq9resistance = mq9seriesResistor * ((1023.0 / mq9rawInput) - 1.0);
  O3resistance = NO2seriesResistor * ((1023.0 / O3rawInput) - 1.0);
}

void readTemperatureAndHumidity() {
  DHT.read11(dht_dpin);
  humidity = (int) DHT.humidity;
  temperature = (int) DHT.temperature;
}


void timestampSerial() {
  Serial.print("Milliseconds since the program started: ");
  Serial.println(millis());
}

void printRunningPMDataToSerial() {
  Serial.println("Particulate Matter Data");
  Serial.print("Measurement Count:  ");
  Serial.println(measurementCount);
  Serial.print("Small PM detector 1: ");
  Serial.println(smallPM1percentRunning);
  Serial.print("Large PM detector 1: ");
  Serial.println(largePM1percentRunning);
  Serial.print("Small PM detector 2: ");
  Serial.println(smallPM2percentRunning);
  Serial.print("Large PM detector 2: ");
  Serial.println(largePM2percentRunning);
  Serial.println();
}

void printGasDataToSerial() {
  Serial.println("Gas Sensor Data");
  Serial.print("MQ-2 Resistance: ");
  Serial.println(mq2resistance);
  Serial.print("NO2 Resistance: ");
  Serial.println(NO2resistance);
  Serial.print("MQ-9 (CO2) Resistance: ");
  Serial.println(mq9resistance);
  Serial.print("Ozone Resistance: ");
  Serial.println(O3resistance);
  Serial.println();
}

void printTempAndHumidityToSerial() {
  Serial.println("Temperature and Humidity Data");
  Serial.print("temperature = ");
  Serial.print(temperature);
  Serial.print("C   ");
  Serial.print("Current humidity = ");
  Serial.print(humidity);
  Serial.println("%  ");
  Serial.println();
}

void displayLCD() {

  lcd.clear();
  lcd.setCursor(0, 0);

  switch (displayNumber) {
    case 0:
      lcd.print("MeasurementTime");
      lcd.setCursor(0, 1);
      lcd.print(millis() / 1000);
      break;
    case 1:
      lcd.print("SmallPM#1:");
      lcd.setCursor(0, 1);
      lcd.print(smallPM1percentRunning);
      lcd.print(" %");
      break;
    case 3:
      lcd.print("LargePM#1:");
      lcd.setCursor(0, 1);
      lcd.print(largePM1percentRunning);
      lcd.print(" %");
      break;
    case 2:
      lcd.print("SmallPM#2:");
      lcd.setCursor(0, 1);
      lcd.print(smallPM2percentRunning);
      lcd.print(" %");
      break;
    case 4:
      lcd.print("LargePM#2:");
      lcd.setCursor(0, 1);
      lcd.print(largePM2percentRunning);
      lcd.print(" %");
      break;
    case 5:
      lcd.print("MQ2 Resistance:");
      lcd.setCursor(0, 1);
      lcd.print((long) mq2resistance);
      lcd.print(" Ohm");
      break;
    case 6:
      lcd.print("MQ9 Resistance:");
      lcd.setCursor(0, 1);
      lcd.print((long) mq9resistance);
      lcd.print(" Ohm");
      break;
    case 7:
      lcd.print("NO2 Resistance:");
      lcd.setCursor(0, 1);
      lcd.print((long) NO2resistance);
      lcd.print(" Ohm");
      break;
    case 8:
      lcd.print("O3 Resistance:");
      lcd.setCursor(0, 1);
      lcd.print((long) O3resistance);
      lcd.print(" Ohm");
      break;
    case 9:
      lcd.print("Temperature");
      lcd.setCursor(0, 1);
      lcd.print(temperature);
      lcd.print("C");
      break;
    case 10:
      lcd.print("Humidity");
      lcd.setCursor(0, 1);
      lcd.print(temperature);
      lcd.print("%");
      break;
    default:
      displayNumber = -1;
      lcd.clear();
  }
  displayNumber += 1;
}
