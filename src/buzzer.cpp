#include "buzzer.h"

// Variáveis internas
bool buzzerActive = false;
unsigned long lastBuzzerTime = 0;
bool buzzerState = false;

// Variáveis para o modo customizado
unsigned long lastCustomBuzzerTime = 0;
bool customBuzzerState = false;

// --- NOVA VARIÁVEL DE CONTROLE ---
// Guarda qual frequência está tocando no momento. 0 = silêncio.
int currentActiveFreq = 0; 

void initBuzzer() {
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
}

void buzzerOff() {
    // Só envia o comando de parar se estiver tocando algo
    if (currentActiveFreq != 0 || buzzerActive || customBuzzerState) {
        noTone(PIN_BUZZER);
        digitalWrite(PIN_BUZZER, LOW);
        
        // Reseta todos os estados
        buzzerActive = false;
        buzzerState = false;
        customBuzzerState = false;
        currentActiveFreq = 0; 
    }
}

// --- MODO ANTIGO (Simples - Alerta de Tela) ---
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

// --- NOVO MODO CUSTOMIZADO (Para a Roda) ---
void buzzerCustomTone(int frequency, int intervalMs) {
    // Caso 1: Som Contínuo
    if (intervalMs == 0) {
        // CORREÇÃO DO FLICKER:
        // Só chama tone() se a frequência for diferente da atual.
        // Se já estiver tocando 800Hz, não faz nada (mantém estável).
        if (currentActiveFreq != frequency) {
            tone(PIN_BUZZER, frequency);
            currentActiveFreq = frequency;
        }
        return;
    }

    // Caso 2: Som Intermitente (Bipando)
    unsigned long current = millis();
    if (current - lastCustomBuzzerTime >= intervalMs) {
        lastCustomBuzzerTime = current;
        customBuzzerState = !customBuzzerState;
        
        if (customBuzzerState) {
            // Hora de ligar o som
            if (currentActiveFreq != frequency) {
                tone(PIN_BUZZER, frequency);
                currentActiveFreq = frequency;
            }
        } else {
            // Hora de silenciar
            if (currentActiveFreq != 0) {
                noTone(PIN_BUZZER);
                currentActiveFreq = 0;
            }
        }
    }
}