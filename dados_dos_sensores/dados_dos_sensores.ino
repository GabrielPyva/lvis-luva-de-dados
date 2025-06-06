#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(74880);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  mpu6050.update();
  Serial.print(String(int(mpu6050.getAngleX())) + ",");
  Serial.print(String(int(mpu6050.getAngleY())) + ",");
  Serial.println(int(mpu6050.getAngleZ()));
  delay(50);
}
