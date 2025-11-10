void loop() {
  int valor = analogRead(A2);
  int angulo = map(valor, 642, 583, 90, 0); // Invertido porque 635>586
  
  Serial.print("Indicador: ");
  Serial.print(valor);
  Serial.print(" | ");
  Serial.print(angulo);
  Serial.println("°");
  
  delay(500);
}
