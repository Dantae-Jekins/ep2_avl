#ifndef J_BSTAVL_H
#define J_BSTAVL_H

// V 1.0

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX(a, b) (a>=b?a:b)

// Estrutura do node
struct bNode
{
    struct bNode *dir; // right
    struct bNode *esq; // left
    struct bNode *par; // parent
    int height;
    int val;
} typedef * bnode;


// Estrutura da  BST
struct bTree
{
    struct bNode *root; // root
} typedef bsTree;


// -------------------------------------------------------------- //
// FUNÇÕES SUJEITAS A MUDANÇA, NECESSÁRIO ADAPTAR DE PROJETO PARA PROJETO

// Gera um novo node de memória e o retorna 
// É necessário desalocar o espaço
bnode __bnode_New( int val )
{
    bnode ret = (bnode) malloc(sizeof(struct bNode));
    ret->dir = NULL;
    ret->esq = NULL;
    ret->par = NULL;
    ret->val = val;
    ret->height = 1;
    return ret;
}

/// Função que recalcula fatores de balanceamento e alturas dos nós,
/// utilizada no momento da inserção
void __bnode_Recalc( bnode target)
{
    bnode This;
    This = target;
    while(This)
    {
      int alturaEsq = 0;
      int alturaDir = 0;

      if ( This->esq )
          alturaEsq = This->esq->height;

      if ( This->dir )
          alturaDir = This->dir->height;
      //recalcula
      This->height = MAX(alturaDir, alturaEsq) + 1; 
      This = This->par;
    }
}


// Função auxiliar para a bst_Insert, procura e insere um valor
// Retorna se o valor foi inserido ou se já existe, true se foi inserido
bool __bnode_Insert( bnode target, int val)
{

    // se o valor for maior
    if ( target->val < val)
    {
        // se houver espaço disp
        if (target->dir == NULL)
        {
            bnode newn = __bnode_New( val );
            newn->par = target;
            target->dir = newn;
            __bnode_Recalc(target);
            return true;
        }

        // se não
        else
        {
            return __bnode_Insert(target->dir, val);
        }
    }

    // se o valor for menor
    else if ( target->val > val)
    {
        // se houver espaço disp
        if (target->esq == NULL)
        {
            bnode newn = __bnode_New( val );
            newn->par = target;
            target->esq = newn;
            __bnode_Recalc(target);
            return true;
        }

        // se não
        else
        {
            return __bnode_Insert(target->esq, val);
        }
    }

    // se for =
    else
    {
        return false;
    }
}


// Printa uma árvora ordenadamente a partir de um nó
void __bnode_inOrder( bnode target)
{
    if (target->esq != NULL)
    {
        __bnode_inOrder( target->esq );
    }

    printf("%d, ", target->val);

    if (target->dir != NULL)
    {
        __bnode_inOrder( target->dir );
    }
    
    return;
}


// Printa uma árvore de forma gráfica para análise
void __bnode_Graphical( bnode target, int n)
{
    if (target->dir != NULL)
    {
        __bnode_Graphical( target->dir, n+5 );
    }
    
    // printa
    if (target->par != NULL)
    {
        if (target == target->par->dir)
        {
            printf("%*c/ %d |\n", n, ' ', target->val);
        }
        
        else
            printf("%*c\\ %d |\n", n, ' ', target->val);
    }
    else
    {
        printf("%*c> %d |\n", n, ' ', target->val);
    }

    if (target->esq != NULL)
    {
        __bnode_Graphical( target->esq, n+5 );
    }
    
    return;
}


// Informa se o node é o filho direito de seu pai
// Retorna true se for o direito, ou false se for o esquerdo ou não ter um pai
bool __bnode_isRChild(bnode target)
{
    if (target->par == NULL)
        return false;

    if (target->par->dir == target)
        return true;

    else
        return false;
}


// Libera o espaço de memória de um node, deletando-o
// Retorna um nó, se a raiz for removida
bnode __bnode_Remove(bnode target)
{
    if (target == NULL)
        return NULL;

    bnode esq = target->esq;
    bnode dir = target->dir;
    bnode pai = target->par;

    bool rchild = __bnode_isRChild(target);

    // se possui pai
    if ( pai )
    {
        // possui dois filhos
        if (esq && dir)
        {
            // FAZER!
            return NULL;
        }

        // possui só o esquerdo
        else if ( esq )
        {
            if (rchild)
            {
                // é o filho direito
                pai->dir = esq;
                esq->par = pai;
            }

            else
            {
                pai->esq = esq;
                esq->par = pai;
            }
        }

        // possui só o direito
        else if ( dir )
        {
            if (rchild)
            {
                // é o filho direito
                pai->dir = dir;
                dir->par = pai;
            }

            else
            {
                pai->esq = dir;
                dir->par = pai;
            }
        }

        // possui nenhum dos dois
        else
        {
            if (rchild)
            {
                pai->dir = NULL;
            }

            else
            {
                pai->esq = NULL;
            }
        }

        free(target);
    }

    // é a raiz    
    else
    {
        free(target);

        // possui dois filhos
        if (esq && dir)
        {
            // FAZER!
            return NULL;
        }

        // possui só o esquerdo
        else if ( esq )
        {
            esq->par = NULL;
            return esq;            
        }

        // possui só o direito
        else if ( dir )
        {
            dir->par = NULL;
            return dir;
        }
    }

    return NULL;
}


// Percorre uma subárvore e retorna o seu menor elemento
bnode __bnode_minNode(bnode target)
{
    bnode This = target;
    while(This->esq != NULL)
    {
        This = This->esq;
    }

    return This;
}


// Percorre uma subárvore e retorna o seu maior elemento
bnode __bnode_maxNode(bnode target)
{
    bnode This = target;
    while(This->dir != NULL)
    {
        This = This->dir;
    }

    return This;
}


// Retorna o node com o valor descrito, ou null se não existir
bnode __bnode_Search(bnode target, int val)
{
    bnode This = target;
    while(This != NULL)
    {
        if (This->val == val)
        {
            return This;
        }

        else if (This->val > val)
        {
            This = This->esq;
        }

        else
        {
            This = This->dir;
        }
    }

    return NULL;
}





// ------------------------------------------------------------- //
/// FUNÇÕES NÃO SUJEITAS A MUDANÇA, NÃO TOQUE, SÓ EM CASO DE BUG


// Gera uma nova árvore com valores nulos e retorna
bsTree bst_New()
{
    bsTree ret;
    ret.root = NULL;
    return ret;
}


// Insere um novo elemento na BST
// Retorna se a inserção do elemento foi um sucesso
bool bst_Insert( bsTree *target, int value)
{
    // se a bst está vazia, inicializa
    if (target->root == NULL)
    {
        bnode newn = __bnode_New(value);
        target->root = newn;
        return true;
    }

    // se a bst já contém valores
    else
    {
        return __bnode_Insert(target->root, value);
    }
}


// Printa a árvore ordenadamente
void bst_inOrder( bsTree target)
{
    if (target.root != NULL)
    {
        printf("\n");
        __bnode_inOrder( target.root );
        printf(" end");
    }

    else
    {
        printf("\nnull");
    }
}


// Printa um modelo gráfico da árvore
void bst_Graphical( bsTree target)
{
    if (target.root != NULL)
    {
        printf("\n\n");
        __bnode_Graphical(target.root, 1);
    }
}


// Realiza um clear na bst, liberando toda sua memória
void bst_Free( bsTree *target)
{
    if ( target->root == NULL )
    {
        return;
    }

    else
    {
        bnode This = target->root;
        do
        {
            // vai para a esquerda
            if (This->esq != NULL)
            {
                This = This->esq;
            }

            // vai para direita
            else if (This->dir != NULL)
            {
                This = This->dir;
            }

            // nada na direita e nem na esquerda
            else
            {
                if (This->par == NULL)
                {
                    target->root = NULL;
                    __bnode_Remove(This);
                    break;
                }

                bnode aux = This->par;
                __bnode_Remove(This);
                This = aux;
                
                // remove esse nó
                // se é a raiz

            }

        } while (1);

        target->root = NULL;
        return;
    }
}


// Retorna o menor número da BST
int bst_minVal(bsTree target)
{
    if(target.root == NULL)
    {
        return 0;
    }

    bnode element = __bnode_minNode(target.root);
    return element->val;
}


// Retorna o nó do menor elemento da subárvore
int bst_maxVal(bsTree target)
{
    if (target.root == NULL)
    {
        return 0;
    }

    bnode element = __bnode_maxNode(target.root);
    return element->val;
}


// Procura por um valor na árvore binária
// Retorna true se o valor existe ou false se o valor não existe
bool bst_Search(bsTree target, int value)
{
    bnode found = __bnode_Search(target.root, value);

    if (found)
    {
        return true;
    }

    return false;
}


// Procura por um valor na árvore binária e o remove
// Retorna true se o valor existe e foi removido ou false caso contrário
bool bst_Remove( bsTree *target, int value )
{
    bnode found = __bnode_Search(target->root, value);
    
    if (found)
    {
        bnode new_root = __bnode_Remove(found);
        
        if (new_root)
        {
            target->root = new_root;
        }

        return true;
    }

    return false;
}


#endif
