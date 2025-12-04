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

// NOVA FUNÇÃO: Efeito melódico (Sweep)
// Faz uma transição de startFreq para endFreq durante o tempo ligado
void buzzerMelodicTone(int startFreq, int endFreq, int intervalMs);

// Para o som imediatamente
void buzzerOff();

#endif