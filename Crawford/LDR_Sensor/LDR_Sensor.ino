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
int multiplyngParameterToStart = 5;   //6 100
int multiplyngParameterToFinish = 3;  //3 30


void setup() {
  Serial.begin(9600);  //Serial intialization

  // Declaration of LEDs
  pinMode(ledR, OUTPUT);
  pinMode(ledY, OUTPUT);
  pinMode(ledG, OUTPUT);

  time = millis();  // Start of the time

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

    // If the time of configuration hasn´t finished, then get the average of the LDR values
    if (calibrationFinish == false) {
      ldrCalibration1 = (ldrCalibration1 + ldrVal1) / 2;
      ldrCalibration2 = (ldrCalibration2 + ldrVal2) / 2;
      ldrCalibration3 = (ldrCalibration3 + ldrVal3) / 2;

      Serial.println(String(ldrVal1) + "  " + String(ldrVal2) + "  " + String(ldrVal2) + "  ");

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
          
          testTime = millis() - testTimeInit; //Get the total time of test by substracting actual time from the time when it started
          Serial.println("Total duration of test: " + String(testTime));
        }
      }
    }
  }