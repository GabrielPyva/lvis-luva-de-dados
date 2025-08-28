using UnityEngine;
using System;
using System.IO.Ports;
using System.Threading;
using System.Collections.Generic; // Para usar List<byte>
using System.Text;              // Para converter bytes em string

public class SerialController : MonoBehaviour
{
    // --- Configurações da Porta Serial ---
    public string portName = "COM3";
    public int baudRate = 9600;

    // --- Variáveis da Thread ---
    private Thread serialThread;
    private volatile bool isThreadRunning = false;
    private Queue<string> receivedLinesQueue = new Queue<string>(); // Fila para múltiplas linhas
    private object queueLockObject = new object();

    private SerialPort stream;

    void Start()
    {
        Connect();
    }

    void Connect()
    {
        try
        {
            stream = new SerialPort(portName, baudRate);
            stream.ReadTimeout = 1; // Timeout mínimo, já que não vamos mais bloquear
            stream.DtrEnable = true;
            stream.Open();

            isThreadRunning = true;
            serialThread = new Thread(ReadSerialData);
            serialThread.IsBackground = true;
            serialThread.Start();
            
            Debug.Log("Porta serial aberta e thread de leitura robusta iniciada!");
        }
        catch (Exception e)
        {
            Debug.LogError($"Erro ao abrir a porta serial: {e.Message}");
        }
    }

    // --- ESTA É A NOVA LÓGICA DE LEITURA (EXECUTADA PELA THREAD SECUNDÁRIA) ---
    private void ReadSerialData()
    {
        // Buffer para acumular os bytes que chegam
        List<byte> byteBuffer = new List<byte>();

        // Dá um tempo para o Arduino reiniciar após a conexão
        Thread.Sleep(2000);

        while (isThreadRunning && stream != null && stream.IsOpen)
        {
            try
            {
                // 1. Verifica se há bytes para ler, sem bloquear
                if (stream.BytesToRead > 0)
                {
                    // 2. Lê todos os bytes disponíveis no buffer do sistema
                    int bytesToRead = stream.BytesToRead;
                    byte[] readBuffer = new byte[bytesToRead];
                    stream.Read(readBuffer, 0, bytesToRead);

                    // 3. Adiciona os bytes lidos ao nosso buffer local
                    byteBuffer.AddRange(readBuffer);

                    // 4. Procura por uma mensagem completa (terminada em '\n')
                    int newlineIndex = byteBuffer.IndexOf((byte)'\n');
                    if (newlineIndex >= 0)
                    {
                        // 5. Converte os bytes da mensagem para uma string
                        string completeLine = Encoding.ASCII.GetString(byteBuffer.ToArray(), 0, newlineIndex + 1).Trim();
                        
                        // 6. Adiciona a linha completa à fila para a thread principal processar
                        lock (queueLockObject)
                        {
                            receivedLinesQueue.Enqueue(completeLine);
                        }

                        // 7. Remove a mensagem processada do nosso buffer
                        byteBuffer.RemoveRange(0, newlineIndex + 1);
                    }
                }
            }
            catch (Exception e)
            {
                // Se um erro real acontecer (ex: Arduino desconectado)
                Debug.LogWarning($"Erro na thread serial: {e.Message}");
                isThreadRunning = false; // Sinaliza para o loop parar
            }
            // Pequena pausa para não sobrecarregar a CPU
            Thread.Sleep(1); 
        }
        Debug.Log("Thread de leitura finalizada.");
    }

    // --- Esta função é executada pela THREAD PRINCIPAL (Unity) ---
    void Update()
    {
        // Processa todas as linhas que foram recebidas pela outra thread
        lock (queueLockObject)
        {
            while (receivedLinesQueue.Count > 0)
            {
                string line = receivedLinesQueue.Dequeue();
                if (!string.IsNullOrEmpty(line))
                {
                    Debug.Log("DADO RECEBIDO: " + line);
                    //
                    // AQUI VOCÊ COLOCARÁ A LÓGICA PARA PARSEAR A STRING E ANIMAR A MÃO
                    // Ex: string[] values = line.Split(',');
                    //
                }
            }
        }
    }

    // --- Garante que a porta e a thread sejam fechadas corretamente ---
    void OnDestroy()
    {
        isThreadRunning = false; 

        if (serialThread != null && serialThread.IsAlive)
        {
            serialThread.Join(100);
        }

        if (stream != null && stream.IsOpen)
        {
            stream.Close();
            Debug.Log("Porta serial fechada.");
        }
    }
}