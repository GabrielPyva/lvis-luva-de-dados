#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  mpu6050.update();
  Serial.print(String(int(mpu6050.getAngleX())%180) + ",");
  Serial.print(String(int(mpu6050.getAngleY())%180) + ",");
  Serial.println(int(mpu6050.getAngleZ())%180);
  delay(50);
}
