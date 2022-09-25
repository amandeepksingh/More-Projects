/*
 * Honors 391AH (Electronics for Research and Prototyping): Stress Tester
 * By Ibrahima Keita, Amandeep Kaur Singh
 */

#include "Configuration.hpp"

// Needed to interface with the sensor.
PulseOximeter pox;

// LED arrays
CRGB thumbLEDS[THUMB_LEDS];
CRGB pointerLEDS[POINTER_LEDS];
CRGB ringLEDS[RING_LEDS];
CRGB pinkyLEDS[PINKY_LEDS];
CRGB middleLEDS[MIDDLE_LEDS];
CRGB wristLEDS[WRIST_LEDS];

// Needed for calculating average heart rate, and SpO2.
float heartRateOverTime[QSIZE]; // heart rate
float spO2OverTime[QSIZE // oxygen
double noStressHeartRateUpperBound, someStressHeartRateUpperBound; // bounds 
int queueIndex = 0; // index for queues
bool calibration = true;

void setup() {
    Serial.begin(9600); // BAUD RATE 
    LED_Setup();
    switch (CURRENT_MODE) {
      // Mode 0: Stress Detection.
      case MODE_STRESS: {
        // Check if the MAX30100 is connected over I2C. If it is, then the loop will terminate.
        while (!pox.begin()) {
          Serial.println("Sensor not connected to the I2C bus. Will retry in a few seconds...");
          delay(1000);
        }
        Serial.println("Sensor has been detected on the I2C bus. Starting calculations.");
        pox.setOnBeatDetectedCallback(onBeatDetected); // Set callback to update BPM.
        break;
      }
      // Mode 1: Rainbow.
      case MODE_RAINBOW: {
        break;
      }
      // Mode 2: Stress Ball.
      case MODE_STRESS_BALL: {
        pinMode(THUMB, INPUT);
        break;
      }      
      // Mode 3: Thanos Mode (Don't ask).
      case MODE_THANOS: {
        break;
      }
    }
}

void loop() {
   switch (CURRENT_MODE) {
    // Mode 0: Stress Detection.
    case MODE_STRESS: {
      pox.update();
      break;
    }
    // Mode 1: Rainbow.
    case MODE_RAINBOW: {
      break;
    }
    // Mode 2: Stress Ball.
    case MODE_STRESS_BALL: {
      Serial.println(analogRead(THUMB));
      break;
    }      
    // Mode 3: Thanos Mode (Don't ask).
    case MODE_THANOS: {
      break;
    }
  }
}

void onBeatDetected() {
  double heartRate = pox.getHeartRate(), spO2 = pox.getSpO2();
  if (queueIndex >= QSIZE) {
    if (!calibration) {
      // We have enough data to calculate what we want.
      double normalizedHeartRate = getAverage(heartRateOverTime), normalizedSpO2 = getAverage(spO2OverTime);
      Serial.println(normalizedHeartRate);
      Serial.println(normalizedHeartRate < noStressHeartRateUpperBound);
      if (normalizedHeartRate < noStressHeartRateUpperBound) {
        // No stress.
        LED_StaticColorSet(CRGB::Green);
      } else if (normalizedHeartRate >= noStressHeartRateUpperBound && normalizedHeartRate < someStressHeartRateUpperBound) {
        // Some stress.
        LED_StaticColorSet(CRGB::Yellow);
      } else {
        // Lots of stress.
        LED_StaticColorSet(CRGB::Red);
      }
    } else { 
      float restingHeartRateAverage = 0.0, restingSpO2Average = 0.0;
      for (int i = 0; i < QSIZE; ++i) {
        restingHeartRateAverage += heartRateOverTime[i];
        restingSpO2Average += spO2OverTime[i];
      }
      restingHeartRateAverage /= QSIZE;
      restingSpO2Average /= QSIZE;
      
      Serial.print("Resting heart rate: ");
      Serial.println(restingHeartRateAverage);
      Serial.print("Resting SpO2: ");
      Serial.println(restingSpO2Average);
      
      noStressHeartRateUpperBound = restingHeartRateAverage + (restingHeartRateAverage * NO_STRESS_THRESHOLD);
      someStressHeartRateUpperBound = restingHeartRateAverage + (restingHeartRateAverage * SOME_STRESS_THRESHOLD);

      
      Serial.print("Lower bound: ");
      Serial.println(noStressHeartRateUpperBound);
      Serial.print("Upper bound: ");
      Serial.println(someStressHeartRateUpperBound);
      calibration = false;
    }
    queueIndex = 0; 
  }
  heartRateOverTime[queueIndex] = heartRate;
  spO2OverTime[queueIndex++] = spO2;
}


// Helpers
void LED_Setup() {
    // Add all of the LEDs, so we can individually control them.
    FastLED.addLeds<WS2812, THUMB_LED_PIN, GRB>(thumbLEDS, THUMB_LEDS);
    FastLED.addLeds<WS2812, POINTER_LED_PIN, GRB>(pointerLEDS, POINTER_LEDS);
    FastLED.addLeds<WS2812, RING_LED_PIN, GRB>(ringLEDS, RING_LEDS);
    FastLED.addLeds<WS2812, PINKY_LED_PIN, GRB>(pinkyLEDS, PINKY_LEDS);
    FastLED.addLeds<WS2812, MIDDLE_LED_PIN, GRB>(middleLEDS, MIDDLE_LEDS);
    FastLED.addLeds<WS2812, WRIST_LED_PIN, GRB>(wristLEDS, WRIST_LEDS);
    FastLED.clear();
    FastLED.show();
}

void LED_StaticColorSet(CRGB color) {
  fill_solid(thumbLEDS, THUMB_LEDS, color);
  fill_solid(pointerLEDS, POINTER_LEDS, color);
  fill_solid(ringLEDS, RING_LEDS, color);
  fill_solid(middleLEDS, MIDDLE_LEDS, color);
  fill_solid(pinkyLEDS, PINKY_LEDS, color);
  fill_solid(wristLEDS, WRIST_LEDS, color);
  FastLED.show();
}

float getAverage(float data[QSIZE]) {
  float sum = 0.0;
  for (unsigned char count = 0; count < QSIZE; ++count) {
    sum += data[count];
  }
  return sum / QSIZE;
}