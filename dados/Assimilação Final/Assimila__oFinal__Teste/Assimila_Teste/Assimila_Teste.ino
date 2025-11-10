/**
 * SISTEMA DE CAPTURA DE MOVIMENTOS PARA LUVA INTELIGENTE
 * Universidade de Brasília - Laboratório L-VIS
 * 
 * Este código implementa o sistema de calibração automática e captura
 * de dados de sensores flex para controle de mão virtual no Unity.
 * 
 * Características principais:
 * - Calibração automática de 1 minuto com feedback visual
 * - Filtragem de ruído e suavização de dados
 * - Integração com sensor MPU6050 para orientação espacial
 * - Correção do erro inercial (drift) do giroscópio
 * - Saída formatada para comunicação com Unity3D
 */

// ============================================================================
// CONFIGURAÇÃO DE HARDWARE - MAPEAMENTO DE PINOS
// ============================================================================

/**
 * Array de pinos analógicos para os sensores flex dos dedos
 * Ordem: Indicador, Médio, Anelar, Mindinho
 */
int pinosSensores[] = {A0, A1, A2, A3};

/**
 * Nomenclatura dos dedos para exibição nos resultados
 */
char* nomesDedos[] = {"Indic", "Medio", "Anelar", "Mindin"};

// ============================================================================
// VARIÁVEIS DE CALIBRAÇÃO E CONFIGURAÇÃO
// ============================================================================

/**
 * Arrays para armazenar valores de calibração
 * minVal: Valores mínimos (dedos completamente abertos)
 * maxVal: Valores máximos (dedos completamente fechados)
 */
int minVal[4] = {1023, 1023, 1023, 1023};
int maxVal[4] = {0, 0, 0, 0};

/**
 * PARÂMETROS DE SUAVIZAÇÃO E FILTRAGEM
 * 
 * SUAVIZACAO: Fator de filtro exponencial (0.1-0.9)
 *   Valores mais altos = mais suavização, menos ruído
 *   Valores mais baixos = maior responsividade, mais ruído
 * 
 * ZONA_MORTA: Limiar para ignorar pequenas variações (0-10 graus)
 *   Reduz tremores e ruídos de baixa amplitude
 */
const float SUAVIZACAO = 0.29;
const int ZONA_MORTA = 3;

/**
 * Buffer para armazenar os últimos ângulos processados
 * Utilizado para comparação na zona morta
 */
int ultimosAngulos[4] = {0, 0, 0, 0};

// ============================================================================
// CORREÇÃO DO DRIFT DO GIROSCÓPIO - VALORES CALCULADOS EMPIRICAMENTE
// ============================================================================

/**
 * OFFSETS DE DRIFT DO GIROSCÓPIO
 * Valores obtidos através de análise estatística de 1000 amostras
 * Unidade: graus por segundo (°/s)
 * 
 * Análise de convergência:
 * - Estabilização: 200 amostras
 * - Variação total X: 0.0136 °/s
 * - Variação total Y: 0.0015 °/s  
 * - Variação total Z: 0.0022 °/s
 */
const float DRIFT_GYRO_X = -5.728137;  // Drift no eixo X (°/s)
const float DRIFT_GYRO_Y = 1.026649;   // Drift no eixo Y (°/s)
const float DRIFT_GYRO_Z = -1.785832;  // Drift no eixo Z (°/s)

/**
 * Variáveis para correção temporal do drift
 * Armazenam o tempo da última leitura para cálculo deltaT
 */
unsigned long ultimoTempoLeitura = 0;
float anguloXCorrigido = 0;
float anguloYCorrigido = 0; 
float anguloZCorrigido = 0;

// ============================================================================
// BIBLIOTECAS EXTERNAS - SENSOR MPU6050
// ============================================================================

#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu6050(Wire);

// ============================================================================
// CONFIGURAÇÃO INICIAL DO SISTEMA
// ============================================================================

void setup() {
  // Inicialização da comunicação serial com Unity
  Serial.begin(4800);
  
  // Configuração do barramento I2C e sensor MPU6050
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true); // Calibração automática do giroscópio
  
  // Inicialização do temporizador para correção do drift
  ultimoTempoLeitura = millis();
  
  // Interface de usuário - instruções de calibração
  Serial.println();
  Serial.println("=== SISTEMA DE CALIBRAÇÃO - LUVA INTELIGENTE ===");
  Serial.println("PROCEDIMENTO DE CALIBRAÇÃO:");
  Serial.println("1. Mantenha a mão relaxada sobre uma superfície plana");
  Serial.println("2. Execute movimentos naturais de abrir e fechar");
  Serial.println("3. Varie a velocidade e intensidade dos movimentos");
  Serial.println("4. Duração total: 60 segundos");
  Serial.println();
  
  // Informações sobre correção do drift
  Serial.println("⚙️  CONFIGURAÇÃO DO SISTEMA:");
  Serial.print("   • Correção de drift X: "); Serial.print(DRIFT_GYRO_X, 6); Serial.println(" °/s");
  Serial.print("   • Correção de drift Y: "); Serial.print(DRIFT_GYRO_Y, 6); Serial.println(" °/s");
  Serial.print("   • Correção de drift Z: "); Serial.print(DRIFT_GYRO_Z, 6); Serial.println(" °/s");
  Serial.println("   • Baseado em análise de 1000 amostras");
  Serial.println();
  
  // ========================================================================
  // FASE DE CALIBRAÇÃO AUTOMÁTICA - 60 SEGUNDOS
  // ========================================================================
  
  Serial.println("🕐 INICIANDO FASE DE CALIBRAÇÃO");
  Serial.println("⏰ Tempo estimado: 60 segundos");
  Serial.println();
  
  const unsigned long TEMPO_CALIBRACAO = 60000; // 60 segundos em milissegundos
  unsigned long inicioCalibracao = millis();
  static int ultimoSegundoReportado = 60;
  
  // Loop principal de calibração
  while (millis() - inicioCalibracao < TEMPO_CALIBRACAO) {
    unsigned long tempoRestante = TEMPO_CALIBRACAO - (millis() - inicioCalibracao);
    int segundosRestantes = tempoRestante / 1000;
    
    // ----------------------------------------------------------------------
    // ATUALIZAÇÃO DE STATUS - FEEDBACK VISUAL PARA O USUÁRIO
    // ----------------------------------------------------------------------
    
    // Exibe contagem regressiva a cada 5 segundos
    if (segundosRestantes != ultimoSegundoReportado && segundosRestantes % 5 == 0) {
      Serial.print("⏰ Tempo restante: ");
      Serial.print(segundosRestantes);
      Serial.println(" segundos");
      ultimoSegundoReportado = segundosRestantes;
    }
    
    // Barra de progresso gráfica a cada 10 segundos
    if (segundosRestantes % 10 == 0 && tempoRestante % 1000 < 50) {
      int progresso = 60 - segundosRestantes;
      Serial.print("📊 Progresso: [");
      // Renderização da barra de progresso (12 caracteres)
      for (int i = 0; i < 12; i++) {
        if (i < (progresso / 5)) Serial.print("█"); // Bloco preenchido
        else Serial.print(" ");                     // Espaço vazio
      }
      Serial.print("] ");
      Serial.print(progresso);
      Serial.println("/60 segundos");
    }
    
    // ----------------------------------------------------------------------
    // AQUISIÇÃO E PROCESSAMENTO DE DADOS DOS SENSORES
    // ----------------------------------------------------------------------
    
    for (int i = 0; i < 4; i++) {
      int leituraSensor = analogRead(pinosSensores[i]);
      
      // Atualização dos valores de calibração
      if (leituraSensor < minVal[i]) minVal[i] = leituraSensor;
      if (leituraSensor > maxVal[i]) maxVal[i] = leituraSensor;
    }
    
    delay(30); // Intervalo entre leituras para estabilidade
  }
  
  // ========================================================================
  // RELATÓRIO FINAL DE CALIBRAÇÃO
  // ========================================================================
  
  Serial.println();
  Serial.println("✅ CALIBRAÇÃO CONCLUÍDA COM SUCESSO!");
  Serial.println();
  
  // Tabela de resultados da calibração
  Serial.println("📊 RELATÓRIO DE CALIBRAÇÃO - VALORES OBTIDOS");
  Serial.println("Dedo     |  Aberto  | Fechado  | Variação");
  Serial.println("-----------------------------------------");
  
  for (int i = 0; i < 4; i++) {
    int variacao = maxVal[i] - minVal[i];
    Serial.print(nomesDedos[i]);
    Serial.print("    | ");
    Serial.print(minVal[i]);
    Serial.print("     | ");
    Serial.print(maxVal[i]);
    Serial.print("     | ");
    Serial.println(variacao);
  }
  
  Serial.println();
  Serial.println("🎯 INICIANDO MODO DE OPERAÇÃO - ENVIANDO DADOS PARA UNITY");
  Serial.println();
}

// ============================================================================
// FUNÇÃO PARA CORRIGIR DRIFT DO GIROSCÓPIO
// ============================================================================

/**
 * Aplica correção do drift nos ângulos do giroscópio
 * @param anguloX Ângulo bruto do eixo X
 * @param anguloY Ângulo bruto do eixo Y  
 * @param anguloZ Ângulo bruto do eixo Z
 * @return Array com ângulos corrigidos [X, Y, Z]
 */
void corrigirDriftGiroscopio(float &anguloX, float &anguloY, float &anguloZ) {
  // Calcula o tempo decorrido desde a última leitura
  unsigned long tempoAtual = millis();
  float deltaT = (tempoAtual - ultimoTempoLeitura) / 1000.0; // Converter para segundos
  ultimoTempoLeitura = tempoAtual;
  
  // Aplica correção baseada no drift medido
  // Fórmula: Ângulo_corrigido = Ângulo_bruto - (Drift * tempo_decorrido)
  anguloX -= DRIFT_GYRO_X * deltaT;
  anguloY -= DRIFT_GYRO_Y * deltaT; 
  anguloZ -= DRIFT_GYRO_Z * deltaT;
}

// ============================================================================
// LOOP PRINCIPAL - OPERAÇÃO CONTÍNUA DO SISTEMA
// ============================================================================

void loop() {
  // Atualização dos dados do sensor de orientação MPU6050
  mpu6050.update();
  
  // --------------------------------------------------------------------------
  // CORREÇÃO DO DRIFT DO GIROSCÓPIO
  // --------------------------------------------------------------------------
  
  float anguloX = mpu6050.getAngleX();
  float anguloY = mpu6050.getAngleY();
  float anguloZ = mpu6050.getAngleZ();
  
  // Aplica correção do drift
  corrigirDriftGiroscopio(anguloX, anguloY, anguloZ);
  
  // --------------------------------------------------------------------------
  // FORMATAÇÃO DE DADOS PARA COMUNICAÇÃO COM UNITY
  // Formato: roll,pitch,yaw,polegar,indicador,medio,anelar,mindinho
  // --------------------------------------------------------------------------
  
  // Dados de orientação espacial corrigidos (MPU6050)
  Serial.print((int)anguloX); Serial.print(',');
  Serial.print((int)anguloY); Serial.print(',');
  Serial.print((int)anguloZ); Serial.print(',');
  
  // Dedo polegar (valor fixo para demonstração)
  Serial.print("0");
  Serial.print(',');
  
  // Processamento dos 4 dedos com sensores flex
  for (int i = 0; i < 4; i++) {
    // Leitura do sensor analógico
    int valorSensor = analogRead(pinosSensores[i]);
    
    // Conversão para ângulo (0° = aberto, 90° = fechado)
    int angulo = map(valorSensor, minVal[i], maxVal[i], 90, 0);
    
    // Saturação para garantir faixa válida
    if (angulo < 0) angulo = 0;
    if (angulo > 90) angulo = 90;
    
    // ----------------------------------------------------------------------
    // APLICAÇÃO DE FILTROS PARA REDUÇÃO DE RUÍDO
    // ----------------------------------------------------------------------
    
    // Filtro de zona morta - ignora variações menores que o limiar
    if (abs(angulo - ultimosAngulos[i]) < ZONA_MORTA) {
      angulo = ultimosAngulos[i]; // Mantém valor anterior
    }
    
    // Atualização do buffer de histórico
    ultimosAngulos[i] = angulo;
    
    // Envio do dado formatado
    Serial.print(angulo);
    if (i < 3) Serial.print(','); // Separador para todos exceto o último
  }
  
  // Finalização do frame de dados
  Serial.println();
  
  // Controle de taxa de atualização (≈20 Hz)
  delay(50);
}
