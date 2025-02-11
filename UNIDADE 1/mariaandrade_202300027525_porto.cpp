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
    Node* esq = nullptr;
    Node* dir = nullptr;
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
    if(raiz == nullptr){
        return nullptr;
    }else if(raiz->registro.cadastro == chave || raiz == nullptr){
        return raiz;
    }else if(raiz->registro.cadastro < chave){
        return buscaBinaryTree(raiz->dir, chave);
    }else{
        return buscaBinaryTree(raiz->esq, chave);
    };
}; 

//ajustar as entradas com o tamanho do registro
void ordem(Registro registros[], int ini, int fim) {
    if (ini >= fim) return;

    int meio = ini + (fim - ini) / 2;
    ordem(registros, ini, meio);
    ordem(registros, meio + 1, fim);

    // Criar vetores auxiliares
    int n1 = meio - ini + 1;
    int n2 = fim - meio;
    Registro* esq = new Registro[n1];
    Registro* dir = new Registro[n2];

    for (int i = 0; i < n1; i++) {
        esq[i] = registros[ini + i];
    };
    for (int j = 0; j < n2; j++) {
        dir[j] = registros[meio + 1 + j]; 
    };

    // Mesclagem ordenada
    int i = 0, j = 0, k = ini;
    while (i < n1 && j < n2) {
        if (esq[i].porcentagem > dir[j].porcentagem || 
            (esq[i].porcentagem == dir[j].porcentagem && esq[i].indice < dir[j].indice)) {
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

/*DESENVOLVIMENTO INCOMPLETO DO PROCESSAMENTO DAS LISTAS 
    1 - prioridade a diferença de cnpj 
    2 - armazenar dados da ordem de cadastrados
    3 - passar os dados de indice, do cnpj diferente e o peso*/


    Registro* registroCnpj = new Registro[numSels];
    Registro* registroPeso =  new Registro[numSels];
    int cont1 = 0, cont2 = 0;
    Node* caminho = nullptr; 
    for (int i = 0; i < numCads; i++ ){
        caminho = buscaBinaryTree(primeiraRaiz, registroCadastrados[i].cadastro); 
        if(caminho == nullptr){
            continue;
        }else if (registroCadastrados[i].cnpj != caminho->registro.cnpj){
            registroCnpj[cont1].cadastro = registroCadastrados[i].cadastro;
            registroCnpj[cont1].cnpj = caminho->registro.cnpj;
            registroCnpj[cont1].cnpjErro = registroCadastrados[i].cnpj;
            registroCnpj[cont1].indice = caminho->registro.indice;
            cont1++;
       } else if (caminho->registro.peso!=registroCadastrados[i].peso) {
            float diferenca = abs(caminho->registro.peso - registroCadastrados[i].peso);
            float porc = round(diferenca*100/registroCadastrados[i].peso);
            if (porc>10){
                registroCadastrados[i].porcentagem = porc;
                registroCadastrados[i].diferenca = diferenca;
                registroPeso[cont2] = registroCadastrados[i];
                cont2++;
            };
       }; 
    }; 

    
    ordem(registroCnpj, 0, cont1 - 1);
    ordem(registroPeso, 0, cont2 - 1);

    for(int i = 0; i < cont1; i++){
        arquivoOut << registroCnpj[i].cadastro+":"+registroCnpj[i].cnpj+"<->"+registroCnpj[i].cnpjErro<<endl;
    };
    for(int i = 0; i < cont2; i++){
        arquivoOut << registroPeso[i].cadastro<<":"<<registroPeso[i].diferenca<<"kg("<<registroPeso[i].porcentagem<<"%)"<<endl;
    };

    arquivoIn.close();
    arquivoOut.close();
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tempo de execução: " << duration.count() << " microsegundos" << endl;
    
    return 0;
};

//.\mariaandrade_202300027525_porto.exe porto.input.txt porto.output.txt