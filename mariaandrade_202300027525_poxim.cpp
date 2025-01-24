#include <iostream>  // Para std::cout, std::cerr
#include <fstream>   // Para std::ifstream
#include <sstream>   // Para std::istringstream
#include <string>
using namespace std;


struct Registro{
    string cadastro;
    string cnpj;
    int peso;
    int indice;
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

void imprimirArvore(Node* raiz) {
    if (raiz != nullptr) {
        imprimirArvore(raiz->esq);
        cout << "Cadastro: " << raiz->registro.cadastro << ", CNPJ: " << raiz->registro.cnpj << ", Peso: " << raiz->registro.peso << ", Indice: " << raiz->registro.indice << endl;
        imprimirArvore(raiz->dir);
    };
};

/*Função principal: 
1 - Leitura do arquivoIn e esrita do arquivoOut
2 - Criação da árvore binária para busca na lista de cadastrados dos selecionados
3 - Tratamento de dados com o retorno das buscas (diferença de cnpj e peso)
4 - Ordenação com o merge sort e escrita no arquivoOut*/ 
int main (int argc, char* argv[]){

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
    //MUDAR LEITURA DE ARQUIVO
    //leitura do número de conteiners cadastrados
    getline(arquivoIn, linha);
    istringstream num(linha);
    num >> numCads;

    cout << numCads <<endl;
    for (int i = 0; i < numCads; i++){

        getline(arquivoIn, linha);
        cout <<linha <<endl;//TIRAR DEPOIS -------------------------------
        istringstream dados(linha);
        Registro registroCadastrados; 
        getline(dados, registroCadastrados.cadastro, ' ');
        getline(dados, registroCadastrados.cnpj, ' ');
        getline(dados, linha,' ');
        istringstream peso(linha);
        peso >> registroCadastrados.peso;
        registroCadastrados.indice = i; 
        raiz = criarBinaryTree(raiz, registroCadastrados);
    };

    //leitura dos selecionados 

    getline(arquivoIn, linha);
    istringstream cont(linha);
    cont >> numSels;//TIRAR DEPOIS -------------------------------------------
    Registro* registroSelecionados = new Registro[numSels];

    cout << numSels <<endl;
    for (int i = 0; i < numSels; i++){
        getline(arquivoIn, linha);
        istringstream dadosMod(linha);
        getline(dadosMod, registroSelecionados[i].cadastro, ' ');
        getline(dadosMod, registroSelecionados[i].cnpj, ' ');
        getline(dadosMod, linha, ' ');
        istringstream peso(linha);
        peso >> registroSelecionados[i].peso;        
    };
    imprimirArvore(raiz);

    for (int i = 0; i < numSels, i++ ){
       Node registro = buscaBinaryTree(raiz, registroSelecionados->cadastro)[i]; 

    }; 

    return 0;
};

//ajustar as entradas com o tamanho do registro
void mergeSort( Registro* registros, int ini, int fim, int tamCads, int tamSele){
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

