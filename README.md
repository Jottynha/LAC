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


## Bibliotecas Utilizadas

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

<h4>Utilização de LSH e Buckets</h4>

A implementação final incorporou técnicas avançadas como Locality-Sensitive Hashing (LSH) para otimizar a classificação das mãos de poker.

- **Otimização com LSH**: Buckets foram criados para reduzir o tempo de execução e melhorar a precisão do algoritmo. O uso de mutex garantiu a integridade das variáveis durante a execução em múltiplas threads.

- **Arquivos de Buckets e Suporte**: Arquivos dedicados foram criados para armazenar buckets e valores de suporte, permitindo uma análise detalhada e ajustes finos no modelo de categorização.

## Observações

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

## Conclusão

O algoritmo evoluiu de uma implementação inicial básica para uma solução avançada e otimizada para análise de mãos de poker. O uso de técnicas como LSH melhorou significativamente a performance, permitindo uma análise precisa e eficiente de grandes volumes de dados.

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
