import processing.serial.*;

final int BAUDRATE = 74880;
final String PORTA = "COM6";

Serial myPort;
float cameraX, cameraY, cameraZ;
float centerX, centerY, centerZ;
float upX, upY, upZ;

float camRotX = 0;
float camRotY = 0;

float mouseX_prev;
float mouseY_prev;

float roll, pitch, yaw;

void setup()
{
  size(800, 600, P3D);
  
  centerX = 0;
  centerY = 0;
  centerZ = 0;

  cameraX = width/2.0;
  cameraY = height/2.0;
  cameraZ = (height/2.0) / tan(PI*60.0 / 360.0);
  
  upX = 0;
  upY = 1;
  upZ = 0;

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
  
  textSize(24);
  fill(200);
}

void draw()
{
  background(50);
  lights();
  
  if (mousePressed)
  {
    float dx = mouseX - mouseX_prev;
    float dy = mouseY - mouseY_prev;

    camRotY += dx * 0.005;
    camRotX -= dy * 0.005;
    
    //camRotX = constrain(camRotX, -HALF_PI, HALF_PI);
  }
  
  mouseX_prev = mouseX;
  mouseY_prev = mouseY;
  
  translate(width/2, height/2, 0);

  rotateY(camRotY);
  rotateX(camRotX);
  
  rotateX(radians(roll));
  rotateY(radians(pitch));
  rotateZ(radians(yaw));
  
  fill(180, 180, 180);
  box(200, 200, 50);
  
  pushMatrix();
  rotateZ(radians(90));
  translate(80, -150, 0);
  fill(180, 180, 180);
  box(40, 100, 50);
  popMatrix();
  
  pushMatrix();
  translate(-75, -200, 0);
  fill(180, 180, 180);
  box(50, 200, 50);
  popMatrix();
  
  pushMatrix();
  translate(-25, -210, 0);
  fill(180, 180, 180);
  box(50, 220, 50);
  popMatrix();
  
  pushMatrix();
  translate(25, -200, 0);
  fill(180, 180, 180);
  box(50, 200, 50);
  popMatrix();
  
  pushMatrix();
  translate(75, -190, 0);
  fill(180, 180, 180);
  box(50, 180, 50);
  popMatrix();
  
  pushMatrix();
  translate(-75, 0, -30);
  fill(0, 0, 170);
  box(50, 80, 10);
  popMatrix();
  
  stroke(255, 0, 0);
  line(0, 0, 0, 200, 0, 0);
  stroke(0, 255, 0);
  line(0, 0, 0, 0, 200, 0);
  stroke(0, 0, 255);
  line(0, 0, 0, 0, 0, -200);
  stroke(255);
}

void serialEvent(Serial myPort)
{
  String inString = myPort.readStringUntil('\n');
  
  if (inString != null)
  {
    inString = trim(inString);
    String[] list = splitTokens(inString, ",");
    if (list.length >= 3) {
      try
      {
        roll = -float(list[0]);
        pitch = -float(list[1]);
        yaw = float(list[2]);
      }
      catch (NumberFormatException e)
      {
        println("Erro ao converter dados: " + inString);
      }
    }
  }
}
