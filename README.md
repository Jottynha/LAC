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
O código em treinamento.hpp faz parte de um projeto que utiliza o algoritmo LAC (Locality-Aware Clustering) para analisar o dataset Pokerhand. Abaixo, explico as principais funcionalidades e estruturas do código:
Estrutura da Tabela Hash

O código utiliza a função de hash personalizada para permitir que a unordered_map do C++ aceite tuplas como chaves. Isso é feito através da especialização do template da estrutura hash dentro do namespace std. A função hash_tuple é responsável por calcular o hash para cada elemento da tupla, gerando uma chave única.
Funções Principais

    lerArquivo: Esta função lê o arquivo de treino (nomeArquivo) e armazena os dados em um vetor de tuplas, onde cada tupla representa um par de valores (coluna, valor). As classes associadas a cada linha do dataset são armazenadas em um vetor separado.

    criarTabelaHash: A partir das tuplas lidas, esta função cria uma tabela hash (unordered_map<tuple<int, int>, set<int>>) que mapeia cada par (coluna, valor) para um conjunto de índices das linhas que contêm esse par.

    criarTabelaHashClasses: Similar à criarTabelaHash, essa função cria uma tabela hash que mapeia cada classe para um conjunto de índices das linhas que pertencem a essa classe.

    buscarFeature: Permite buscar todas as linhas que possuem uma determinada feature (representada por um par (coluna, valor)).

    buscarClasse: Retorna o conjunto de índices de todas as linhas que pertencem a uma determinada classe.

    treinamento: Função que integra as operações de leitura de dados e criação das tabelas hash, preparando o sistema para realizar as análises baseadas no algoritmo LAC.

Estrutura Global

O código também define variáveis globais para armazenar as tabelas hash e as tuplas:

    unordered_map<tuple<int, int>, set<int>> tabelaHashTreino; — Armazena a tabela hash das features.
    unordered_map<int, set<int>> tabelaHashClassesTreino; — Armazena a tabela hash das classes.
    vector<vector<tuple<int, int>>> tuplasTreino; — Armazena as tuplas do dataset.
    int totalLinhas; — Armazena o total de linhas no dataset.

Essas estruturas globais permitem que as funções interajam e manipulem os dados de forma eficiente durante o processo de treinamento.

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
