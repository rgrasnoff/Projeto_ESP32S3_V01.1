#ifndef DISPLAY_H
#define DISPLAY_H

#include <FS.h>
#include <LittleFS.h>
#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <SPI.h>
#include "define.h"

enum class DisplayState {
    IDLE,                 
    ALERT_SHOWING,        
    ALERT_PAUSE           
};

class LGFX : public lgfx::LGFX_Device {
  lgfx::Bus_SPI _bus_instance;
  lgfx::Panel_ST7789P3 _panel_instance;
public:
  LGFX(void);
};

extern LGFX display;
extern DisplayState currentDisplayState; 

void initDisplay();

// Alertas laterais/traseiros
void startAlert(const char* nomeArquivo);
void handleDisplayAlert();

// NOVA FUNÇÃO: Atualiza a imagem da roda baseada no arquivo passado
// Só redesenha se a imagem mudar para evitar "flicker"
void updateWheelImage(const char* filename);

// Limpa a tela ao sair do modo roda
void clearWheelMode();

#endif