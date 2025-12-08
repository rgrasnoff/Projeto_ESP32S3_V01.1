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
