#include "outputs.h"

// CORREÇÃO: Usar o nome de classe sugerido pelo compilador
Adafruit_MCP23X17 mcp; 

void initOutputs() {
  Serial.println("Iniciando Módulo de Saídas...");
  
  Wire.begin(PIN_SDA, PIN_SCL); 

  if (!mcp.begin_I2C(MCP_I2C_ADDRESS, &Wire)) {
    Serial.println("ERRO CRITICO: Falha ao encontrar o MCP23017 no endereço 0x20");
    while (true);
  }
  
  // Configura os pinos restantes (apenas relés) como OUTPUT
  mcp.pinMode(MCP_RELE_PCE, OUTPUT);
  mcp.pinMode(MCP_RELE_PCD, OUTPUT);
  mcp.pinMode(MCP_RELE_FREIO, OUTPUT);
  mcp.pinMode(MCP_RELE_PISCA, OUTPUT);
  
  // Inicializa relés como DESLIGADOS 
  mcp.digitalWrite(MCP_RELE_PCE, LOW);
  mcp.digitalWrite(MCP_RELE_PCD, LOW);
  mcp.digitalWrite(MCP_RELE_FREIO, LOW);
  mcp.digitalWrite(MCP_RELE_PISCA, LOW);
  
  Serial.println("MCP23017 e Saídas inicializadas.");
}

/**
 * @brief Controla o estado de um relé conectado ao MCP23017 (Lógica Ativa LOW).
 * @param pin O número do pino do MCP23017 (ex: MCP_RELE_PCE).
 * @param state true (LIGA) ou false (DESLIGA).
 */
void setRelay(uint8_t pin, bool state) {
  // Se 'state' é true (LIGAR), a saída deve ser LOW. Se 'state' é false (DESLIGAR), a saída deve ser HIGH.
  mcp.digitalWrite(pin, state ? LOW : HIGH);
}
// toggleBuzzer() removida deste arquivo.