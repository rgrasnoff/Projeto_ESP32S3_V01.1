#ifndef DEFINE_H
#define DEFINE_H

#include <Arduino.h>

// --- CONFIGURAÇÃO DE TEMPO ---
#define TEMPO_POS_ATIVACAO      2000 
#define TEMPO_POS_ATIVACAO_TRAS 500  
#define TEMPO_PISCA_TRAS        500  

// --- FREQUÊNCIAS DO BUZZER (Novas) ---
#define FREQ_400HZ      300
#define FREQ_600HZ      500
#define FREQ_800HZ      800
#define FAST_BIP_MS     300
#define SLOW_BIP_MS     500

// --- LÓGICA DOS RELÉS (Active HIGH) ---
#define RELE_LIGADO     HIGH
#define RELE_DESLIGADO  LOW

// --- PINAGEM DO HARDWARE ---
// DISPLAY (SPI)
#define PIN_SCLK  12
#define PIN_MOSI  11
#define PIN_DC    4
#define PIN_RST   5
#define PIN_CS    3
#define PIN_BLK   14 

// BUZZER
#define PIN_BUZZER 17

// --- SENSORES ULTRASSÔNICOS ---
#define TRIG_TRAS  1
#define ECHO_TRAS  2
#define DIST_TRAS_MAX 60 
#define DIST_TRAS_PERIGO 29 

#define TRIG_ESQ   42
#define ECHO_ESQ   41
#define DIST_LADO_MAX 30 

#define TRIG_DIR   40
#define ECHO_DIR   39

#define TRIG_RODA  6 
#define ECHO_RODA  7
#define DIST_RODA_MIN 20  // Distância mínima de medição
#define DIST_RODA_MAX 100 // Distância máxima de medição

// --- CONFIGURAÇÕES DA TELA ---
#define TELA_LARGURA  76
#define TELA_ALTURA   284
#define OFFSET_X      82
#define OFFSET_Y      17

// --- PINAGEM DOS BOTÕES ---
#define BOTAO_FREIO  15 
#define BOTAO_RODA   16

// --- PINAGEM I2C PARA MCP23017 ---
#define PIN_SCL   9
#define PIN_SDA   8

// --- SAÍDAS DO MCP23017 ---
#define MCP_RELE_PCE   0
#define MCP_RELE_PCD   1
#define MCP_RELE_FREIO 2
#define MCP_RELE_PISCA 3

#endif