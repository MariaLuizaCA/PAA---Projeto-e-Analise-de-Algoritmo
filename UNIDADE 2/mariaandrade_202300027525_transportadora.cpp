#include <iostream>  // Para std::cout, std::cerr
#include <fstream>   // Para std::ifstream
#include <sstream>   // Para std::istringstream
#include <string>
#include <chrono>
#include <cmath>
using namespace std::chrono;
using namespace std;


/*STRUCTS: 
    1. CAMINHÕES: armazena listas das placas, pesos e volumes
    2. PACOTES: armazena lista de códigos, valores, pesos e volumes
    3. CARREGAMENTO: *** TALVEZ *** criar esse registro para facilitar a escrita no arquivo de saída

    vão ser utilizados para armazenar os dados lidos do arquivo de entrada, caminhões só para serem lidos, 
    pacotes para formarem a matriz 3D, o registro de selecionados dessa matriz de busca dinâmica 
    e os pacotes em espera para serem carregados


    vou me preocupar com a leitura inicialmente e o retorno dos pacotes corretos depois o retorno das porcentagens 

    acho q o uma função possa me ajudar a retornar os pacotes corretos e fazer o processo da matriz ajudaria, 
    mas percorrer a nova lista de pacotes não deve melhorar a complexidade, além da lista de pacotes não carregados

*/ 

struct Caminhao{
    string placa;
    int peso;
    int volume;
};

struct Pacote{
    string codigo;
    int valor;
    int peso;
    int volume; 
    bool carregado = false;
};

/*
Criar duas funções para essa tabela, uma para retornar o melhor valor para cada pacote e 
outra para retornar os pacotes escolhidos e os que não foram escolhidos
*/

int tabelaMelhorValor(Caminhao &peso, Caminhao &volume, Pacote* pacotes,int numPac){//falta testar essa função

    int tabela[numPac][peso.peso][volume.volume];

    for (int i = 1; i < numPac; i++){
        for (int j = peso.peso; j >= pacotes[i].peso; j--){
            for (int k = volume.volume; k >= pacotes[i].volume; k--){
                if(j < pacotes[i].peso && k < pacotes[i].volume){
                    tabela[i][j][k] = tabela[i-1][j][k];
                }else{//caso eu pegue o pacote 
                    tabela[i][j][k] = max(tabela[i-1][j][k], tabela[i-1][j - pacotes[i].peso][k - pacotes[i].volume] + pacotes[i].valor);
                };    
                //caso eu pegue o pacote
            };
        };
    };

    return tabela[numPac][peso.peso][volume.volume];
};

/*
TALVEZ eu consiga alterar minhas comparações para que elas já calculem o peso e o volume total da carga
--- agora eu estou retirando o peso e o volume dos pacotes do valor total que o caminhaõ pode carregar, 
    mas eu posso mudar isso para fazer com q ele some os valores até ele atingir o valor max possivel e coloque os valores do carregamento no registro de carregamento 
--- se eu fizer isso minha outra função será cortada pela metade, mas como a função já faria isso eu conseguiria dar apenas um objetivo para ela e ela seria mais eficiente
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

    int numCam, numPac;

    //leitura do número de caminhões
    arquivoIn >> numCam;

    Caminhao* caminhoes = new Caminhao[numCam];

    for (int i = 0; i<numCam; i++){
        arquivoIn >> caminhoes[i].placa >> caminhoes[i].peso >> caminhoes[i].volume;
    };

    //leitura do número de pacotes 
    arquivoIn >> numPac;

    Pacote* pacotes = new Pacote[numPac];

    for(int i = 0; i < numPac; i++){
        arquivoIn >> pacotes[i].codigo >> pacotes[i].valor >> pacotes[i].peso >> pacotes[i].volume; 
    };

    //busca dos pacotes que podem ser carregados (busca dinâmica)
    for(int i = 0; i < numPac; i++){
        
    };





    arquivoIn.close();
    arquivoOut.close();
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tempo de execução: " << duration.count() << " microsegundos" << endl;
    
    return 0;
};

//.\mariaandrade_202300027525_transportadora.exe porto.input.txt porto.output.txt