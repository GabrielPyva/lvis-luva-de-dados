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

typedef struct Dedo
{
  int porta, leitura, angulo;
  String nome;
};

// Funcoes e Subrotinas

String angulo(int, int, int);
void mostra(char);

// Variaveis e Instancias de objeto

MPU6050 mpu6050(Wire);
char buffer[5];
Dedo dedo[5];

void setup()
{
  dedo[0].porta = POLEGAR; dedo[0].nome = "polegar";
  dedo[1].porta = INDICADOR; dedo[1].nome = "indicador";
  dedo[2].porta = MEDIO; dedo[2].nome = "medio";
  dedo[3].porta = ANELAR; dedo[3].nome = "anelar";
  dedo[4].porta = MINIMO; dedo[4].nome = "minimo";
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  for (int i = 0; i < 5; i++)
  {
    pinMode(dedo[i].porta, INPUT);
  }
}

void loop()
{
  mpu6050.update();
  dedo[0].leitura = digitalRead(dedo[0].porta);
  for (int i = 1; i < 5; i++) { dedo[i].leitura = analogRead(dedo[i].porta); }
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
      Serial.print(String(dedo[0].angulo) + ',');
      Serial.print(angulo(dedo[1].leitura, XfI, XaI) + ',');
      Serial.print(angulo(dedo[2].leitura, XfM, XaM) + ',');
      Serial.print(angulo(dedo[3].leitura, XfA, XaA) + ',');
      Serial.println(angulo(dedo[4].leitura, Xfm, Xam));
      break;
    case 'c': // Calibração
      Serial.print(String(int(mpu6050.getAngleX())) + ',');
      Serial.print(String(int(mpu6050.getAngleY())) + ',');
      Serial.print(String(int(mpu6050.getAngleZ())) + ',');
      Serial.print(String(dedo[0].angulo) + ',');
      Serial.print(String(dedo[1].leitura) + ',');
      Serial.print(String(dedo[2].leitura) + ',');
      Serial.print(String(dedo[3].leitura) + ',');
      Serial.println(String(dedo[4].leitura));
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
      Serial.print(String(dedo[0].leitura ? "ABER" : "FECH") + " | ");
      sprintf(buffer, "%4d", dedo[1].leitura);
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", dedo[2].leitura);
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", dedo[3].leitura);
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", dedo[4].leitura);
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
      Serial.print(String(dedo[0].leitura ? "ABER" : "FECH") + " | ");
      sprintf(buffer, "%4d", angulo(dedo[1].leitura, XfI, XaI));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", angulo(dedo[2].leitura, XfM, XaM));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", angulo(dedo[3].leitura, XfA, XaA));
      Serial.print(String(buffer) + " | ");
      sprintf(buffer, "%4d", angulo(dedo[4].leitura, Xfm, Xam));
      Serial.print(String(buffer) + " |\n");
      break;
  }
}