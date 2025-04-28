/*
   Parking sensor

   Author: Domonkos Lakics

   Last modified: 2024.12.30.

   Description:
        The ultrasonic sensor detects the distance between the vehicle and the other object, the buzzer and the LED dot matrix give audio and visual feedback on this distance.
*/

#include "SR04.h"         // Header file for ultrasonic sensor
#include "LedControl.h"   // Header file for LED dotmatrix
#include "pitches.h"      // Header file for passive buzzer

#define TRIG_PIN 6
#define ECHO_PIN 7

// MAX7219 dotmatrix module
LedControl lc = LedControl(12, 10, 11, 1);

// SR04 ultrasonic sensor module
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long a;
int lastRowsToUnlit = -1; // Store the previous state to avoid unnecessary updates
const int buzzerPin = 8; // Buzzer connected to pin 8

void setup() {
  /* Disable power saving mode */
  lc.shutdown(0, false);
  /* Set the brightness to a medium value */
  lc.setIntensity(0, 5); // Adjust brightness (0-15)
  /* and clear the display */
  lc.clearDisplay(0);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600); // Initialization of Serial Port
  delay(1000);
}

void loop() {
  a = sr04.Distance(); // Get distance reading in cm

  // Calculate how many rows to leave unlit
  int rowsToUnlit = a / 5;
  if (rowsToUnlit > 8) {
    rowsToUnlit = 8; // Cap at 8 rows
  }

  // Update the display only if the pattern changes
  if (rowsToUnlit != lastRowsToUnlit) {
    lc.clearDisplay(0); // Clear the display before updating the new pattern

    // Light up rows inversely proportional to the distance
    for (int i = 0; i < 8 - rowsToUnlit; i++) {
      lc.setRow(0, i, B11111111); // Turn on all LEDs in row `i`
    }

    // Store the current state
    lastRowsToUnlit = rowsToUnlit;
  }

  // Buzzer feedback logic
  if (a > 0 && a <= 10) { // Constant buzzer sound if within 10 cm
    tone(buzzerPin, 600); // Fixed tone frequency (800 Hz)
  } else if (a > 10 && a <= 40) { // Intermittent buzzer sound if between 10 cm and 40 cm
    int duration = map(a, 11, 40, 50, 500); // Map distance to tone interval (closer = shorter interval)
    tone(buzzerPin, 600, 100); // Fixed tone frequency (800 Hz) with fixed duration (100 ms)
    delay(duration); // Delay based on the distance
  } else {
    noTone(buzzerPin); // Stop the buzzer if out of range
  }

  Serial.print(a);
  Serial.println("cm"); // Print distance in cm

  delay(50); // Short delay for smoother updates
}
