#include <MPU6050_tockn.h>
#include <Wire.h>

#define POLEGAR 13
#define INDICADOR A1
#define MEDIO A2
#define ANELAR A3
#define MINIMO A0

// VALORES DE CALIBRAÇÃO

#define X0P 520
#define X0I 500
#define X0M 540
#define X0A 520
#define X0m 600
#define XpiP 403
#define XpiI 430
#define XpiM 365
#define XpiA 371
#define Xpim 349

MPU6050 mpu6050(Wire);
int polegar, indicador, medio, anelar, minimo;

String angulo(int bits, int x0, int xpi)
{
  return String(90 * (bits - xpi)/(x0 - xpi));
}

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
  Serial.print(String(int(mpu6050.getAngleX())) + ",");
  Serial.print(String(int(mpu6050.getAngleY())) + ",");
  Serial.print(String(int(mpu6050.getAngleZ())) + ",");
  Serial.println(String(polegar) + "," + angulo(indicador, X0I, XpiI) + "," + angulo(medio, X0M, XpiM) + "," + angulo(anelar, X0A, XpiA) + "," + angulo(minimo, X0m, Xpim));
  //Serial.println(String(polegar) + "," + String(indicador) + "," + String(medio) + "," + String(anelar) + "," + String(minimo));
  delay(50);
}
