#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);
int flex_bruto;

int angulo(int leitura) { return int(495 - 0.75 * leitura); }

void setup()
{
  Serial.begin(74880);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  pinMode(A0, INPUT);
}

void loop()
{
  mpu6050.update();
  flex_bruto = analogRead(A0);
  Serial.print(String(int(mpu6050.getAngleX())) + ",");
  Serial.print(String(int(mpu6050.getAngleY())) + ",");
  Serial.print(String(int(mpu6050.getAngleZ())) + ",");
  Serial.println(angulo(flex_bruto));
  delay(50);
}
