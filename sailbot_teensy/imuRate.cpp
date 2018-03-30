#include "imuRate.h"
#include <Arduino.h>

imuRate::imuRate(int hz) {
    periodUs = (1.0/(double)hz) * 1000000;
    lastUs = 0;
}

bool imuRate::needsRun() {
    unsigned long dt = micros() - lastUs;
    if ( dt >= periodUs ) {
        lastUs = micros();
        return true;
    }
    return false;
}

void imuRate::sleep() {
    unsigned long dt = micros() - lastUs;

    if ( dt < periodUs )
        delayMicroseconds(periodUs-dt);

    lastUs = micros();
}
