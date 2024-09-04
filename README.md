<h1 align="center" font-size="200em"><b>LAC | Trabalho Final de AEDS I</b></h1>

<div align="center">

![Static Badge](https://img.shields.io/badge/C%2B%2B-blue?style=flat&logo=C%2B%2B)
![Static Badge](https://img.shields.io/badge/Ubuntu-orange?style=flat&logo=ubuntu)

</div>



<p align="justify">
Esse é o repositório do trabalho final, proposto em laboratório, da disciplina de Algoritmos e Estruturas de Dados I, ministrada no Campus Divinópolis do CEFET-MG pelo professor Michel Pires Da Silva. Aqui, desenvolvemos um LAC com o objetivo de torná-lo o mais rápido possível, com as condições de funcionamento estabelecidas.
</p>


## Introdução:


#### Sobre algoritmos de classificação:
<p align="justify">
A seguinte explicação sobre algoritmos de classificação tem o objetivo de contextualizar o problema a ser lidado do código, com forte base e inspiração no resumo expandido da tese de VELOSO (2009).
</p>

<p align="justify">
Os algoritmos de classificação são métodos que aprendem padrões em dados em um treinamento e usam esse aprendizado para classificar outros dados em classes definidas. Dessa forma, esses são tipos de algoritmos muito relacionados com a área de ciência de dados e especialmente com <i>machine learning</i>, já que estamos ensinando a máquina a encontrar os padrões nos dados e classificar novos dados.
</p>

<p align="justify">
O principal problema dos algoritmos de classificação é que eles possuem um fator limitante: o grau de precisão do mapeamento. Ou o algoritmo consegue gerar uma função pouco precisa, ou gasta um tempo inaceitável de processamento para conseguir uma função um pouco mais precisa. Dentre as inúmeras estratégias, a classificação associativa é uma estratégia de classificação que explora relações entre as entradas e as saídas (no caso, as classes) [1].
</p>

#### Sobre o LAC:
<p align="justify">
No contexto da disciplina de AEDS I, o <i>Lazy Associative Classification</i> (LAC) é um algoritmo de classificação que ilustra bem o problema de classificação do uso de listas, filas, pilhas e tabelas hash. 
</p>
<p align="justify">
Também conhecido como algoritmo de classificação associativa sob demanda, o LAC se baseia no fato de que o problema da classificação pode ser dividido em sub-problemas mas simples, e eles podem ser resolvidos individualmente. Fazendo isso, o algoritmo é treinado de forma a se reduzir o número de regras que são geradas ao trabalhar com domínios menores. Resumindo brevemente, de acordo com o próprio enunciado da atividade, o LAC utiliza uma abordagem <i>"lazy"</i> de classificação, que é feita durante o momento da predição e não como algo que está incluído em um treinamento antecipado. Comparada com outros algoritmos de classificação, essa é uma abordagem bem mais eficiente.
</p>

#### Sobre o trabalho final:
<p align="justify">
O objetivo desse trabalho, como já dito anteriormente, foi o de criar uma implementação de LAC que seja correta e a mais eficiente possível, dentro dos conhecimentos vistos na disciplina de AEDS I e com as estratégias de otimização necessárias para que isso seja uma realidade.
</p>

<p align="justify">
O desafio desse trabalho, além de criar uma abordagem didática do LAC, foi de criar uma abordagem eficiente em custo computacional. Apesar de tudo, o LAC é um algoritmo de classificação que passa pelo problema da relação inversamente proporcional entre eficiência e acurácia. No geral, ou temos um algoritmo rápido com pouca acurácia, ou um algoritmo mais preciso que gasta um tempo inaceitável para isso.  
</p>

## Datasets utilizados:


### Poker Hands:
<p align="justify">
O Poker Hands [2] é um dataset disponibilizado pela UC Irvine Machine Leaning Repository. Seu objetivo é o de ser utilizado em algoritmos de aprendizagem de máquina para prever as mãos de um jogo de poker. Portanto, está diretamente relacionado com a tarefa de classificação. No total (em ambos os arquivos de treinamento e teste), possui 10 features e 1025010 instâncias. As features são categóricas e do tipo inteiro. 
</p>

<p align="justify">
Cada linha é um exemplo de uma mão de poker. Cada carta do baralho é explicitada com dois atributos: o naipe e valor. Com isso, cada mão no poker possui 5 cartas, e cada linha possui 10 atributos. Além disso, existe um atributo dedicado para a classe da mão do poker. 
</p>

<p align="justify">
Cambronero (2021) cita em <i>Poker Hand Dataset: A Machine Learning Analysis and a Practical Linear Transformation</i> [3] que o dataset Poker Hand é bastante desbalanceado, já que 90% das classes são 0 e 1. Isso é algo que complica as coisas, mas torna o dataset mais interessante para estudos de machine learning, já que os algoritmos de classificação costumam esperar a mesma (ou quase a mesma) quantidade de valores por classe.
</p>

<p align="justify">
O artigo de Cambronero também nos proporciona uma descrição detalhada dos valores do dataset do Poker Hands, como podemos ver a seguir:
</p>


<p align="center">
  
| Numero | Naipe   |
|--------|:-------:|
| 1      | Copas   |  
| 2      | Espadas |   
| 3      | Ouros   |
| 4      | Paus    |    
</p>


<p align="center">
  
| Numero | Valor   |
|--------|:-------:|
| 1      | Ás      |  
| 2 a 10 | Números |   
| 11     | Valete  |
| 12     | Dama    |
| 13     | Rei     |    
</p>


<p align="center">
  
| Numero | Classe         |
|--------|:--------------:|
| 0      | Carta alta     |  
| 1      | Um Par         |   
| 2      | Dois Pares     |
| 3      | Trinca         |
| 4      | Sequência      |
| 5      | Flush          |
| 6      | Full House     |
| 7      | Quadra         |
| 8      | Straight Flush |
| 9      | Royal Flush    | 
</p>


## Bibliotecas Utilizadas:

O código utiliza diversas bibliotecas padrão da linguagem C++:
- `<vector>`: Para armazenar sequências dinâmicas de elementos.
- `<tuple>`: Para criar tuplas com elementos de tipos distintos.
- `<string>`: Para manipulação de strings.
- `<unordered_map>`: Para criar tabelas hash eficientes.
- `<set>`: Para armazenar elementos únicos.
- `<fstream>`: Para manipulação de arquivos.
- `<sstream>`: Para operar em strings como fluxos de dados.
- `<iostream>`: Para operações de entrada e saída.
- `<chrono>`: Para medições de tempo.
- `<iomanip>`: Para formatação de saídas com precisão.
- `<map>`: Para armazenamento de pares chave-valor com ordenação.
- `<algorithm>`: Para operações em containers, como ordenação e busca.
- `<utility>`: Para utilitários como `std::pair`.
- `<numeric>`: Para operações numéricas, como geração de sequências.
- `<iterator>`: Para inserção e manipulação de iteradores.
- `<thread>`: Para suporte a programação multithread.
- `<mutex>`: Para sincronização e controle de acesso a recursos compartilhados.

  
## Sobre o código:

O arquivo principal `treinamento.hpp` contém a definição de funções e estruturas utilizadas para processar e analisar o dataset. Aqui está uma visão geral das principais funcionalidades:
<div style="background-color:#f0f0f0; padding:15px; border-radius:10px;">
  <h4>Funções Principais</h4>
  <ul>
    <li><strong>lerArquivo:</strong> Lê o dataset e armazena as características e classes em estruturas adequadas.</li>
    <li><strong>criarTabelaHash:</strong> Cria uma tabela hash para mapeamento das características.</li>
    <li><strong>criarTabelaHashClasses:</strong> Cria uma tabela hash para mapeamento das classes.</li>
    <li><strong>buscarFeature:</strong> Busca todas as linhas que possuem uma determinada característica.</li>
    <li><strong>buscarClasse:</strong> Retorna as linhas que pertencem a uma determinada classe.</li>
    <li><strong>treinamento:</strong> Integra as operações de leitura e criação das tabelas hash.</li>
  </ul>
</div>
<h4>Especialização do Template de Hash para Tuplas</h4>
No projeto, utilizamos `std::unordered_map` para mapear tuplas a conjuntos de inteiros. No entanto, as tuplas não possuem uma função de hash nativa. Para resolver isso, foi implementada uma especialização do template `std::hash` para `std::tuple`:

```cpp
namespace std {
    template <typename... Types>
    struct hash<std::tuple<Types...>> {
        size_t operator()(const std::tuple<Types...>& t) const {
            return hash_tuple(t, std::index_sequence_for<Types...>{});
        }
    private:
        template <std::size_t... I>
        size_t hash_tuple(const std::tuple<Types...>& t, std::index_sequence<I...>) const {
            size_t seed = 0;
            (..., (seed ^= hash<std::decay_t<decltype(std::get<I>(t))>>{}(std::get<I>(t)) + 0x9e3779b9 + (seed << 6) + (seed >> 2)));
            return seed;
        }
    };
}
```
<h4>Variáveis Globais</h4>
            <p>O projeto faz uso de variáveis globais para armazenar as tabelas hash e as tuplas lidas do dataset. Essas variáveis são essenciais para a organização dos dados e para a eficiência nas buscas e operações subsequentes:</p>
            <ul>
                <li><strong><code>tabelaHashTreino</code></strong>: <code>unordered_map&lt;tuple&lt;int, int&gt;, set&lt;int&gt;&gt;</code>
                    <ul>
                        <li>Mapeia características (representadas como tuplas de inteiros) para conjuntos de IDs de linhas no dataset.</li>
                    </ul>
                </li>
                <li><strong><code>tabelaHashClassesTreino</code></strong>: <code>unordered_map&lt;int, set&lt;int&gt;&gt;</code>
                    <ul>
                        <li>Mapeia classes (representadas como inteiros) para conjuntos de IDs de linhas correspondentes a cada classe.</li>
                    </ul>
                </li>
                <li><strong><code>tuplasTreino</code></strong>: <code>vector&lt;vector&lt;tuple&lt;int, int&gt;&gt;&gt;</code>
                    <ul>
                        <li>Armazena as tuplas lidas do arquivo de dados, onde cada tupla representa as características de uma linha.</li>
                    </ul>
                </li>
                <li><strong><code>totalLinhas</code></strong>: <code>int</code>
                    <ul>
                        <li>Armazena o número total de linhas lidas do dataset.</li>
                    </ul>
                </li>
            </ul>
            <h4>Função <code>lerArquivo</code></h4>
    <p>A função <code>lerArquivo</code> lê um arquivo CSV contendo dados de treino e armazena as tuplas e classes:</p> 
    
```cpp
vector<vector<tuple<int,int>>> lerArquivo(const string& nomeArquivo, vector<int>& classes) {
    ifstream arquivo(nomeArquivo);
    vector<vector<tuple<int, int>>> tuplas;
    string linha;
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return tuplas;
    }
    while (getline(arquivo, linha)) {
        vector<tuple<int, int>> linhaTuplas;
        stringstream ss(linha);
        string item;
        vector<int> linhaValores;
        while (getline(ss, item, ',')) {
            linhaValores.push_back(stoi(item));
        }
        // O último valor é a classe, então removemos ele da linha de valores
        if (!linhaValores.empty()) {
            classes.push_back(linhaValores.back());
            linhaValores.pop_back();        
        }
        // Adiciona tuplas contendo o índice e o valor
        for (size_t i = 0; i < linhaValores.size(); ++i) {
            linhaTuplas.push_back(make_tuple(i + 1, linhaValores[i]));
        }
        tuplas.push_back(linhaTuplas);
    }
    arquivo.close();
    return tuplas;
}
```
<h4>Função <code>criarTabelaHash</code></h4>
    <p>A função <code>criarTabelaHash</code> cria uma tabela hash a partir do vetor de tuplas:</p>
    
```cpp
unordered_map<tuple<int, int>, set<int>> criarTabelaHash(const vector<vector<tuple<int, int>>>& tuplas) {
    unordered_map<tuple<int, int>, set<int>> tabelaHash;
    for (size_t i = 0; i < tuplas.size(); ++i) {
        for (const auto& t : tuplas[i]) {
            tabelaHash[t].insert(i + 1);
        }
    }
    return tabelaHash;
}
```                              
   <h4>Função <code>criarTabelaHashClasses</code></h4>
    <p>A função <code>criarTabelaHashClasses</code> cria uma tabela hash para as classes:</p>
    
```cpp
unordered_map<int, set<int>> criarTabelaHashClasses(const vector<int>& classes) {
    unordered_map<int, set<int>> tabelaHashClasses;
    for (size_t i = 0; i < classes.size(); ++i) {
        tabelaHashClasses[classes[i]].insert(i + 1);
    }
    return tabelaHashClasses;
}
```
<h4>Função <code>buscarFeature</code></h4>
    <p>A função <code>buscarFeature</code> busca uma feature na tabela hash e retorna as linhas onde a chave aparece:</p>
    
```cpp
set<int> buscarFeature(const unordered_map<tuple<int, int>, set<int>>& tabelaHash, int coluna, int valor) {
    tuple<int, int> chave = make_tuple(coluna, valor);
    auto it = tabelaHash.find(chave);
    if (it != tabelaHash.end()) {
        return it->second; // Retorna o conjunto de linhas onde a chave aparece
    } else {
        return {}; // Retorna um conjunto vazio se a chave não for encontrada
    }
}
```
  <h4>Função <code>buscarClasse</code></h4>
    <p>A função <code>buscarClasse</code> busca uma classe na tabela hash e retorna as linhas onde a classe aparece:</p>
    
```cpp
set<int> buscarClasse(const unordered_map<int, set<int>>& tabelaHashClasses, int classe) {
    auto it = tabelaHashClasses.find(classe);
    if (it != tabelaHashClasses.end()) {
        return it->second; // Retorna o conjunto de linhas onde a classe aparece
    } else {
        return {}; // Retorna um conjunto vazio se a classe não for encontrada
    }
}
```
<h4>Função <code>treinamento</code></h4>
    <p>A função <code>treinamento</code> realiza o processo de leitura do arquivo, criação das tabelas hash e exibe as tabelas:</p>
    
```cpp
void treinamento(string nomeArquivo){
    vector<int> classes; // Vetor para armazenar as classes
    tuplasTreino = lerArquivo(nomeArquivo, classes);
    tabelaHashTreino = criarTabelaHash(tuplasTreino);
    tabelaHashClassesTreino = criarTabelaHashClasses(classes);
    totalLinhas = tuplasTreino.size();
    // Impressão da Tabela Hash de Linhas
    cout << "\nTabela Hash:" << endl;
    for (const auto& [chave, linhas] : tabelaHashTreino) {
        cout << "Chave: (Índice: " << get<0>(chave) << ", Valor: " << get<1>(chave) << ") - Aparece nas linhas: ";
        for (const auto& linha : linhas) {
            cout << linha << " ";
        }
        cout << endl;
    }
    // Impressão da Tabela Hash das Classes
    cout << "\nTabela Hash das Classes:" << endl;
    for (const auto& [classe, linhas] : tabelaHashClassesTreino) {
        cout << "Classe: " << classe << " Aparece nas linhas: ";
        for (const auto& linha : linhas) {
            cout << linha << " ";
        }
        cout << endl;
    }
}
```
O desenvolvimento inicial focou na criação dos arquivos `treinamento.cpp` e `treinamento.hpp`, que formam o núcleo do módulo de treinamento. O objetivo foi processar o dataset de treinamento de forma eficiente.

- **Leitura do Dataset**: O dataset foi processado linha por linha, armazenando informações em um vetor de tuplas. Cada tupla continha o índice e o valor das cartas.
  
- **Criação de Tabelas Hash**: Foram criadas duas tabelas hash globais:
  - Uma para armazenar as características das cartas (índice e valor).
  - Outra para armazenar as classes das mãos de poker.
    
- **Funções de Busca**: Implementadas para permitir buscas rápidas e eficientes com base nas tabelas hash.
  
- **Interface de Análise Interativa**: Permitida a realização de buscas específicas no dataset, facilitando a validação do algoritmo.
  
 <h4>Perspectivas de Evolução</h4>
 
Após a criação da base, o foco foi melhorar a análise do arquivo de teste e ajustar as estratégias de cálculo de suporte.

- **Desafios e Soluções**: Inicialmente, o algoritmo lidava com até 4 interseções, o que era caro computacionalmente. A restrição a 3 interseções ajudou a balancear precisão e eficiência.

- **Função de Avaliação Combinatória**: Avalia a classe combinatória com base em features e classes, utilizando intersecções e suporte.
  
<h2>Desenvolvimento do Arquivo de Teste</h2>
<p> Os arquivos <code>teste.cpp</code> e <code>teste.hpp</code> são essenciais para o processo de validação e avaliação do modelo de classificação. O arquivo <code>teste.cpp</code> implementa a lógica necessária para testar a precisão do modelo, analisando novas entradas e determinando suas classes com base no treinamento prévio. Ele realiza a leitura dos dados de teste, utiliza as tabelas hash para identificar a classe correspondente e avalia a performance do modelo. O arquivo <code>teste.hpp</code> declara as funções e estruturas de dados necessárias para essa operação, promovendo uma organização clara e eficiente do código, facilitando futuras modificações e expansões. </p>

<h4>Constantes <code>THRESHOLD_SIMILARIDADE</code> e <code>maxComb</code></h4> <p>A constante <code>THRESHOLD_SIMILARIDADE</code> define o valor mínimo de similaridade exigido para considerar duas combinações como semelhantes. Este valor é crucial para garantir que apenas combinações que superem esse limite sejam consideradas relevantes durante a análise. Um valor de <code>0.95</code> indica que uma correspondência deve ter pelo menos 95% de similaridade para ser aceita.</p> <p>Já a constante <code>maxComb</code> estabelece o número máximo de combinações que serão geradas ou analisadas. Esta restrição é importante para limitar a complexidade computacional, garantindo que o programa funcione de maneira eficiente ao lidar com grandes conjuntos de dados.</p>


```cpp
const double THRESHOLD_SIMILARIDADE = 0.95;
const int maxComb = 5;
```

<h4>Objetos <code>mutexArquivo</code> e <code>mutexContadores</code></h4> <p>Os objetos <code>mutexArquivo</code> e <code>mutexContadores</code> são instâncias da classe <code>mutex</code>, utilizados para sincronização em um ambiente multithread. Esses objetos garantem que diferentes threads possam acessar recursos compartilhados de forma segura, evitando condições de corrida.</p> <ul> <li><code>mutexArquivo</code>: Protege o acesso ao arquivo de saída, garantindo que apenas uma thread escreva nele por vez, evitando inconsistências nos dados gravados.</li> <li><code>mutexContadores</code>: Protege os contadores de acertos e erros, assegurando que as operações de incremento ou decremento sejam realizadas de forma atômica.</li></ul>

```cpp
mutex mutexArquivo;
mutex mutexContadores;  
```
<h4>Função <code>selecionarLinhasAleatorias</code></h4>
<p>A função <code>selecionarLinhasAleatorias</code> é responsável por selecionar um número especificado de linhas aleatórias de um arquivo de entrada e salvar essas linhas em um arquivo de saída. Ela lê todas as linhas do arquivo de entrada, armazena-as em um vetor, e, em seguida, escolhe aleatoriamente as linhas desejadas para gravar no arquivo de saída. Essa função é útil para amostrar dados de um arquivo maior, permitindo que você trabalhe com subconjuntos menores de maneira eficiente e sem necessidade de processamento manual.</p>

```cpp
void selecionarLinhasAleatorias(const string& inputFile, const string& outputFile, int numLinhas) {
    ifstream arquivoEntrada(inputFile);
    ofstream arquivoSaida(outputFile);
    vector<string> linhas;
    string linha;
    if (!arquivoEntrada.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada!" << endl;
        return;
    }
    while (getline(arquivoEntrada, linha)) {
        linhas.push_back(linha);
    }
    arquivoEntrada.close();
    // Inicializa a semente para o gerador de números aleatórios
    srand(time(nullptr));
    // Seleciona aleatoriamente as linhas e escreve no arquivo de saída
    for (int i = 0; i < numLinhas; ++i) {
        int indiceAleatorio = rand() % linhas.size();
        arquivoSaida << linhas[indiceAleatorio] << endl;
    }
    arquivoSaida.close();
}
```
<h4>Função <code>calcularSimilaridade</code></h4>
<p>A função <code>calcularSimilaridade</code> é utilizada para calcular a similaridade entre duas linhas de dados, ignorando as colunas que não são relevantes para a comparação. Especificamente, são ignoradas as colunas 0, 2, 4, 6, 8 e a última coluna, considerando apenas as colunas 1, 3, 5, e 7, que contêm os valores relevantes para a análise.</p>

```cpp
double calcularSimilaridade(const vector<int>& linha1, const vector<int>& linha2) {
    double similaridade = 0.0;
    int count = 0;
    // Itera sobre as colunas relevantes (1, 3, 5, 7)
    for (long unsigned int i = 1; i < linha1.size() - 1; i += 2) {
        // Considera apenas as colunas que representam valores (não os naipes)
        if (linha1[i] == linha2[i]) {
            similaridade += 1.0;
        }
        count++;
    }
    // Normaliza a similaridade pelo número de colunas consideradas
    if (count > 0) {
        similaridade /= count;
    }
    return similaridade;
}
```

<h4>Função <code>combinacoes</code></h4>
<p>A função <code>combinacoes</code> é responsável por gerar todas as combinações possíveis de um conjunto de índices, selecionando um número específico de elementos em cada combinação. Dado um vetor de índices e um valor <code>n</code>, a função retorna um vetor contendo todas as combinações de tamanho <code>n</code>, sem repetição, utilizando a técnica de permutação. Essa função é útil para analisar subconjuntos de dados, permitindo a exploração de diferentes combinações de características ou parâmetros.</p>

```cpp
vector<vector<int>> combinacoes(const vector<int>& indices, int n) {
    vector<vector<int>> result;
    vector<bool> v(indices.size());
    fill(v.begin(), v.begin() + n, true);
    do {
        vector<int> comb;
        for (long unsigned int i = 0; i < indices.size(); ++i) {
            if (v[i]) {
                comb.push_back(indices[i]);
            }
        }
        result.push_back(comb);
    } while (prev_permutation(v.begin(), v.end()));
    return result;
}
```

<h4>Função <code>calcularSuportePorCombinacaoThread</code></h4> <p>A função <code>calcularSuportePorCombinacaoThread</code> é responsável por calcular o suporte de classes com base em combinações de features, utilizando um processamento paralelo. Ela recebe um conjunto de linhas, as combinações de índices de features, um mapa de classes e seus conjuntos de linhas associados, e uma referência para o mapa de relevância de classes que será atualizado de forma segura entre threads. A variável <code>mtx</code> é uma instância da classe <code>std::mutex</code>, utilizada para gerenciar a sincronização entre threads no acesso a recursos compartilhados. No contexto desta função, <code>mtx</code> é usada para proteger a atualização da variável global <code>relevanciaClasse</code>, garantindo que apenas uma thread por vez possa modificar essa estrutura de dados. Isso evita condições de corrida e garante a integridade dos dados durante a execução paralela.</p> <p>A função realiza as seguintes operações:</p> <ul> <li>Inicializa uma variável local <code>relevanciaLocal</code> para armazenar o suporte calculado de cada classe em uma thread específica.</li> <li>Para cada combinação de features, a interseção de suas linhas é calculada.</li> <li>Para cada classe no mapa <code>tabelaHashClasses</code>, calcula-se a interseção entre as linhas resultantes da combinação e as linhas onde a classe aparece.</li> <li>Se houver suporte (ou seja, a interseção não for vazia), o suporte é calculado e adicionado à relevância local.</li> <li>Por fim, a função utiliza um <code>std::lock_guard</code> para garantir que a atualização da relevância global seja feita de maneira segura e atômica.</li> </ul>

```cpp
std::mutex mtx;
void calcularSuportePorCombinacaoThread(const vector<set<int>>& linhas,
                                        const vector<vector<int>>& combs,
                                        const unordered_map<int, set<int>>& tabelaHashClasses,
                                        unordered_map<int, double>& relevanciaClasse,
                                        int totalLinhas) {
    unordered_map<int, double> relevanciaLocal;
    for (const auto& comb : combs) {
        // Inicializa a interseção com o conjunto de linhas da primeira feature na combinação
        set<int> intersecao = linhas[comb[0]];
        // Itera sobre as outras features na combinação, calculando a interseção sucessiva
        for (size_t idx = 1; idx < comb.size(); ++idx) {
            set<int> tempIntersecao;
            set_intersection(intersecao.begin(), intersecao.end(),
                             linhas[comb[idx]].begin(), linhas[comb[idx]].end(),
                             inserter(tempIntersecao, tempIntersecao.begin()));
            intersecao = tempIntersecao;
        }
        // Itera sobre todas as classes no mapa `tabelaHashClasses`
        for (const auto& classe : tabelaHashClasses) {
            set<int> linhasClasse = classe.second; // Conjunto de linhas onde a classe aparece
            set<int> linhasIntersecao;
            // Calcula a interseção entre as linhas da combinação e as linhas da classe
            set_intersection(intersecao.begin(), intersecao.end(),
                             linhasClasse.begin(), linhasClasse.end(),
                             inserter(linhasIntersecao, linhasIntersecao.begin()));
            // Se a interseção não for vazia, significa que há suporte para essa classe
            if (!linhasIntersecao.empty()) {
                double suporte = static_cast<double>(linhasIntersecao.size()) / totalLinhas;
                relevanciaLocal[classe.first] += suporte;
            }
        }
    }
    // Lock para atualizar a relevância global
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& [classe, suporte] : relevanciaLocal) {
        relevanciaClasse[classe] += suporte;
    }
}
```
<h4>Função <code>avaliarClasseCombinatoria</code></h4> <p>A função <code>avaliarClasseCombinatoria</code> é responsável por avaliar a classe mais provável para um conjunto de features específicas em uma linha de dados, utilizando uma abordagem combinatória e paralela. A função calcula o suporte de diferentes classes com base nas combinações de features e determina a classe com maior relevância.</p> <p>A função realiza as seguintes operações:</p> <ul> <li>Inicializa o mapa <code>relevanciaClasse</code> para armazenar o suporte de cada classe.</li> <li>Coleciona as linhas correspondentes a cada feature presente em <code>featuresLinha</code>, utilizando a função <code>buscarFeature</code>.</li> <li>Gera todas as combinações possíveis de índices de features e distribui o trabalho de calcular o suporte entre múltiplas threads.</li> <li>Para cada combinação, a função <code>calcularSuportePorCombinacaoThread</code> é chamada em paralelo, contribuindo para o cálculo do suporte total de cada classe.</li> <li>As threads são sincronizadas utilizando a função <code>join</code> para garantir que todas as threads completem seu trabalho antes de prosseguir.</li> <li>Após o cálculo, as classes são ordenadas com base no suporte em ordem decrescente.</li> <li>Se a diferença percentual entre o suporte da classe 0 e a soma das outras classes for menor ou igual a -0.5, a função retorna 1, indicando que a classe 1 deve ser escolhida. Caso contrário, retorna a classe com maior suporte.</li> </ul>

```cpp
int avaliarClasseCombinatoria(const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                              const unordered_map<int, set<int>>& tabelaHashClasses,
                              const vector<tuple<int, int>>& featuresLinha, int totalLinhas) {
    unordered_map<int, double> relevanciaClasse;
    vector<set<int>> linhas;
    for (const auto& tupla : featuresLinha) {
        int coluna = get<0>(tupla);
        int valor = get<1>(tupla);
        set<int> linhasFeature = buscarFeature(tabelaHash, coluna, valor);
        if (!linhasFeature.empty()) {
            linhas.push_back(linhasFeature);
        }
    }
    int numLinhas = linhas.size();
    vector<int> indices(numLinhas);
    iota(indices.begin(), indices.end(), 0);
    vector<std::thread> threads;
    size_t numThreads = std::thread::hardware_concurrency(); // Obtem o número de threads suportadas pelo hardware
    for (int interacao = 1; interacao <= maxComb; interacao++) {
        auto combs = combinacoes(indices, interacao);
        size_t blockSize = combs.size() / numThreads;
        for (size_t i = 0; i < numThreads; ++i) {
            auto start = combs.begin() + i * blockSize;
            auto end = (i == numThreads - 1) ? combs.end() : start + blockSize;
            vector<vector<int>> combBlock(start, end);
            threads.emplace_back(calcularSuportePorCombinacaoThread, ref(linhas), combBlock, ref(tabelaHashClasses), ref(relevanciaClasse), totalLinhas);
        }
        for (auto& t : threads) {
            t.join(); // Aguarda todas as threads terminarem
        }
        threads.clear();
    }
    vector<pair<int, double>> suporteClasses(relevanciaClasse.begin(), relevanciaClasse.end());
    sort(suporteClasses.begin(), suporteClasses.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second > b.second;
    });
    double suporteClasse0 = 0.0;
    double somaOutrasClasses = 0.0;
    for (const auto& [classe, suporte] : suporteClasses) {
        if (classe == 0) {
            suporteClasse0 = suporte;
        } else {
            somaOutrasClasses += suporte;
            if (classe == 8) {
            }
        }
    }
    double diferencaPercentual = 0.0;
    if (somaOutrasClasses > 0) {
        diferencaPercentual = ((suporteClasse0 - somaOutrasClasses) / somaOutrasClasses) * 100;
    } else {
        diferencaPercentual = -1;
    }
    if (diferencaPercentual <= -0.5) {
        return 1;
    }
    if (!suporteClasses.empty()) {
        return suporteClasses.front().first;
    } else {
        return 0;
    }
}
```
<h4>Função <code>calcularSuporteBucket</code></h4> <p>A função <code>calcularSuporteBucket</code> é projetada para calcular o suporte médio de um bucket de linhas, determinando a classe mais provável com base em uma análise combinatória das features de cada linha dentro do bucket. Este cálculo é fundamental para avaliar a qualidade de agrupamentos gerados em abordagens como Locality-Sensitive Hashing (LSH).</p> <p>A função realiza as seguintes operações:</p> <ul> <li><strong>Inicialização:</strong> <code>somaClasses</code> é inicializado para acumular as classes atribuídas durante o processo de avaliação. O número de linhas a serem analisadas é limitado a 1 (ou o tamanho do bucket se for menor) para controlar o custo computacional.</li> <li><strong>Iteração sobre as linhas:</strong> Para cada linha no bucket (até o limite definido), a função converte as features da linha em uma lista de tuplas (<code>featuresLinha</code>) onde cada tupla representa um índice de coluna e o valor correspondente.</li> <li><strong>Avaliação da Classe:</strong> A função chama <code>avaliarClasseCombinatoria</code> para determinar a classe provável da linha, com base nas features e nas tabelas de hash que mapeiam features e classes às suas respectivas linhas.</li> <li><strong>Cálculo do Suporte:</strong> A classe retornada para cada linha é somada em <code>somaClasses</code>. Em seguida, calcula-se o suporte médio dividindo a soma das classes pelo número de linhas analisadas.</li> <li><strong>Arredondamento:</strong> O suporte médio é arredondado para o valor inteiro mais próximo, com um ajuste para garantir que os valores muito próximos de um inteiro superior sejam arredondados para cima.</li> </ul> <p>A função retorna o suporte médio calculado para o bucket analisado.</p>

```cpp
double calcularSuporteBucket(const vector<pair<vector<int>, int>>& bucket,
                             int totalLinhas,
                             const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                             const unordered_map<int, set<int>>& tabelaHashClasses) {
    double somaClasses = 0.0;
    size_t numLinhas = min(bucket.size(), size_t(1)); // Limita o número de linhas analisadas a 3;
    for (size_t j = 0; j < numLinhas; ++j) {
        const vector<int>& linha = bucket[j].first;
        // Calcula a classe usando a função avaliarClasseCombinatoria
        vector<tuple<int, int>> featuresLinha;
        for (size_t i = 0; i < linha.size(); ++i) {
            featuresLinha.push_back(make_tuple(i + 1, linha[i]));
        }
        int classe = avaliarClasseCombinatoria(tabelaHash, tabelaHashClasses, featuresLinha, totalLinhas);
        // Soma a classe atribuída
        somaClasses += classe;
    }
    // Calcula o suporte médio dividindo a soma das classes pelo número de linhas analisadas
    double suporteMedio = (numLinhas > 0) ? (somaClasses / numLinhas) : 0.0;
    // Arredondamento do suporte total
    if (suporteMedio - floor(suporteMedio) > 0.51) {
        suporteMedio = ceil(suporteMedio);
    } else {
        suporteMedio = floor(suporteMedio);
    }
    return suporteMedio;
}
```
<h4>Função <code>criarBucketsPorThread</code></h4> <p>A função <code>criarBucketsPorThread</code> é projetada para distribuir linhas em diferentes buckets com base em uma medida de similaridade. Cada bucket armazena um conjunto de pares (linha, classe) e um valor de suporte, que será calculado posteriormente. Esta função é ideal para ser executada em paralelo, com cada thread processando uma parte das linhas.</p> <p>A função realiza as seguintes operações:</p> <ul> <li><strong>Inicialização de Buckets:</strong> Um número fixo de buckets é criado com identificadores inteiros. Cada bucket é inicializado como um par contendo um vetor de pares (linha, classe) e um suporte inicializado a 0.0.</li>

  ```cpp
unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> criarBucketsPorThread(
    const vector<pair<vector<int>, int>>& linhas, 
    const set<int>& classesDistintas,
    int inicio, int fim) {
    classesDistintas.size();
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets;
    unordered_map<int, double> suportesExistentes;
    int numBuckets = linhas.size()/6 + 100;
    for (int i = 0; i < numBuckets; ++i) {
        buckets[i] = make_pair(vector<pair<vector<int>, int>>(), 0.0);
    }
    for (int i = inicio; i < fim; ++i) {
        const auto& linha = linhas[i];
        int melhorBucket = -1;
        double maiorSimilaridade = 0.0;
        for (auto& bucket : buckets) {
            size_t comparacoes = min(bucket.second.first.size(), static_cast<size_t>(10));
            for (size_t i = 0; i < comparacoes; ++i) {
                double similaridade = calcularSimilaridade(linha.first, bucket.second.first[i].first);
                if (similaridade > maiorSimilaridade) {
                    maiorSimilaridade = similaridade;
                    melhorBucket = bucket.first;
                }
            }
        }
        if (maiorSimilaridade > 2) {
            buckets[melhorBucket].first.push_back({linha.first, linha.second});
        } else {
            bool adicionado = false;
            for (auto& bucket : buckets) {
                if (bucket.second.first.empty()) {
                    bucket.second.first.push_back({linha.first, linha.second});
                    adicionado = true;
                    break;
                }
            }
            if (!adicionado) {
                double maiorMediaSimilaridade = 0.0;
                int melhorBucketParaAdicionar = -1;
                for (const auto& bucket : buckets) {
                    double somaSimilaridade = 0.0;
                    int count = 0;
                    size_t comparacoes = min(bucket.second.first.size(), static_cast<size_t>(10));
                    for (size_t i = 0; i < comparacoes; ++i) {
                        somaSimilaridade += calcularSimilaridade(linha.first, bucket.second.first[i].first);
                        count++;
                    }
                    double mediaSimilaridade = (count > 0) ? somaSimilaridade / count : 0.0;
                    if (mediaSimilaridade > maiorMediaSimilaridade) {
                        maiorMediaSimilaridade = mediaSimilaridade;
                        melhorBucketParaAdicionar = bucket.first;
                    }
                }
                if (melhorBucketParaAdicionar != -1) {
                    buckets[melhorBucketParaAdicionar].first.push_back({linha.first, linha.second});
                }
            }
        }
    }
    return buckets;
}
```
<h4>Função <code>criarBucketsComSimilaridade</code></h4> <p>A função <code>criarBucketsComSimilaridade</code> é responsável por processar um arquivo de teste contendo dados em formato de linhas e organizá-los em diferentes buckets com base na similaridade entre essas linhas. Cada bucket armazena um conjunto de pares (linha, classe) e um valor de suporte calculado posteriormente.</p> <p>A função realiza as seguintes operações:</p> <ul> <li><strong>Abertura do Arquivo:</strong> A função abre o arquivo de teste especificado pelo nome <code>nomeArquivoTeste</code> para leitura. Se o arquivo não puder ser aberto, a função exibe uma mensagem de erro e retorna um mapa vazio.</li> <li><strong>Leitura e Processamento das Linhas:</strong> O arquivo é lido linha por linha. Cada linha é dividida em valores inteiros que são armazenados em um vetor. O último valor de cada linha é considerado como a classe. As linhas são armazenadas em um vetor <code>linhas</code> e as classes distintas são adicionadas a um conjunto <code>classesDistintas</code>. O total de linhas lidas é registrado na variável <code>totalLinhas</code>.</li> <li><strong>Criação dos Buckets:</strong> A função chama <code>criarBucketsPorThread</code> para distribuir as linhas em buckets com base na similaridade. Essa distribuição é feita em paralelo, com cada thread processando uma parte das linhas. Os buckets são identificados por números inteiros e são inicializados com um vetor vazio e suporte inicial de 0.0.</li> <li><strong>Cálculo do Suporte:</strong> Para cada bucket criado, a função calcula o suporte usando a função <code>calcularSuporteBucket</code>. O suporte é armazenado no par associado ao bucket e também é registrado em um mapa <code>suportesExistentes</code>.</li> </ul> <p> Ao final, a função retorna um mapa <code>unordered_map</code> onde as chaves são os identificadores dos buckets e os valores são pares contendo o vetor de pares (linha, classe) e o suporte calculado.</p>

```cpp
unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> criarBucketsComSimilaridade(
    const string& nomeArquivoTeste, int& totalLinhas,
    const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
    const unordered_map<int, set<int>>& tabelaHashClasses) {
    ifstream arquivoTeste(nomeArquivoTeste);
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets;
    unordered_map<int, double> suportesExistentes;
    vector<pair<vector<int>, int>> linhas;
    set<int> classesDistintas;
    totalLinhas = 0;
    if (!arquivoTeste) {
        cerr << "Erro ao abrir o arquivo de teste." << endl;
        return {};
    }
    string linha;
    while (getline(arquivoTeste, linha)) {
        stringstream ss(linha);
        string item;
        vector<int> linhaValores;
        int classe;
        while (getline(ss, item, ',')) {
            linhaValores.push_back(stoi(item));
        }
        classe = linhaValores.back();
        classesDistintas.insert(classe);
        linhas.push_back({linhaValores, classe});
        totalLinhas++;
    }
    buckets = criarBucketsPorThread(linhas, classesDistintas, 0, totalLinhas);
    for (auto& bucket : buckets) {
        double suporte = calcularSuporteBucket(bucket.second.first, totalLinhas, tabelaHash, tabelaHashClasses);
        bucket.second.second = suporte;
        suportesExistentes[bucket.first] = suporte;
    }
    return buckets;
}
```
<h4>Função <code>avaliarClasse</code></h4> <p>A função <code>avaliarClasse</code> tem como objetivo determinar a classe associada a uma linha de dados, verificando se a linha está presente em algum dos buckets existentes. Cada bucket armazena um conjunto de pares (linha, classe) e um valor de suporte. A função retorna a classe associada ao bucket em que a linha foi encontrada.</p> <p>A função realiza as seguintes operações:</p> <ul> <li><strong>Verificação de Buckets:</strong> A função itera sobre cada bucket presente no mapa <code>buckets</code>. Cada bucket contém um vetor de pares (linha, classe) e um valor de suporte.</li> <li><strong>Comparação de Linhas:</strong> Para cada bucket, a função itera sobre as linhas armazenadas no bucket. Para cada linha do bucket, a função cria uma cópia e remove o último valor (que representa a classe). A linha é então comparada com a linha fornecida.</li> <li><strong>Retorno da Classe:</strong> Se a linha fornecida corresponder a uma das linhas no bucket, a função retorna a classe associada a esse bucket. Caso contrário, a função continua a verificação até que todos os buckets sejam processados.</li> <li><strong>Retorno Padrão:</strong> Se a linha não for encontrada em nenhum dos buckets, a função retorna 0, indicando que a classe não pôde ser determinada com base nos buckets.</li> </ul> <p>Essa função é útil para classificar linhas de dados com base em uma estrutura de buckets criada anteriormente, onde cada bucket pode conter múltiplas linhas associadas a uma classe específica.</p>

```cpp
int avaliarClasse(const vector<int>& linha, 
                  const unordered_map<int, pair<vector<pair<vector<int>, int>>, double>>& buckets) {
    // Verifica se a linha está em algum bucket
    for (const auto& bucket : buckets) {
        // Itera sobre as linhas no bucket
        for (const auto& linhaBucket : bucket.second.first) {
            // Cria uma cópia da linha do bucket
            vector<int> linhaBucketCopy = linhaBucket.first;
            linhaBucketCopy.pop_back();
            
            // Se a linha do bucket e a linha a ser comparada forem iguais
            if (linha == linhaBucketCopy) {
                // Linha está em um bucket, retorna a classe associada ao bucket
                return bucket.second.second;
            }
        }
    }
    return -1;
}
```
<h4>Função <code>processarLinhas</code></h4> <p>A função <code>processarLinhas</code> é responsável por processar um subconjunto de linhas de dados e avaliar a classe de cada linha com base em buckets predefinidos. Ela utiliza multithreading para permitir o processamento paralelo de linhas, o que pode melhorar o desempenho quando se trabalha com grandes conjuntos de dados. Além disso, a função atualiza contadores de acertos e erros e escreve os resultados em um arquivo de saída.</p> <p>A função realiza as seguintes operações:</p> <ul> <li><strong>Processamento de Linhas:</strong> A função itera sobre um subconjunto de linhas definido pelos índices <code>inicio</code> e <code>fim</code>. Cada linha é processada individualmente para determinar sua classe atribuída.</li> <li><strong>Leitura e Conversão:</strong> Para cada linha, a função cria um <code>stringstream</code> e lê os valores separados por vírgulas, armazenando-os em um vetor <code>linhaValores</code>. O último valor no vetor representa a classe original, que é removida antes de processar os dados.</li> <li><strong>Avaliação da Classe:</strong> A função chama <code>avaliarClasse</code> para determinar a classe atribuída à linha com base nos dados e buckets fornecidos. Essa avaliação é feita comparando a linha com as linhas armazenadas em cada bucket.</li> <li><strong>Escrita no Arquivo de Saída:</strong> A função utiliza um <code>mutex</code> para garantir acesso exclusivo ao arquivo de saída. O resultado da avaliação da linha é escrito no arquivo, incluindo o índice da linha e a classe atribuída.</li> <li><strong>Atualização de Contadores:</strong> Outro <code>mutex</code> é usado para garantir acesso exclusivo aos contadores de acertos e erros. Se a classe atribuída corresponder à classe original, o contador de acertos é incrementado; caso contrário, o contador de erros é incrementado.</li> </ul> <p>A função é projetada para operar em um ambiente multithreaded, com cada thread processando uma parte do conjunto de dados e contribuindo para a análise geral. O uso de mutexes garante que o acesso aos recursos compartilhados, como o arquivo de saída e os contadores, seja sincronizado corretamente para evitar condições de corrida.</p>

```cpp
void processarLinhas(int threadId, vector<string>& linhas, int inicio, int fim,
                     const unordered_map<int, pair<vector<pair<vector<int>, int>>, double>>& buckets, int& acertos, int& erros, ofstream& arquivoSaida) {
    // Loop que percorre um subconjunto de linhas de acordo com o intervalo [inicio, fim]
    for (int i = inicio; i < fim; ++i) {
        // Mensagem de depuração (não faz nada com threadId atualmente, mas pode ser usado para depuração)
        threadId = threadId;
        // Cria um stringstream a partir da linha do vetor de linhas
        stringstream ss(linhas[i]);
        string item;
        vector<int> linhaValores;
        // Lê os valores separados por vírgulas na linha e os armazena no vetor linhaValores
        while (getline(ss, item, ',')) {
            linhaValores.push_back(stoi(item));
        }
        // Verifica se o vetor linhaValores não está vazio
        if (!linhaValores.empty()) {
            // A última posição do vetor linhaValores contém a classe original
            int classeOriginal = linhaValores.back();
            linhaValores.pop_back(); // Remove a classe original dos valores para processar apenas os dados
            // Avalia a classe da linha com base nos dados e buckets fornecidos
            int classeAtribuida = avaliarClasse(linhaValores, buckets);
            // Bloqueia o mutex para garantir acesso exclusivo ao arquivo de saída
            {
                lock_guard<mutex> lock(mutexArquivo);
                // Escreve o resultado da linha processada no arquivo de saída
                arquivoSaida << "Linha " << (i + 1) << ", Classe = " << classeAtribuida << endl;
            }
            // Bloqueia o mutex para garantir acesso exclusivo aos contadores de acertos e erros
            {
                lock_guard<mutex> lock(mutexContadores);
                // Verifica se a classe atribuída corresponde à classe original
                if (classeAtribuida == classeOriginal) {
                    acertos++; // Incrementa o contador de acertos se a classe estiver correta
                } else {
                    erros++; // Incrementa o contador de erros se a classe estiver incorreta
                }
            }
        }
    }
}
```
<h4>Função <code>teste</code></h4> <p>A função <code>teste</code> é responsável por executar o processo completo de avaliação do modelo de classificação usando um conjunto de dados de teste. Ela inclui a leitura dos dados, a criação de buckets com base em similaridade, o processamento paralelo das linhas de teste e a geração de um relatório de desempenho. A função também mede o tempo de execução para avaliar a eficiência do processamento.</p> <p>A função realiza as seguintes operações:</p> <ul> <li><strong>Leitura do Arquivo de Teste:</strong> A função começa abrindo o arquivo de teste especificado pelo parâmetro <code>nomeArquivoTeste</code>. Se a abertura do arquivo falhar, uma mensagem de erro é exibida e a função retorna.</li> <li><strong>Medida do Tempo de Execução:</strong> O tempo de início da execução é registrado usando <code>chrono::high_resolution_clock</code> para medir a duração total do processo.</li> <li><strong>Criação de Buckets:</strong> A função <code>criarBucketsComSimilaridade</code> é chamada para criar buckets com base na similaridade das linhas do arquivo de teste. A função também retorna o total de linhas processadas. Se o arquivo de teste não puder ser aberto, uma mensagem de erro é exibida e a função retorna.</li> <li><strong>Criação e Abertura do Arquivo de Saída:</strong> Um arquivo de saída chamado "dataset/output.txt" é aberto para gravar os resultados. Se a abertura do arquivo falhar, uma mensagem de erro é exibida e a função retorna.</li> <li><strong>Leitura das Linhas de Teste:</strong> Todas as linhas do arquivo de teste são lidas e armazenadas em um vetor de strings chamado <code>linhas</code>. O total de linhas lidas é contado.</li> <li><strong>Configuração de Threads:</strong> O número de threads é definido com base na capacidade do hardware. Se a consulta falhar, o número de threads é ajustado para pelo menos uma. O número de linhas por thread é calculado com base no total de linhas e no número de threads.</li> <li><strong>Criação e Execução de Threads:</strong> Threads são criadas para processar subconjuntos de linhas usando a função <code>processarLinhas</code>. Cada thread é responsável por um intervalo de linhas e atualiza os contadores de acertos e erros.</li> <li><strong>Aguarda a Conclusão das Threads:</strong> A função espera que todas as threads terminem sua execução usando <code>t.join()</code>.</li> <li><strong>Relatório de Desempenho:</strong> O percentual de acertos e erros é calculado e registrado no arquivo de saída. O percentual é formatado com duas casas decimais.</li> <li><strong>Fechamento dos Arquivos:</strong> Ambos os arquivos (teste e saída) são fechados após o processamento.</li> <li><strong>Medida do Tempo de Execução Final:</strong> O tempo total de execução é calculado e exibido no console.</li> </ul> <p>A função utiliza técnicas de paralelismo para otimizar o processamento de grandes volumes de dados e garantir uma avaliação eficiente do modelo de classificação.</p>

```cpp
void teste(const string& nomeArquivoTeste) {
    ifstream arquivoTeste(nomeArquivoTeste);
    auto inicio = chrono::high_resolution_clock::now();
    int totalLinhas;
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets = criarBucketsComSimilaridade(nomeArquivoTeste, totalLinhas, tabelaHashTreino, tabelaHashClassesTreino);
    if (!arquivoTeste) {
        cerr << "Erro ao abrir o arquivo de teste." << endl;
        return;
    }
    ofstream arquivoSaida("dataset/output.txt");
    if (!arquivoSaida) {
        cerr << "Erro ao abrir o arquivo de saída." << endl;
        return;
    }
    // Variáveis para contagem de acertos e erros
    int totalLinhasArquivo = 0;
    int acertos = 0;
    int erros = 0;
    // Lê todas as linhas do arquivo em um vetor
    vector<string> linhas;
    string linha;
    while (getline(arquivoTeste, linha)) {
        linhas.push_back(linha);
        totalLinhasArquivo++;
    }
    // Define o número de threads com base na capacidade do hardware
    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 1;  // Fallback para pelo menos uma thread se a consulta falhar
    }
    int linhasPorThread = totalLinhasArquivo / numThreads;
    vector<thread> threads;
    // Cria threads para processar subconjuntos de linhas
    for (int i = 0; i < numThreads; ++i) {
        int inicio = i * linhasPorThread;
        int fim = (i == numThreads - 1) ? totalLinhasArquivo : inicio + linhasPorThread;
        threads.push_back(thread(processarLinhas, i, ref(linhas), inicio, fim, ref(buckets),
                         ref(acertos), ref(erros), ref(arquivoSaida)));
    }
    // Aguarda todas as threads terminarem
    for (auto& t : threads) {
        t.join();
    }
    double porcentagemAcertos = (static_cast<double>(acertos) / totalLinhasArquivo) * 100.0;
    double porcentagemErros = (static_cast<double>(erros) / totalLinhasArquivo) * 100.0;
    arquivoSaida << "Total de acertos: " << acertos << " (" << fixed << setprecision(2) << porcentagemAcertos << "%)" << endl;
    arquivoSaida << "Total de erros: " << erros << " (" << fixed << setprecision(2) << porcentagemErros << "%)" << endl;
    arquivoTeste.close();
    arquivoSaida.close();
    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> duracao = fim - inicio;
    cout << "[ Tempo de execução: " << duracao.count() << " segundos ]" << endl;
}
```
<h4>Função <code>main</code></h4>
<p>A função <code>main</code> é o ponto de entrada principal do programa. Ela coordena a execução do fluxo de trabalho para treinamento e teste de um modelo de classificação de mãos de poker.</p<p>O programa realiza as seguintes etapas:</p><ul>
 <li><strong>Treinamento:</strong> A função <code>treinamento</code> é chamada com o caminho para o arquivo de dados de treinamento. Ela processa o conjunto de dados para criar e ajustar o modelo de classificação.</li>
 <li><strong>Teste:</strong> Após o treinamento, a função <code>teste</code> é chamada com o caminho para um arquivo de teste. Ela avalia o modelo treinado usando os dados do arquivo de teste e gera relatórios sobre o desempenho do modelo.</li>
 </ul>
 <p>Este arquivo configura o ambiente necessário para a execução do programa e garante que as funções de treinamento e teste sejam executadas sequencialmente.</p>
 
```cpp
#include <iostream>
#include "treinamento.hpp"
#include "teste.hpp"
using namespace std;
int main() {
    selecionarLinhasAleatorias("dataset/poker-hand-testing.data", "dataset/20linhas.txt", 20);
    treinamento("dataset/poker-hand-training.data");
    teste("dataset/20linhas.txt");
    return 0;
}
```

## Observações
As implementações realizadas no projeto foram fundamentais para aumentar a acurácia do modelo de classificação e reduzir o tempo de processamento. A criação de buckets com base em medidas de similaridade permitiu uma organização eficiente das linhas de dados, facilitando a localização de linhas similares e melhorando a precisão das previsões. A utilização de multithreading para processar subconjuntos de linhas em paralelo otimizou significativamente o desempenho, permitindo que o modelo fosse treinado e testado de maneira mais rápida. Além disso, a implementação de uma função de avaliação eficiente garantiu que as classificações fossem feitas com alta precisão, minimizando erros e melhorando a confiabilidade dos resultados. Essas estratégias não apenas contribuíram para uma maior acurácia, mas também reduziram o tempo total de execução, tornando o sistema mais eficiente e escalável.

### Custo Computacional das Intersecções

Durante o desenvolvimento do algoritmo, observou-se que o custo computacional das intersecções entre as características aumentava exponencialmente com o número de combinações consideradas. Testes mostraram que, à medida que o número de intersecções aumentava, o valor calculado para a classe de uma linha se aproximava cada vez mais do valor real. No entanto, o aumento no número de combinações também acarretava um aumento significativo no tempo de execução.
Para equilibrar a precisão e a eficiência, decidimos limitar o número máximo de combinações a 3. Esta abordagem proporcionou um bom comprometimento entre a precisão dos resultados e o custo computacional, permitindo uma análise eficiente sem comprometer a acurácia.

### Estudo da Acurácia

Para avaliar a precisão do algoritmo, foi criada uma função que seleciona linhas aleatórias do dataset `pokerhand.data` para estudo. Essa função é usada para verificar a acurácia do modelo com diferentes amostras de dados. Além disso, o dataset `iris.data` também foi utilizado para validar o algoritmo em um conjunto de dados diferente, proporcionando uma visão mais abrangente do desempenho do modelo.

### Arquivos para Análise

Para facilitar a análise e o entendimento dos resultados, foram criados arquivos dedicados:
- **Buckets**: Contém as classes de mãos de poker, cada uma associada aos conjuntos de valores de cartas que pertencem a essa classe. Este arquivo é útil para verificar como as linhas de dados são agrupadas em diferentes buckets.
- **Suporte**: Registra os valores de suporte calculados para cada classe dentro dos buckets. Este arquivo ajuda a identificar quais combinações de cartas são mais relevantes para cada mão de poker e permite ajustes finos no modelo.
Esses arquivos são essenciais para a análise detalhada dos resultados e para o ajuste contínuo do algoritmo, garantindo que o modelo final seja robusto e eficiente para a categorização de mãos de poker.

### Threads e Paralelismo

A utilização de threads no processamento de dados é uma técnica crucial para melhorar a eficiência e o desempenho dos programas, especialmente quando se lida com grandes volumes de dados ou tarefas complexas que podem ser paralelizadas. No contexto da implementação discutida, a divisão do trabalho em múltiplas threads trouxe melhorias significativas na velocidade de processamento.

O processamento de linhas de dados em paralelo permitiu que o trabalho fosse distribuído entre várias threads, cada uma responsável por um subconjunto específico de dados. Isso reduziu o tempo total necessário para processar o arquivo de teste, uma vez que múltiplas threads podem executar tarefas simultaneamente em diferentes núcleos do processador. A criação e gerenciamento de threads foram configurados de forma a utilizar a capacidade total do hardware disponível, garantindo que cada núcleo do processador fosse utilizado eficientemente. Como resultado, o tempo de execução foi significativamente reduzido, permitindo uma análise mais rápida e eficiente das mãos de poker.

Além disso, a utilização de threads ajudou a manter a responsividade e a eficiência geral do sistema. Em vez de realizar todas as operações sequencialmente, o processamento paralelo permitiu que o sistema gerenciasse melhor a carga de trabalho, minimizando o tempo ocioso e maximizando a utilização dos recursos computacionais. Isso é particularmente importante em aplicações que requerem análise em tempo real ou onde grandes volumes de dados precisam ser processados de forma rápida.
Entretanto, é importante notar que o uso de threads também trouxe desafios adicionais, como a necessidade de sincronização entre threads para evitar condições de corrida e garantir a integridade dos dados. No caso da implementação discutida, mecanismos de sincronização, como mutexes, foram usados para proteger o acesso a recursos compartilhados, como arquivos de saída e contadores de acertos e erros, garantindo que os resultados fossem precisos e consistentes.

### Utilização de LSH e Buckets

No contexto do projeto de classificação de mãos de poker, a aplicação direta de técnicas padrão de Locality-Sensitive Hashing (LSH) e buckets revelou-se insatisfatória devido às particularidades dos dados e à complexidade do problema. O LSH e as abordagens tradicionais de buckets, que funcionam bem em cenários gerais, falharam em capturar de forma eficaz as nuances e a variabilidade das mãos de poker. Essas técnicas geralmente utilizam funções hash para distribuir dados em buckets com base em similaridades pré-definidas, mas, no caso das mãos de poker, a diversidade das combinações e a necessidade de precisão na classificação revelaram limitações significativas.

Diante desses desafios, foi necessário desenvolver uma lógica própria para os buckets. A nova abordagem personalizada envolveu a análise mais detalhada das similaridades entre as mãos de poker, ajustando os critérios de distribuição e introduzindo métricas específicas para avaliar a qualidade das correspondências. Essa lógica própria permitiu uma organização mais eficaz das linhas de dados, considerando as características únicas do problema e melhorando a acurácia das classificações. Com essa adaptação, foi possível superar as limitações das técnicas padrão, alcançando um desempenho mais robusto e preciso para a classificação das mãos de poker.

Os buckets, como utilizados em técnicas de hashing e classificação, são estruturas projetadas para agrupar dados com base em critérios de similaridade ou características compartilhadas. Em sistemas de hash, cada item é atribuído a um bucket com base em uma função de hash, garantindo que itens semelhantes acabem no mesmo bucket. O processo começa com a criação e inicialização de um número fixo de buckets, onde cada bucket é identificado por um índice único e começa vazio. Dados são então distribuídos entre esses buckets com base em uma função de hash ou critério de similaridade, reduzindo o número de comparações necessárias durante a busca e análise, pois só é necessário verificar os itens no bucket relevante.

No contexto da classificação de mãos de poker, as técnicas padrão de Locality-Sensitive Hashing (LSH) e hashing revelaram-se ineficazes. Isso se deve à alta variabilidade e complexidade das mãos de poker, que fazem com que as funções de hash padrão não capturem adequadamente as nuances das combinações e padrões das mãos. A distribuição de dados baseada em funções de hash genéricas não conseguiu refletir corretamente as semelhanças entre as mãos de poker, resultando em uma classificação menos precisa.

Para superar essas limitações, foi desenvolvida uma lógica personalizada para a implementação dos buckets. Em vez de usar funções de hash genéricas, foram criados critérios específicos de distribuição que consideram as características detalhadas das mãos de poker. A análise mais aprofundada das combinações e padrões das mãos permitiu ajustar a medida de similaridade de forma a refletir melhor as nuances do jogo. Isso envolveu a criação de critérios personalizados que garantiram que itens semelhantes fossem agrupados de maneira mais eficaz, melhorando significativamente a precisão da classificação e reduzindo o tempo de processamento. As modificações feitas nos buckets tornaram a organização dos dados mais eficaz, superando as limitações das abordagens tradicionais e resultando em uma acurácia muito maior na classificação das mãos de poker.

A implementação final incorporou técnicas avançadas semelhantes a Locality-Sensitive Hashing (LSH) para otimizar a classificação das mãos de poker.

- **Otimização com LSH**: Buckets foram criados para reduzir o tempo de execução e melhorar a precisão do algoritmo. O uso de mutex garantiu a integridade das variáveis durante a execução em múltiplas threads.

- **Arquivos de Buckets e Suporte**: Arquivos dedicados foram criados para armazenar buckets e valores de suporte, permitindo uma análise detalhada e ajustes finos no modelo de categorização.

### A Evolução do Algoritmo

O crescimento do algoritmo, desde a implementação inicial da lógica própria até o projeto final, reflete um processo contínuo de aprimoramento e adaptação para otimizar a eficiência e a precisão da solução. Inicialmente, a lógica própria foi crucial para superar as limitações dos métodos padrão, como o Locality-Sensitive Hashing (LSH), que não se mostraram eficazes para o problema específico do poker-hand. Os buckets padrão não conseguiram oferecer a precisão necessária e apresentaram um alto tempo de processamento, exigindo uma abordagem personalizada para organizar e comparar os dados de maneira mais eficiente.

A introdução de uma lógica própria para a criação e gerenciamento dos buckets permitiu adaptar a solução às características específicas dos dados. Essa personalização aprimorou a capacidade do sistema de identificar padrões relevantes e reduzir o processamento redundante, resultando em melhorias significativas na precisão das classificações e na eficiência geral do sistema. A lógica personalizada ajustada ao problema específico foi um passo fundamental para resolver as deficiências encontradas nas abordagens anteriores.

À medida que o projeto avançou para sua fase final, a incorporação de técnicas adicionais, como o uso de threads para processamento paralelo, trouxe avanços significativos. A paralelização permitiu dividir o trabalho entre múltiplas threads, reduzindo o tempo total de execução e aumentando a escalabilidade do sistema. Esses aprimoramentos não apenas melhoraram a eficiência, mas também garantiram que o sistema pudesse lidar com grandes volumes de dados de forma mais rápida e eficaz.

O crescimento contínuo do algoritmo evidenciou a importância de uma abordagem adaptativa no desenvolvimento de soluções de software. A capacidade de ajustar e refinar o algoritmo ao longo do tempo foi essencial para superar as limitações iniciais, melhorar a precisão das previsões e otimizar o desempenho geral. Esse processo de evolução e aprimoramento não apenas assegurou uma solução eficaz para o problema, mas também preparou o sistema para enfrentar desafios futuros e atender a requisitos mais complexos.



## Conclusão

O algoritmo evoluiu de uma implementação inicial básica para uma solução avançada e otimizada para análise de mãos de poker. O uso de técnicas como LSH melhorou significativamente a performance, permitindo uma análise precisa e eficiente de grandes volumes de dados. Além disso, por meio dos diversos desafios enfrentados na tentativa de otimizar o LAC, foi possível desenvolver noções de otimização, principalmente a forma como a relação entre a acurácia e o tempo de execução afetam o desenvolvimento de um programa dentro dos padrões esperados.

Dessa forma, baseado em todo o trabalho desenvolvido nestre projeto, a aplicação do algoritmo LAC em um contexto acadêmico, usando C++ como linguagem de implementação, a combinação de estruturas eficientes e, também, técnicas de otimização de tempo e intersecções de features resultou em um estudo aprofundado e completo, servindo como um exemplo de como conceitos fundamentais de estruturas de dados e algoritmos podem ser aplicados a problemas reais de machine learning, destacando a importância de uma boa compreensão teórica para a implementação prática.

---

Esse README oferece uma visão geral da evolução do algoritmo, as bibliotecas utilizadas e o código relevante, proporcionando uma base clara para entender e utilizar o projeto.
## Referências Bibliográficas:
[1] Veloso, A. A. (2009). **Classificação associativa sob demanda**. 
http://hdl.handle.net/1843/SLSS-7WFMGG
[2] Cattral,Robert and Oppacher,Franz. (2007). **Poker Hand**. UCI Machine Learning Repository. https://doi.org/10.24432/C5KW38.

[3] Cambronero, Walinton. (2021) **Poker Hand Dataset: A Machine Learning Analysis and a Practical Linear Transformation**. https://walintonc.github.io/papers/ml_pokerhand.pdf


## Compilação e Execução

 Especificações da máquina em que o código foi rodado:
  * Processador Intel Core i5, 10th Gen;
  * Sistema Operacional Windows 11 Home;
  * Terminal do WSL: Ubuntu 22.04.5;
  * 16GB de RAM.
* | Comando                |  Função                                                                                           |                     
  | -----------------------| ------------------------------------------------------------------------------------------------- |
  |  `make clean`          | Apaga a última compilação realizada contida na pasta build.                                       |
  |  `make`                | Executa a compilação do programa utilizando o g++, e o resultado vai para a pasta build.          |
  |  `make run`            | Executa o programa da pasta build após a realização da compilação.                                |

---
Esse README oferece uma visão geral da evolução do algoritmo, as bibliotecas utilizadas e o código relevante, proporcionando uma base clara para entender e utilizar o projeto.
## Referências Bibliográficas:
[1] Veloso, A. A. (2009). **Classificação associativa sob demanda**. 
http://hdl.handle.net/1843/SLSS-7WFMGG
[2] Cattral,Robert and Oppacher,Franz. (2007). **Poker Hand**. UCI Machine Learning Repository. https://doi.org/10.24432/C5KW38.
[3] Cambronero, Walinton. (2021) **Poker Hand Dataset: A Machine Learning Analysis and a Practical Linear Transformation**. https://walintonc.github.io/papers/ml_pokerhand.pdf
## Integrantes:
**João Pedro Rodrigues Silva**  
[![Static Badge](https://img.shields.io/badge/%7C%20Jottynha-black?style=flat-square&logo=github)](https://github.com/Jottynha)
[![Static Badge](https://img.shields.io/badge/%7C%20jprs1308%40gmail.com%20-black?style=flat-square&logo=gmail)](mailto:jprs1308@gmail.com)
**Jader Oliveira Silva**  
[![Static Badge](https://img.shields.io/badge/%7C%200livas-black?style=flat-square&logo=github)](https://github.com/0livas)
[![Static Badge](https://img.shields.io/badge/%7C%20jaderoliveira28%40gmail.com%20%20-black?style=flat-square&logo=gmail)](mailto:jaderoliveira28@gmail.com)
**Pedro Augusto Moura**  
[![Static Badge](https://img.shields.io/badge/%7C%20PedroAugusto08-black?style=flat-square&logo=github)](https://github.com/PedroAugusto08)
[![Static Badge](https://img.shields.io/badge/%7C%20pedroaugustomoura70927%40gmail.com-black?style=flat-square&logo=gmail)](mailto:pedroaugustomoura70927@gmail.com)
**Henrique de Freitas Araujo**  
[![Static Badge](https://img.shields.io/badge/%7C%20Ak4ai-black?style=flat-square&logo=github)
](https://github.com/Ak4ai)
[![Static Badge](https://img.shields.io/badge/%7C%20ricosgames.henrique%40gmail.com%20-black?style=flat-square&logo=gmail)](mailto:ricosgames.henrique@gmail.com )
**Rafael Adolfo Silva Ferreira**  
[![Static Badge](https://img.shields.io/badge/%7C%20Radsfer-black?style=flat-square&logo=github)](https://github.com/Radsfer)
[![Static Badge](https://img.shields.io/badge/%7C%20rafael.ferreira11.98%40gmail.com%20-black?style=flat-square&logo=gmail)](mailto:rafael.ferreira11.98@gmail.com )
**Samuel Silva Gomes**  
[![Static Badge](https://img.shields.io/badge/%7C%20samuelsilvg-black?style=flat-square&logo=github)](https://github.com/samuelsilvg)
[![Static Badge](https://img.shields.io/badge/%7C%20silvagomes881%40gmail.com%20-black?style=flat-square&logo=gmail)](mailto:silvagomes881@gmail.com)
___
<!---
✉️ jprs1308@gmail.com (**João Pedro Rodrigues Silva**)
✉️ jaderoliveira28@gmail.com (**Jader Oliveira Silva**)
✉️ pedroaugustomoura70927@gmail.com (**Pedro Augusto Moura**)
✉️ ricosgames.henrique@gmail.com (**Henrique de Freitas Araujo**)
✉️ rafael.ferreira11.98@gmail.com (**Rafael Adolfo Silva Ferreira**)  
✉️ silvagomes881@gmail.com (**Samuel Silva Gomes**)
---> 
