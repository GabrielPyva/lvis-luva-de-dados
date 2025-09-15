#include <Wire.h>

#define PORTA_POLEGAR 13
#define PORTA_INDICADOR 9
#define PORTA_MEDIO 6
#define PORTA_ANELAR 5
#define PORTA_MINIMO 3

#define VALOR_ABERTO 500
#define VALOR_FECHADO 400

void atualiza_dedos();
void atualiza_orientacao();

void abre();

struct Dedo
{
  int porta, valor;
  char nome[10];
};

String comando;
int polegar, indicador, medio, anelar, minimo, l = 0;
char buffer[5];
Dedo dedo[5];

void setup()
{
  dedo[0].porta = PORTA_POLEGAR;
  dedo[1].porta = PORTA_INDICADOR;
  dedo[2].porta = PORTA_MEDIO;
  dedo[3].porta = PORTA_ANELAR;
  dedo[4].porta = PORTA_MINIMO;
  for (int i = 0; i < 5; i++) { pinMode(dedo[i].porta, OUTPUT); }
  Wire.begin();
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {
    comando = Serial.readString();
    comando.trim();

    if (comando == "abrir")
    {
      Serial.println("Abrindo a mão...");
      abre();
    }
  }
  atualiza_dedos();
  delay(100);
  l++;
}

void atualiza_dedos()
{
  digitalWrite(dedo[0].porta, dedo[0].valor);
  for (int i = 1; i < 5; i++)
  {
    analogWrite(dedo[i].porta, dedo[i].valor);
  }
}

void abre()
{
  bool aberto;
  do
  {
    aberto = true;
    for (int i = 1; i < 5; i++)
    {
      if (dedo[i].valor != VALOR_ABERTO)
        aberto = false;
    }
    if (aberto) continue;
    if (dedo[0].valor != 1) dedo[0].valor = 1;
    for (int i = 1; i < 5; i++)
    {
      if (dedo[i].valor < VALOR_ABERTO)
        dedo[i].valor++;
    }
    delay(20);
  }
  while (!aberto);
}

void atualiza_orientacao()
{
  
}
