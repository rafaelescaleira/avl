/* * * * * * * * * * * * * * * * * * * * * * * * *
 *                                               *
 * Nome : Rafael Escaleira Ferreira dos Santos.  *
 * Universidade Federal do Mato Grosso do Sul    *
 * Disciplina : Estrutura de Dados e Programação *
 *                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include "avl.h"

/* * * * * * * * * * * * * Árvore AVL * * * * * * * * * * * * * */

int compara (const void * a, const void * b) {
    
    return (* (treg *)a).id - (* (treg *)b).id;
    
}

treg * aloca_reg (int id) {
    
    treg * aux = (treg *) malloc (sizeof (treg));
    aux -> id = id;
    
    return aux;
    
}

typedef struct _pilha {

    tnode * * no;
    struct _pilha * prox;

} tpilha;

void empilha (tpilha * * ppilha, tnode * * no) {

    tpilha * pno = (tpilha *) malloc (sizeof (tpilha));
    pno -> no = no;
    pno -> prox = * ppilha;
    * ppilha = pno;

}

tnode * * desempilha (tpilha * * ppilha) {

    tpilha * aux;
    tnode * * ret;

    ret = NULL;

    if (* ppilha != NULL) {

        aux = (* ppilha);
        * ppilha = (* ppilha) -> prox;
        ret = aux -> no; 
        free (aux);

    }

    return ret;
}

int max (int a, int b) {

    return a > b ? a : b;

}

int altura (tnode * pnode) {

    int ret = - 1;

    if (pnode != NULL) {

        ret = pnode -> h;

    }

    return ret;

}

void avl_constroi (tarv * parv, int (* cmp) (const void * a, const void * b)) {

    parv -> raiz = NULL; 
    parv -> compara = cmp;

}

void rotacao_direita (tnode * * parv) {

    tnode * x = * parv;
    tnode * y = x -> esq;
    tnode * A = y -> esq;
    tnode * B = y -> dir;
    tnode * C = x -> dir;

    x -> dir = C; 
    x -> esq = B;
    y -> esq = A;
    y -> dir = x;
    * parv = y;

    x -> h = max (altura (B), altura (C)) + 1;
    y -> h = max (altura (A), altura (x)) + 1;

}

void rotacao_esquerda (tnode * * parv) {

    tnode * y = * parv;
    tnode * x = y -> dir;
    tnode * A = y -> esq;
    tnode * B = x -> esq;
    tnode * C = x -> dir;

    y -> dir = B; 
    y -> esq = A;
    x -> dir = C;
    x -> esq = y;

    * parv = x;

    y -> h = max (altura (A), altura (B)) + 1;
    x -> h = max (altura (y), altura (C)) + 1;

}

void rebalancear (tnode * * pnode) {

    int fb, fbf;
    tnode * filho;

    fb = altura ((* pnode) -> esq) - altura ((* pnode) -> dir);

    if (fb == 2) {

        filho = (* pnode) -> esq;
        fbf = altura (filho -> esq) - altura (filho -> dir);

        if (fbf == - 1) {

            rotacao_esquerda (&((* pnode) -> esq));

        }

        rotacao_direita (pnode);

    }

    else if (fb == - 2) {

        filho = (* pnode) -> dir;
        fbf = altura (filho -> esq) - altura (filho -> dir);

        if (fbf == 1) {

            rotacao_direita (&((* pnode) -> dir));

        }

        rotacao_esquerda (pnode);

    }

}

void * _avl_busca (tarv * parv, tnode * * pnode, void * preg) {

    void * ret;

    if (* pnode == NULL) {

        ret = NULL;

    }

    else if (parv -> compara (((* pnode) -> reg), preg) == 0) {

        ret = (* pnode) -> reg;

    }

    else {

        if (parv -> compara (((* pnode) -> reg), preg) > 0) {

            ret = _avl_busca (parv, &((* pnode) -> esq), preg);

        }

        else {

            ret = _avl_busca (parv, &((* pnode) -> dir), preg);

        }

    }

    return ret;

}

void * avl_busca (tarv * parv, void * preg) {

    return _avl_busca (parv, &(parv -> raiz), preg);

}

int _avl_insere (tarv * parv, tnode * * pnode, void * preg) {

    int ret = 1;

    if (* pnode == NULL) {

        * pnode = (tnode *) malloc (sizeof (tnode));

        if (* pnode == NULL) {
            ret = 0;
        }

        else {
            (*pnode) -> reg = preg;
            (*pnode) -> esq = NULL;
            (*pnode) -> dir = NULL;
            (*pnode) -> h = 0;
        }

    }

    else {

        if (parv -> compara (((* pnode) -> reg), preg) > 0) {

            ret = _avl_insere (parv, &((* pnode) -> esq) , preg);

        }

        else {

            ret = _avl_insere (parv, &((* pnode) -> dir) , preg);

        }

        (* pnode) -> h = max (altura ((* pnode) -> esq), altura ((* pnode) -> dir)) + 1;
        rebalancear (pnode);

    }


    return ret;

}


int avl_insere (tarv * parv, void * preg) {

    return _avl_insere (parv, &(parv -> raiz), preg);

}

tnode * * sucessor (tnode * * pnode) {

    tnode * aux, * * paux;

    aux = (* pnode) -> dir;
    paux = &((* pnode) -> dir);

    while (aux -> esq != NULL) {

        paux = &(aux -> esq);
        aux = aux -> esq;

    }

    return paux;

}

int _avl_remove (tarv * parv, tnode * * pnode, void * preg) {

    int ret;
    tnode * aux, * * paux, * * pai;
    tpilha * pilha;

    if (* pnode == NULL) {

        ret = 0;

    }

    else {

        if (parv -> compara (((* pnode) -> reg), preg) > 0) {

            ret = _avl_remove (parv, &((* pnode) -> esq) , preg);

        }

        else if (parv -> compara (((* pnode) -> reg), preg) < 0) {

            ret = _avl_remove (parv, &((* pnode) -> dir) , preg);

        }

        else {

            /* Caso 1: Sem Filhos */

            if ((* pnode) -> dir == NULL && (* pnode) -> esq == NULL) {

                free ((* pnode) -> reg);
                free (* pnode);
                * pnode = NULL;
                ret = 1;

            }

            /* Caso 2: Um Filho */

            else if (((* pnode) -> dir != NULL && (* pnode) -> esq == NULL)) {

                free ((* pnode) -> reg);
                aux = (* pnode);
                * pnode = aux -> dir;
                free (aux);
                ret = 1;
                
            }

            else if (((* pnode) -> dir == NULL && (* pnode) -> esq != NULL)) {

                free ((* pnode) -> reg);
                aux = (* pnode);
                * pnode = aux -> esq;
                free (aux);
                ret = 1;

            }

            /* Caso 3: Dois Filhos */

            else {

                pilha = NULL;
                empilha (&pilha, pnode);
                paux = &((* pnode) -> dir);
                aux = (* pnode) -> dir;

                while (aux -> esq != NULL) {

                    empilha (&pilha, paux);
                    paux = &(aux -> esq);
                    aux = aux -> esq;
                }

                free ((* pnode) -> reg);
                (* pnode) -> reg = aux -> reg;
                * paux = aux -> dir;
                free (aux);
                pai = desempilha (&pilha);

                while (pai != NULL) {
 					
 					(* pai) -> h = max (altura ((* pai) -> esq ), altura ((* pai) -> dir)) + 1;
                    rebalancear (pai);
                    pai = desempilha (&pilha);

                }

                ret = 1;

            }

        }

        if (* pnode != NULL) {

            (* pnode) -> h = max (altura ((* pnode) -> esq), altura ((* pnode) -> dir)) + 1;
            rebalancear (pnode);

        }

    }

    return ret;

}

int avl_remove (tarv * parv, void * preg) {

    return _avl_remove (parv, &(parv -> raiz), preg);

}

void imprime_arvore_avl (tnode * raiz, int profundidade, int elemento_inserido) {
    
    int anda = 3;
    int i;
    
    if (raiz != NULL) {
        
        imprime_arvore_avl (raiz -> dir, profundidade + anda, elemento_inserido);
        
        for (i = 0; i < profundidade; i ++) {
            
            printf (" ");
            
        }
        
        if ((*(treg *)(raiz -> reg)).id == elemento_inserido) {
            
            cor_texto (verde);
            //estilo_texto (piscar);
            estilo_texto (negrito);
            
        }
        
        else {
            
            estilo_texto (resetar);
            
        }
        
        printf ("%d", (*(treg *)(raiz -> reg)).id);
        cor_texto (vermelho);
        estilo_texto (fosco);
        printf (" (%d)\n", raiz -> h);
        estilo_texto (resetar);
        imprime_arvore_avl (raiz -> esq, profundidade + anda, elemento_inserido);
        
    }
    
}

void free_node_avl (tnode * raiz) {
    
    if (raiz != NULL) {
        
        free_node_avl (raiz -> dir);
        free (raiz -> reg);
        free (raiz);
        free_node_avl(raiz -> esq);
        
    }

}

void pre_ordem (tnode * pnode) {
    
    if (pnode != NULL) {
        
        printf ("%d ", (* (treg *) (pnode -> reg)).id);
        pre_ordem (pnode -> esq);
        pre_ordem (pnode -> dir);
        
    }
    
}

void in_ordem (tnode * pnode) {
    
    if (pnode != NULL) {
        
        in_ordem (pnode -> esq);
        printf ("%d ", (* (treg *) (pnode -> reg)).id);
        in_ordem (pnode -> dir);
        
    }
    
}

void pos_ordem (tnode * pnode) {
    
    if (pnode != NULL) {
        
        pos_ordem (pnode -> esq);
        pos_ordem (pnode -> dir);
        printf ("%d ",(* (treg *) (pnode -> reg)).id);
        
    }

}

/* * * * * * * * * * * * * * * Funções de Teste * * * * * * * * * * * * * * */

void teste_construtor_avl () {

    int a, b;
    tarv arv;

    avl_constroi (&arv, &compara);

    a = 10;
    b = 20;

    assert (arv.raiz == NULL);
    assert (arv.compara (&a, &b) == - 10);
    assert (arv.compara (&b, &a) == 10);
    assert (arv.compara (&b, &b) == 0);
    assert (arv.compara (&a, &a) == 0);

    cor_texto (ciano);
    printf("\t*");
    estilo_texto (resetar);

    cor_texto (vermelho);
    printf("         * ");
    estilo_texto (resetar);

    printf ("Construção AVL ");
    cor_texto (verde);
    printf("                OK");
    estilo_texto (resetar);

    cor_texto (ciano);
    printf("       *\n");
    estilo_texto (resetar);

}

void teste_insere_avl () {

    tarv arv;
    treg * aux = (treg *) malloc (sizeof (treg));

    avl_constroi (&arv, &compara);
    
    avl_insere (&arv, aloca_reg (10));

    assert (arv.raiz -> esq == NULL);
    aux -> id = 10;
    assert (compara (arv.raiz, aux));
    assert (arv.raiz -> dir == NULL);
   
    avl_insere (&arv, aloca_reg (20)); 
    aux -> id = 10;
    assert (compara (arv.raiz, aux));
    assert (arv.raiz -> esq == NULL);
    
    assert (arv.raiz -> dir -> esq == NULL);
    aux -> id = 20;
    assert (compara (arv.raiz -> dir, aux));
    assert (arv.raiz -> dir -> dir == NULL);

    avl_insere (&arv, aloca_reg (30)); 
    assert (arv.raiz -> esq -> dir == NULL);
    aux -> id = 10;
    assert (compara (arv.raiz -> esq, aux));
    assert (arv.raiz -> esq -> esq == NULL);
 
    aux -> id = 20;
    assert (compara (arv.raiz, aux));
    assert (arv.raiz -> dir -> dir == NULL);

    aux -> id = 30;
    assert (compara (arv.raiz -> dir, aux));
    assert (arv.raiz -> dir -> esq == NULL);

    avl_insere (&arv, aloca_reg (40)); 
    assert (arv.raiz -> esq -> esq == NULL);
    aux -> id = 10;
    assert (compara (arv.raiz -> esq, aux));
    assert (arv.raiz -> esq -> dir == NULL);

    aux -> id = 20;
    assert (compara (arv.raiz, aux));
    
    aux -> id = 30;
    assert (compara (arv.raiz -> dir, aux));
    assert (arv.raiz -> dir -> esq == NULL);
    
    assert (arv.raiz -> dir -> dir -> dir == NULL);
    aux -> id = 40;
    assert (compara (arv.raiz -> dir -> dir, aux));
    assert (arv.raiz -> dir -> dir -> esq == NULL);
    
    avl_insere (&arv, aloca_reg (35)); 
    assert (arv.raiz -> esq -> esq == NULL);
    aux -> id = 10;
    assert (compara (arv.raiz -> esq, aux));
    assert (arv.raiz -> esq -> dir == NULL);
    
    aux -> id = 20;
    assert (compara (arv.raiz, aux));
    
    aux -> id = 35;
    assert (compara (arv.raiz -> dir, aux));
    
    assert (arv.raiz -> dir -> esq -> esq == NULL);
    aux -> id = 30;
    assert (compara (arv.raiz -> dir -> esq, aux));
    assert (arv.raiz -> dir -> esq -> dir == NULL);
    
    assert (arv.raiz -> dir -> dir -> esq == NULL);
    aux -> id = 40;
    assert (compara (arv.raiz -> dir -> dir, aux));
    assert (arv.raiz -> dir -> dir -> dir == NULL);
 
    avl_insere (&arv, aloca_reg (5)); 
    avl_insere (&arv, aloca_reg (7)); 
   
    assert (arv.raiz -> esq -> esq -> esq == NULL);
    aux -> id = 5;
    assert (compara (arv.raiz -> esq -> esq, aux));
    assert (arv.raiz -> esq -> esq -> dir == NULL);
     
    aux -> id = 7;
    assert (compara (arv.raiz -> esq, aux));

    assert (arv.raiz -> esq -> dir -> esq == NULL);
    aux -> id = 10;
    assert (compara (arv.raiz -> esq -> dir, aux));
    assert (arv.raiz -> esq -> dir -> dir == NULL);
    
    aux -> id = 20;
    assert (compara (arv.raiz, aux));
   
    assert (arv.raiz -> dir -> esq -> esq == NULL);
    aux -> id = 30;
    assert (compara (arv.raiz -> dir -> esq, aux));
    assert (arv.raiz -> dir -> esq -> dir == NULL);
   
    aux -> id = 35;
    assert (compara (arv.raiz -> dir, aux));
    
    assert (arv.raiz -> dir -> dir -> esq == NULL);
    aux -> id = 40;
    assert (compara (arv.raiz -> dir -> dir, aux));
    assert (arv.raiz -> dir -> dir -> dir == NULL);

    cor_texto (ciano);
    printf("\t*");
    estilo_texto (resetar);

    cor_texto (vermelho);
    printf("         * ");
    estilo_texto (resetar);

    printf ("Inserção AVL ");
    cor_texto (verde);
    printf("                  OK");
    estilo_texto (resetar);

    cor_texto (ciano);
    printf("       *\n");
    estilo_texto (resetar);
    
}

void teste_busca_avl () {

    int v[] = {30, 15, 50, 7, 20, 35, 55};
    int n = sizeof(v) / sizeof(v[0]);
    int i;
    tarv arv;

    avl_constroi (&arv, &compara);
    
    for (i = 0; i < n; i ++) {

        avl_insere (&arv, aloca_reg (v[i]));

    }
   
    tnode * pnode = arv.raiz;
    treg * aux = (treg *) malloc (sizeof (treg));
    treg * aux_busca = (treg *) malloc (sizeof (treg));

    aux -> id = 0;

    aux_busca = avl_busca (&arv, aux);
    assert (aux_busca == NULL);

    aux -> id = 30;

    aux_busca = avl_busca (&arv, aux);
    assert (compara (pnode -> reg, aux_busca) == 0);

    aux -> id = 15;

    aux_busca = avl_busca (&arv, aux);
    assert (compara (pnode -> esq -> reg, aux_busca) == 0);

    aux -> id = 50;

    aux_busca = avl_busca (&arv, aux);
    assert (compara (pnode -> dir -> reg, aux_busca) == 0);

    aux -> id = 7;

    aux_busca = avl_busca (&arv, aux);
    assert (compara (pnode -> esq -> esq -> reg, aux_busca) == 0);

    aux -> id = 20;

    aux_busca = avl_busca (&arv, aux);
    assert (compara (pnode -> esq -> dir -> reg, aux_busca) == 0);

    aux -> id = 35;

    aux_busca = avl_busca (&arv, aux);
    assert (compara (pnode -> dir -> esq -> reg, aux_busca) == 0);

    aux -> id = 55;

    aux_busca = avl_busca (&arv, aux);
    assert (compara (pnode -> dir -> dir -> reg, aux_busca) == 0);

    cor_texto (ciano);
    printf("\t*");
    estilo_texto (resetar);

    cor_texto (vermelho);
    printf("         * ");
    estilo_texto (resetar);

    printf ("Busca AVL ");
    cor_texto (verde);
    printf("                     OK");
    estilo_texto (resetar);

    cor_texto (ciano);
    printf("       *\n");
    estilo_texto (resetar);

}

void teste_remove_avl_1 () {

    tarv arv;
    int v[] = {20, 15, 30, 10, 17, 25, 36, 27};
    int n, i;

    n = sizeof(v) / sizeof(v[0]);
    treg * aux = (treg *) malloc (sizeof (treg));

    avl_constroi (&arv, &compara);

    for (i = 0; i < n; i ++) {

        avl_insere (&arv, aloca_reg (v[i]));

    }

    aux -> id = 20;
    avl_remove (&arv, aux);

    aux -> id = 25;
    assert (compara (arv.raiz, aux));
    aux -> id = 15;
    assert (compara (arv.raiz -> esq, aux));
    aux -> id = 30;
    assert (compara (arv.raiz -> dir, aux));
    aux -> id = 10;
    assert (compara (arv.raiz -> esq -> esq, aux));
    aux -> id = 17;
    assert (compara (arv.raiz -> esq -> dir, aux));
    aux -> id = 27;
    assert (compara (arv.raiz -> dir -> esq, aux));
    aux -> id = 36;
    assert (compara (arv.raiz -> dir -> dir, aux));

    assert (arv.raiz -> esq -> esq -> esq == NULL);
    assert (arv.raiz -> esq -> esq -> dir == NULL);
    assert (arv.raiz -> esq -> dir -> esq == NULL);
    assert (arv.raiz -> esq -> dir -> dir == NULL);
    assert (arv.raiz -> dir -> esq -> esq == NULL);
    assert (arv.raiz -> dir -> esq -> dir == NULL);
    assert (arv.raiz -> dir -> dir -> esq == NULL);
    assert (arv.raiz -> dir -> dir -> dir == NULL);

    aux -> id = 25;
    avl_remove (&arv, aux);

    aux -> id = 27;
    assert (compara (arv.raiz, aux));
    aux -> id = 15;
    assert (compara (arv.raiz -> esq, aux));
    aux -> id = 30;
    assert (compara (arv.raiz -> dir, aux));
    aux -> id = 10;
    assert (compara (arv.raiz -> esq -> esq, aux));
    aux -> id = 17;
    assert (compara (arv.raiz -> esq -> dir, aux));
    aux -> id = 36;
    assert (compara (arv.raiz -> dir -> dir, aux));

    assert (arv.raiz -> esq -> esq -> esq == NULL);
    assert (arv.raiz -> esq -> esq -> dir == NULL);
    assert (arv.raiz -> esq -> dir -> esq == NULL);
    assert (arv.raiz -> esq -> dir -> dir == NULL);
    assert (arv.raiz -> dir -> esq == NULL);
    assert (arv.raiz -> dir -> dir -> esq == NULL);
    assert (arv.raiz -> dir -> dir -> dir == NULL);

    aux -> id = 27;
    avl_remove (&arv, aux);

    aux -> id = 30;
    assert (compara (arv.raiz, aux));
    aux -> id = 15;
    assert (compara (arv.raiz -> esq, aux));
    aux -> id = 36;
    assert (compara (arv.raiz -> dir, aux));
    aux -> id = 10;
    assert (compara (arv.raiz -> esq -> esq, aux));
    aux -> id = 17;
    assert (compara (arv.raiz -> esq -> dir, aux));

    assert (arv.raiz -> esq -> esq -> esq == NULL);
    assert (arv.raiz -> esq -> esq -> dir == NULL);
    assert (arv.raiz -> esq -> dir -> esq == NULL);
    assert (arv.raiz -> esq -> dir -> dir == NULL);
    assert (arv.raiz -> dir -> esq == NULL);
    assert (arv.raiz -> dir -> dir == NULL);

    aux -> id = 30;
    avl_remove (&arv, aux);

    aux -> id = 15;
    assert (compara (arv.raiz, aux));
    aux -> id = 10;
    assert (compara (arv.raiz -> esq, aux));
    aux -> id = 36;
    assert (compara (arv.raiz -> dir, aux));
    aux -> id = 17;
    assert (compara (arv.raiz -> dir -> esq, aux));

    assert (arv.raiz -> esq -> esq == NULL);
    assert (arv.raiz -> esq -> dir == NULL);
    assert (arv.raiz -> dir -> esq -> esq == NULL);
    assert (arv.raiz -> dir -> esq -> dir == NULL);
    assert (arv.raiz -> dir -> dir == NULL);

    aux -> id = 10;
    avl_remove (&arv, aux);

    aux -> id = 17;
    assert (compara (arv.raiz, aux));
    aux -> id = 15;
    assert (compara (arv.raiz -> esq, aux));
    aux -> id = 36;
    assert (compara (arv.raiz -> dir, aux));

    assert (arv.raiz -> esq -> esq == NULL);
    assert (arv.raiz -> esq -> dir == NULL);
    assert (arv.raiz -> dir -> esq == NULL);
    assert (arv.raiz -> dir -> dir == NULL);
 
    aux -> id = 17;
    avl_remove (&arv, aux);

    aux -> id = 36;
    assert (compara (arv.raiz, aux));
    aux -> id = 15;
    assert (compara (arv.raiz -> esq, aux));

    assert (arv.raiz -> esq -> esq == NULL);
    assert (arv.raiz -> esq -> dir == NULL);
    assert (arv.raiz -> dir == NULL);

    aux -> id = 36;
    avl_remove (&arv, aux);

    aux -> id = 15;
    assert (compara (arv.raiz, aux));

    assert (arv.raiz -> dir == NULL);
    assert (arv.raiz -> esq == NULL);
 
    aux -> id = 15;
    avl_remove (&arv, aux);

    assert (arv.raiz == NULL);

    cor_texto (ciano);
    printf("\t*");
    estilo_texto (resetar);

    cor_texto (vermelho);
    printf("         * ");
    estilo_texto (resetar);

    printf ("Remoção AVL (1) ");
    cor_texto (verde);
    printf("               OK");
    estilo_texto (resetar);

    cor_texto (ciano);
    printf("       *\n");
    estilo_texto (resetar);

}

void teste_remove_avl_2 () {

    tarv arv;
    int v[] = 
    {20, 12, 50, 7, 17, 25, 100, 4, 10, 15, 19, 23, 40, 75, 
    125, 2, 5, 8, 11, 13, 16, 18, 21, 24, 30, 45, 70, 90, 110, 140, 
    1, 3, 6, 9, 14, 22, 27, 35, 43, 47, 60, 80, 95, 105, 120, 130, 
    150, 0, 37, 55, 77, 93, 98, 103, 107, 123, 160, 97, 104};
    int n, i;

    n = sizeof(v) / sizeof(v[0]);
    treg * aux = (treg *) malloc (sizeof (treg));

    avl_constroi (&arv, &compara);

    for (i = 0; i < n; i ++) {

        avl_insere (&arv, aloca_reg (v[i]));

    }

    aux -> id = 20;
    avl_remove (&arv, aux);

    aux -> id = 21;
    assert (compara (arv.raiz, aux));
    aux -> id = 50;
    assert (compara (arv.raiz -> dir, aux));
    aux -> id = 30;
    assert (compara (arv.raiz -> dir -> esq, aux));
    aux -> id = 25;
    assert (compara (arv.raiz -> dir -> esq -> esq, aux));
    aux -> id = 23;
    assert (compara (arv.raiz -> dir -> esq -> esq, aux));
    aux -> id = 22;
    assert (compara (arv.raiz -> dir -> esq -> esq, aux));
    aux -> id = 40;
    assert (compara (arv.raiz -> dir -> esq -> dir, aux));
    aux -> id = 45;
    assert (compara (arv.raiz -> dir -> esq -> dir -> dir, aux));
    aux -> id = 47;
    assert (compara (arv.raiz -> dir -> esq -> dir -> dir -> dir, aux));

    cor_texto (ciano);
    printf("\t*");
    estilo_texto (resetar);

    cor_texto (vermelho);
    printf("         * ");
    estilo_texto (resetar);

    printf ("Remoção AVL (2) ");
    cor_texto (verde);
    printf("               OK");
    estilo_texto (resetar);

    cor_texto (ciano);
    printf("       *\n");
    estilo_texto (resetar);

}

void teste_tempo_execucao (int n) {

	clock_t inicio, fim;
    double tempo [2], insersao [4], remocao [4];
    tarv arv;
    treg * aux;
    int i, * v;

    srand (time (NULL));

    v = (int *) malloc (n * sizeof (int));
    
    for (i = 0; i < n; i ++) {

    	* (v + i) = rand () % (n * 4);

    }
    
    cor_texto (ciano);
    printf ("\t* * * * * * * * ");
    estilo_texto (resetar);

    cor_texto (amarelo);
    printf("TEMPO DE EXECUÇÃO (TESTE)");
    estilo_texto (resetar);

    cor_texto (ciano);
    printf (" * * * * * *\n");
    printf("\t*                                                   *\n");
    printf ("\t*           ");
    estilo_texto (resetar);

    cor_texto (vermelho);
    printf("Quantidade de Elementos : %d", n);
    estilo_texto (resetar);

    cor_texto (ciano);
    printf ("\t    *\n");
    printf("\t*                                                   *\n");
    estilo_texto (resetar);

	/* Árvore AVL */

    avl_constroi (&arv, &compara);

    aux = (treg *) malloc (sizeof (treg));
    inicio = clock ();

    for (i = 0; i < n; i ++) {

        avl_insere (&arv, aloca_reg (* (v + i)));

    }

    fim = clock ();
    tempo [0] = ( (double) (fim - inicio) ) / CLOCKS_PER_SEC;

    inicio = clock ();

    for (i = 0; i < n; i ++) {

        aux = arv.raiz -> reg;
        avl_remove (&arv, aloca_reg (aux -> id));

    } 

    fim = clock ();
    tempo [1] = ( (double) (fim - inicio) ) / CLOCKS_PER_SEC;

    insersao [1] = tempo [0];
    remocao [1] = tempo [1];

    free_node_avl (arv.raiz);

    cor_texto (ciano);
    printf("\t*");
    estilo_texto (resetar);

    cor_texto (verde);
    printf ("\t  * ");
    cor_texto (magenta);
    printf ("AVL ");
    estilo_texto (resetar);
    printf("Inserção : %.6lf segundos\t", insersao [1]);

    cor_texto (ciano);
    printf("    *\n");
    estilo_texto (resetar);

    cor_texto (ciano);
    printf("\t*");
    estilo_texto (resetar);

    cor_texto (verde);
    printf ("\t  * ");
    cor_texto (magenta);
    printf ("AVL ");
    estilo_texto (resetar);
    printf ("Remoção  : %.6lf segundos\t", remocao [1]);

    cor_texto (ciano);
    printf("    *\n");
    estilo_texto (resetar);

    cor_texto (ciano);
    printf("\t*                                                   *\n");
    printf("\t* * * * * * * * * * * * * * * * * * * * * * * * * * *\n\n");
    estilo_texto (resetar);

    free (v);

}
