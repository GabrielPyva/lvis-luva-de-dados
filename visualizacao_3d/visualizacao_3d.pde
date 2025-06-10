import processing.serial.*;

final int BAUDRATE = 74880;
final String PORTA = "COM8";

Serial myPort;
float cameraX, cameraY, cameraZ;
float centroX, centroY, centroZ;
float upX, upY, upZ;

float camRotX = 0, camRotY = PI;
float mouseX_prev, mouseY_prev;
int roll, pitch, yaw, flex;

void setup()
{
  size(300, 300, P3D);
  
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
  background(60, 0, 60);
  lights();
  
  fill(255, 0, 0);
  text("Roll: " + nf(roll) + "°", 10, 40); 
  fill(0, 255, 0);
  text("Pitch: " + nf(pitch) + "°", 10, 80); 
  fill(0, 0, 255);
  text("Yaw: " + nf(yaw) + "°", 10, 120);
  fill(255, 255, 255);
  text("Flex: " + nf(flex) + "°", 10, 160);
  
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
  desenhaEixos(1, 1, -1);
}

void serialEvent(Serial myPort)
{
  String inString = myPort.readStringUntil('\n');
  
  if (inString != null)
  {
    inString = trim(inString);
    String[] list = splitTokens(inString, ",");
    if (list.length >= 4)
    {
      try
      {
        roll = -int(list[0]);
        pitch = -int(list[1]);
        yaw = int(list[2]);
        flex = int(list[3]);
        flex = constrain(flex, -5, 90);
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
  translate(-75, 0, -30);
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

void desenhaEixos(int x, int y, int z)
{
  stroke(255, 0, 0);
  line(0, 0, 0, 200*x, 0, 0);
  stroke(0, 255, 0);
  line(0, 0, 0, 0, 200*y, 0);
  stroke(0, 0, 255);
  line(0, 0, 0, 0, 0, 200*z);
  stroke(255);
}

void desenhaMao()
{
  fill(180, 180, 180);
  box(200, 200, 50);
  
  pushMatrix();
  translate(150, 80, 0);
  box(100, 40, 50);
  
  translate(-75, -180, 25);
  rotateX(radians(-flex));
  translate(0, -35, -25);
  box(50, 70, 50);
  
  translate(0, -35, 25);
  rotateX(radians(-flex));
  translate(0, -35, -25);
  box(50, 70, 50);
  
  translate(0, -35, 25);
  rotateX(radians(-2*flex));
  translate(0, -30, -25);
  box(50, 60, 50);
  popMatrix();
  
  pushMatrix();
  translate(25, -210, 0);
  
  for (int i=0;i!=3;i++)
  {
    box(50, 220-i*20, 50);
    translate(-50, 10, 0);
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
