import processing.serial.*;

Serial myPort;
float cameraX, cameraY, cameraZ; // Posição da câmera (não vamos usá-las diretamente para o movimento do mouse, mas é bom saber que existem)
float centerX, centerY, centerZ; // O ponto para onde a câmera está olhando (o centro do cubo)
float upX, upY, upZ;           // A direção "para cima" da câmera

float rotX = 0; // Variável para controlar a rotação da câmera no eixo X (pitch)
float rotY = 0; // Variável para controlar a rotação da câmera no eixo Y (yaw)

// Variáveis para rastrear a posição anterior do mouse para calcular o movimento
float pmouseX_prev;
float pmouseY_prev;

float roll, pitch, yaw;

void setup() {
  size(800, 600, P3D); // Define o tamanho da janela e habilita o renderizador 3D
  
  // Define o ponto central onde o cubo estará
  centerX = 0;
  centerY = 0;
  centerZ = 0;

  // Define a posição inicial da câmera
  // Padrão para "camera()" sem argumentos, que é uma boa base para começar
  cameraX = width/2.0;
  cameraY = height/2.0;
  cameraZ = (height/2.0) / tan(PI*60.0 / 360.0); // Perspectiva padrão do Processing
  
  upX = 0;
  upY = 1; // Y para cima
  upZ = 0;

  // Inicializa as posições anteriores do mouse
  pmouseX_prev = mouseX;
  pmouseY_prev = mouseY;
  
  try {
    myPort = new Serial(this, "COM6", 115200);
    myPort.bufferUntil('\n');
  } catch (Exception e) {
    println("Erro ao abrir a porta serial: " + e.getMessage());
    println("Por favor, verifique se o Arduino está conectado e a porta serial correta.");
    exit();
  }
  textSize(24);
  fill(200);
}

void draw() {
  background(50); // Fundo cinza escuro
  // Habilita a iluminação para dar profundidade ao cubo
  lights();
  // ***** Controle da Câmera com o Mouse *****
  // Só aplica rotação se o botão do mouse estiver pressionado
  if (mousePressed) {
    // Calcula a diferença de movimento do mouse desde o último frame
    float dx = mouseX - pmouseX_prev;
    float dy = mouseY - pmouseY_prev;

    // Atualiza as rotações com base no movimento do mouse
    // Multiplica por um fator pequeno para controlar a sensibilidade
    rotY += dx * 0.005; // Movimento horizontal do mouse (eixo Y da câmera)
    rotX -= dy * 0.005; // Movimento vertical do mouse (eixo X da câmera)
    
    // Limita a rotação X (pitch) para evitar virar a câmera de cabeça para baixo
    rotX = constrain(rotX, -HALF_PI, HALF_PI); // Entre -90 e 90 graus
  }
  
  // Atualiza as posições anteriores do mouse para o próximo frame
  pmouseX_prev = mouseX;
  pmouseY_prev = mouseY;
  
  // Traduz a cena para o centro da tela antes de aplicar as rotações da câmera
  // Isso faz com que a rotação seja em torno do cubo
  translate(width/2, height/2, 0);

  // Aplica as rotações da câmera
  // Note que estamos aplicando as rotações ANTES de desenhar o cubo.
  // Isso efetivamente rotaciona o "mundo" ao redor da câmera, dando a ilusão de que a câmera está se movendo.
  rotateY(rotY); // Rotação Yaw (horizontal)
  rotateX(rotX); // Rotação Pitch (vertical)
  
  rotateX(radians(roll));
  rotateY(radians(pitch));
  rotateZ(radians(yaw));
  
  // Desenha a palma no centro da cena
  fill(200, 100, 255);
  box(200, 200, 50);
  // Desenha dedo indicador
  pushMatrix();
  translate(-75, -200, 0);
  fill(200, 100, 255);
  box(50, 200, 50);
  popMatrix();
  // Desenha dedo medio
  pushMatrix();
  translate(-25, -210, 0);
  fill(200, 100, 255);
  box(50, 220, 50);
  popMatrix();
  // Desenha dedo anelar
  pushMatrix();
  translate(25, -200, 0);
  fill(200, 100, 255);
  box(50, 200, 50);
  popMatrix();
  // Desenha dedo mindinho
  pushMatrix();
  translate(75, -190, 0);
  fill(200, 100, 255);
  box(50, 180, 50);
  popMatrix();
  // Desenha polegar 
  pushMatrix();
  rotateZ(radians(-90));
  translate(-80, -150, 0);
  fill(200, 100, 255);
  box(40, 100, 50);
  popMatrix();
  // Opcional: Desenha eixos para referência (remova ou comente se não precisar)
  stroke(255, 0, 0); // Eixo X (vermelho)
  line(0, 0, 0, 200, 0, 0);
  stroke(0, 255, 0); // Eixo Y (verde)
  line(0, 0, 0, 0, 200, 0);
  stroke(0, 0, 255); // Eixo Z (azul)
  line(0, 0, 0, 0, 0, 200);
  stroke(255); // Volta para a cor padrão do traço 
}

void serialEvent(Serial myPort) {
  String inString = myPort.readStringUntil('\n');
  
  if (inString != null) {
    inString = trim(inString);
    String[] list = splitTokens(inString, ",");
    if (list.length >= 3) {
      try {
        roll = float(list[0]);
        pitch = float(list[1]);
        yaw = float(list[2]);
      } catch (NumberFormatException e) {
        println("Erro ao converter dados: " + inString);
      }
    }
  }
}
