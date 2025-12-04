# Projeto_ESP32S3
Projeto de automação automotiva para segurança de condução

Compilação pelo PLatformIO via VS Code, para uma melhor compreensão e facilidade de processamento o código está dividido em módulos (funções).

O projeto tem a finalidade básica de porporcionar segurança automotiva, isto é:

Sensores ultrasônicos instalados ao redor do veículo fazem leituras/medições para retornar informações que devem atender parâmetros específicos e dessa forma apresentar na tela/display: resultados, alertas, mensagens e/ou uma barra de proximidade progressiva de blocos (12 blocos) em tempo real da proximidade de objeto, além de acender, apagar ou piscar sinalizadores das lanternas do veículo (luz de freio e pisca-pisca) e também um buzzer para um caso específico que exige mais atenção.

Existem 2 sensores laterais (ECHO_ESQ e TRIG_ESQ - ECHO_DIR e TRIG_DIR) que devem alertar o motorista a presença de objetos no ponto cego do veículo à Esquerda ou à Direita ou ainda em ambos os lados. Deve apresentar uma imagem ".bmp" na tela (display TFT) além de acender uma lâmpada/led no espelho retrovisor do lado pertinente, entenda Direita, Esquerda ou em ambos quando for o caso.

Existe 1 sensor próximo a roda do veículo (ECHO_RODA e TRIG_RODA) que deve ser acionado por um botão liga/desliga para que suas medições sejam avaliadas e apresentadas, caso contrário desconsiderado. Quando estiver ligado essa medida deve ter 4 parâmetros para alertas distintos, são eles: >30cm, <30cm, <20cm e <10cm, da seguinte forma: >30cm bips lentos, >=30cm e <=20cm a barra de proximidade progressiva proporcional até 6 blocos na cor verde com bips lentos de 400Hz, dos 19cm à 10cm da mesma forma continuará a preencher a barra com os próximos 4 blocos na cor amarela e o buzzer deve emitir bips intermitentes na razão de 800Hz com um interlavo para ligado e desligado mais rápidos. Quando o parâmetro estiver entre 9cm e 0cm a barra de proximidade deve continuar com os últimos 2 blocos na cor vermelha e o buzzerr toca direto à 800Hz. As medidas devem apenas aceitar numeros inteiros sem decimais, um relógio com atraso de 500milis mantêm a perfeita montagem da barra para não ficar piscando entre a variação de medição. Se o sensor da roda (ECHO_RODA - TRIG_RODA) estiver ligado os sensores lateriais e traseiro ficam inativos. 
Também considere que quando os sensores laterais estiverem acionados e atendendo a parâmetros de resultados tanto a imagem na tela quanto a luz do retrovisor devem permanecer ativos e quando o parâmetro não for mais atendido a imagem e a luz devem permanecer ativos por pelo mais 2 segundos. Caso exista uma medição que atenda um parâmentro dos lados opostos ao mesmo tempo outra imagem deve aparecer alertando ambos os dois lados e as luzes correspondentes devem ser acessas em conjunto e permanecerá acessa enquanto continuar atendendo o parâmetro por mais 2 segundos após.
Utiliza um extensor MCP23017 para conetar os relês de acionamento de lâmpadas ou leds.
Um sensor na traseira do veículo (ECHO_TRAS - TRIG_TRAS) deve medir distância entre 60cm e 0cm, considerando que o pedal de freio NÃO esteja acionado. O pedal de freio será um interruptor liga/desliga que quando acionado (HIGH) inativa este sensor. Caso contrário o sensor fará medições e as variáveis devem atender aparâmentros específicos, são esse: de 60cm a 30cm pisca as luzes do "pisca-pisca" com um intervalo de 1 segundos ligado/aceso(HIGH) e 1 segundo desligado/apagado(LOW) e se mantem piscando apenas enquando esse parâmetro for atendido. De 29cm à 0cm liga/acende(HIGH) a "luz de freio" sem piscar e se mantem assim enquando esse parâmetro for atendido.
O EPS32S3-WROOM-1 N16R8 Genérico. Deve desabilitar tanto o WiFi como o Bluetooth para não disperdiçar processamento As imagens (4 imagens) devem ser feito upload para a memória do ESP32S3
O Display é diferenciado: TFT SPI 2.5" polegadas ST7789P3 - resolução 76 pixel altura X 284 pixel largura e aproveitamento da tela iniciando em 18x82 (canto superior esquerdo no modo paisagem, sempre verificar a rotação).
// --- CONFIGURAÇÕES DA TELA PAISAGEM---
TELA_LARGURA  76
TELA_ALTURA   284
OFFSET_X      82
OFFSET_Y      18

Descrição dos pinos conectados ao ESP32S3:
// DISPLAY (SPI)
PIN_SCLK  12
PIN_MOSI  11
PIN_DC    4
PIN_RST   5
PIN_CS    3
PIN_BLK   14 

// BUZZER
PIN_BUZZER 17

// --- SENSORES ULTRASSÔNICOS ---
// Traseira (0 a 60cm)
TRIG_TRAS  1
ECHO_TRAS  2

// Esquerda (0 a 30cm)
TRIG_ESQ   42
ECHO_ESQ   41

// Direita (0 a 30cm)
TRIG_DIR   40
ECHO_DIR   39

// Roda (0 a 30cm)
TRIG_RODA  6 
ECHO_RODA  7  

// Bibliotecas utilizadas
<FS>
<LittleFS>
<Arduino>
<LovyanGFX>
<SPI>
<Adafruit-MCP23017>
<Adafruit_BusIO>