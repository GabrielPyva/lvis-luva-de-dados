#include <MPU6050_tockn.h>
#include <Wire.h>

#define PORTA_POLEGAR 13
#define PORTA_INDICADOR A1
#define PORTA_MEDIO A2
#define PORTA_ANELAR A3
#define PORTA_MINIMO A4

// VALORES DE CALIBRAÇÃO

/*#define X0I 500
#define X0M 540
#define X0A 520
#define X0m 600
#define XpiI 430
#define XpiM 365
#define XpiA 371
#define Xpim 349*/

#define X0I 500
#define X0M 500
#define X0A 500
#define X0m 500
#define XpiI 400
#define XpiM 400
#define XpiA 400
#define Xpim 400

struct Dedo
{
  int porta, valor, aberto, fechado;
  char nome[10];
};

// Funcoes e Subrotinas

int ajuste_linear(int, int, int);
void calcula_angulos();
void mostra(char);
void le_dedos();

// Variaveis e Instancias de objeto

MPU6050 mpu6050(Wire);
int polegar, indicador, medio, anelar, minimo;
char buffer[5];
Dedo dedo[5];
  dedo[0].porta = PORTA_POLEGAR;
  dedo[1].porta = PORTA_INDICADOR;
  dedo[2].porta = PORTA_MEDIO;
  dedo[3].porta = PORTA_ANELAR;
  dedo[4].porta = PORTA_MINIMO;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  for (int i = 0; i < 5; i++) { pinMode(dedo[i].porta, INPUT) }
}

void loop()
{
  mpu6050.update();
  le_dedos();
  calcula_angulos();
  mostra('r');
  delay(100);
}

int ajuste_linear(int bits, int x0, int xpi)
{
  return 90 * (bits - xpi)/(x0 - xpi);
}

void calcula_angulos()
{
  for (int i = 1; i < 5; i++)
  {
    dedo[i].angulo = ajuste_linear(dedo[i].valor, dedo[i].fechado, dedo[i].aberto);
  }
}

void le_dedos()
{
  dedo[0].valor = digitalRead(dedo[0].porta);
  for (int i = 1; i < 5; i++)
  {
    dedo[i].valor = analogRead(dedo[i].porta);
  }
}

void mostra(char modo)
{
  switch (modo) {
    case 'v': // Visualizacao 3D
      Serial.print(String(int(mpu6050.getAngleX())) + ',');
      Serial.print(String(int(mpu6050.getAngleY())) + ',');
      Serial.print(String(int(mpu6050.getAngleZ())) + ',');
      for (int i = 0; i < 5; i++)
      {
        Serial.print(angulo(dedo[i].valor) + ',');
      }
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