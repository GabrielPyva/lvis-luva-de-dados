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
            float thumbFlex = float.Parse(values[3]) * 90.0f; // Converte 0/1 para 0/90 graus
            float indexFlex = float.Parse(values[4]);
            float middleFlex = float.Parse(values[5]);
            float ringFlex = float.Parse(values[6]);
            float pinkyFlex = float.Parse(values[7]);

            // 3. Aplica as rotações
            // A orientação (Roll, Pitch, Yaw) é aplicada ao pulso/mão inteira.
            // O Euler usa a ordem Z, X, Y. Você talvez precise ajustar a ordem ou inverter valores (-roll).
            if (wrist != null)
                wrist.localRotation = Quaternion.Euler(pitch, yaw, roll);

            // A flexão é aplicada aos ossos dos dedos.
            // Geralmente, a flexão acontece em apenas um eixo (Z na maioria dos rigs).
            // Dividimos o valor total da flexão entre as juntas do dedo.
            ApplyFingerFlex(thumb_01, thumb_02, thumb_03, thumbFlex);
            ApplyFingerFlex(index_01, index_02, index_03, indexFlex);
            ApplyFingerFlex(middle_01, middle_02, middle_03, middleFlex);
            ApplyFingerFlex(ring_01, ring_02, ring_03, ringFlex);
            ApplyFingerFlex(pinky_01, pinky_02, pinky_03, pinkyFlex);
        }
        catch (System.Exception e)
        {
            Debug.LogWarning("Erro ao processar dados: " + e.Message);
        }
    }

    // Função auxiliar para aplicar a rotação de flexão a um dedo
    void ApplyFingerFlex(Transform joint1, Transform joint2, Transform joint3, float flexValue)
    {
        // A flexão total é dividida entre as juntas. Ajuste as proporções se necessário.
        // Eixo de rotação: Vector3.forward corresponde ao eixo Z.
        // Se o dedo dobrar para o lado errado, tente Vector3.right (X) ou Vector3.up (Y),
        // ou use valores negativos (ex: -flexValue / 3).
        if (joint1 != null)
            joint1.localRotation = Quaternion.Euler(0, 0, flexValue / 3);
        if (joint2 != null)
            joint2.localRotation = Quaternion.Euler(0, 0, flexValue / 3);
        if (joint3 != null)
            joint3.localRotation = Quaternion.Euler(0, 0, flexValue / 3);
    }
}