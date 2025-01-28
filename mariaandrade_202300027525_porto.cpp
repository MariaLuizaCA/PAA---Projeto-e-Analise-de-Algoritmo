#include <iostream>  // Para std::cout, std::cerr
#include <fstream>   // Para std::ifstream
#include <sstream>   // Para std::istringstream
#include <string>
#include <chrono>
#include <cmath>
using namespace std::chrono;
using namespace std;


struct Registro{
    string cadastro;
    string cnpj;
    string cnpjErro;
    float peso;
    int indice;
    float porcentagem;
    float diferenca;

};

struct Node{
    Registro registro;
    Node* esq;
    Node* dir;
    Node(Registro registro, Node* esq = nullptr, Node* dir = nullptr)
        : registro(registro), esq(esq), dir(dir) {}};


Node* criarBinaryTree(Node* raiz, Registro registro){

    if(raiz == nullptr){
        return new Node(registro);

    }else if(registro.cadastro < raiz->registro.cadastro){
        raiz->esq = criarBinaryTree(raiz->esq, registro);

    } else {
        raiz->dir = criarBinaryTree(raiz->dir, registro);
    }
    return raiz;
};


Node* binaryTreeRegistro(Registro* registro, int tamanho) {
    Node* raiz = nullptr;  // Inicializa a árvore como vazia
    for (int i = 0; i < tamanho; i++) {
        raiz = criarBinaryTree(raiz, *registro);  // Insere cada registro
    };
    return raiz;  // Retorna a raiz da árvore criada
};

Node* buscaBinaryTree(Node* raiz, const std::string& chave){
    if(raiz->registro.cadastro == chave || raiz == nullptr){
        return raiz;
    }else if(raiz->registro.cadastro < chave){
        return buscaBinaryTree(raiz->dir, chave);
    }else{
        return buscaBinaryTree(raiz->esq, chave);
    };
}; 

//ajustar as entradas com o tamanho do registro
void ordem( Registro registros[], int ini, int fim){
    if (ini >= fim){
        return;
    };

    int pivo = ini + (fim - ini)/2;
    ordem(registros, ini, pivo);
    ordem(registros, pivo+1, fim); 

    int i = ini, j = pivo + 1;
    while (i <= pivo && j <= fim){
        if(registros[i].indice <= registros[j].indice){
                i++;
        }else{ 
            Registro tem = registros[j];
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



/*Função principal: 
1 - Leitura do arquivoIn e esrita do arquivoOut
2 - Criação da árvore binária para busca na lista de cadastrados dos selecionados
3 - Tratamento de dados com o retorno das buscas (diferença de cnpj e peso)
4 - Ordenação com o merge sort e escrita no arquivoOut*/ 
int main (int argc, char* argv[]){


auto start = high_resolution_clock::now();
    //leitura e escrita de arquivo
    ifstream arquivoIn(argv[1]);
    ofstream arquivoOut(argv[2]);

    if(!arquivoIn.is_open()){
        std::cerr << "\n Erro ao abrir o arquivo" <<std::endl; 
        return 1;
    };

    int numCads, numSels;
    string linha, comp;
    Node* raiz = nullptr;
    Node* primeiraRaiz = nullptr;

    cout << "PASSO 1 "<<endl;

    //leitura do número de conteiners cadastrados
    getline(arquivoIn, linha);
    istringstream cont(linha);
    cont >> numCads;
    Registro* registroCadastrados = new Registro[numCads];

    for (int i = 0; i < numCads; i++){
        getline(arquivoIn, linha);
        istringstream dadosMod(linha);
        getline(dadosMod, registroCadastrados[i].cadastro, ' ');
        getline(dadosMod, registroCadastrados[i].cnpj, ' ');
        getline(dadosMod, linha, ' ');
        istringstream peso(linha);
        peso >> registroCadastrados[i].peso;    
        registroCadastrados[i].indice = i; 
    };
    
    cout << "PASSO 2 "<<endl;


    //leitura dos selecionados
    getline(arquivoIn, linha);
    istringstream num(linha);
    num >> numSels;

    for (int i = 0; i < numSels; i++){

        getline(arquivoIn, linha);
        istringstream dados(linha);
        Registro registroSelecionados; 
        getline(dados, registroSelecionados.cadastro, ' ');
        getline(dados, registroSelecionados.cnpj, ' ');
        getline(dados, linha,' ');
        istringstream peso(linha);
        peso >> registroSelecionados.peso;
        if (raiz == nullptr) {
            raiz = criarBinaryTree(nullptr, registroSelecionados);
            primeiraRaiz = raiz; // Armazena a primeira raiz
        } else {
            raiz = criarBinaryTree(raiz, registroSelecionados);
        };
    };

    cout << "PASSO 3 "<<endl;

/*DESENVOLVIMENTO INCOMPLETO DO PROCESSAMENTO DAS LISTAS 
    1 - prioridade a diferença de cnpj 
    2 - armazenar dados da ordem de cadastrados
    3 - passar os dados de indice, do cnpj diferente e o peso*/


    Registro* registroCnpj = new Registro[numSels];
    cout << "PROBLEMA 1"<<endl;
    Registro* registroPeso =  new Registro[numSels];
    cout << "PROBLEMA 2"<<endl;
    int cont1 = 0, cont2 = 0;
    Node* caminho = nullptr; 
    for (int i = 0; i < numSels; i++ ){
        cout << "PROBLEMA 3"<<endl;
        caminho = buscaBinaryTree(primeiraRaiz, registroCadastrados[i].cadastro); 
        cout << "PROBLEMA 4"<<endl;
        if (registroCadastrados[i].cnpj != caminho->registro.cnpj){
            cout << "PROBLEMA 5"<<endl;
            registroCnpj[cont1].cadastro = registroCadastrados[i].cadastro;
            cout << "PROBLEMA 6"<<endl;
            registroCnpj[cont1].cnpj = caminho->registro.cnpj;
            cout << "PROBLEMA 7"<<endl;
            registroCnpj[cont1].cnpjErro = registroCadastrados[i].cnpj;
            cout << "PROBLEMA 8"<<endl;
            registroCnpj[cont1].indice = caminho->registro.indice;
            cout << "PROBLEMA 8"<<endl;
            cont1++;
       } else if (caminho->registro.peso!=registroCadastrados[i].peso) {
        cout << "PROBLEMA 9"<<endl;
            float diferenca = abs(caminho->registro.peso - registroCadastrados[i].peso);
            cout << "PROBLEMA 10"<<endl;
            float porc = round(diferenca*100/registroCadastrados[i].peso);
            cout << "PROBLEMA 11"<<endl;
            if (porc>10){
                cout << "PROBLEMA 12"<<endl;
                registroCadastrados[i].porcentagem = porc;
                cout << "PROBLEMA 13"<<endl;
                registroCadastrados[i].diferenca = diferenca;
                cout << "PROBLEMA 14"<<endl;
                registroPeso[cont2] = registroCadastrados[i];
                cout << "PROBLEMA 15"<<endl;
                cont2++;
                cout << "PROBLEMA 16"<<endl;
            };
       }; 
    }; 

    cout << "PASSO 4 "<<endl;
    
    ordem(registroCnpj, 0, cont1 - 1);
    ordem(registroPeso, 0, cont2 - 1);

    cout << "PASSO 5 "<<endl;

    for(int i = 0; i < cont1; i++){
        arquivoOut << registroCnpj[i].cadastro+":"+registroCnpj[i].cnpj+"<->"+registroCnpj[i].cnpjErro<<endl;
    };
    for(int i = 0; i < cont2; i++){
        arquivoOut << registroPeso[i].cadastro<<":"<<registroPeso[i].diferenca<<"kg("<<registroPeso[i].porcentagem<<"%)"<<endl;
    };

    cout << "PASSO 6 "<<endl;
    arquivoIn.close();
    arquivoOut.close();
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tempo de execução: " << duration.count() << " microsegundos" << endl;
    
    return 0;
};

//.\mariaandrade_202300027525_porto.exe porto.input.txt porto.output.txt