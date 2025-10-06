#include <MPU6050_tockn.h>
#include <Wire.h>

#define POLEGAR 3
#define INDICADOR A0
#define MEDIO A1
#define ANELAR A2
#define MINIMO A3

// VALORES DE CALIBRAÇÃO

#define X0I 500
#define X0M 540
#define X0A 520
#define X0m 600
#define XpiI 430
#define XpiM 365
#define XpiA 371
#define Xpim 349

// Funcoes e Subrotinas

String angulo(int, int, int);
void mostra(char);

// Variaveis e Instancias de objeto

MPU6050 mpu6050(Wire);
int polegar, indicador, medio, anelar, minimo;
char buffer[5];

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  pinMode(POLEGAR, INPUT);
  pinMode(INDICADOR, INPUT);
  pinMode(MEDIO, INPUT);
  pinMode(ANELAR, INPUT);
  pinMode(MINIMO, INPUT);
}

void loop()
{
  mpu6050.update();
  polegar = digitalRead(POLEGAR);
  indicador = analogRead(INDICADOR);
  medio = analogRead(MEDIO);
  anelar = analogRead(ANELAR);
  minimo = analogRead(MINIMO);
  mostra('r');
  delay(100);
}

String angulo(int bits, int x0, int xpi)
{
  return String(90 * (bits - xpi)/(x0 - xpi));
}

void mostra(char modo)
{
  switch (modo) {
    case 'v': // Visualizacao 3D
      Serial.print(String(int(mpu6050.getAngleX())) + ',');
      Serial.print(String(int(mpu6050.getAngleY())) + ',');
      Serial.print(String(int(mpu6050.getAngleZ())) + ',');
      Serial.print(String(polegar*90) + ',');
      Serial.print(angulo(indicador, X0I, XpiI) + ',');
      Serial.print(angulo(medio, X0M, XpiM) + ',');
      Serial.print(angulo(anelar, X0A, XpiA) + ',');
      Serial.println(angulo(minimo, X0m, Xpim));
      break;
    case 'r': // (RAW) Monitor Serial: Leituras Diretas
      Serial.println("\t\t\t\t\t\t===================================\t==============================================");
      Serial.println("\t\t\t\t\t\t ORIENTAÇÃO |    X |    Y |    Z |\t DEDO    | POLE | INDI | MEDI | ANEL | MINI |");
      Serial.print("\t\t\t\t\t\t GRAUS      | ");
      sprintf(buffer, "%4d", int(mpu6050.getAngleX()));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", int(mpu6050.getAngleY()));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", int(mpu6050.getAngleZ()));
      Serial.print(String(buffer) + " | ");
      Serial.print("\t LEITURA | ");
      Serial.print(String(polegar ? "ABER" : "FECH") + " | ");
      sprintf(buffer, "%4d", indicador);
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", medio);
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", anelar);
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", minimo);
      Serial.print(String(buffer) + " |\n");
      break;
    case 'm': // Monitor Serial: Com tratamento de ângulo para os dedos
      Serial.println("\t\t\t\t\t\t===================================\t==============================================");
      Serial.println("\t\t\t\t\t\t ORIENTAÇÃO |    X |    Y |    Z |\t DEDO    | POLE | INDI | MEDI | ANEL | MINI |");
      Serial.print("\t\t\t\t\t\t GRAUS      | ");
      sprintf(buffer, "%4d", int(mpu6050.getAngleX()));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", int(mpu6050.getAngleY()));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", int(mpu6050.getAngleZ()));
      Serial.print(String(buffer) + " | ");
      Serial.print("\t LEITURA | ");
      Serial.print(String(polegar ? "ABER" : "FECH") + " | ");
      sprintf(buffer, "%4d", angulo(indicador, X0I, XpiI));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", angulo(medio, X0M, XpiM));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", angulo(anelar, X0A, XpiA));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", angulo(minimo, X0m, Xpim));
      Serial.print(String(buffer) + " |\n");
      break;
  }
}