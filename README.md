Árvore AVL (Genérica)
=====================

![Build](https://img.shields.io/gitlab/pipeline/rafaelescaleira/avl.svg)
![License](https://img.shields.io/github/license/rafaelescaleira/avl.svg)

Esse repositório possui funções implementadas em linguagem C.
Segue a lista das funções :

* Inserção;
* Remoção;
* Busca;

Instruções para Compilação
--------------------------
Para compilar `` avl.c `` com `` teste_avl.c `` , ou seja, caso deseja testar o código de `` avl.c `` :

* `` ~$ gcc avl.c teste_avl.c -Wall -std=c99 -pedantic ``

Para compilar `` avl.c `` com `` main.c `` , ou seja, caso deseja executar o código e realizar seus próprios testes :

* `` ~$ gcc avl.c main.c -Wall -std=c99 -pedantic ``

Instruções para Execução
------------------------
Para executar o arquivo já compilado com `` teste_avl.c `` :

* `` ~$ ./a.out ``

Para executar o arquivo já compilado com `` main.c `` :

* `` ~$ ./a.out ``
* `` ~$ ./a.out < arquivo_entrada.txt ``

Desempenho da Árvore AVL
------------------------

Árvore AVL pode ser empregada na implementação de conjuntos, principalmente aqueles cujas chave não são números inteiros.

A complexidade das principais operações de conjuntos usando árvore AVL:

| Algoritmo | Caso Médio | Pior Caso |
| --------- |:----------:|:---------:|
| Espaço | O (n) | O (n) |
| Busca | O (log n) | O (log n) |
| Inserção | O (log n) | O (log n) |
| Remoção | O (log n) | O (log n) |

Execução do Programa (Teste)
----------------------------
![grab-landing-page](https://j.gifs.com/E9M1xl.gif)

Execução do Programa (Principal)
--------------------------------
![grab-landing-page](https://j.gifs.com/Q0pj75.gif)
