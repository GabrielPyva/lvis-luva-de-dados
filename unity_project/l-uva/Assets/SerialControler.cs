// 1. Chame a biblioteca necessária no início do seu script
using System.IO.Ports;
using UnityEngine;

public class SerialController : MonoBehaviour
{
    // 2. Crie uma instância da classe SerialPort
    // A porta (ex: "COM5") e o Baud Rate (ex: 9600) devem ser os mesmos do seu Arduino.
    SerialPort stream = new SerialPort("COM3", 9600);

    void Start()
    {
        try
        {
            // 3. Abra a conexão serial
            stream.Open();
            Debug.Log("Porta serial aberta com sucesso!");
        }
        catch (System.Exception e)
        {
            Debug.LogError("Erro ao abrir a porta serial: " + e.Message);
        }
    }

    void Update()
    {
        // 4. Verifique se a porta está aberta antes de ler
        if (stream.IsOpen)
        {
            try
            {
                // 5. Leia uma linha de dados da porta serial
                string dataString = stream.ReadLine();
                
                // Imprime a string recebida no console do Unity para teste
                Debug.Log("Dado recebido: " + dataString);

                // AQUI você vai processar a string (dataString) para animar sua mão
                // Ex: "roll,pitch,yaw,dedo1,dedo2,..."
                // Você usaria string.Split(',') para separar os valores.
            }
            catch (System.Exception e)
            {
                // Se ocorrer um erro (ex: o Arduino foi desconectado), ele será mostrado aqui
                Debug.LogWarning("Erro ao ler da porta serial: " + e.Message);
            }
        }
    }

    // 6. Feche a porta quando o jogo terminar para evitar erros
    void OnApplicationQuit()
    {
        if (stream != null && stream.IsOpen)
        {
            stream.Close();
            Debug.Log("Porta serial fechada.");
        }
    }
}