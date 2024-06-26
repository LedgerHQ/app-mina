#pragma once

#include "globals.h"

void handle_test_crypto(uint8_t p1, uint8_t p2, uint8_t *dataBuffer,
                        uint8_t dataLength, volatile unsigned int *flags);

void ui_test_crypto(uint8_t* dataBuffer);
void test_crypto(void);
