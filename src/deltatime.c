#include "deltatime.h"

double _deltatime;

void deltatime_set(double deltatime) {
    _deltatime = deltatime;
}

double deltatime() {
    return _deltatime;
}
