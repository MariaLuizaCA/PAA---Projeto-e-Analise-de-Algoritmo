#include <iomanip>
#include <iostream>  // Para std::cout, std::cerr
#include <fstream>   // Para std::ifstream
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

//armazena as informaçõs dos caminhões
struct Caminhao{
    string placa;
    int peso;
    int volume;
};

//armazena as informações dos pacotes
struct Pacote{
    string codigo;
    double valor;
    int peso;
    int volume; 
    bool carregado; //indica se o pacote já foi escolhido
};

/*
Criar duas funções para essa tabela, uma para retornar o melhor valor para cada pacote e 
outra para retornar os pacotes escolhidos e os que não foram escolhidos
*/


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

    //leitura do número de caminhões
    int numCam;
    arquivoIn >> numCam;
    Caminhao* caminhoes = new Caminhao[numCam];
    for (int i = 0; i<numCam; i++){
        arquivoIn >> caminhoes[i].placa >> caminhoes[i].peso >> caminhoes[i].volume;
    };

    //leitura do número de pacotes 
    int numPac;
    arquivoIn >> numPac;
    Pacote* pacotes = new Pacote[numPac];
    for(int i = 0; i < numPac; i++){
        arquivoIn >> pacotes[i].codigo >> pacotes[i].valor >> pacotes[i].peso >> pacotes[i].volume; 
        pacotes[i].carregado = false;
    };

    //execução do algoritmo de programação dinâmica 2d em cada caminhão
    for(int i = 0; i < numCam; i++){
        int pesoMax = caminhoes[i].peso;
        int volumeMax = caminhoes[i].volume;
         
        //pacotes disponiveis para cada caminhão
        int contDisponiveis = 0;
        int* disponiveisIndice = new int[numPac];
        for (int j = 0; j < numPac; j++){
            if(!pacotes[j].carregado){
                disponiveisIndice[contDisponiveis++] = j;
            }
        }
        
        if (contDisponiveis == 0){
            arquivoOut << "[" << caminhoes[i].placa << "]R$0.00,0KG(0%),0L(0%)->" << "\n";
            delete [] disponiveisIndice;
            continue;
        }

        // aloca a tabela PD e a de busca inversa
        int pdTamanho = (pesoMax + 1) * (volumeMax + 1);
        double* pd = new double[pdTamanho];
        int* indice = new int[pdTamanho];

        //inicializa a tabela
        for (int j = 0; j < pdTamanho; j++){
            pd[j] = 0.0;
            indice[j] = -1;
        }

        //preenche a tabela
        for (int x = 0; x < contDisponiveis; x++){
            int indicePac = disponiveisIndice[x];
            int pesoPac = pacotes[indicePac].peso;
            int volumePac = pacotes[indicePac].volume;
            double valorPac = pacotes[indicePac].valor;
            //para economizar processamento iniciamo de trás para frente 
            for (int y = pesoMax; y >= pesoPac; y--){
                for (int z = volumeMax; z>= volumePac; z--){
                    int indAtual = y * (volumeMax + 1) + z;
                    int indAnterior = (y - pesoPac) * (volumeMax + 1) + (z - volumePac);
                    double valorAtual = pd[indAnterior] + valorPac;
                    if (valorAtual > pd[indAtual]){
                        pd[indAtual] = valorAtual;
                        indice[indAtual] = x;
                    }
                }
            }
        }

        //o melhor valor para o caminhão
        double melhorValor = pd[pesoMax * (volumeMax + 1) + volumeMax];

        //repassa a tabela para pegar as informações necessárias
        int atualPeso = pesoMax, atualVolume = volumeMax;
        int* pacotesEscolhidos = new int[contDisponiveis];
        int contEscolhidos = 0;
        while(atualPeso>=0 && atualVolume >=0){
            int guardar = indice[atualPeso * (volumeMax + 1) +atualVolume];
            if (guardar == -1) break;
            int indicePac = disponiveisIndice[guardar];
            pacotesEscolhidos[contEscolhidos++] = indicePac;
            //atualiza as capacidades tirando o pacolte escolhido
            atualPeso -= pacotes[indicePac].peso;
            atualVolume -= pacotes[indicePac].volume;
            
        }

        for (int k = 0; k < contEscolhidos; k++) {
            int indicePac = pacotesEscolhidos[k];
            pacotes[indicePac].carregado = true;  // Marca o pacote como carregado
        }

        //peso e volume final usados 
        int somaPeso = 0, somaVolume = 0;
        for (int i = 0; i < contEscolhidos; i++){
            int indicePac = pacotesEscolhidos[i];
            somaPeso += pacotes[indicePac].peso;
            somaVolume += pacotes[indicePac].volume;
        };

        //porcentagens
        int porcentagemPeso = (int)((somaPeso * 100) / caminhoes[i].peso) + 0.5;
        int porcentagemVolume = (int)((somaVolume * 100) / caminhoes[i].volume) + 0.5;


        //escreve os dados no arquivo

        arquivoOut << "[" << caminhoes[i].placa << "]"
        << "R$" <<fixed << setprecision(2)<< melhorValor << ", " 
        << somaPeso << "KG (" << porcentagemPeso << "%), " 
        << somaVolume << "L (" << porcentagemVolume << "%) -> ";
        for (int i = contEscolhidos - 1; i >=0; i--){
            arquivoOut << pacotes[pacotesEscolhidos[i]].codigo;
            if( i != 0){
                arquivoOut << ", ";
            }
        }
        arquivoOut << "\n";


        // libera as memórias alocadas para este caminhão
        delete [] pd;
        delete [] indice;
        delete [] pacotesEscolhidos;
        delete [] disponiveisIndice;
    };

    //os não selecionados
    double pacotesNaoSelecionados = 0.0;
    int naoSelecionadosPeso = 0, naoSelecionadosVolume = 0;
    for (int i = 0; i <numPac; i++){
        if (!pacotes[i].carregado){
            pacotesNaoSelecionados += pacotes[i].valor;
            naoSelecionadosPeso += pacotes[i].peso;
            naoSelecionadosVolume += pacotes[i].volume;
        }
    }

    arquivoOut << "PENDENTE: "
    << "R$" <<fixed << setprecision(2)<< pacotesNaoSelecionados << ", "
    << naoSelecionadosPeso << "KG, "
    << naoSelecionadosVolume << "L -> ";
    bool primeiro = true;
    for (int i = 0; i < numPac; i++){
        if (!pacotes[i].carregado){
            if (!primeiro)
                arquivoOut << ", ";
            arquivoOut << pacotes[i].codigo;
            primeiro = false;
        }
    }
    arquivoOut << "\n";

    delete[] caminhoes;
    delete[] pacotes;

    arquivoIn.close();
    arquivoOut.close();
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tempo de execução: " << duration.count() << " microsegundos" << endl;
    
    return 0;
};

