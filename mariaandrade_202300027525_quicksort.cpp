#include <iostream>  // Para std::cout, std::cerr
#include <fstream>   // Para std::ifstream
#include <sstream>   // Para std::istringstream
#include <string>
#include <chrono>
#include <cmath>
#include <stdint.h>

using namespace std::chrono;
using namespace std;
int cont = 0;

//Lista da velocidade dos vetores e futura organização
struct Quick{
    string tipo;
    int contador;
};

//lista de controle dos vetores de tamanho  e ordem
struct Vetores{
    int tamanho;
    int* vetor;
    Quick listaQuicks;    

};

//troca de valores
//troca de valores
void ordem(int* vetor1, int* vetor2){
    int temp = *vetor1;
    *vetor1 = *vetor2;
    *vetor2 = temp;
    cont++;
};

//definição dos tipos de ordenação
int LP(int* vetor, int inicio, int fim){
    int pivo = vetor[fim], marcador = inicio - 1, ponteiro = fim;
    for (ponteiro = inicio; ponteiro < fim; ponteiro++ ){
        if(vetor[ponteiro] <= pivo){
            ordem(&vetor[++marcador], &vetor[ponteiro]);
        };
    };
    ordem(&vetor[marcador + 1], &vetor[fim]);
    return marcador + 1;
};

int LM(int* vetor, int inicio, int fim){

    int v1 = inicio + (fim - inicio + 1) / 4;
    int v2 = inicio + (fim - inicio + 1) / 2;
    int v3 = inicio + 3 * (fim - inicio + 1) / 4;

    if (vetor[v1] > vetor[v2]  && vetor[v1] < vetor[v3] || vetor[v1] < vetor[v2] && vetor[v1] > vetor[v3]){
       ordem(&vetor[fim], &vetor[v1]);
    } else if(vetor[v2] > vetor[v1]  && vetor[v2] < vetor[v3] || vetor[v2] < vetor[v1] && vetor[v2] > vetor[v3]){
        ordem(&vetor[fim], &vetor[v2]);
    } else {
        ordem(&vetor[fim], &vetor[v3]);
    };
        return LP(vetor, inicio, fim);
};

int LA(int* vetor, int inicio, int fim){
    int aleatorio = inicio + (abs(vetor[inicio]) % (fim - inicio + 1));
    ordem(&vetor[fim], &vetor[aleatorio]);
    return LP(vetor, inicio, fim);
};

int HP(int* vetor, int inicio, int fim){
    int pivo = vetor[inicio], marcador = inicio - 1, ponteiro = fim + 1; 
    while(true){
        do {
            ponteiro--;
        } while (vetor[ponteiro] > pivo);

        // Move o marcador para a direita até encontrar um elemento >= pivô
        do {
            marcador++;
        } while (vetor[marcador] < pivo);

        // Se os ponteiros se cruzaram, a partição está completa
        if (marcador >= ponteiro) {
            return ponteiro; // Retorna o ponto de divisão
        }

        // Troca os elementos fora de ordem e incrementa o contador de trocas
        ordem(&vetor[marcador], &vetor[ponteiro]);
    
    };
};

int HM(int* vetor, int inicio, int fim){

    int v1 = inicio + (fim - inicio + 1) / 4;
    int v2 = inicio + (fim - inicio + 1) / 2;
    int v3 = inicio + 3 * (fim - inicio + 1) / 4;

    if (vetor[v1] > vetor[v2]  && vetor[v1] < vetor[v3] || vetor[v1] < vetor[v2] && vetor[v1] > vetor[v3]){
       ordem(&vetor[inicio], &vetor[v1]);
    } else if(vetor[v2] > vetor[v1]  && vetor[v2] < vetor[v3] || vetor[v2] < vetor[v1] && vetor[v2] > vetor[v3]){
        ordem(&vetor[inicio], &vetor[v2]);
    } else {
        ordem(&vetor[inicio], &vetor[v3]);
    };
        return HP(vetor, inicio, fim);
    };


int HA(int* vetor, int inicio, int fim){
    int aleatorio = inicio + (abs(vetor[inicio]) % (fim - inicio + 1));
    ordem(&vetor[inicio], &vetor[aleatorio]);
    return HP(vetor, inicio, fim);
};


//definição do quicksort 
void quickSort(int* vetor, int inicio, int fim, string tipo){
    cont ++;
    //seleção de ordenação
    int pivo;
    if (inicio<fim){
        if(tipo == "LP"){ 
            pivo = LP(vetor, inicio, fim);
            cont++;
            quickSort(vetor, inicio, pivo-1, tipo);
            quickSort(vetor, pivo+1, fim, tipo);
        } else if(tipo == "LM"){
            pivo = LM(vetor, inicio, fim);
            quickSort(vetor, inicio, pivo-1, tipo);
            quickSort(vetor, pivo+1, fim, tipo);   
        } else if(tipo == "LA"){
            pivo = LA(vetor, inicio, fim);
            quickSort(vetor, inicio, pivo-1, tipo);
            quickSort(vetor, pivo+1, fim, tipo);
        } else if(tipo == "HP"){
            pivo = HP(vetor, inicio, fim);
            quickSort(vetor, inicio, pivo, tipo);
            quickSort(vetor, pivo + 1, fim, tipo);
        } else if(tipo == "HM"){
            pivo = HM(vetor, inicio, fim);
            quickSort(vetor, inicio, pivo, tipo);
            quickSort(vetor, pivo + 1, fim, tipo);
        }else if(tipo == "HA"){
            pivo = HA(vetor, inicio, fim);
            quickSort(vetor, inicio, pivo, tipo);
            quickSort(vetor, pivo+1, fim, tipo);
        };
    };
};


void ordemQuick(Quick* a, Quick* b) {
    Quick temp = *a;
    *a = *b;
    *b = temp;
};

int PPQuick(Quick* quicks, int inicio , int fim){
    int pivo = quicks[fim].contador;
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        if (quicks[j].contador < quicks[fim].contador) {
            i++;
            ordemQuick(&quicks[i], &quicks[j]);
        }
    }
    ordemQuick(&quicks[i + 1], &quicks[fim]);
    return i + 1;
};

void quickSortQuick(Quick* quicks, int inicio, int fim) {
    if (inicio < fim) {
        int pivo = PPQuick(quicks, inicio, fim);
        quickSortQuick(quicks, inicio, pivo - 1);
        quickSortQuick(quicks, pivo + 1, fim);
    }
}


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

    int numVetores;

    arquivoIn >> numVetores;
    cout << numVetores << endl;
    Vetores* leituraVetores = new Vetores[numVetores];
    Quick* leituraQuicks = new Quick[7]; //eu vou sobrepor os valores de quicks

    for (int i = 0; i < numVetores; i++){

        //definição dos tipos de ordenação para voltar ao padrão em cada iteração
        leituraQuicks[1].tipo = "LP"; 
        leituraQuicks[2].tipo = "LM";
        leituraQuicks[3].tipo = "LA"; 
        leituraQuicks[4].tipo = "HP"; 
        leituraQuicks[5].tipo = "HM";  
        leituraQuicks[6].tipo = "HA";  

        arquivoIn >> leituraVetores[i].tamanho;
        leituraVetores[i].vetor = new int[leituraVetores[i].tamanho];


        for (int j = 0; j < leituraVetores[i].tamanho; j++){    
            
                arquivoIn >> leituraVetores[i].vetor[j];
                cout << leituraVetores[i].vetor[j] << " ";
            };

        leituraQuicks[0].contador = leituraVetores[i].tamanho; 



        cont = 0;
        quickSort(leituraVetores[i].vetor, 0, leituraVetores[i].tamanho - 1, "LP");  
        leituraQuicks[1].contador = cont;

        cont = 0; 
        quickSort(leituraVetores[i].vetor, 0, leituraVetores[i].tamanho - 1, "LM");  
        leituraQuicks[2].contador = cont;

        cont = 0; 
        quickSort(leituraVetores[i].vetor, 0, leituraVetores[i].tamanho - 1, "LA");  
        leituraQuicks[3].contador = cont;

        cont = 0;  
        quickSort(leituraVetores[i].vetor, 0, leituraVetores[i].tamanho - 1, "HP");
        leituraQuicks[4].contador = cont;  

        cont = 0; 
        quickSort(leituraVetores[i].vetor, 0, leituraVetores[i].tamanho - 1, "HM");  
        leituraQuicks[5].contador = cont;

        cont = 0; 
        quickSort(leituraVetores[i].vetor, 0, leituraVetores[i].tamanho - 1, "HA");  
        leituraQuicks[6].contador = cont;

        cont = 0; 
       // quickSortQuick(leituraQuicks, 1, 6);


        arquivoOut << i << ":N(" << leituraQuicks[0].contador << ")," 
        << leituraQuicks[1].tipo << "(" << leituraQuicks[1].contador << ")," 
        << leituraQuicks[2].tipo << "(" << leituraQuicks[2].contador << ")," 
        << leituraQuicks[3].tipo << "(" << leituraQuicks[3].contador << ")," 
        << leituraQuicks[4].tipo << "(" << leituraQuicks[4].contador << ")," 
        << leituraQuicks[5].tipo << "(" << leituraQuicks[5].contador << ")," 
        << leituraQuicks[6].tipo << "(" << leituraQuicks[6].contador << ")" << endl;

    };
    cout << "cheguei" << endl;

    arquivoIn.close();
    arquivoOut.close();
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tempo de execução: " << duration.count() << " microsegundos" << endl;
    
    return 0;
};

