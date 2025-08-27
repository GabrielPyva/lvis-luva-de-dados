using UnityEngine;

// 1. A biblioteca necessária para a comunicação serial já está declarada.
using System.IO.Ports;

public class SerialController : MonoBehaviour
{
    // --- Variáveis de Configuração ---
    // No futuro, as variáveis para configurar a porta serial (nome, baud rate, etc.)
    // serão adicionadas aqui.
    // Ex: public string portName = "COM5";
    // Ex: public int baudRate = 9600;


    // O método Start é chamado uma vez, antes do primeiro frame.
    void Start()
    {
        // A lógica para encontrar e abrir a porta serial será implementada aqui no futuro.
        // Debug.Log("Controlador Serial iniciado.");
    }

    // O método Update é chamado a cada frame.
    void Update()
    {
        // A lógica para ler os dados da luva a cada frame e processá-los
        // será implementada aqui no futuro.
    }

    // O método OnDestroy (ou OnApplicationQuit) será usado no futuro
    // para garantir que a porta serial seja fechada corretamente ao sair do jogo.
}