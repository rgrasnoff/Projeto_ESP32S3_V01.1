#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "define.h"

void initBuzzer();

// Lógica antiga de alerta simples
void setBuzzerAlert(bool state);
void handleBuzzerBeeping();

// NOVA FUNÇÃO: Controle preciso de tom e intervalo
// se intervalMs == 0, toca contínuo.
void buzzerCustomTone(int frequency, int intervalMs);

// Para o som imediatamente
void buzzerOff();

#endif