void setup() {
  Serial.begin(4800);
  Serial.println("=== TESTE Médio ===");
  Serial.println("Valor Analogico | Angulo Mapeado");
  Serial.println("-------------------------------");
}

void loop() {
  // Lê o sensor do mindinho (porta A1)
  int valorAnalogico = analogRead(A1);
  
  // Mapeia para ângulo (0-90 graus)
  // Valores baseados no resistor de 47kΩ:
  int angulo = map(valorAnalogico, 580, 643, 0, 90);
 
  int anguloF = limitador(angulo);
  
  // Mostra os resultados
  Serial.print("Analogico: ");
  Serial.print(valorAnalogico);
  Serial.print(" | Angulo Final: ");
  Serial.print(anguloF);
  Serial.println("°");
  
  delay(500); // Meio segundo entre leituras
}

int limitador(int angulo) {
   if (angulo > 90) {        
      angulo = 90;            
   }
   if (angulo < 0) {        
      angulo = 0;             
   }
   return angulo; 
}
