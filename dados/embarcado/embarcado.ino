#include <MPU6050_tockn.h>
#include <Wire.h>

#define PORTA_POLEGAR 13
#define PORTA_INDICADOR A0
#define PORTA_MEDIO A1
#define PORTA_ANELAR A2
#define PORTA_MINIMO A3

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
  int porta, valor, aberto, fechado, angulo;
  char nome[10];
};

// Funcoes e Subrotinas

int ajuste_linear(int, int, int);
void calcula_angulos_dos_dedos();
void mostra(char);
void le_dedos();

// Variaveis e Instancias de objeto

MPU6050 mpu6050(Wire);
int polegar, indicador, medio, anelar, minimo;
char buffer[5];
Dedo dedo[5];

void setup()
{
  dedo[0].porta = PORTA_POLEGAR;
  dedo[1].porta = PORTA_INDICADOR;
  dedo[2].porta = PORTA_MEDIO;
  dedo[3].porta = PORTA_ANELAR;
  dedo[4].porta = PORTA_MINIMO;

  dedo[1].aberto = X0I;
  dedo[2].aberto = X0M;
  dedo[3].aberto = X0A;
  dedo[4].aberto = X0m;
  dedo[1].fechado = XpiI;
  dedo[2].fechado = XpiM;
  dedo[3].fechado = XpiA;
  dedo[4].fechado = Xpim;
  
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  for (int i = 0; i < 5; i++) { pinMode(dedo[i].porta, INPUT); }
}

void loop()
{
  mpu6050.update();
  le_dedos();
  calcula_angulos_dos_dedos();
  mostra('r');
  delay(100);
}

int ajuste_linear(int bits, int x0, int xpi)
{
  return 90 * (bits - xpi)/(x0 - xpi);
}

void calcula_angulos_dos_dedos()
{
  for (int i = 1; i < 5; i++)
  {
    dedo[i].angulo = ajuste_linear(dedo[i].valor, dedo[i].fechado, dedo[i].aberto);
  }
}

void le_dedos()
{
  dedo[0].valor = digitalRead(dedo[0].porta);
  for (int i = 1; i < 5; i++) { dedo[i].valor = analogRead(dedo[i].porta); }
}

void mostra(char modo)
{
  switch (modo) {
    case 'v': // Visualizacao 3D
      
      break;
    case 'r': // (RAW) Monitor Serial: Leituras Diretas
      
      break;
    case 'm': // Monitor Serial: Com tratamento de ângulo para os dedos
      
      break;
  }
}