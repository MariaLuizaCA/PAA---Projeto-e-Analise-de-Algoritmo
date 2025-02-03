#include <iostream>  // Para std::cout, std::cerr
#include <fstream>   // Para std::ifstream
#include <sstream>   // Para std::istringstream
#include <string>
#include <chrono>
#include <cmath>
#include <stdint.h>

using namespace std::chrono;
using namespace std;

//Lista da velocidade dos vetores e futura organização
struct Quick{
    string tipo;
    int contador;
};

//lista de controle dos vetores de tamanho  e ordem
struct Vetores{
    int tamanho;
    int vetor;
    Quick listaQuicks;    

};

//troca de valores
void ordem(int* vetor1, int* vetor2){
    int temp = *vetor1;
    *vetor1 = *vetor2;
    *vetor2 = temp;
};

//definição dos tipos de ordenação
int LP(int* vetor, int inicio, int fim){
    int pivo = vetor[fim], marcador = inicio - 1, ponteiro = fim;
    for (ponteiro = inicio; ponteiro < fim; ponteiro++ ){
        if(vetor[ponteiro] <= pivo){
            ordem(&vetor[++marcador], &vetor[ponteiro]);
        };
    };
};

int LM(int* vetor, int inicio, int fim, int tamanho){
    if(inicio == fim){
        return inicio;
    } else {
        int mediana = LP(vetor, inicio, fim);
        if (mediana - inicio + 1 >= tamanho){
            return LM(vetor, inicio,mediana, tamanho );//coloquei return
        } else {
            return LM(vetor, mediana + 1, fim, tamanho - (mediana - inicio + 1));
        };
    };
};

int LA(int* vetor, int inicio, int fim){
    ordem(&vetor[fim], &vetor[inicio+(rand()%(fim - inicio + 1))]);
    return LP(vetor, inicio, fim);
};

int HP(int* vetor, int inicio, int fim){
    int pivo = vetor[inicio], marcador = inicio - 1, ponteiro = fim +1; 
    while(1){
        while(vetor[--ponteiro]>pivo);
        while(vetor[++marcador]<pivo);
        if( marcador < ponteiro) {
            ordem(&vetor[marcador], &vetor[ponteiro]);
        } else {
          return ponteiro;  
        };
    };
};

int HM(int* vetor, int inicio, int fim, int tamanho){
        if(inicio == fim){
        return inicio; //vetor[inicio];
    } else {
        int mediana = HP(vetor, inicio, fim);
        if (mediana - inicio + 1 == tamanho){
            return HM(vetor, inicio, mediana - 1, tamanho ); //colocquei return
        } else {
            return HM(vetor, mediana + 1, fim, tamanho - (mediana - inicio + 1)); //coloquei return
        };
    };
};

int HA(int* vetor, int inicio, int fim){
    ordem(&vetor[inicio], &vetor[inicio+(rand()%(fim - inicio + 1))]);
    return LP(vetor, inicio, fim);
};


//definição do quicksort 
void quickSort(int* vetor, int inicio, int fim, string tipo){
    //seleção de ordenação
    if (inicio<fim){
        if(tipo == "LP"){ // função feita
            int pivo = LP(vetor, inicio, fim);
            quickSort(vetor, inicio, fim-1, tipo);
            quickSort(vetor, inicio+1, fim, tipo);

        } else if(tipo == "LM"){//função feita
            int pivo = LM(vetor, inicio, fim, 3);
            quickSort(vetor, inicio, fim-1, tipo);
            quickSort(vetor, inicio+1, fim, tipo);        
        } else if(tipo == "LA"){//função feita
            int pivo = LA(vetor, inicio, fim);
            quickSort(vetor, inicio, fim-1, tipo);
            quickSort(vetor, inicio+1, fim, tipo);
        } else if(tipo == "HP"){//função feita
            int pivo = HP(vetor, inicio, fim);
            quickSort(vetor, inicio, pivo, tipo);
            quickSort(vetor, pivo + 1, fim, tipo);
        } else if(tipo == "HM"){
            int pivo = HM(vetor, inicio, fim, 3);
            quickSort(vetor, inicio, fim-1, tipo);
            quickSort(vetor, inicio+1, fim, tipo);
        }else if(tipo == "HA"){
            int pivo = HA(vetor, inicio, fim);
            quickSort(vetor, inicio, fim-1, tipo);
            quickSort(vetor, inicio+1, fim, tipo);
        };



    };
};


/*Função principal: 
1 - Leitura do arquivoIn e esrita do arquivoOut
2 - 

*/
int main (int argc, char* argv[]){


auto start = high_resolution_clock::now();
    //leitura e escrita de arquivo
    ifstream arquivoIn(argv[1]);
    ofstream arquivoOut(argv[2]);

    //verificação da leitura do arquivo 
    if(!arquivoIn.is_open()){
        std::cerr << "\n Erro ao abrir o arquivo" <<std::endl; 
        return 1;
    };

    int numVetores, tamanhoVetor;
    string linha;

    getline(arquivoIn, linha);
    istringstream cont(linha);
    cont >> numVetores;
    Vetores* leituraVetores = new Vetores[numVetores];

    for (int i = 0; i < numVetores; i++){
        getline(arquivoIn, linha);
        istringstream dadosMod(linha);
        dadosMod >> leituraVetores[i].tamanho;
        for (int j = 0; j < leituraVetores[i].tamanho; j++){        
            istringstream celulasV(linha);
            celulasV >> leituraVetores[j].vetor;
        };
        
    };

    arquivoIn.close();
    arquivoOut.close();
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tempo de execução: " << duration.count() << " microsegundos" << endl;
    
    return 0;
};

//.\mariaandrade_202300027525_porto.exe porto.input.txt porto.output.txt