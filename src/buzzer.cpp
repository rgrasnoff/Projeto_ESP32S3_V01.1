#include "buzzer.h"

// Variáveis internas
bool buzzerActive = false;
unsigned long lastBuzzerTime = 0;
bool buzzerState = false;

// Variáveis para o modo customizado
unsigned long lastCustomBuzzerTime = 0;
bool customBuzzerState = false;

// Guarda qual frequência está tocando no momento. 0 = silêncio.
int currentActiveFreq = 0; 

void initBuzzer() {
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
}

void buzzerOff() {
    if (currentActiveFreq != 0 || buzzerActive || customBuzzerState) {
        noTone(PIN_BUZZER);
        digitalWrite(PIN_BUZZER, LOW);
        
        buzzerActive = false;
        buzzerState = false;
        customBuzzerState = false;
        currentActiveFreq = 0; 
    }
}

// ... (Mantenha as funções setBuzzerAlert e handleBuzzerBeeping iguais) ...
void setBuzzerAlert(bool state) {
    buzzerActive = state;
    if (!state) {
        buzzerOff();
    }
}

void handleBuzzerBeeping() {
    if (buzzerActive) {
        unsigned long current = millis();
        if (current - lastBuzzerTime >= 100) {
            lastBuzzerTime = current;
            buzzerState = !buzzerState;
            if (buzzerState) {
                tone(PIN_BUZZER, 2000);
                currentActiveFreq = 2000;
            } else {
                noTone(PIN_BUZZER);
                currentActiveFreq = 0;
            }
        }
    }
}

// ... (Mantenha buzzerCustomTone igual para compatibilidade) ...
void buzzerCustomTone(int frequency, int intervalMs) {
    if (intervalMs == 0) {
        if (currentActiveFreq != frequency) {
            tone(PIN_BUZZER, frequency);
            currentActiveFreq = frequency;
        }
        return;
    }

    unsigned long current = millis();
    if (current - lastCustomBuzzerTime >= intervalMs) {
        lastCustomBuzzerTime = current;
        customBuzzerState = !customBuzzerState;
        
        if (customBuzzerState) {
            if (currentActiveFreq != frequency) {
                tone(PIN_BUZZER, frequency);
                currentActiveFreq = frequency;
            }
        } else {
            if (currentActiveFreq != 0) {
                noTone(PIN_BUZZER);
                currentActiveFreq = 0;
            }
        }
    }
}

// --- NOVA FUNÇÃO MELÓDICA ---
void buzzerMelodicTone(int startFreq, int endFreq, int intervalMs) {
    // Se intervalo for 0, trata como tom contínuo fixo na frequência final
    if (intervalMs == 0) {
        buzzerCustomTone(endFreq, 0);
        return;
    }

    unsigned long current = millis();
    
    // Gerencia o tempo de Ligar/Desligar (Blink)
    if (current - lastCustomBuzzerTime >= intervalMs) {
        lastCustomBuzzerTime = current;
        customBuzzerState = !customBuzzerState;
        
        // Se desligou, silencia imediatamente
        if (!customBuzzerState) {
            if (currentActiveFreq != 0) {
                noTone(PIN_BUZZER);
                currentActiveFreq = 0;
            }
        }
    }

    // Se está no estado LIGADO, calcula a frequência atual (Sweep)
    if (customBuzzerState) {
        unsigned long elapsed = current - lastCustomBuzzerTime;
        if (elapsed > intervalMs) elapsed = intervalMs;

        // Interpolação linear de startFreq até endFreq
        float progress = (float)elapsed / (float)intervalMs; 
        int currentFreq = startFreq + (int)((endFreq - startFreq) * progress);

        // Atualiza o tom se a frequência mudou
        if (currentActiveFreq != currentFreq) {
            tone(PIN_BUZZER, currentFreq);
            currentActiveFreq = currentFreq;
        }
    }
}