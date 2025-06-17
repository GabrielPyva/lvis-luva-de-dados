#include <MPU6050_tockn.h>
#include <Wire.h>

#define POLEGAR A0
#define INDICADOR A1
#define MEDIO A2
#define ANELAR A3
#define MINIMO A4

// VALORES DE CALIBRAÇÃO

#define X0P 415
#define X0I 430
#define X0M 420
#define X0A 430
#define X0m 660
#define XpiP 270
#define XpiI 230
#define XpiM 230
#define XpiA 220
#define Xpim 627

MPU6050 mpu6050(Wire);
int polegar, indicador, medio, anelar, minimo;

String angulo(int bits, int x0, int xpi)
{
  return String(90 * (bits - xpi)/(x0 - xpi));
}

void setup()
{
  Serial.begin(74880);
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
  polegar = analogRead(POLEGAR);
  indicador = analogRead(INDICADOR);
  medio = analogRead(MEDIO);
  anelar = analogRead(ANELAR);
  minimo = analogRead(MINIMO);
  Serial.print(String(int(mpu6050.getAngleX())) + ",");
  Serial.print(String(int(mpu6050.getAngleY())) + ",");
  Serial.print(String(int(mpu6050.getAngleZ())) + ",");
  Serial.println(angulo(polegar, X0P, XpiP) + "," + angulo(indicador, X0I, XpiI) + "," + angulo(medio, X0M, XpiM) + "," + angulo(anelar, X0A, XpiA) + "," + angulo(minimo, X0m, Xpim));
  delay(50);
}
