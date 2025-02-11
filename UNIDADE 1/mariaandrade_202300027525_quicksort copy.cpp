#include <iostream>  // Para std::cout, std::cerr
#include <fstream>   // Para std::ifstream
#include <sstream>   // Para std::istringstream
#include <string>
#include <cmath>
#include <stdint.h>
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
    int* vetorLP;
    int* vetorLM;
    int* vetorLA;
    int* vetorHP;
    int* vetorHM;
    int* vetorHA;
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
    int pivo = vetor[fim], marcador = inicio - 1;
    for (int ponteiro = inicio; ponteiro < fim; ponteiro++ ){
        if(vetor[ponteiro] <= pivo){
            ordem(&vetor[++marcador], &vetor[ponteiro]);
        };
    };
    ordem(&vetor[++marcador], &vetor[fim]);
    return marcador;
};

int LM(int* vetor, int inicio, int fim){

    int v1 = inicio + (fim - inicio + 1) / 4;
    int v2 = inicio + (fim - inicio + 1) / 2;
    int v3 = inicio + 3 * (fim - inicio + 1) / 4;

    if (vetor[v1] >= vetor[v2]  && vetor[v1] <= vetor[v3] || vetor[v1] <= vetor[v2] && vetor[v1] >= vetor[v3]){
        return v1;
    } else if(vetor[v2] >= vetor[v1]  && vetor[v2] <= vetor[v3] || vetor[v2] <= vetor[v1] && vetor[v2] >= vetor[v3]){
        return v2;
    } else {
        return v3;
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

    while(true) {
        while(vetor[--ponteiro] > pivo);
        while(vetor[++marcador] < pivo);
        if (marcador < ponteiro) ordem(&vetor[marcador], &vetor[ponteiro]);
        else return ponteiro;
    }
};

int HM(int* vetor, int inicio, int fim){

    int v1 = inicio + (fim - inicio + 1) / 4;
    int v2 = inicio + (fim - inicio + 1) / 2;
    int v3 = inicio + 3 * (fim - inicio + 1) / 4;

    if (vetor[v1] >= vetor[v2]  && vetor[v1] <= vetor[v3] || vetor[v1] <= vetor[v2] && vetor[v1] >= vetor[v3]){
       return v1;
    } else if(vetor[v2] >= vetor[v1]  && vetor[v2] <= vetor[v3] || vetor[v2] <= vetor[v1] && vetor[v2] >= vetor[v3]){
       return v2;
    } else {
       return v3;
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
            quickSort(vetor, inicio, pivo-1, tipo);
            quickSort(vetor, pivo+1, fim, tipo);
        } else if(tipo == "LM"){
            int indice = LM(vetor, inicio, fim);
            ordem(&vetor[fim], &vetor[indice]);
            pivo = LP(vetor, inicio, fim);
            quickSort(vetor, inicio, pivo-1, tipo);
            quickSort(vetor, pivo+1, fim, tipo);
        } else if(tipo == "LA"){
            pivo = LA(vetor, inicio, fim);
            quickSort(vetor, inicio, pivo-1, tipo);
            quickSort(vetor, pivo+1, fim, tipo);
        } else if(tipo == "HP"){
            pivo = HP(vetor, inicio, fim);
            quickSort(vetor, inicio, pivo, tipo);
            quickSort(vetor, pivo+1, fim, tipo);
        } else if(tipo == "HM"){
            int indice = HM(vetor, inicio, fim);
            ordem(&vetor[inicio], &vetor[indice]);
            pivo = HP(vetor, inicio, fim);
            quickSort(vetor, inicio, pivo, tipo);
            quickSort(vetor, pivo+1, fim, tipo);
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

void merge(Quick registros[], int ini, int fim) {
    if (ini >= fim) return;

    int meio = ini + (fim - ini) / 2;
    merge(registros, ini, meio);
    merge(registros, meio + 1, fim);

    // Criar vetores auxiliares
    int n1 = meio - ini + 1;
    int n2 = fim - meio;
    Quick* esq = new Quick[n1];
    Quick* dir = new Quick[n2];

    for (int i = 0; i < n1; i++) {
        esq[i] = registros[ini + i];
    };
    for (int j = 0; j < n2; j++) {
        dir[j] = registros[meio + 1 + j]; 
    };

    // Mesclagem ordenada
    int i = 0, j = 0, k = ini;
    while (i < n1 && j < n2) {
        if (esq[i].contador < dir[j].contador) {
            registros[k++] = esq[i++];
        } else {
            registros[k++] = dir[j++];
        }
    }

    // Copiar os elementos restantes
    while (i < n1) {
        registros[k++] = esq[i++];
    };
    while (j < n2) {
        registros[k++] = dir[j++];
    };

    delete[] esq;
    delete[] dir;
}

int main (int argc, char* argv[]){
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
        leituraVetores[i].vetorLP = new int[leituraVetores[i].tamanho];
        leituraVetores[i].vetorLM = new int[leituraVetores[i].tamanho];
        leituraVetores[i].vetorLA = new int[leituraVetores[i].tamanho];
        leituraVetores[i].vetorHP = new int[leituraVetores[i].tamanho];
        leituraVetores[i].vetorHM = new int[leituraVetores[i].tamanho];
        leituraVetores[i].vetorHA = new int[leituraVetores[i].tamanho];


        for (int j = 0; j < leituraVetores[i].tamanho; j++){
            int num;
            arquivoIn >> num;
            leituraVetores[i].vetorLP[j] = num;
            leituraVetores[i].vetorLM[j] = num;
            leituraVetores[i].vetorLA[j] = num;
            leituraVetores[i].vetorHP[j] = num;
            leituraVetores[i].vetorHM[j] = num;
            leituraVetores[i].vetorHA[j] = num;
        };

        leituraQuicks[0].contador = leituraVetores[i].tamanho; 



        cont = 0;
        quickSort(leituraVetores[i].vetorLP, 0, leituraVetores[i].tamanho - 1, "LP");  
        leituraQuicks[1].contador = cont;
        
        cont = 0;
        quickSort(leituraVetores[i].vetorLM, 0, leituraVetores[i].tamanho - 1, "LM");  
        leituraQuicks[2].contador = cont;

        cont = 0; 
        quickSort(leituraVetores[i].vetorLA, 0, leituraVetores[i].tamanho - 1, "LA");  
        leituraQuicks[3].contador = cont;

        cont = 0;  
        quickSort(leituraVetores[i].vetorHP, 0, leituraVetores[i].tamanho - 1, "HP");
        leituraQuicks[4].contador = cont;  

        cont = 0; 
        quickSort(leituraVetores[i].vetorHM, 0, leituraVetores[i].tamanho - 1, "HM");  
        leituraQuicks[5].contador = cont;
        
        cont = 0; 
        quickSort(leituraVetores[i].vetorHA, 0, leituraVetores[i].tamanho - 1, "HA");  
        leituraQuicks[6].contador = cont;
        
        cont = 0; 
        merge(leituraQuicks, 1, 6);


        arquivoOut << i << ":N(" << leituraQuicks[0].contador << ")," 
        << leituraQuicks[1].tipo << "(" << leituraQuicks[1].contador << ")," 
        << leituraQuicks[2].tipo << "(" << leituraQuicks[2].contador << ")," 
        << leituraQuicks[3].tipo << "(" << leituraQuicks[3].contador << ")," 
        << leituraQuicks[4].tipo << "(" << leituraQuicks[4].contador << ")," 
        << leituraQuicks[5].tipo << "(" << leituraQuicks[5].contador << ")," 
        << leituraQuicks[6].tipo << "(" << leituraQuicks[6].contador << ")" << endl;

    };

    arquivoIn.close();
    arquivoOut.close();
    
    return 0;
};// .\mariaandrade_202300027525_quicksort.exe quicksort.input.txt quicksort.output.txt