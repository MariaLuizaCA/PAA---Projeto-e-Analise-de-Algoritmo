#include <iostream>  // Para std::cout, std::cerr
#include <fstream>   // Para std::ifstream
#include <sstream>   // Para std::istringstream
#include <string>
#include <chrono>
#include <cmath>
#include <cstring>  // Para calloc e free
using namespace std::chrono;
using namespace std;

/*STRUCTS: 
    1. CAMINHÕES: armazena listas das placas, pesos e volumes
    2. PACOTES: armazena lista de códigos, valores, pesos, volumes e indice.
    3. CARREGAMENTO: *** TALVEZ *** criar esse registro para facilitar a escrita no arquivo de saída
*/ 

struct Caminhao {
    string placa;
    int peso;
    int volume;
};

struct Pacote {
    string codigo;
    float valor;
    int peso;
    int volume; 
    bool carregado = false;
    int indice;
};

struct Carregamento {
    string placa;
    string* codigo = nullptr;
    float valor = 0;
    int peso = 0;
    int volume = 0;
    int porcentagemPeso = 0;
    int porcentagemVolume = 0;
    int numPacotes = 0;
};

/*FUNÇÃO DIÂMICA: 
    MATRIZ 3D (item, peso, volume), ela vai ser escrita da forma inversa, para podermos desconsiderar os valores nulos da tabela
*/

void snapsack(Carregamento& carregamento, int pesoMax, int volumeMax, Pacote* pacotes, int numPacotes) {
    // Aloca a tabela 3D usando calloc
    float*** dp = (float***)calloc(numPacotes + 1, sizeof(float**));
    for (int i = 0; i <= numPacotes; i++) {
        dp[i] = (float**)calloc(pesoMax + 1, sizeof(float*));
        for (int j = 0; j <= pesoMax; j++) {
            dp[i][j] = (float*)calloc(volumeMax + 1, sizeof(float));
        }
    }

    // Preenche a tabela usando programação dinâmica
    for (int i = 1; i <= numPacotes; i++) {
        for (int j = 0; j <= pesoMax; j++) {
            for (int k = 0; k <= volumeMax; k++) {
                if (pacotes[i - 1].peso <= j && pacotes[i - 1].volume <= k) {
                    dp[i][j][k] = max(dp[i - 1][j][k], dp[i - 1][j - pacotes[i - 1].peso][k - pacotes[i - 1].volume] + pacotes[i - 1].valor);
                } else {
                    dp[i][j][k] = dp[i - 1][j][k];
                }
            }
        }
    }

    // Faz o traceback para encontrar os pacotes selecionados
    int j = pesoMax;
    int k = volumeMax;
    for (int i = numPacotes; i > 0; i--) {
        if (dp[i][j][k] != dp[i - 1][j][k]) {
            carregamento.codigo[carregamento.numPacotes] = pacotes[i - 1].codigo;
            carregamento.numPacotes++;
            carregamento.valor += pacotes[i - 1].valor;
            carregamento.peso += pacotes[i - 1].peso;
            carregamento.volume += pacotes[i - 1].volume;
            j -= pacotes[i - 1].peso;
            k -= pacotes[i - 1].volume;
        }
    }

    // Calcula as porcentagens de peso e volume
    carregamento.porcentagemPeso = (carregamento.peso * 100) / pesoMax;
    carregamento.porcentagemVolume = (carregamento.volume * 100) / volumeMax;

    // Libera a memória alocada para a tabela 3D
    for (int i = 0; i <= numPacotes; i++) {
        for (int j = 0; j <= pesoMax; j++) {
            free(dp[i][j]);
        }
        free(dp[i]);
    }
    free(dp);
}

/*MAIN:
    Leitura e escrita dos arquivos e chamadas de função knapsack
*/

int main(int argc, char* argv[]) {
    auto start = high_resolution_clock::now();

    // Leitura e escrita de arquivo
    ifstream arquivoIn(argv[1]);
    ofstream arquivoOut(argv[2]);

    // Verifica se o arquivo abriu corretamente
    if (!arquivoIn.is_open()) {
        cerr << "\n Erro ao abrir o arquivo" << endl;
        return 1;
    }

    int numCam, numPac;

    // Leitura do número de caminhões
    arquivoIn >> numCam;
    Caminhao* caminhoes = (Caminhao*)calloc(numCam, sizeof(Caminhao));
    for (int i = 0; i < numCam; i++) {
        arquivoIn >> caminhoes[i].placa >> caminhoes[i].peso >> caminhoes[i].volume;
    }

    // Leitura do número de pacotes
    arquivoIn >> numPac;
    Pacote* pacotes = (Pacote*)calloc(numPac, sizeof(Pacote));
    for (int i = 0; i < numPac; i++) {
        arquivoIn >> pacotes[i].codigo >> pacotes[i].valor >> pacotes[i].peso >> pacotes[i].volume;
    }

    // Busca dos pacotes que podem ser carregados (busca dinâmica)
    Carregamento* selecionados = (Carregamento*)calloc(numCam, sizeof(Carregamento));
    for (int i = 0; i < numCam; i++) {
        selecionados[i].codigo = (string*)calloc(numPac, sizeof(string));
        selecionados[i].placa = caminhoes[i].placa;
        snapsack(selecionados[i], caminhoes[i].peso, caminhoes[i].volume, pacotes, numPac);
    }

    // Escrita do struct carregamento
    for (int i = 0; i < numCam; i++) {
        arquivoOut << "[" << selecionados[i].placa << "] R$" << selecionados[i].valor << ", " << selecionados[i].peso << "KG (" << selecionados[i].porcentagemPeso << "%), " << selecionados[i].volume << "L (" << selecionados[i].porcentagemVolume << "%) -> ";
        for (int j = 0; j < selecionados[i].numPacotes; j++) {
            arquivoOut << selecionados[i].codigo[j];
            if (j < selecionados[i].numPacotes - 1) arquivoOut << ", ";
        }
        arquivoOut << endl;
    }

    arquivoIn.close();
    arquivoOut.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tempo de execução: " << duration.count() << " microsegundos" << endl;

    // Libera a memória alocada
    for (int i = 0; i < numCam; i++) {
        free(selecionados[i].codigo);
    }
    free(selecionados);
    free(caminhoes);
    free(pacotes);

    return 0;
}