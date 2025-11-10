void setup() {
  Serial.begin(4800);
  Serial.println("=== TESTE Indicador ===");
  Serial.println("Valor Analogico | Angulo Mapeado");
  Serial.println("-------------------------------");
}

void loop() {
  // Lê o sensor do mindinho (porta A0)
  int valorAnalogico = analogRead(A0);
  
  // Mapeia para ângulo (0-90 graus)
  // Valores baseados no resistor de 47kΩ:
  int angulo = map(valorAnalogico, 478, 600, 0, 90);
 
  int anguloF = limitador(angulo);
  
  // Mostra os resultados
  Serial.print("Analogico: ");
  Serial.print(valorAnalogico);
  Serial.print(" | Angulo: ");
  Serial.print(anguloF);
  Serial.println("°");

  delay(500);
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
