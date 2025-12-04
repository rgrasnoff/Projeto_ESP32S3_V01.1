#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include "define.h"

// Inicializa os pinos dos botões/sensores
void initInputs();

// Retorna true se o botão estiver pressionado (com debounce básico)
bool lerBotaoFreio();
bool lerBotaoRoda();

// Futuro: Função para ler sensores de radar/ultrassom
// bool lerSensorPontoCegoEsq();
// bool lerSensorPontoCegoDir();

#endif