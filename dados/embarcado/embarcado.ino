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
void imprime(char='v');
void le_dedos();
void calcula_angulos_dos_dedos();
void calibra_dedos(int = 100, int = 8000);

void setup()
{
  dedo[0].porta = 3;  dedo[0].nome = "polegar";   dedo[0].aberto = XaP; dedo[0].fechado = XfP;
  dedo[1].porta = A0; dedo[1].nome = "indicador"; dedo[1].aberto = XaI; dedo[1].fechado = XfI;
  dedo[2].porta = A1; dedo[2].nome = "medio";     dedo[2].aberto = XaM; dedo[2].fechado = XfM;
  dedo[3].porta = A2; dedo[3].nome = "anelar";    dedo[3].aberto = XaA; dedo[3].fechado = XfA;
  dedo[4].porta = A3; dedo[4].nome = "minimo";    dedo[4].aberto = Xam; dedo[4].fechado = Xfm;
  for (int i = 0; i < 5; i++) pinMode(dedo[i].porta, INPUT);
  Serial.begin(9600);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  Wire.begin();
  calibra_dedos(true);
}

void loop()
{
  mpu6050.update();
  le_dedos();
  calcula_angulos_dos_dedos();
  imprime();
  delay(1000);
}

void le_dedos()
{
  dedo[0].leitura = digitalRead(dedo[0].porta);
  for (int i = 1; i < 5; i++) dedo[i].leitura = analogRead(dedo[i].porta);
}

void calcula_angulos_dos_dedos()
{
  for (int i = 0; i < 5; i++)
    dedo[i].angulo = ajuste(dedo[i].leitura, dedo[i].fechado, dedo[i].aberto);
}

int ajuste(int bits, int xf, int xa)
{
  return 90 * (bits - xa)/(xf - xa);
}

void imprime(char modo)
{
  Serial.print(mpu6050.getAngleX(), 0); Serial.print(',');
  Serial.print(mpu6050.getAngleY(), 0); Serial.print(',');
  Serial.print(mpu6050.getAngleZ(), 0); Serial.print(',');
  for (int i = 0; i < 5; i++)
  {
    Serial.print(modo == 'v' ? dedo[i].angulo : dedo[i].leitura);
    Serial.print(i != 4 ? ',' : '\n');
  }
}

void calibra_dedos(int total_de_amostras, int espera)
{
  for (int d = 0; d < 5; d++)
  {
    dedo[d].fechado = 0;
    dedo[d].aberto  = 0;
  }
  Serial.println("FECHA");
  delay(espera);
  Serial.println("MEDINDO...");
  for (int a = 0; a < total_de_amostras; a++)
  {
    le_dedos();
    for (int i = 0; i < 5; i++)
      dedo[i].fechado += dedo[i].leitura / total_de_amostras;
    delay(50);
  }
  Serial.println("ABRE");
  delay(espera);
  Serial.println("MEDINDO...");
  for (int a = 0; a < total_de_amostras; a++)
  {
    le_dedos();
    for (int i = 0; i < 5; i++)
      dedo[i].aberto += dedo[i].leitura / total_de_amostras;
    delay(50);
  }
  for (int d = 0; d < 5; d++)
  {
    Serial.print(dedo[d].nome + " => ");
    Serial.print(dedo[d].fechado);
    Serial.print(", ");
    Serial.println(dedo[d].aberto);
  }
}