#include <stdio.h>
#include <stdlib.h>

#include "libs/j_avlTree.h"
#include "libs/j_binaryTree.h"
#include "libs/jg_strings.h"


#define QUANT_ERR \
  printf("\nQuantidade inválida de argumento \nuso: avl_test <amostras> <nós>\n Limite de 99 amostras e 999 nós.")


#define PRINT(a, b) \
  printf("\n var.%d %d\n", a, b)


// funções específicas
void fill_bst( bsTree *target, int amount);
void fill_avl( avlTree *target, int amount);
void print_time(clock_t start, clock_t end, char *ops);

// função main responsável pela comunicação
int main(int argc, char *argv[])
{
  // obrigatório ter as 
  if (argc != 3)
  {
    QUANT_ERR;
    return -1;
  }

  // variáveis
  int amostras;
  int nodes;
  avlTree *AVL_trees;
  bsTree *BST_trees;
  
  clock_t bstkoniec;
  clock_t bstpoczatek;

  clock_t avlkoniec;
  clock_t avlpoczatek;


  // parse nos argumentos
  amostras = ret_int( argv[1] );
  nodes    = ret_int( argv[2] ); 
  printf("\n Experimento com A = %d e N = %d", amostras, nodes);
  printf("\n --------------------------------\n");
  

  // alocamos o espaço das árvores
  AVL_trees = malloc(sizeof(avlTree)* amostras);
  BST_trees = malloc(sizeof(bsTree) * amostras);

  
  // inicializamos as árvores e adicionamos os nós
  time_t t;  srand((unsigned) time(&t));  

  // marcamos os tempos para a bst
  bstpoczatek = clock();
  for(int i = 0; i < amostras; i++)
  {
    BST_trees[i] = bst_New();
    fill_bst(&BST_trees[i], nodes); 
  } bstkoniec = clock();
 
  // marcamos os tempos para a avl
  avlpoczatek = clock();
  for(int i = 0; i < amostras; i++)
  {
    AVL_trees[i] = avl_New();
    fill_avl(&AVL_trees[i], nodes);
  } avlkoniec = clock();
  
  
  // parte principal
  // calculamos as médias
  double media_all;
  double media_bst = 0;
  double media_avl = 0;
  
  for (int i = 0; i < amostras; i++)
  {
    media_bst += BST_trees[i].root->height;
    media_avl += AVL_trees[i].root->height;
  }

  media_bst /= amostras;
  media_avl /= amostras;
  media_all = (media_bst + media_avl)/2;
  printf("\nBST -----------------------------------\n");
  printf(" Altura média BST : %.3f", media_bst);
  print_time(bstpoczatek, bstkoniec, "Geração-BST");

  printf("\nAVL -----------------------------------\n");
  printf(" Altura média AVL : %.3f", media_avl);
  print_time(avlpoczatek, avlkoniec, "Geração-AVL");
  
  printf("\nMEDIA ---------------------------------\n");
  printf(" Altura média     : %.3f", media_all);
  print_time((avlpoczatek+bstpoczatek)/2, (avlkoniec+bstkoniec)/2, "Geração-ALL");
  // desalocamos o espaço
  for (int i = 0; i < amostras; i++)
  {
    bst_Free(&BST_trees[i]);
    avl_Free(&AVL_trees[i]);
  }

  free(AVL_trees);
  free(BST_trees);

  return 0;
}


// preenche uma bst com "amount" números
void fill_bst( bsTree *target, int amount)
{
  // intervalo dos números 
  int interval = amount * 5;
  int counter = 0;
  while (counter < amount)
  {
    int number = rand() % interval;
    // se não existe adicionamos
    if (bst_Insert( target, number ) )
    {
      counter++;
    }
  }
}


// preenche uma avl com "amount" números
void fill_avl( avlTree *target, int amount)
{
  // intervalo dos números 
  int interval = amount * 5;
  int counter = 0;
  while (counter < amount)
  {
    int number = rand() % interval;
    // se não existe adicionamos
    if (avl_Insert( target, number ) )
    {
      counter++;
    }
  }
}

void print_time(clock_t start, clock_t end, char *ops)
{

  clock_t difference = end - start;
  double time =((double) difference / CLOCKS_PER_SEC);
  printf("\n\n Tempo de operação -%s:", ops);
  printf("\n %f \033[32m segundos \033[0m", time);
  printf("\n %f \033[32m milisegundos \033[0m\n", time*1000);
}

