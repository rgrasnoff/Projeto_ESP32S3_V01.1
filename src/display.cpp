#include "display.h"

LGFX display;

DisplayState currentDisplayState = DisplayState::IDLE;
unsigned long alertStartTime = 0;
const char* currentAlertFile = nullptr;

// Variável para controlar qual imagem está na tela atualmente
String lastWheelFile = ""; 

LGFX::LGFX(void) {
  // ... (Mantenha o código de configuração do construtor IGUAL ao original) ...
  // Copie o conteúdo do construtor original aqui
    {
    auto cfg = _bus_instance.config();
    cfg.spi_host = SPI2_HOST;
    cfg.spi_mode = 3; 
    cfg.freq_write = 40000000; 
    cfg.freq_read  = 16000000;
    cfg.pin_sclk = PIN_SCLK;
    cfg.pin_mosi = PIN_MOSI;
    cfg.pin_miso = -1;
    cfg.pin_dc   = PIN_DC;
    _bus_instance.config(cfg);
    _panel_instance.setBus(&_bus_instance);
  }

  {
    auto cfg = _panel_instance.config();
    cfg.pin_cs   = PIN_CS;
    cfg.pin_rst  = PIN_RST;
    cfg.pin_busy = -1;
    cfg.memory_width  = 240;
    cfg.memory_height = 320;
    cfg.panel_width   = TELA_LARGURA;
    cfg.panel_height  = TELA_ALTURA;
    cfg.offset_x      = OFFSET_X;
    cfg.offset_y      = OFFSET_Y;
    cfg.invert        = false;
    cfg.rgb_order     = true;
    cfg.dlen_16bit    = false;
    cfg.bus_shared    = false;
    _panel_instance.config(cfg);
  }
  setPanel(&_panel_instance);
}

void initDisplay() {
  pinMode(PIN_BLK, OUTPUT);
  digitalWrite(PIN_BLK, HIGH); 

  pinMode(PIN_RST, OUTPUT);
  digitalWrite(PIN_RST, LOW);  delay(100);
  digitalWrite(PIN_RST, HIGH); delay(100);

  display.init();
  display.setRotation(3);
  display.fillScreen(TFT_BLACK);

  if (!LittleFS.begin()) {
    Serial.println("ERRO CRITICO: Falha ao montar LittleFS");
    while (true); 
  }
  Serial.println("Display e Arquivos inicializados.");
}

void startAlert(const char* nomeArquivo) {
  if (currentDisplayState == DisplayState::IDLE) {
    currentAlertFile = nomeArquivo;
    currentDisplayState = DisplayState::ALERT_SHOWING;
    alertStartTime = millis(); 
    
    digitalWrite(PIN_BLK, HIGH); 
    
    if (LittleFS.exists(currentAlertFile)) {
        display.drawBmpFile(LittleFS, currentAlertFile, 0, 0);
    } 
    
    digitalWrite(PIN_BLK, LOW); 
  }
}

void handleDisplayAlert() {
  unsigned long currentTime = millis();
  
  switch (currentDisplayState) {
    case DisplayState::IDLE:
      break;

    case DisplayState::ALERT_SHOWING:
      if (currentTime - alertStartTime >= 2000) {
        digitalWrite(PIN_BLK, HIGH); 
        alertStartTime = currentTime; 
        currentDisplayState = DisplayState::ALERT_PAUSE;
      }
      break;

    case DisplayState::ALERT_PAUSE:
      if (currentTime - alertStartTime >= 500) {
        currentDisplayState = DisplayState::IDLE; 
        currentAlertFile = nullptr;
      }
      break;
  }
}

// --- NOVA IMPLEMENTAÇÃO: IMAGEM DA RODA ---
void updateWheelImage(const char* filename) {
    // Liga o backlight se estiver desligado
    digitalWrite(PIN_BLK, LOW);

    // Verifica se a imagem mudou antes de desenhar (evita flicker e lentidão)
    if (String(filename) != lastWheelFile) {
        Serial.printf("Mudando imagem para: %s\n", filename);
        
        // Verifica se arquivo existe
        if (LittleFS.exists(filename)) {
            display.drawBmpFile(LittleFS, filename, 0, 0);
            lastWheelFile = String(filename);
        } else {
             // Opcional: tratar erro ou manter imagem anterior
             Serial.println("Arquivo nao encontrado!");
        }
    }
}

void clearWheelMode() {
    if (lastWheelFile != "") {
        display.fillScreen(TFT_BLACK);
        lastWheelFile = "";
        digitalWrite(PIN_BLK, HIGH); // Desliga tela ao sair
    }
}