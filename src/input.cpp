#include "input.h"

void initInputs() {
    // Configura os pinos como entrada com resistor de pull-up interno
    pinMode(BOTAO_FREIO, INPUT_PULLUP);
    pinMode(BOTAO_RODA, INPUT_PULLUP);
}

// Função auxiliar para leitura limpa (evita ruído elétrico)
bool lerPinoComDebounce(int pino) {
    if (digitalRead(pino) == LOW) { // LOW significa pressionado (Pull-up)
        delay(10); // Pequeno delay de debounce (10ms é imperceptível)
        if (digitalRead(pino) == LOW) {
            return true;
        }
    }
    return false;
}

bool lerBotaoFreio() {
    return lerPinoComDebounce(BOTAO_FREIO);
}

bool lerBotaoRoda() {
    return lerPinoComDebounce(BOTAO_RODA);
}