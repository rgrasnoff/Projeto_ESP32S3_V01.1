#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "define.h"

void initBuzzer();

// Lógica antiga de alerta simples
void setBuzzerAlert(bool state);
void handleBuzzerBeeping();

// Controle preciso de tom e intervalo (Frequência Fixa)
void buzzerCustomTone(int frequency, int intervalMs);

// Para o som imediatamente
void buzzerOff();

#endif