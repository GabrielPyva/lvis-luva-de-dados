import processing.serial.*;

final int BAUDRATE = 19200;
final String PORTA = "COM5";

Serial myPort;
float cameraX, cameraY, cameraZ;
float centroX, centroY, centroZ;
float upX, upY, upZ;

float camRotX = 0, camRotY = PI;
float mouseX_prev, mouseY_prev;
int roll, pitch, yaw;
int[] dedo = new int[5];

void setup()
{
  size(1200, 800, P3D);
  
  centroX = centroY = centroZ = 0;

  cameraX = width/2.0;
  cameraY = height/2.0;
  cameraZ = (height/2.0) / tan(PI*60.0 / 360.0);
  
  upX = upZ = 0;
  upY = 1;

  mouseX_prev = mouseX;
  mouseY_prev = mouseY;
  
  try
  {
    myPort = new Serial(this, PORTA, BAUDRATE);
    myPort.bufferUntil('\n');
  }
  catch (Exception e)
  {
    println("Erro ao abrir a porta serial: " + e.getMessage());
    println("Por favor, verifique se o Arduino está conectado e a porta serial correta.");
    exit();
  }
  textSize(40);
}

void draw()
{
  background(20, 20, 20);
  lights();
  
  fill(255, 0, 0);
  text("Roll: " + nf(roll) + "°", 10, 40); 
  fill(0, 255, 0);
  text("Pitch: " + nf(pitch) + "°", 10, 80); 
  fill(0, 0, 255);
  text("Yaw: " + nf(yaw) + "°", 10, 120);
  fill(255, 255, 255);
  text("Polegar: " +   nf(dedo[0]) + "°", 10, 160);
  text("Indicador: " + nf(dedo[1]) + "°", 10, 200);
  text("Medio: " +     nf(dedo[2]) + "°", 10, 240);
  text("Anelar: " +    nf(dedo[3]) + "°", 10, 280);
  text("Mindinho: " +  nf(dedo[4]) + "°", 10, 320);
  
  if (mousePressed)
  {
    float dx = mouseX - mouseX_prev;
    float dy = mouseY - mouseY_prev;

    camRotY += dx * 0.005;
    camRotX -= dy * 0.005;
  } 
  
  mouseX_prev = mouseX;
  mouseY_prev = mouseY;
  
  translate(width/2, height/2, 0);

  rotateY(camRotY);
  rotateX(camRotX);
  
  rotateX(radians(roll));
  rotateY(radians(pitch));
  rotateZ(radians(yaw));
  
  desenhaMao();
  desenhaIMU();
  desenhaEixos(1, 0.5, -0.5);
}

void serialEvent(Serial myPort)
{
  String inString = myPort.readStringUntil('\n');
  
  if (inString != null)
  {
    inString = trim(inString);
    String[] list = splitTokens(inString, ",");
    if (list.length >= 8)
    {
      try
      {
        roll = -int(list[0]);
        pitch = -int(list[1]);
        yaw = int(list[2]);
        
        for (int i=0;i<5;i++)
        {
          dedo[i] = int(list[i+3]);
        }
      }
      catch (NumberFormatException e)
      {
        println("Erro ao converter dados: " + inString);
      }
    }
  }
}

void desenhaIMU()
{
  pushMatrix();
  translate(0, 0, -30);
  fill(0, 0, 170);
  box(50, 80, 10);
  translate(0, 0, -6);
  fill(1, 1, 1);
  box(15, 15, 2);
  translate(-20, -35, 0);
  fill(200, 200, 200);
  
  for (int i=0;i<8;i++)
  {
    circle(0, i*10, 4);
  }
  popMatrix();
}

void desenhaEixos(float x, float y, float z)
{
  stroke(255, 0, 0);
  line(0, 0, 0, 400*x, 0, 0);
  stroke(0, 255, 0);
  line(0, 0, 0, 0, 400*y, 0);
  stroke(0, 0, 255);
  line(0, 0, 0, 0, 0, 400*z);
  stroke(255);
}

void desenhaMao()
{
  pushMatrix();
  fill(200, 200, 200);
  box(200, 200, 50);
  translate(100, 75, 0);
  desenhaDedo(213, 50, 50, dedo[1], false);
  
  for (int i=0;i<3;i++)
  {
    translate(0, -50, 0);
    desenhaDedo(222-i*12, 50, 50, dedo[i+2], false);
  }
  
  translate(-175, 175, 0);
  rotateZ(radians(90));
  desenhaDedo(150, 50, 50, dedo[0], true);
  popMatrix();
}

void desenhaDedo(int comprimento, int largura, int altura, int rot, boolean polegar)
{
  pushMatrix();
  translate(-comprimento/6, 0, 0);
  for (int i = polegar ? 2 : 3; i > 0; i--)
  {
    translate(comprimento/6, 0, altura/2);
    rotateY(radians(-rot));
    translate(comprimento/6, 0, -altura/2);
    box(comprimento/3, largura, altura);
  }
  popMatrix();
}

void keyPressed()
{
  if (key == ' ')
  {
    camRotY = PI;
    camRotX = -QUARTER_PI;
  }
}
