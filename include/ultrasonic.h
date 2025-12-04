#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include "define.h"

// Inicializa os pinos dos sensores
void initUltrasonics();

// Lê a distância em cm de um sensor específico
// Retorna -1 se houver erro ou fora de alcance
float lerDistancia(int pinTrig, int pinEcho);

#endif