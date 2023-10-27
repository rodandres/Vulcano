const int ledR = 6;
const int ledA = 5;
const int ledV = 3;

const int ldr1 = A5;
const int ldr2 = A4;
const int ldr3 = A3;

int ldrVal1 = 0;
int ldrVal2 = 0;
int ldrVal3 = 0;

int ldrCalibration1 = 0;
int ldrCalibration2 = 0;
int ldrCalibration3 = 0;

unsigned long time;
int interval = 200;
int calibrationTime = 5000;
int testTimeInit = 0;
int testTime = 0;

bool configFinish = false;
bool testStarted = false;
bool testFinish = false;

int multiplyngParameterToStart = 5;//6 100 
int multiplyngParameterToFinish = 3;//3 30


void setup() {
  Serial.begin(9600);

  pinMode(ledR, OUTPUT);
  pinMode(ledA, OUTPUT);
  pinMode(ledV, OUTPUT);
  time = millis();
}

void loop() {

  /*if (Serial.available() > 0) {
    char read = Serial.read();
    if (read == '\n' || read == '\r') {
    } else {
      Serial.println(read);
    }
  }*/

  ldrVal1 = analogRead(ldr1);
  ldrVal2 = analogRead(ldr2);
  ldrVal3 = analogRead(ldr3);

  if (configFinish == false) {
    ldrCalibration1 = (ldrCalibration1 + ldrVal1) / 2;
    ldrCalibration2 = (ldrCalibration2 + ldrVal2) / 2;
    ldrCalibration3 = (ldrCalibration3 + ldrVal3) / 2;

    Serial.println(String(ldrVal1) + "  " + String(ldrVal2) + "  " + String(ldrVal2) + "  ");

    if (millis() > time + 500) {
      digitalWrite(ledA, !digitalRead(ledA));
      time = millis();
    }
  }

  if (millis() > calibrationTime && configFinish == false) {
    configFinish = true;
    digitalWrite(ledA, LOW);
    digitalWrite(ledV, HIGH);
    Serial.println(millis());
    Serial.println("Last LDR Values: " + String(ldrVal1) + "  " + String(ldrVal2) + "  " + String(ldrVal2) + "  ");
    Serial.println("Calibration LDR Values: " + String(ldrCalibration1) + "  " + String(ldrCalibration2) + "  " + String(ldrCalibration3) + "  ");
  }

  if (configFinish == true && testFinish == false) {

    if (((ldrVal1 > (multiplyngParameterToStart * ldrCalibration1)) && (ldrVal2 > (multiplyngParameterToStart * ldrCalibration2))) || ((ldrVal1 > (multiplyngParameterToStart * ldrCalibration1)) && (ldrVal3 > (multiplyngParameterToStart * ldrCalibration3))) || ((ldrVal2 > (multiplyngParameterToStart * ldrCalibration2)) && (ldrVal3 > (multiplyngParameterToStart * ldrCalibration3)))) {
      testStarted = true;
      Serial.println("Test has started");
      digitalWrite(ledV, LOW);
      digitalWrite(ledR, HIGH);
      testTimeInit = millis();
    }

    while (testStarted) {
      ldrVal1 = analogRead(ldr1);
      ldrVal2 = analogRead(ldr2);
      ldrVal3 = analogRead(ldr3);
      Serial.println("Test going on");
      Serial.println("TEST LDR Values: " + String(ldrVal1) + "  " + String(ldrVal2) + "  " + String(ldrVal2) + "  ");

      if (((ldrVal1 < (multiplyngParameterToFinish * ldrCalibration1)) && (ldrVal2 < (multiplyngParameterToFinish * ldrCalibration2))) || ((ldrVal1 < (multiplyngParameterToFinish * ldrCalibration1)) && (ldrVal3 < (multiplyngParameterToFinish * ldrCalibration3))) || ((ldrVal2 < (multiplyngParameterToFinish * ldrCalibration2)) && (ldrVal3 < (multiplyngParameterToFinish * ldrCalibration3)))) {
        testStarted = false;
        testFinish = true;
        Serial.println("Test Finish");
        digitalWrite(ledR, HIGH);
        digitalWrite(ledA, HIGH);
        digitalWrite(ledV, HIGH);
        testTime = millis() - testTimeInit;
        Serial.println("Total duration of test: " + String(testTime));
      }
    }
  }
}