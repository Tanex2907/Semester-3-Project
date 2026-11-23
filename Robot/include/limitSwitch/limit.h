#pragma once
#include "locomotion/locomotion.h"

#define PIN_LIMIT               17

void setupLimitSwitches();

void IRAM_ATTR isrRecognizeHit();