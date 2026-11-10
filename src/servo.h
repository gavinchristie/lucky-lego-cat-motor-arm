#pragma once

#include <ESP32Servo.h>

extern Servo servo;

enum class WaveState { IDLE, UP, DOWN };

/**
 * Start a wave going.
 * 
 * count - number of times to wave
 */
void startWave(int count);

/**
 * Update the wave.
 * 
 * Switch between going up and down. If current state is idle don't do anything.
 */
void updateWave();
