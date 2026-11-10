#include "servo.h"

constexpr size_t UP_STATE = 180;
constexpr size_t DOWN_STATE = 0;
constexpr unsigned long MOVE_DELAY_MS = 500;

WaveState state = WaveState::IDLE;
int wavesLeft = 0;
unsigned long lastMovement = 0;

void startWave(int count) {
    if (state != WaveState::IDLE) return;

    wavesLeft = count;
    state = WaveState::UP;
    servo.write(UP_STATE);
    lastMovement = millis();
}

void updateWave() {
    if (state == WaveState::IDLE) return;

    if(millis() - lastMovement < MOVE_DELAY_MS) return;

    lastMovement = millis();

    if(state == WaveState::UP) {
        servo.write(DOWN_STATE);
        state = WaveState::DOWN;
    } else {
        if (--wavesLeft > 0) {
            servo.write(UP_STATE);
            state = WaveState::UP;
        } else {
            state = WaveState::IDLE;
        }
    }
}
