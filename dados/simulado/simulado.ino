int dedo[5] = {0, 0, 0, 0, 0};
int giro[3] = {0, 0, 0};

void abre_fecha(int duracao)
{
  //abre
  for (int angulo = 0; angulo <= 90; angulo++)
  {
    for (int i = 0; i < 5; i++)
    {
      dedo[i] = angulo;
    }
    Serial.print("0,0,0,");
    for (int i = 0; i < 5; i++)
    {
      Serial.print(dedo[i]);
      Serial.print(i != 4 ? ',' : '\n');
    }
    delay(int(duracao/180));
  }

  //fecha
  for (int angulo = 90; angulo >= 0; angulo--)
  {
    for (int i = 0; i < 5; i++)
    {
      dedo[i] = angulo;
    }
    Serial.print("0,0,0,");
    for (int i = 0; i < 5; i++)
    {
      Serial.print(dedo[i]);
      Serial.print(i != 4 ? ',' : '\n');
    }
    delay(int(duracao/180));
  }
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  abre_fecha(5000);
}
