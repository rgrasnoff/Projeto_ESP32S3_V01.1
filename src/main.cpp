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

// Monitoramento em tempo real da roda
unsigned long lastMonitoringTime = 0;
const unsigned long MONITORING_INTERVAL = 100; // Atualiza a cada 100ms          

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
  // CASO 1: MODO RODA (NOVA LÓGICA MELÓDICA)
  // ==========================================
  if (modoRodaAtivo) {
      // ... (desliga relés igual) ...
      setRelay(MCP_RELE_PCE, RELE_DESLIGADO);
      setRelay(MCP_RELE_PCD, RELE_DESLIGADO);
      setRelay(MCP_RELE_FREIO, RELE_DESLIGADO);
      setRelay(MCP_RELE_PISCA, RELE_DESLIGADO);

      float distRoda = lerDistancia(TRIG_RODA, ECHO_RODA);
      
      // Monitoramento em tempo real da distância
      unsigned long currentTime = millis();
      if (currentTime - lastMonitoringTime >= MONITORING_INTERVAL) {
          Serial.print("[RODA] Leitura bruta: ");
          Serial.print(distRoda);
          Serial.print(" cm | ");
          
          if (distRoda < 0) {
              Serial.println("SEM LEITURA (timeout)");
          } else if (distRoda < 20) {
              Serial.println("MUITO PRÓXIMO (< 20cm)");
          } else if (distRoda > 100) {
              Serial.println("MUITO DISTANTE (> 100cm)");
          } else {
              Serial.println("OK");
          }
          lastMonitoringTime = currentTime;
      }
      
      const char* imgFile = "";
      
      // Se estiver fora da faixa de medição, desativa som e exibe imagem de espera
      if (distRoda < 0 || distRoda < 20 || distRoda > 100) {
          imgFile = "/0B.bmp";
          buzzerOff();
      }
      // ZONA 1 (Distante): Efeito 500Hz -> 400Hz
      else if (distRoda >= 97) {
          imgFile = "/1B.bmp";
          buzzerCustomTone(500, SLOW_BIP_MS);
      }
      else if (distRoda >= 94) {
          imgFile = "/2B.bmp";
          buzzerCustomTone(500, SLOW_BIP_MS);
      }
      else if (distRoda >= 90) {
          imgFile = "/3B.bmp";
          buzzerCustomTone(500, SLOW_BIP_MS);
      }
      else if (distRoda >= 85) {
          imgFile = "/4B.bmp";
          buzzerCustomTone(500, ((SLOW_BIP_MS) - 100));
      }
      else if (distRoda >= 80) {
          imgFile = "/5B.bmp";
          buzzerCustomTone(500, ((SLOW_BIP_MS) - 100));
      }
      else if (distRoda >= 77) {
          imgFile = "/6B.bmp";
          buzzerCustomTone(500, ((SLOW_BIP_MS) - 150));
      }
      // ZONA 2 (Média): Efeito 700Hz -> 600Hz (Um pouco mais agudo)
      else if (distRoda >= 70) {
          imgFile = "/7B.bmp";
          buzzerCustomTone(700, FAST_BIP_MS);
      }
      else if (distRoda >= 64) {
          imgFile = "/8B.bmp";
          buzzerCustomTone(700, FAST_BIP_MS);
      }
      else if (distRoda >= 57) {
          imgFile = "/9B.bmp";
          buzzerCustomTone(700, ((FAST_BIP_MS)-100));
      }
      else if (distRoda >= 54) {
          imgFile = "/10B.bmp";
          buzzerCustomTone(700, ((FAST_BIP_MS)-150));
      }
      // ZONA 3 (Crítica): Som Contínuo Agudo (Mantém o padrão de alerta máximo)
      else if (distRoda >= 50) {
          imgFile = "/11B.bmp";
          buzzerCustomTone(FREQ_800HZ, 0); 
      }
      else if (distRoda >= 45) {
          imgFile = "/12B.bmp";
          buzzerCustomTone(FREQ_800HZ, 0);
      }
      else if (distRoda > 0) {
          imgFile = "/13B.bmp";
          buzzerCustomTone(FREQ_800HZ, 0);
      }
      else {
          buzzerOff();
      }

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
      // Se o botão de freio estiver ativo, garantir que tanto o relé de freio quanto o pisca estejam desligados
      // Mas continuar lendo os sensores laterais (ESQ/DIR). Apenas ignorar o sensor traseiro.
      if (modoFreioAtivo) {
          setRelay(MCP_RELE_FREIO, RELE_DESLIGADO);
          setRelay(MCP_RELE_PISCA, RELE_DESLIGADO);
          // Resetar estado de piscagem para evitar intermitência quando sair do freio
          blinkState = false;
          lastBlinkTimer = currentTime;
          // Garantir que a detecção traseira esteja zerada
          lastDetectionTras = 0;
          lastValidDistTras = 0;
          // Não sair do loop; permitir leitura dos sensores laterais abaixo
      }

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
          // Quando há detecção em AMBOS os lados, faz o pisca piscar
          if (ativoEsq && ativoDir) {
              if (currentTime - lastBlinkTimer >= TEMPO_PISCA_TRAS) {
                  blinkState = !blinkState; 
                  lastBlinkTimer = currentTime;
              }
              setRelay(MCP_RELE_PISCA, blinkState ? RELE_LIGADO : RELE_DESLIGADO);
          } else {
              setRelay(MCP_RELE_PISCA, RELE_DESLIGADO);
          }
      }
  }
  
  delay(30); 
}