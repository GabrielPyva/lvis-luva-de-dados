using UnityEngine;

public class HandAnimator : MonoBehaviour
{
    // --- Referência para o Controlador Serial ---
    // Precisamos pegar os dados que o outro script está lendo.
    public SerialController serialController;

    // --- Referências para os Ossos (Transforms) da Mão ---
    // Criei campos para o pulso e para as 3 juntas de cada dedo.
    // Você vai arrastar os objetos da Hierarchy para estes campos no Inspector.
    [Header("Orientação da Mão")]
    public Transform wrist; // O objeto raiz da mão, para Roll, Pitch, Yaw

    [Header("Dedos")]
    public Transform thumb_01;
    public Transform thumb_02;
    public Transform thumb_03;

    public Transform index_01;
    public Transform index_02;
    public Transform index_03;

    public Transform middle_01;
    public Transform middle_02;
    public Transform middle_03;

    public Transform ring_01;
    public Transform ring_02;
    public Transform ring_03;

    public Transform pinky_01;
    public Transform pinky_02;
    public Transform pinky_03;

    // ... depois da declaração dos Transforms dos dedos
    // Variáveis privadas para guardar a rotação inicial (de repouso) dos ossos

    private Quaternion initialWristRotation;
    private Quaternion initialThumb1Rotation, initialThumb2Rotation, initialThumb3Rotation;
    private Quaternion initialIndex1Rotation, initialIndex2Rotation, initialIndex3Rotation;
    private Quaternion initialMiddle1Rotation, initialMiddle2Rotation, initialMiddle3Rotation;
    private Quaternion initialRing1Rotation, initialRing2Rotation, initialRing3Rotation;
    private Quaternion initialPinky1Rotation, initialPinky2Rotation, initialPinky3Rotation;

    void Start()
    {
        // Salva a rotação inicial de cada junta no primeiro frame
        if (wrist != null) initialWristRotation = wrist.localRotation;

        if (thumb_01 != null) initialThumb1Rotation = thumb_01.localRotation;
        if (thumb_02 != null) initialThumb2Rotation = thumb_02.localRotation;
        if (thumb_03 != null) initialThumb3Rotation = thumb_03.localRotation;

        if (index_01 != null) initialIndex1Rotation = index_01.localRotation;
        if (index_02 != null) initialIndex2Rotation = index_02.localRotation;
        if (index_03 != null) initialIndex3Rotation = index_03.localRotation;

        // ...faça o mesmo para middle, ring e pinky
        if (middle_01 != null) initialMiddle1Rotation = middle_01.localRotation;
        if (middle_02 != null) initialMiddle2Rotation = middle_02.localRotation;
        if (middle_03 != null) initialMiddle3Rotation = middle_03.localRotation;

        if (ring_01 != null) initialRing1Rotation = ring_01.localRotation;
        if (ring_02 != null) initialRing2Rotation = ring_02.localRotation;
        if (ring_03 != null) initialRing3Rotation = ring_03.localRotation;

        if (pinky_01 != null) initialPinky1Rotation = pinky_01.localRotation;
        if (pinky_02 != null) initialPinky2Rotation = pinky_02.localRotation;
        if (pinky_03 != null) initialPinky3Rotation = pinky_03.localRotation;
    }
    void Update()
    {
        // Pega a linha de dados mais recente do SerialController
        // NOTA: Esta é uma forma simplificada. O ideal é usar a fila como no seu
        // SerialController, mas para começar, isso funciona.
        // Vamos refatorar para usar a fila do SerialController no futuro.
        
        // **A LÓGICA DE PROCESSAMENTO DE DADOS SERÁ ADICIONADA AQUI**
        // Por enquanto, vamos deixar o Update vazio para focar na configuração.
    }

    // Criaremos uma função pública para ser chamada pelo SerialController
    public void ProcessSerialData(string data)
    {
        // O formato esperado do Arduino é: "roll,pitch,yaw,polegar,indicador,medio,anelar,mindinho"
        //

        string[] values = data.Split(',');

        // Verificação de segurança para evitar erros se uma linha incompleta for recebida
        if (values.Length < 8)
        {
            return;
        }

        try
        {
            // 1. Lê os valores de orientação
            float roll = float.Parse(values[0]);
            float pitch = float.Parse(values[1]);
            float yaw = float.Parse(values[2]);

            // 2. Lê os valores de flexão dos dedos
            // O seu .ino envia 0 ou 1 para o polegar, e ângulos para os outros.
            // Vamos tratar o polegar de forma especial.
            float thumbFlex = float.Parse(values[3]);
            float indexFlex = float.Parse(values[4]);
            float middleFlex = float.Parse(values[5]);
            float ringFlex = float.Parse(values[6]);
            float pinkyFlex = float.Parse(values[7]);

            // 3. Aplica as rotações
            // A orientação (Roll, Pitch, Yaw) é aplicada ao pulso/mão inteira.
            // O Euler usa a ordem Z, X, Y. Você talvez precise ajustar a ordem ou inverter valores (-roll).
            if (wrist != null)
                wrist.localRotation = initialWristRotation * Quaternion.Euler(pitch, yaw, roll);

            // Passe as rotações iniciais salvas para a função de flexão
            ApplyFingerFlex(thumb_01, thumb_02, thumb_03, initialThumb1Rotation, initialThumb2Rotation, initialThumb3Rotation, thumbFlex);
            ApplyFingerFlex(index_01, index_02, index_03, initialIndex1Rotation, initialIndex2Rotation, initialIndex3Rotation, indexFlex);
            ApplyFingerFlex(middle_01, middle_02, middle_03, initialMiddle1Rotation, initialMiddle2Rotation, initialMiddle3Rotation, middleFlex);
            ApplyFingerFlex(ring_01, ring_02, ring_03, initialRing1Rotation, initialRing2Rotation, initialRing3Rotation, ringFlex);
            ApplyFingerFlex(pinky_01, pinky_02, pinky_03, initialPinky1Rotation, initialPinky2Rotation, initialPinky3Rotation, pinkyFlex);
        }
        catch (System.Exception e)
        {
            Debug.LogWarning("Erro ao processar dados: " + e.Message);
        }
    }

    // Função auxiliar para aplicar a rotação de flexão a um dedo
    void ApplyFingerFlex(Transform joint1, Transform joint2, Transform joint3, Quaternion initialRot1, Quaternion initialRot2, Quaternion initialRot3, float flexValue)
    {
        // A flexão total é dividida entre as juntas. Ajuste as proporções se necessário.
        // Eixo de rotação: Vector3.forward corresponde ao eixo Z.
        // Se o dedo dobrar para o lado errado, tente Vector3.right (X) ou Vector3.up (Y),
        // ou use valores negativos (ex: -flexValue / 3).
        // Crie a rotação de flexão a partir do valor do sensor
        // Lembre-se de usar o eixo correto que você descobriu antes (X, Y ou Z)
        Quaternion flexRotation = Quaternion.Euler(-2*flexValue / 3, 0, 0); // Exemplo usando o eixo Y

        // Aplica a rotação de forma aditiva
        if (joint1 != null)
            joint1.localRotation = initialRot1 * flexRotation;
        if (joint2 != null)
            joint2.localRotation = initialRot2 * flexRotation;
        if (joint3 != null)
            joint3.localRotation = initialRot3 * flexRotation;
    }
}