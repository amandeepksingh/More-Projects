#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include <FastLED.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

// Modes to toggle through. This determines what sensors are available, and what actions are performed.
#define MODE_STRESS 0
#define MODE_RAINBOW 1
#define MODE_STRESS_BALL 2
#define MODE_THANOS 3

// Current mode.
#define CURRENT_MODE MODE_STRESS_BALL

// LEDs to add.
#define THUMB_LEDS 6
#define POINTER_LEDS 17
#define MIDDLE_LEDS 6
#define RING_LEDS 6
#define PINKY_LEDS 6
#define WRIST_LEDS 17

// LED IO pins.
#define WRIST_LED_PIN 7
#define THUMB_LED_PIN 8
#define PINKY_LED_PIN 9
#define RING_LED_PIN 10
#define MIDDLE_LED_PIN 11
#define POINTER_LED_PIN 12 

// Potentiometers.
#define THUMB A0

// Needed for stress calculations.
#define NO_STRESS_THRESHOLD 0.3 // Lower bound
#define SOME_STRESS_THRESHOLD 1.4 // Middle Bound

// Needed for initial heart rate calculations.
#define INITIAL_SAMPLE_SIZE 20

// Needed to determine how much data to use when calculating.
#define QSIZE 5

#endif