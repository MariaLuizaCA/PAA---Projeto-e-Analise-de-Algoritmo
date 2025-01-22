#include <iostream>  // Para std::cout, std::cerr
#include <fstream>   // Para std::ifstream
#include <sstream>   // Para std::istringstream
#include <string>
using namespace std;


struct Registro{
    std :: string cadastro[15];
    std :: string cnpj[18];
    int peso;
    int indice;
};


void lerRegistros (const string& input){

    ifstream arquivo(input);

    if(!arquivo.is_open()){
        std::cerr << "Erro ao abrir o arquivo" <<std::endl; 
        return;
    }; 
    /*
    int numRegistros, numSelecionados;
    arquivo >> numRegistros;

    Registro* registrosCadastrados = new Registro[numRegistros];
    std :: string linha;
    
    for (int i = 0; i <= numRegistros; i++){
        std :: getline(arquivo, linha);
        std :: istringstream dados(linha);
        dados >> registrosCadastrados[i].cadastro;

    };*/


    int numRegistros, numSelecionados;
    arquivo >> numRegistros; 

    Registro* registrosCadastrados = new Registro[numRegistros];
    for (int i =0; i < numRegistros; i++) {
        arquivo >> registrosCadastrados[i].cadastro;
        arquivo >> registrosCadastrados[i].cnpj; 
        arquivo >> registrosCadastrados[i].peso;
        registrosCadastrados[i].indice = i; 
    };

    arquivo >> numSelecionados;
    Registro* registrosSelecionados = new Registro[numSelecionados];
    for (int i =0; i < numSelecionados; i++) {
        arquivo >> registrosSelecionados[i].cadastro;
        arquivo >> registrosSelecionados[i].cnpj;
        arquivo >> registrosSelecionados[i].peso;
    };
    return registrosCadastrados, registrosSelecionados, numRegistros, numSelecionados;
};

//ajustar as entradas com o tamanho do registro
void mergeSort( Registro registros, int ini, int fim, int tamCads, int tamSele){
    if (ini >= fim){
        return;
    };

    int pivo = ini + (fim - ini)/2;
    mergeSort(registros, ini, pivo, tamCads, tamSele);
    mergeSort(registros, pivo+1, fim, tamCads, tamSele); 

    int i = ini, j = pivo + 1;
    while (i <= pivo && j <= fim){
        if(registros[i].indice <= registros[j].indice){
                i++;
        }else{ 
            Registro* tem = registros[j].indice;
            int  k = j;
        
        while (k > i) {
            registros[k] = registros[k-1];
            k--;
        };
        registros[i] = tem;
        i++;
        j++;
        pivo++; 

        };
    };
};