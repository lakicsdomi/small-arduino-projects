// Temperature and humidity sensor
// Author: Lakics Domonkos

#include <dht_nonblocking.h>
#include "LedControl.h"
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 2;

LedControl lc = LedControl(12, 10, 11, 1); // MAX7219 dotmatrix module
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

/*
 * Initialize the serial port.
 */
void setup()
{
  // Setting up dotmatrix
  lc.shutdown(0, false);
  lc.setIntensity(0, 0);
  lc.clearDisplay(0);
  Serial.begin(9600); // for debugging
}

/*
 * Poll for a measurement, keeping the state machine alive. Returns
 * true if a measurement is available.
 */
static bool measure_environment(float *temperature, float *humidity)
{
  static unsigned long measurement_timestamp = millis();

  /* Measure once every 4 second. */
  if (millis() - measurement_timestamp > 4000ul)
  {
    if (dht_sensor.measure(temperature, humidity) == true)
    {
      measurement_timestamp = millis();
      return true;
    }
  }

  return false;
}

/*
 * Main program loop.
 */
void loop()
{
  float temperature;
  float humidity;

  /* Measure temperature and humidity. If the function returns
     true, then a measurement is available. */
  if (measure_environment(&temperature, &humidity) == true)
  {
    lc.clearDisplay(0);

    int leds_to_light = (int)temperature; // Number of LEDs to light
    leds_to_light = constrain(leds_to_light, 0, 64); // Ensure it's within matrix range

    for (int led = 0; led < leds_to_light; led++)
    {
      int row = led / 8; // Calculate row (0-7)
      int col = led % 8; // Calculate column (0-7)
      lc.setLed(0, row, col, true);
    }

    debug(temperature, humidity);
  }
}

void debug(int temp, int hum){
    Serial.print( "T = " );
    Serial.print( temp, 1 );
    Serial.print( " °C, H = " );
    Serial.print( hum, 1 );
    Serial.println( "%" );
}
