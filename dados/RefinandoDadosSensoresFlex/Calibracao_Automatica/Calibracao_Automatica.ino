/**
 * SISTEMA LUVA - CALIBRAÇÃO SIMPLIFICADA
 */

int pinos[] = {A0, A1, A2, A3};
char* nomes[] = {"Indic", "Medio", "Anelar", "Mindin"};
int minVal[4] = {1023, 1023, 1023, 1023};
int maxVal[4] = {0, 0, 0, 0};

void setup() {
  Serial.begin(4800);
  
  Serial.println();
  Serial.println("=== CALIBRACAO DA LUVA ===");
  Serial.println("INSTRUCOES:");
  Serial.println("1. Mao ABERTA na mesa");
  Serial.println("2. Feche e abra a mao");
  Serial.println("3. Aguarde 10 segundos");
  Serial.println();
  Serial.println("Iniciando em 3 segundos...");
  delay(3000);
  
  Serial.println("CALIBRANDO...");
  
  // Calibração rápida - 10 segundos
  unsigned long inicio = millis();
  while (millis() - inicio < 10000) {
    for (int i = 0; i < 4; i++) {
      int val = analogRead(pinos[i]);
      if (val < minVal[i]) minVal[i] = val;
      if (val > maxVal[i]) maxVal[i] = val;
    }
    delay(50);
  }
  
  // Mostra resultados
  Serial.println();
  Serial.println("RESULTADOS:");
  Serial.println("Dedo   | Aberto | Fechado");
  Serial.println("-----------------------");
  for (int i = 0; i < 4; i++) {
    Serial.print(nomes[i]);
    Serial.print("   | ");
    Serial.print(minVal[i]);
    Serial.print("    | ");
    Serial.println(maxVal[i]);
  }
  
  Serial.println();
  Serial.println("PRONTO! Iniciando leitura...");
  Serial.println("Dedo   | Analog | Angulo");
  Serial.println("-----------------------");
}

void loop() {
  for (int i = 0; i < 4; i++) {
    int valor = analogRead(pinos[i]);
    int angulo = map(valor, minVal[i], maxVal[i], 90, 0);
    
    // Limitação simples
    if (angulo < 0) angulo = 0;
    if (angulo > 90) angulo = 90;
    
    // Uma linha por dedo
    Serial.print(nomes[i]);
    Serial.print("   | ");
    Serial.print(valor);
    Serial.print("   | ");
    Serial.print(angulo);
    Serial.println("°");
  }
  
  Serial.println("-----------------------");
  delay(300);
}
