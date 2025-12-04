#ifndef OUTPUTS_H
#define OUTPUTS_H

#include <Arduino.h>
#include <Wire.h> 
#include <Adafruit_MCP23X17.h> 
#include "define.h"

// Endereço I2C padrão: 0x20 (A0, A1 e A2 em GND)
#define MCP_I2C_ADDRESS 0x20

// Protótipos das funções
void initOutputs();
void setRelay(uint8_t pin, bool state);
// toggleBuzzer foi REMOVIDA, pois está em main.cpp

#endif // OUTPUTS_H