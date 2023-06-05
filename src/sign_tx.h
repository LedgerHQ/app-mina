#pragma once

#include "globals.h"

void handle_sign_tx(uint8_t p1, uint8_t p2, uint8_t *dataBuffer,
                    uint8_t dataLength, volatile unsigned int *flags);

void ui_sign_tx(uint8_t* dataBuffer, uint8_t dataLength);
