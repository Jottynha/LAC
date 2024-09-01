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
