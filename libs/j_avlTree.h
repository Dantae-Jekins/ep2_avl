#ifndef J_BSTCOMMON_H
#define J_BSTCOMMON_H

// V 1.0

#define MAX(a, b) (a>=b?a:b)

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// Estrutura do node
struct aNode
{
    struct aNode *dir; // right
    struct aNode *esq; // left
    struct aNode *par; // parent
    int val;

    int balanc; // fator de balanceamento
    int height; // altura do nó

} typedef * anode;


// Estrutura da  BST
struct aTree
{
    struct aNode *root; // root
} typedef avlTree;


// -------------------------------------------------------------- //
// FUNÇÕES SUJEITAS A MUDANÇA, NECESSÁRIO ADAPTAR DE PROJETO PARA PROJETO

// Gera um novo node de memória e o retorna 
// É necessário desalocar o espaço
anode __anode_New( int val )
{
    anode ret = (anode) malloc(sizeof(struct aNode));
    ret->dir = NULL;
    ret->esq = NULL;
    ret->par = NULL;
    ret->val = val;
    ret->height = 1;
    ret->balanc = 0; 
    return ret;
}


// Informa se o node é o filho direito de seu pai
// Retorna true se for o direito, ou false se for o esquerdo ou não ter um pai
bool __anode_isRChild(anode target)
{
    if (target->par == NULL)
        return false;

    if (target->par->dir == target)
        return true;

    else
        return false;
}


/// Função que recalcula fatores de balanceamento e alturas dos nós,
/// utilizada no momento da inserção, retorna se está desbalanceado
void __anode_Recalc( anode target)
{
    anode This;
    This = target;
    if (target == NULL)
      return;

    int alturaEsq = 0;
    int alturaDir = 0;

    if ( This->esq )
        alturaEsq = This->esq->height;

    if ( This->dir )
        alturaDir = This->dir->height;

    //recalcula
    This->height = MAX(alturaDir, alturaEsq) + 1; 
    This->balanc = alturaDir - alturaEsq;
}


/// Rotaciona uma subárvore à direita e retorna o nó da subarvore
anode __anode_rotateRight( anode target)
{
    anode node1 = target;
    anode node2 = target->esq;
    anode root = target->par;

    bool dir = __anode_isRChild( target);

    // node 1 deixa de apontar para node 2
    // e aponta para o direito do node 2
    
    node1->esq = node2->dir;

    // se esse nó não é nulo
    if (node1->esq)
        node1->esq->par = node1;

    // node 2 deixa de apontar para direito
    // e aponta para o node 1
    node2->dir = node1;
    node1->par = node2;

    // ajustamos o pai
    // se não é nulo
    if (root)
    {
        if (dir)
        {
            root->dir = node2;
            node2->par = root;
        }

        else
        {
            root->esq = node2;
            node2->par = root;
        }

    }
    __anode_Recalc(node1);
    __anode_Recalc(node2);
    __anode_Recalc(root);
    node2->par = root; 
    return node2;
}


/// Rotaciona uma subárvore à esquerda e retorna o nó da subarvore
anode __anode_rotateLeft( anode target)
{
    anode node1 = target;
    anode node2 = target->dir;
    anode root = target->par;

    bool dir = __anode_isRChild( target);

    // node 1 deixa de apontar para node 2
    // e aponta para o esquerdo do node 2

    node1->dir = node2->esq;

    // se esse nó não é nulo
    if (node1->dir)
        node1->dir->par = node1;

    // node 2 deixa de apontar para esquerdo
    // e aponta para o node 1
    node2->esq = node1;
    node1->par = node2;

    // ajustamos o pai
    // se não é nulo
    if (root)
    {
        if (dir)
        {
            root->dir = node2;
            node2->par = root;
        }

        else
        {
            root->esq = node2;
            node2->par = root;
        }

    }

    __anode_Recalc(node1);
    __anode_Recalc(node2);
    __anode_Recalc(root);
    node2->par = root;
    return node2;
}


/// Ajusta (Otimiza) a árvore em caso de desbalanceamento
void __anode_Balance( anode target)
{
    anode This = target;
    while(This)
    {
        __anode_Recalc(This);
        // desbalanceado a esquerda
        if (This->balanc < -1)
        {
          __anode_Recalc(This->esq);
            // e com a subarvore a direita
            if (This->esq->balanc == 1)
            {
                This->esq = __anode_rotateLeft(This->esq);
            }

            This = __anode_rotateRight(This);
        }

        // desbalanceado a direita
        else if (This->balanc > 1)
        {
          __anode_Recalc(This->dir);
            // e com a subarvore a esquerda
            if (This->dir->balanc == -1)
            {
                __anode_rotateRight(This->dir);
                
            }

            This = __anode_rotateLeft(This);
        }
        This = This->par;
    }
}


// Função auxiliar para a avl_Insert, procura e insere um valor
// Retorna se o valor foi inserido ou se já existe, true se foi inserido
bool __anode_Insert( anode target, int val)
{
    // se o valor for maior
    if ( target->val < val)
    {
        // se houver espaço disp
        if (target->dir == NULL)
        {
            anode newn = __anode_New( val );
            newn->par = target;
            target->dir = newn;
            __anode_Balance( target);
            return true;
        }

        // se não
        else
        {
            return __anode_Insert(target->dir, val);
        }
    }

    // se o valor for menor
    else if ( target->val > val)
    {
        // se houver espaço disp
        if (target->esq == NULL)
        {
            anode newn = __anode_New( val );
            newn->par = target;
            target->esq = newn;
            __anode_Balance( target);
            return true;
        }

        // se não
        else
        {
            return __anode_Insert(target->esq, val);
        }
    }

    // se for =
    else
    {
        return false;
    }
}


// Printa uma árvora ordenadamente a partir de um nó
void __anode_inOrder( anode target)
{
    if (target->esq != NULL)
    {
        __anode_inOrder( target->esq );
    }

    printf("%d, ", target->val);

    if (target->dir != NULL)
    {
        __anode_inOrder( target->dir );
    }
    
    return;
}


// Printa uma árvore de forma gráfica para análise
void __anode_Graphical( anode target, int n)
{
    if (target->dir != NULL)
    {
        __anode_Graphical( target->dir, n+5 );
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
        __anode_Graphical( target->esq, n+5 );
    }
    
    return;
}


// Libera o espaço de memória de um node, deletando-o
// Retorna um nó, se a raiz for removida
anode __anode_Remove(anode target)
{
    if (target == NULL)
        return NULL;

    anode esq = target->esq;
    anode dir = target->dir;
    anode pai = target->par;

    bool rchild = __anode_isRChild(target);

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
anode __anode_minNode(anode target)
{
    anode This = target;
    while(This->esq != NULL)
    {
        This = This->esq;
    }

    return This;
}


// Percorre uma subárvore e retorna o seu maior elemento
anode __anode_maxNode(anode target)
{
    anode This = target;
    while(This->dir != NULL)
    {
        This = This->dir;
    }

    return This;
}


// Retorna o node com o valor descrito, ou null se não existir
anode __anode_Search(anode target, int val)
{
    anode This = target;
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
avlTree avl_New()
{
    avlTree ret;
    ret.root = NULL;
    return ret;
}


// Insere um novo elemento na BST
// Retorna se a inserção do elemento foi um sucesso
bool avl_Insert( avlTree *target, int value)
{
    // se a avl está vazia, inicializa
    if (target->root == NULL) // dando erro aqui por motivos desc
    {
        anode newn = __anode_New(value);
        target->root = newn;

        __anode_Recalc(target->root);
        return true;
    }

    // se a avl já contém valores
    else
    {
        if(__anode_Insert(target->root, value))
        {
            // pegamos a raiz, se alterada
            while(target->root->par != NULL)
            {
                target->root = target->root->par;
            }

            
            return true;
        }

        
        else
            return false;
    }
}


// Printa a árvore ordenadamente
void avl_inOrder( avlTree target)
{
    if (target.root != NULL)
    {
        printf("\n");
        __anode_inOrder( target.root );
        printf(" end");
    }

    else
    {
        printf("\nnull");
    }
}


// Printa um modelo gráfico da árvore
void avl_Graphical( avlTree target)
{
    if (target.root != NULL)
    {
        printf("\n\n");
        __anode_Graphical(target.root, 1);
    }
}


// Realiza um clear na avl, liberando toda sua memória
void avl_Free( avlTree *target)
{
    if ( target->root == NULL )
    {
        return;
    }

    else
    {
        anode This = target->root;
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

                anode aux = This->par;
                __anode_Remove(This);
                
                // remove esse nó
                // se é a raiz
                if (This->par == NULL)
                {
                    target->root = NULL;
                    break;
                }

                This = aux;
            }

        } while (1);

        target->root = NULL;
        return;
    }
}


// Retorna o menor número da BST
int avl_minVal(avlTree target)
{
    if(target.root == NULL)
    {
        return 0;
    }

    anode element = __anode_minNode(target.root);
    return element->val;
}


// Retorna o nó do menor elemento da subárvore
int avl_maxVal(avlTree target)
{
    if (target.root == NULL)
    {
        return 0;
    }

    anode element = __anode_maxNode(target.root);
    return element->val;
}


// Procura por um valor na árvore binária
// Retorna true se o valor existe ou false se o valor não existe
bool avl_Search(avlTree target, int value)
{
    anode found = __anode_Search(target.root, value);

    if (found)
    {
        return true;
    }

    return false;
}


// Procura por um valor na árvore binária e o remove
// Retorna true se o valor existe e foi removido ou false caso contrário
bool avl_Remove( avlTree *target, int value )
{
    anode found = __anode_Search(target->root, value);
    
    if (found)
    {
        anode new_root = __anode_Remove(found);
        
        if (new_root)
        {
            target->root = new_root;
        }

        return true;
    }

    return false;
}


#endif
