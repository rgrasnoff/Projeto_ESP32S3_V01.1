#include <Arduino.h>
#include "define.h"
#include "display.h"
#include "outputs.h" 
#include "buzzer.h"
#include "input.h" 
#include "ultrasonic.h"

unsigned long lastDetectionEsq = 0;
unsigned long lastDetectionDir = 0;
unsigned long lastDetectionTras = 0;

float lastValidDistTras = 0;     
unsigned long lastBlinkTimer = 0; 
bool blinkState = false;          

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("--- SISTEMA INICIADO ---"); 

  initBuzzer(); 
  initDisplay();
  initOutputs(); 
  initInputs(); 
  initUltrasonics();
}

void loop() {
  handleDisplayAlert(); 

  bool modoRodaAtivo  = lerBotaoRoda();  
  bool modoFreioAtivo = lerBotaoFreio(); 

  // ==========================================
  // CASO 1: MODO RODA (NOVA LÓGICA)
  // ==========================================
  if (modoRodaAtivo) {
      // Desliga relés externos
      setRelay(MCP_RELE_PCE, RELE_DESLIGADO);
      setRelay(MCP_RELE_PCD, RELE_DESLIGADO);
      setRelay(MCP_RELE_FREIO, RELE_DESLIGADO);
      setRelay(MCP_RELE_PISCA, RELE_DESLIGADO);

      float distRoda = lerDistancia(TRIG_RODA, ECHO_RODA);
      
      const char* imgFile = "";
      
      // Lógica de Distância vs Imagem vs Buzzer
      // Usamos 'if else' em cascata para cobrir as faixas
      
      if (distRoda > 40) {
          imgFile = "/0B.bmp";
          buzzerOff();
      }
      // <=40 e >=37 (Buzzer 400Hz / 700ms)
      else if (distRoda >= 37) {
          imgFile = "/1B.bmp";
          buzzerCustomTone(FREQ_400HZ, SLOW_BIP_MS);
      }
      // <=36 e >=34
      else if (distRoda >= 34) {
          imgFile = "/2B.bmp";
          buzzerCustomTone(FREQ_400HZ, SLOW_BIP_MS);
      }
      // <=33 e >=30
      else if (distRoda >= 30) {
          imgFile = "/3B.bmp";
          buzzerCustomTone(FREQ_400HZ, SLOW_BIP_MS);
      }
      // <=29 e >=27
      else if (distRoda >= 27) {
          imgFile = "/4B.bmp";
          buzzerCustomTone(FREQ_400HZ, ((SLOW_BIP_MS) - 100));
      }
      // <=26 e >=24
      else if (distRoda >= 24) {
          imgFile = "/5B.bmp";
          buzzerCustomTone(FREQ_400HZ, ((SLOW_BIP_MS) - 100));
      }
      // <=23 e >=20
      else if (distRoda >= 20) {
          imgFile = "/6B.bmp";
          buzzerCustomTone(FREQ_400HZ, ((SLOW_BIP_MS) - 150));
      }
      // <=19 e >=18 (Muda para 600Hz / 400ms)
      else if (distRoda >= 18) {
          imgFile = "/7B.bmp";
          buzzerCustomTone(FREQ_600HZ, FAST_BIP_MS);
      }
      // <=17 e >=16
      else if (distRoda >= 16) {
          imgFile = "/8B.bmp";
          buzzerCustomTone(FREQ_600HZ, FAST_BIP_MS);
      }
      // <=15 e >=14
      else if (distRoda >= 14) {
          imgFile = "/9B.bmp";
          buzzerCustomTone(FREQ_600HZ, ((FAST_BIP_MS)-100));
      }
      // <=14 e >=13 (Atenção: sobreposição com 14 acima, a ordem do if resolve)
      else if (distRoda >= 13) {
          imgFile = "/10B.bmp";
          buzzerCustomTone(FREQ_600HZ, ((FAST_BIP_MS)-150));
      }
      // <=10 e >=9 (OBS: Há um buraco entre 13 e 10. Esta lógica cobrirá 12 e 11 com 10B até chegar em 10.9)
      else if (distRoda >= 9) {
          imgFile = "/11B.bmp";
          buzzerCustomTone(FREQ_800HZ, 0); // 0 = Contínuo (ou defina intervalo se necessário)
      }
      // <=9 e >=7
      else if (distRoda >= 7) {
          imgFile = "/12B.bmp";
          buzzerCustomTone(FREQ_800HZ, 0);
      }
      // <7
      else if (distRoda > 0) {
          imgFile = "/13B.bmp";
          buzzerCustomTone(FREQ_800HZ, 0);
      }
      else {
          // Leitura inválida ou 0
          buzzerOff();
          // Mantém a última imagem ou limpa? Vamos manter para não piscar
      }

      // Atualiza a tela se houver imagem válida
      if (strlen(imgFile) > 0) {
          updateWheelImage(imgFile);
      }
      
      lastDetectionEsq = 0;
      lastDetectionDir = 0;
      lastDetectionTras = 0;
  }
  
  // ==========================================
  // CASO 2: MODO CONDUÇÃO
  // ==========================================
  else {
      clearWheelMode(); // Limpa a tela do modo roda
      
      if (currentDisplayState == DisplayState::IDLE) buzzerOff();
      else handleBuzzerBeeping(); 

      unsigned long currentTime = millis();

      // --- A. LEITURA ---
      float distEsq = lerDistancia(TRIG_ESQ, ECHO_ESQ);
      if (distEsq > 0 && distEsq <= DIST_LADO_MAX) lastDetectionEsq = currentTime;

      float distDir = lerDistancia(TRIG_DIR, ECHO_DIR);
      if (distDir > 0 && distDir <= DIST_LADO_MAX) lastDetectionDir = currentTime;

      if (!modoFreioAtivo) { 
          float distTras = lerDistancia(TRIG_TRAS, ECHO_TRAS);
          if (distTras > 0 && distTras <= DIST_TRAS_MAX) {
              lastDetectionTras = currentTime; 
              lastValidDistTras = distTras;
          }
      } else {
          lastDetectionTras = 0; 
      }

      // --- B. ESTADO ---
      bool ativoEsq  = (currentTime - lastDetectionEsq < TEMPO_POS_ATIVACAO);
      bool ativoDir  = (currentTime - lastDetectionDir < TEMPO_POS_ATIVACAO);
      bool ativoTras = (currentTime - lastDetectionTras < TEMPO_POS_ATIVACAO_TRAS);

      // --- C. ATUAÇÃO ---
      if (ativoEsq && ativoDir) {
          setRelay(MCP_RELE_PCE, RELE_LIGADO);
          setRelay(MCP_RELE_PCD, RELE_LIGADO);
          startAlert("/ImgAMBOS.bmp"); 
      }
      else if (ativoEsq) {
          setRelay(MCP_RELE_PCE, RELE_LIGADO);
          setRelay(MCP_RELE_PCD, RELE_DESLIGADO);
          startAlert("/ImgPCE.bmp"); 
      }
      else if (ativoDir) {
          setRelay(MCP_RELE_PCD, RELE_LIGADO);
          setRelay(MCP_RELE_PCE, RELE_DESLIGADO);
          startAlert("/ImgPCD.bmp"); 
      }
      else {
          setRelay(MCP_RELE_PCE, RELE_DESLIGADO);
          setRelay(MCP_RELE_PCD, RELE_DESLIGADO);
      }

      // Lógica Traseira
      if (ativoTras) {
          if (lastValidDistTras <= DIST_TRAS_PERIGO) {
              setRelay(MCP_RELE_FREIO, RELE_LIGADO); 
              setRelay(MCP_RELE_PISCA, RELE_DESLIGADO); 
          } 
          else {
              setRelay(MCP_RELE_FREIO, RELE_DESLIGADO); 
              if (currentTime - lastBlinkTimer >= TEMPO_PISCA_TRAS) {
                  blinkState = !blinkState; 
                  lastBlinkTimer = currentTime;
              }
              setRelay(MCP_RELE_PISCA, blinkState ? RELE_LIGADO : RELE_DESLIGADO);
          }
      } 
      else {
          setRelay(MCP_RELE_FREIO, RELE_DESLIGADO);
          if (ativoEsq && ativoDir) {
              setRelay(MCP_RELE_PISCA, RELE_LIGADO); 
          } else {
              setRelay(MCP_RELE_PISCA, RELE_DESLIGADO);
          }
      }
  }
  
  delay(30); 
}