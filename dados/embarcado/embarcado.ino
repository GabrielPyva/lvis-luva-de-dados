#include <MPU6050_tockn.h>
#include <Wire.h>

#define XaP 0
#define XaI 450
#define XaM 550
#define XaA 575
#define Xam 571
#define XfP 1
#define XfI 600
#define XfM 617
#define XfA 657
#define Xfm 596

typedef struct Dedo
{
  int porta, leitura, angulo, aberto, fechado;
  String nome;
};

MPU6050 mpu6050(Wire);
Dedo dedo[5];

int ajuste(int, int, int);
void imprime(char);
void le_dedos();
void calcula_angulos_dos_dedos();
void calibra_dedos(int = 100, int = 8000);

void setup()
{
  dedo[0].porta = 3;  dedo[0].nome = "polegar"; dedo[0].aberto = XaP; dedo[0].fechado = XfP;
  dedo[1].porta = A0; dedo[1].nome = "indicador"; dedo[1].aberto = XaI; dedo[1].fechado = XfI;
  dedo[2].porta = A1; dedo[2].nome = "medio"; dedo[2].aberto = XaM; dedo[2].fechado = XfM;
  dedo[3].porta = A2; dedo[3].nome = "anelar"; dedo[3].aberto = XaA; dedo[3].fechado = XfA;
  dedo[4].porta = A3; dedo[4].nome = "minimo"; dedo[4].aberto = Xam; dedo[4].fechado = Xfm;
  for (int i = 0; i < 5; i++) pinMode(dedo[i].porta, INPUT);
  mpu6050.calcGyroOffsets(true);
  Serial.begin(9600);
  mpu6050.begin();
  Wire.begin();
  //calibra_dedos();
}

void loop()
{
  mpu6050.update();
  le_dedos();
  calcula_angulos_dos_dedos();
  imprime('v');
  delay(100);
}

void le_dedos()
{
  dedo[0].leitura = digitalRead(dedo[0].porta);
  for (int i = 1; i < 5; i++) dedo[i].leitura = analogRead(dedo[i].porta);
}

void calcula_angulos_dos_dedos()
{
  for (int i = 0; i < 5; i++) dedo[i].angulo = ajuste(dedo[i].leitura, dedo[i].fechado, dedo[i].aberto);
}

int ajuste(int bits, int xf, int xa)
{
  return 90 * (bits - xa)/(xf - xa);
}

void calibra_dedos(int amostras, int espera)
{
  Serial.println("FECHA");
  delay(espera);
  Serial.println("MEDINDO...");
  for (int t = 0; t < amostras; t++)
  {
    le_dedos();
    for (int i = 0; i < 5; i++) dedo[i].fechado += dedo[i].leitura / amostras;
    delay(50);
  }
  Serial.println("ABRE");
  delay(espera);
  Serial.println("MEDINDO...");
  for (int t = 0; t < amostras; t++)
  {
    le_dedos();
    for (int i = 0; i < 5; i++) dedo[i].aberto += dedo[i].leitura / amostras;
    delay(50);
  }
}

void imprime(char modo)
{
  char buffer[5];
  switch (modo) {
    case 'v':
      Serial.print(String(int(mpu6050.getAngleX())) + ',');
      Serial.print(String(int(mpu6050.getAngleY())) + ',');
      Serial.print(String(int(mpu6050.getAngleZ())) + ',');
      for (int i = 0; i < 5; i++)
      {
        Serial.print(String(dedo[i].angulo));
        Serial.print(i != 4 ? ',' : '\n');
      }
      break;
    case 'c':
      Serial.print(String(int(mpu6050.getAngleX())) + ',');
      Serial.print(String(int(mpu6050.getAngleY())) + ',');
      Serial.print(String(int(mpu6050.getAngleZ())) + ',');
      for (int i = 0; i < 5; i++)
      {
        Serial.print(String(dedo[0].leitura));
        Serial.print(i != 4 ? ',' : '\n');
      }
      break;
  }
}