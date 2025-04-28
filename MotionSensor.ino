/*  
 *  AUTHOR: Domonkos Lakics
 *  Last modified: 2024. 12. 28.
 *  DESCRIPTION:
    This program is for a motion sensor.
    If the sensor detects movement, a green light flashes with a buzzer sound.
    You can turn it on and off by pressing the button. 
    
*/

int redLed = 12;    // LED on Pin 11 of Arduino, feedback for ON/OFF state, the led shines when sensor is OFF.
int greenLed = 13;  // LED on Pin 13 of Arduino, feedback for motion sensor, shines when movement is detected
int blueLed = 11;   // LED on Pin 11 of Arduino, feedback for booting, flashes until booting is done
int buzzer = 9;     // Buzzer on Pin 9 of Arduino, audio feedback on everything
int button = 4;     // Button on Pin 11 of Arduino, toggle ON/OFF for sensor
int pirPin = 7;     // Input for HC-S501 Motion Sensor
int pirValue;       // Place to store read PIR Value
bool isOn;          // On/Off toggle for the motion sensor


void setup() {

  // Initializing pins
  pinMode(buzzer, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(pirPin, INPUT);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, HIGH);

  // 1 minute initialization of the sensor, so it doesn't send faulty signals.
  initializeMotionSensorFeedback();

  // Switching off by default
  digitalWrite(redLed, HIGH);
  digitalWrite(redLed, HIGH);

}

void loop() {


  // If the sensor is toggled ON
  if (isOn) {
    pirValue = digitalRead(pirPin);       // Reads the signal sent from the sensor
    digitalWrite(greenLed, pirValue);     // Green light flashes if sensor activated
    digitalWrite(buzzer, pirValue);       // Buzzer makes sound if sensor activated

  }

  // Switches from OFF to ON
  if (digitalRead(button) == LOW && !isOn) {
    isOn = true;
    digitalWrite(redLed, LOW);
    buzzerBeepBeep();
    delay(500);
  }
  // Switches from ON to OFF
  else if (digitalRead(button) == LOW && isOn) {
    isOn = false;
    digitalWrite(redLed, HIGH);
    buzzerBeepBeep();
    delay(500);
  }

}



// Additional functions




void initializeMotionSensorFeedback() {

  /*
      The motion sensor needs approx. a minute to initialize, because it would
      send faulty signals, so this function is called to make sure the loop
      doesn't start until the initialization is complete. The blue light and
      the buzzer give feedback on the booting process.
  */


  int seconds = 60;

  // First 30 seconds feedback, LED flashes every second

  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);

  for (int i = 0; i < (seconds / 2); i++) {

    digitalWrite(blueLed, HIGH);
    delay(250);
    digitalWrite(buzzer, HIGH);
    delay(250);
    digitalWrite(buzzer, LOW);
    digitalWrite(blueLed, LOW);
    delay(500);

  }

  // Last 30 seconds feedback, led flashes every 0.5 second

  digitalWrite(buzzer, HIGH);
  delay(500);


  for (int i = 0; i < (seconds / 2); i++) {

    digitalWrite(blueLed, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(250);
    digitalWrite(buzzer, LOW);
    digitalWrite(blueLed, LOW);
    delay(250);
    digitalWrite(blueLed, HIGH);
    delay(250);
    digitalWrite(blueLed, LOW);
    delay(250);
  }


  // Boot complete feedback, led flashes fast with buzzer ringing

  for (int i = 0; i < 10; i++) {
    digitalWrite(blueLed, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(blueLed, LOW);
    digitalWrite(buzzer, LOW);
    delay(50);
  }

  // Funtion complete
}







void buzzerBeepBeep() {

  // Buzzer does 3 small beeps

  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(50);
  }
}
