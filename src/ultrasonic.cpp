#include "ultrasonic.h"

void initUltrasonics() {
    pinMode(TRIG_TRAS, OUTPUT); pinMode(ECHO_TRAS, INPUT);
    pinMode(TRIG_ESQ,  OUTPUT); pinMode(ECHO_ESQ,  INPUT);
    pinMode(TRIG_DIR,  OUTPUT); pinMode(ECHO_DIR,  INPUT);
    pinMode(TRIG_RODA, OUTPUT); pinMode(ECHO_RODA, INPUT);
}

float lerDistancia(int pinTrig, int pinEcho) {
    // 1. Gera o pulso de Trigger
    digitalWrite(pinTrig, LOW);
    delayMicroseconds(2);
    digitalWrite(pinTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrig, LOW);

    // 2. Lê o Echo com timeout maior
    long duration = pulseIn(pinEcho, HIGH, 23200); // ~400cm para ter margem

    if (duration == 0) {
        return -1; // Sem leitura
    }

    // 3. Calcula distância em cm
    float distancia = duration * 0.034 / 2;
    
    return distancia;
}