#include <MPU6050_tockn.h>
#include <Wire.h>

// Aqui estão os dados estáticos de calibração dos sensores flex de cada dedo (P, I, M, A e m são as iniciais dos dedos)

// Valor da leitura do do pino do microcontrolador quando cada sensor está ESTICADO (ou Aberto, por isso o "a" no meio)
#define XaP 0
#define XaI 450
#define XaM 550
#define XaA 575
#define Xam 571

// Valor da leitura do do pino do microcontrolador quando cada sensor está FLEXIONADO (por isso o "f" no meio)
#define XfP 1
#define XfI 600
#define XfM 617
#define XfA 657
#define Xfm 596

// Struct que representa os dedos da mão
typedef struct Dedo
{
  int porta, leitura, angulo;
  float aberto, fechado;
  String nome;
};

// Declaração de variáveis, objetos e prototipagem das funções e subrotinas
MPU6050 mpu6050(Wire);
Dedo dedo[5];

int ajuste(int, int, int, bool = true);
void imprime(char='v');
void le_dedos();
void calcula_angulos_dos_dedos();
void calibra_dedos(int = 1, int = 13000);

void setup()
{
  // Escolha da porta analógica que ficará conectado o sensor de cada dedo, bem como seu nome e valores de calibração
  dedo[0].porta = 3;  dedo[0].nome = "polegar";   dedo[0].aberto = XaP; dedo[0].fechado = XfP;
  dedo[1].porta = A0; dedo[1].nome = "indicador"; dedo[1].aberto = XaI; dedo[1].fechado = XfI;
  dedo[2].porta = A1; dedo[2].nome = "medio";     dedo[2].aberto = XaM; dedo[2].fechado = XfM;
  dedo[3].porta = A2; dedo[3].nome = "anelar";    dedo[3].aberto = XaA; dedo[3].fechado = XfA;
  dedo[4].porta = A3; dedo[4].nome = "minimo";    dedo[4].aberto = Xam; dedo[4].fechado = Xfm;
  for (int i = 0; i < 5; i++) pinMode(dedo[i].porta, INPUT);
  // Procedimento padrão para ligar o IMU e a Comunicação serial
  Serial.begin(4800);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  Wire.begin();
  // Função de calibração automática
  calibra_dedos(1);
}

void loop()
{
  // Todas as subrotinas em ordem para legibilidade do código, os detalhes estão em suas declarações abaixo do loop
  mpu6050.update();
  le_dedos();
  calcula_angulos_dos_dedos();
  imprime();
  // Taxa de atualização da posição de TUDO que pegamos da mão: rotação da palma e flexão dos dedos
  delay(50);
}
/*
 * @brief Atualiza os valores lidos pelas portas de cada dedo
*/
void le_dedos()
{
  dedo[0].leitura = digitalRead(dedo[0].porta);
  for (int i = 1; i < 5; i++) dedo[i].leitura = analogRead(dedo[i].porta);
}

/*
 * @brief Usa a função de ajuste linear (chamada de "ajuste") em cada dedo (é literalmente só isso)
*/
void calcula_angulos_dos_dedos()
{
  for (int i = 0; i < 5; i++)
    dedo[i].angulo = ajuste(dedo[i].leitura, dedo[i].fechado, dedo[i].aberto);
}

/*
 * @brief Estima o valor do ângulo de UM dedo
 *
 * @param bits Valor lido na porta do microcontrolador
 * @param xf Valor de calibração do dedo fechado
 * @param xa Valor de calibração do dedo aberto
 * @param satura Valor que habilita (ou não) a saturação (limita a leitura entre 0 e 90 graus)
 * 
 * @return retorna o valor estimado em graus do ângulo que o sensor flex do dedo flexionou
*/
int ajuste(int bits, int xf, int xa, bool satura)
{
  int resultado = 90 * (bits - xa)/(xf - xa); // aproximação linear, mais conhecida como {y - y0 = m * (x - x0)} (ou "yoyomixoxo")
  if (satura)
  {
    if (resultado > 90) return 90;
    if (resultado < 0) return 0;
  }
  return resultado;
}

/*
 * @brief Printa tudo na serial de forma organizada para que as ferramentas de visualização consigam ler
 * 
 * @param modo Caracter OPCIONAL que muda a forma de visualização (de ângulo para valor em bits) para poder calibrar os dedos
*/
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

/*
 * @brief Calibração automática dos dedos em tempo de execução (muda os valores de calibração da variável "dedo")
 *
 * @param total_de_amostras Quantidade de vezes que os valores serão lidos quando a luva está fechada, e depois aberta
 * @param espera Tempo de espera para que a mão mude de posição para novas leituras
*/
void calibra_dedos(int total_de_amostras, int espera)
{
  for (int d = 0; d < 5; d++)
  {
    dedo[d].fechado = 0;
    dedo[d].aberto  = 0;
  }
  Serial.println("\n A calibração será iniciada...");
  Serial.println("\nFECHA");
  delay(espera);
  Serial.println("MEDINDO...");
  delay(espera);
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
  delay(espera);
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
