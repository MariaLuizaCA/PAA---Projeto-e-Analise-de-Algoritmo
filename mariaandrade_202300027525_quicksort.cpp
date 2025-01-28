#include <iostream>  // Para std::cout, std::cerr
#include <fstream>   // Para std::ifstream
#include <sstream>   // Para std::istringstream
#include <string>
#include <chrono>
#include <cmath>
#include <stdint.h>

using namespace std::chrono;
using namespace std;

struct Quick{
    string tipo;
    int contador;
};
struct Vetores{
    int tamanho;
    int vetor;
    Quick listaQuicks;    

};


void ordem(int* vetor1, int* vetor2){};

void quickSort(int* vetor, int inicio, int fim, string tipo){
    if (inicio<fim){
        if(tipo == "LP"){
            int pivo = LP(vetor, inicio, fim);
            quickSort(vetor, inicio, fim-1, tipo);
            quickSort(vetor, inicio+1, fim, tipo);
        } else if(tipo == "LM"){
            int pivo = LM(vetor, inicio, fim);
            quickSort(vetor, inicio, fim-1, tipo);
            quickSort(vetor, inicio+1, fim, tipo);        
        } else if(tipo == "LA"){
            int pivo = LA(vetor, inicio, fim);
            quickSort(vetor, inicio, fim-1, tipo);
            quickSort(vetor, inicio+1, fim, tipo);
        } else if(tipo == "HP"){
            int pivo = HP(vetor, inicio, fim);
            quickSort(vetor, inicio, fim-1, tipo);
            quickSort(vetor, inicio+1, fim, tipo);
        } else if(tipo == "HM"){
            int pivo = HM(vetor, inicio, fim);
            quickSort(vetor, inicio, fim-1, tipo);
            quickSort(vetor, inicio+1, fim, tipo);
        }else if(tipo == "HA"){
            int pivo = HA(vetor, inicio, fim);
            quickSort(vetor, inicio, fim-1, tipo);
            quickSort(vetor, inicio+1, fim, tipo);
        };
    };
};
int LP(int* vetor, int inicio, int fim){
    int pivo = vetor[fim], marcador = inicio - 1, ponteiro = fim;
    for (ponteiro = inicio; ponteiro < fim; ponteiro++ ){
        if(vetor[ponteiro] <= pivo){
            ordem(&vetor[++marcador], &vetor[ponteiro]);
        };
    };
};
int LM(int* vetor, int inicio, int fim){};
int LA(int* vetor, int inicio, int fim){};
int HP(int* vetor, int inicio, int fim){};
int HM(int* vetor, int inicio, int fim){};
int HA(int* vetor, int inicio, int fim){};

/*Função principal: 
1 - Leitura do arquivoIn e esrita do arquivoOut
2 - 

*/
int main (int argc, char* argv[]){


auto start = high_resolution_clock::now();
    //leitura e escrita de arquivo
    ifstream arquivoIn(argv[1]);
    ofstream arquivoOut(argv[2]);

    if(!arquivoIn.is_open()){
        std::cerr << "\n Erro ao abrir o arquivo" <<std::endl; 
        return 1;
    };
    arquivoIn.close();
    arquivoOut.close();
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tempo de execução: " << duration.count() << " microsegundos" << endl;
    
    return 0;
};

//.\mariaandrade_202300027525_porto.exe porto.input.txt porto.output.txt