#include <MPU6050_tockn.h>
#include <Wire.h>


MPU6050 mpu6050(Wire);

// Variáveis para médias progressivas
const int NUM_INTERVALS = 10; // 100, 200, 300, ..., 1000
const int INTERVAL_SIZE = 100; // Amostras por intervalo
const int TOTAL_SAMPLES = NUM_INTERVALS * INTERVAL_SIZE; // 1000 amostras

float drift_x_sum = 0, drift_y_sum = 0, drift_z_sum = 0;
int sample_count = 0;

// Arrays para armazenar as médias
float medias_x[NUM_INTERVALS];
float medias_y[NUM_INTERVALS]; 
float medias_z[NUM_INTERVALS];
float medias_x_dps[NUM_INTERVALS];
float medias_y_dps[NUM_INTERVALS];
float medias_z_dps[NUM_INTERVALS];

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true); // Calibração automática do gyro
  // OU use: mpu6050.calcGyroOffsets(false); para calibração silenciosa
  
  Serial.println("=== ANÁLISE DE CONVERGÊNCIA DO DRIFT ===");
  Serial.println("Calculando médias progressivas...");
  Serial.println();
  Serial.println("Amostras\tDrift_X\tDrift_Y\tDrift_Z\tX(°/s)\tY(°/s)\tZ(°/s)");
  Serial.println("------------------------------------------------------------");
  
  delay(3000); // Tempo para posicionar o sensor
}

void loop() {
  if (sample_count < TOTAL_SAMPLES) {
    mpu6050.update();
    
    // Ler valores do gyroscópio JÁ CALIBRADOS pela biblioteca
    float gx = mpu6050.getGyroX();
    float gy = mpu6050.getGyroY();
    float gz = mpu6050.getGyroZ();
    
    // Acumular valores (já estão em °/s)
    drift_x_sum += gx;
    drift_y_sum += gy;
    drift_z_sum += gz;
    
    sample_count++;
    
    // Verificar se atingiu um intervalo de medição
    if (sample_count % INTERVAL_SIZE == 0) {
      int interval_index = (sample_count / INTERVAL_SIZE) - 1;
      
      // Calcular média para este intervalo (já em °/s)
      medias_x_dps[interval_index] = drift_x_sum / sample_count;
      medias_y_dps[interval_index] = drift_y_sum / sample_count;
      medias_z_dps[interval_index] = drift_z_sum / sample_count;
      
      // Para compatibilidade, também calcular em valores "brutos" aproximados
      medias_x[interval_index] = medias_x_dps[interval_index] * 131.0;
      medias_y[interval_index] = medias_y_dps[interval_index] * 131.0;
      medias_z[interval_index] = medias_z_dps[interval_index] * 131.0;
      
      // Exibir resultados
      Serial.print(sample_count);
      Serial.print("\t");
      Serial.print(medias_x[interval_index], 2);
      Serial.print("\t");
      Serial.print(medias_y[interval_index], 2);
      Serial.print("\t");
      Serial.print(medias_z[interval_index], 2);
      Serial.print("\t");
      Serial.print(medias_x_dps[interval_index], 6);
      Serial.print("\t");
      Serial.print(medias_y_dps[interval_index], 6);
      Serial.print("\t");
      Serial.print(medias_z_dps[interval_index], 6);
      Serial.println();
    }
    
    delay(10); // 10ms entre amostras
  } else {
    // Finalizou todas as amostras
    exibirResultadosFinais();
    gerarDadosGrafico();
    
    while(true) {
      delay(1000); // Para execução
    }
  }
}

void exibirResultadosFinais() {
  Serial.println();
  Serial.println("=== RESULTADOS FINAIS ===");
  
  for(int i = 0; i < NUM_INTERVALS; i++) {
    int amostras = (i + 1) * INTERVAL_SIZE;
    Serial.print("Média ");
    Serial.print(amostras);
    Serial.print(" amostras: ");
    Serial.print("X=");
    Serial.print(medias_x_dps[i], 6);
    Serial.print("°/s, Y=");
    Serial.print(medias_y_dps[i], 6);
    Serial.print("°/s, Z=");
    Serial.print(medias_z_dps[i], 6);
    Serial.println("°/s");
  }
}

void gerarDadosGrafico() {
  Serial.println();
  Serial.println("=== DADOS PARA GRÁFICO ===");
  Serial.println("Copie e cole em uma planilha:");
  Serial.println();
  
  // Cabeçalho para planilha
  Serial.println("Amostras\tDrift_X\tDrift_Y\tDrift_Z\tX_dps\tY_dps\tZ_dps");
  
  for(int i = 0; i < NUM_INTERVALS; i++) {
    int amostras = (i + 1) * INTERVAL_SIZE;
    Serial.print(amostras);
    Serial.print("\t");
    Serial.print(medias_x[i], 4);
    Serial.print("\t");
    Serial.print(medias_y[i], 4);
    Serial.print("\t");
    Serial.print(medias_z[i], 4);
    Serial.print("\t");
    Serial.print(medias_x_dps[i], 6);
    Serial.print("\t");
    Serial.print(medias_y_dps[i], 6);
    Serial.print("\t");
    Serial.print(medias_z_dps[i], 6);
    Serial.println();
  }
  
  // Análise estatística
  analisarConvergencia();
}

void analisarConvergencia() {
  Serial.println();
  Serial.println("=== ANÁLISE DE CONVERGÊNCIA ===");
  
  float variacao_x = abs(medias_x_dps[NUM_INTERVALS-1] - medias_x_dps[0]);
  float variacao_y = abs(medias_y_dps[NUM_INTERVALS-1] - medias_y_dps[0]);
  float variacao_z = abs(medias_z_dps[NUM_INTERVALS-1] - medias_z_dps[0]);
  
  Serial.print("Variação total X: ");
  Serial.println(variacao_x, 6);
  Serial.print("Variação total Y: ");
  Serial.println(variacao_y, 6);
  Serial.print("Variação total Z: ");
  Serial.println(variacao_z, 6);
  
  // Recomendação de número ideal de amostras
  Serial.println();
  Serial.println("=== RECOMENDAÇÃO ===");
  
  bool encontrou = false;
  for(int i = 1; i < NUM_INTERVALS; i++) {
    float mudanca_x = abs(medias_x_dps[i] - medias_x_dps[i-1]) / abs(medias_x_dps[i]) * 100;
    float mudanca_y = abs(medias_y_dps[i] - medias_y_dps[i-1]) / abs(medias_y_dps[i]) * 100;
    float mudanca_z = abs(medias_z_dps[i] - medias_z_dps[i-1]) / abs(medias_z_dps[i]) * 100;
    
    if (mudanca_x < 1.0 && mudanca_y < 1.0 && mudanca_z < 1.0) {
      Serial.print("Estabiliza em: ");
      Serial.print((i + 1) * INTERVAL_SIZE);
      Serial.println(" amostras");
      encontrou = true;
      break;
    }
  }
  
  if (!encontrou) {
    Serial.println("Recomendado: 1000 amostras (não estabilizou antes)");
  }
}