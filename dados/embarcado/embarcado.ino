#include <MPU6050_tockn.h>
#include <Wire.h>

#define POLEGAR 3
#define INDICADOR A0
#define MEDIO A1
#define ANELAR A2
#define MINIMO A3

// VALORES DE CALIBRAÇÃO

#define XaI 450
#define XaM 550
#define XaA 575
#define Xam 571
#define XfI 600
#define XfM 617
#define XfA 657
#define Xfm 596

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
  mostra('v');
  delay(100);
}

String angulo(int bits, int xf, int xa)
{
  return String(90 * (bits - xa)/(xf - xa));
}

void mostra(char modo)
{
  switch (modo) {
    case 'v': // Visualizacao 3D
      Serial.print(String(int(mpu6050.getAngleX())) + ',');
      Serial.print(String(int(mpu6050.getAngleY())) + ',');
      Serial.print(String(int(mpu6050.getAngleZ())) + ',');
      Serial.print(String(polegar*90) + ',');
      Serial.print(angulo(indicador, XfI, XaI) + ',');
      Serial.print(angulo(medio, XfM, XaM) + ',');
      Serial.print(angulo(anelar, XfA, XaA) + ',');
      Serial.println(angulo(minimo, Xfm, Xam));
      break;
    case 'c': // Calibração
      Serial.print(String(int(mpu6050.getAngleX())) + ',');
      Serial.print(String(int(mpu6050.getAngleY())) + ',');
      Serial.print(String(int(mpu6050.getAngleZ())) + ',');
      Serial.print(String(polegar*90) + ',');
      Serial.print(String(indicador) + ',');
      Serial.print(String(medio) + ',');
      Serial.print(String(anelar) + ',');
      Serial.println(String(minimo));
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
      sprintf(buffer, "%4d", angulo(indicador, XfI, XaI));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", angulo(medio, XfM, XaM));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", angulo(anelar, XfA, XaA));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", angulo(minimo, Xfm, Xam));
      Serial.print(String(buffer) + " |\n");
      break;
  }
}