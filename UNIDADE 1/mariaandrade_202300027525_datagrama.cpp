#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Pacote {
    int num;
    int tamanho;
    int* pecks;
};



void heapify(int lista[], int n, int i) {
    int maior = i;             
    int esquerda = 2 * i + 1;  
    int direita = 2 * i + 2;  
    //esquerdo é maior que a raiz
    if (esquerda < n && lista[esquerda] > lista[maior])
        maior = esquerda;

    //direito é maior que a raiz
    if (direita < n && lista[direita] > lista[maior])
        maior = direita;

    // troca raiz meonor
    if (maior != i) {
        swap(lista[i], lista[maior]);
        heapify(lista, n, maior); 
    }
}

void heapSort(int lista[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(lista, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(lista[0], lista[i]);  
        heapify(lista, i, 0);
    }
}

void maxHeap(int lista[], int n){
    for(int i = n / 2 - 1; i >= 0; i--){
        heapify(lista, n, i);
    }
}



// Função para imprimir os pacotes na saída formatada
void escrever(Pacote* pacotes, int& escreveu, ofstream& arquivoOut) {
    cout << "Escrevendo pacotes" << endl;
    while(1){
        if (escreveu == pacotes[escreveu].num){
        cout << "escreveu" << endl;
        arquivoOut << "|";
        for (int j = 0; j < pacotes[escreveu].tamanho; j++) {
            if (j > 0) arquivoOut << ",";
            arquivoOut << hex << pacotes[escreveu].pecks[j];
        }
        arquivoOut << "|";
        escreveu++;
        cout << "Pacote " << escreveu << endl;
        } else {
            arquivoOut << endl;
            break;
            cout << "Pacote " << escreveu << endl;
        }
    }
    
}

int main(int argc, char* argv[]) {
    auto start = high_resolution_clock::now();

    ifstream arquivoIn(argv[1]);
    ofstream arquivoOut(argv[2]);

    if (!arquivoIn.is_open()) {
        std::cerr << "\nErro ao abrir o arquivo" << std::endl;
        return 1;
    }

    int numPacotes, quantPacotes;
    arquivoIn >> numPacotes >> quantPacotes;

    Pacote* pacotes = new Pacote[numPacotes];
    int escreveu = 0;
    int cont = 1;
    for (int i = 0; i < numPacotes; i++) {
        cout << "Pacote " << i << endl;
        if(cont != quantPacotes){
            arquivoIn >> pacotes[i].num >> pacotes[i].tamanho;
            pacotes[i].pecks = new int[pacotes[i].tamanho];
            for (int j = 0; j < pacotes[i].tamanho; j++) {
                arquivoIn >> hex >> pacotes[i].pecks[j];  
            }
            heapSort(pacotes[i].pecks, i);
            cout<< "ordenado" << endl;               
            cont++;
            cout << "Contador: " << cont << endl;
        } else {
            cout << "Contador: " << cont << endl;



            escrever(pacotes, escreveu, arquivoOut);
            cout << "Escreveu" << endl;
            cont = 1;

            arquivoIn >> pacotes[i].num >> pacotes[i].tamanho;
            pacotes[i].pecks = new int[pacotes[i].tamanho];
            for (int j = 0; j < pacotes[i].tamanho; j++) {
                arquivoIn >> hex >> pacotes[i].pecks[j];  

                cout << "Pacote " << i << endl;
            }
            heapSort(pacotes[i].pecks, i);
            cout<< "ordenado" << endl;               

        };
    };

    for (int i = 0; i < numPacotes; i++) {
        cout<< pacotes[i].num<<endl;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tempo de execução: " << duration.count() << " microsegundos" << endl;

    for (int i = 0; i < numPacotes; i++) {
        delete[] pacotes[i].pecks;
    }


    delete[] pacotes;

    return 0;
}

//.\mariaandrade_202300027525_datagrama.exe datagrama.input.txt datagrama.output.txt