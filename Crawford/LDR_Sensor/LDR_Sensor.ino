/*
LDR DETECTION OF CRAWFORD BOMB 

Needs
* Green LED  connected to digital pin 3
* Yellow LED connected to digital pin 5
* Red Led connected to digital pin 6
* 3 LDR sensor connected to analogic pins A5, A4, A3

How it works?
There are 5 seconds in which the arduino calibrates the sensor, it does it by geting the average bewtween last value of LDR and las calibration value. During calibration, 
yellow light flashes.

After calibration have finished, the green LED iluminates indicating the test is ready to start.

Arduino is capble of detecting when the test has started or finished. The sensibility of this automatic detection can be adjust with the multiplyng parameters.

Once the arduino has detected the start of the test, a red LED will iluminate until the detection of end of the test. Once the test has finished, the serial port will
show the total duration of the test in miliseconds with a small error of +-0.010 seconds.

To start a new test, the arduino must be reset by pressing the red button on it


Future updates:
* Integration and control of new tests by serial port
* Sensibility parameter change throw the serial port
* Integration with a remote control application
* Intelligent integration with remote ignition system of the Crawford Bomb
*/

// Indicators LED pins
const int ledR = 6;
const int ledY = 5;
const int ledG = 3;

// LDR sensors pins
const int ldr1 = A5;
const int ldr2 = A4;
const int ldr3 = A3;

//Values of LDR
int ldrVal1 = 0;
int ldrVal2 = 0;
int ldrVal3 = 0;

//Values of LDR Calibration
int ldrCalibration1 = 0;
int ldrCalibration2 = 0;
int ldrCalibration3 = 0;

// Time management
unsigned long time;
int calibrationTime = 5000;
int testTimeInit = 0;
int testTime = 0;

//  Bools of test status
bool calibrationFinish = false;
bool testStarted = false;
bool testFinish = false;

// Sensibility adjustment of LDRs to automatic start and finish the test
int multiplyngParameterToStart = 3;   //6 100
int multiplyngParameterToFinish = 2;  //3 30


void setup() {
  Serial.begin(9600);  //Serial intialization

  // Declaration of LEDs
  pinMode(ledR, OUTPUT);
  pinMode(ledY, OUTPUT);
  pinMode(ledG, OUTPUT);

  time = millis();  // Start of the time
}

void loop() {

  /*if (Serial.available() > 0) {
    char read = Serial.read();
    if (read == '\n' || read == '\r') {
    } else {
      Serial.println(read);
    }
  }*/

  // Get the values of the LDRs
  ldrVal1 = analogRead(ldr1);
  ldrVal2 = analogRead(ldr2);
  ldrVal3 = analogRead(ldr3);
  Serial.println(String(ldrVal1) + "  " + String(ldrVal2) + "  " + String(ldrVal2) + "  ");

  // If the time of configuration hasn´t finished, then get the average of the LDR values
  if (calibrationFinish == false) {
    ldrCalibration1 = (ldrCalibration1 + ldrVal1) / 2;
    ldrCalibration2 = (ldrCalibration2 + ldrVal2) / 2;
    ldrCalibration3 = (ldrCalibration3 + ldrVal3) / 2;

    // Flash the yellow LED to indicate calibration is in progress
    if (millis() > time + 500) {
      digitalWrite(ledY, !digitalRead(ledY));
      time = millis();
    }
  }

  // When the timer reaches the time designated for the calibration progress
  if (millis() > calibrationTime && calibrationFinish == false) {
    calibrationFinish = true;  //Calibration has finished, set the bool to true
    //Turn off the yellow light and turn on the green light to indicate the test is able to start
    digitalWrite(ledY, LOW);
    digitalWrite(ledG, HIGH);
    //Print last LDR value and final Calibration Value
    Serial.println("Last LDR Values: " + String(ldrVal1) + "  " + String(ldrVal2) + "  " + String(ldrVal2) + "  ");
    Serial.println("Calibration LDR Values: " + String(ldrCalibration1) + "  " + String(ldrCalibration2) + "  " + String(ldrCalibration3) + "  ");
  }

  //Only if the calibration has finished can enter
  if (calibrationFinish == true) {
    //If at least 2 of the LDR values exceed the multiplication of the calibration value and the sensibility values, test has started
    if (((ldrVal1 > (multiplyngParameterToStart * ldrCalibration1)) && (ldrVal2 > (multiplyngParameterToStart * ldrCalibration2))) || ((ldrVal1 > (multiplyngParameterToStart * ldrCalibration1)) && (ldrVal3 > (multiplyngParameterToStart * ldrCalibration3))) || ((ldrVal2 > (multiplyngParameterToStart * ldrCalibration2)) && (ldrVal3 > (multiplyngParameterToStart * ldrCalibration3)))) {
      testStarted = true;
      Serial.println("Test has started");

      // Turon off LED green and turn on LED red to indicate test is going on
      digitalWrite(ledG, LOW);
      digitalWrite(ledR, HIGH);

      testTimeInit = millis();  // Get the moment of the timer in which the test has started
    }

    while (testStarted) {  // While the test is going on
      // Get the values of LDR as it is in a nested while loop of the main loop
      ldrVal1 = analogRead(ldr1);
      ldrVal2 = analogRead(ldr2);
      ldrVal3 = analogRead(ldr3);

      Serial.println("Test going on");
      Serial.println("TEST LDR Values: " + String(ldrVal1) + "  " + String(ldrVal2) + "  " + String(ldrVal2) + "  ");

      // Check that values of LDR, if it goes under the multiplication of LDR calibration values and the sensibility values, the test has finished
      if (((ldrVal1 < (multiplyngParameterToFinish * ldrCalibration1)) && (ldrVal2 < (multiplyngParameterToFinish * ldrCalibration2))) || ((ldrVal1 < (multiplyngParameterToFinish * ldrCalibration1)) && (ldrVal3 < (multiplyngParameterToFinish * ldrCalibration3))) || ((ldrVal2 < (multiplyngParameterToFinish * ldrCalibration2)) && (ldrVal3 < (multiplyngParameterToFinish * ldrCalibration3)))) {
        testStarted = false;
        testFinish = true;
        Serial.println("Test Finish");
        //Indicates that test has finished by turning on all three LEDs
        digitalWrite(ledR, HIGH);
        digitalWrite(ledY, HIGH);
        digitalWrite(ledG, HIGH);

        testTime = millis() - testTimeInit;  //Get the total time of test by substracting actual time from the time when it started
        Serial.println("Total duration of test: " + String(testTime));
        while(true){} //Stuck the code to dont get more values, It is necesary to restart the code/arduino to have another test            
      }
    }
  }
}