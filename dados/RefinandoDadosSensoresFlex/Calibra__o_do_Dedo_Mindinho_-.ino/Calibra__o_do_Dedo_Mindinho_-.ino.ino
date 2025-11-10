void setup() {
  Serial.begin(4800);
  Serial.println("=== TESTE MINDINHO ===");
  Serial.println("Valor Analogico | Angulo Mapeado");
  Serial.println("-------------------------------");
}

void loop() {
  // Lê o sensor do mindinho (porta A3)
  int valorAnalogico = analogRead(A3);
  
  // Mapeia para ângulo (0-90 graus)
  // Valores baseados no resistor de 47kΩ:
  int angulo = map(valorAnalogico, 501, 561, 0, 90);
 
  int anguloF = limitador(angulo);  
  
  // Mostra os resultados
  Serial.print("Analogico: ");
  Serial.print(valorAnalogico);
  Serial.print(" | Angulo: ");
  Serial.print(anguloF);
  Serial.println("°");
  
  delay(500); // Meio segundo entre leituras
}

int limitador(int angulo) {
   if (angulo > 270) {       
      angulo = 90;            
   }
   if (angulo < 180) {       
      angulo = 0;             
   } 
   else {
      angulo = angulo - 180;  
   }
  
   return angulo;  
}
