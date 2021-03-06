/* ------------------------------------------------------------------------------------------------------------------------------------------------------------  

    Universidade Federal De Viçosa - Campus Florestal
    Trabalho prático 1 de Algorítimos e estrutura de dados 2
    
    Professora responsável: 
        Glaucia Braga e Silva
        
    Integrantes do Gupo:
        Caio Oliveira Almeida - 4237
        Guilherme Augusto Schwann Wilke - 4685
        João Vitor Chagas Lobo - 4693

    Arquivo: 
        patricia.c
        Descrição do arquivo: Arquivo de código do TAD árvore patricia;
        Ultima modificação: 27/06 - Por: Guilherme Augusto Schwann Wilke

------------------------------------------------------------------------------------------------------------------------------------------------------------ */

#include "patricia.h"

// Retorna o i-ésimo caractere da palavra, caso esse número exceda o tamanho da palavra, retorna um espaço em branco;
char letra(tipoPalavra k, int i){
    if (strlen(k.valPalavra) < i)
        return ' ';
    return k.valPalavra[i-1];
}

short eExterno(tipoArvore p){
    return (p->nt == externo);
}

//Função que cria nó externo;
tipoArvore criaNoInt(int i, tipoArvore *esq,  tipoArvore *dir){
    // define o nó a ser criado
    tipoArvore p = (tipoArvore)malloc(sizeof(tipoNo));
    p->nt = interno;
    p->no.nInterno.esq = *esq;
    p->no.nInterno.dir = *dir;
    p->no.nInterno.index = i;

    //nó auxiliar para resgatar a palavra mais à esquerda do nó a ser inserido
    tipoArvore aux = (tipoArvore)malloc(sizeof(tipoNo));
    aux = p;
    while (!eExterno(aux)){
        aux = aux->no.nInterno.dir;
    }

    // definição do caractere de diferenciação
    p->no.nInterno.indexLetra = letra(aux->no.palavra, i);
    return p;
} 


tipoArvore criaNoExt(tipoPalavra k){
    // função que gera o nó e define sua natureza (externo)
    tipoArvore p;
    p = (tipoArvore)malloc(sizeof(tipoNo));
    p->nt = externo;
    p->no.palavra = k;
    return p;
}  

tipoArvore insereEntre(tipoPalavra k, tipoArvore *t, int i)
{ tipoArvore p;
  if (eExterno(*t) || i < (*t)->no.nInterno.index){
    // cria um novo no externo
    p = criaNoExt(k);
    if (letra(k, i) == (*t)->no.nInterno.indexLetra) /* caso a i-ésima letra da palavra em questão
    for igual ao caractere (...) de diferenciação, os nós passados para a esquerda e direita do nó a ser
    criado são, respectivamente, o nó na posição de inserção e o nó externo criado para comportar a nova palavra */
        return (criaNoInt(i, t, &p));
    else
        return (criaNoInt(i, &p, t));
  } 
  else{
    if (letra(k, (*t)->no.nInterno.index) == (*t)->no.nInterno.indexLetra)
        (*t)->no.nInterno.dir = insereEntre(k,&(*t)->no.nInterno.dir,i);
    else
        (*t)->no.nInterno.esq = insereEntre(k,&(*t)->no.nInterno.esq,i);
    return (*t);
  }
}

tipoArvore inserePatricia(char* valPalavra, int idDoc, tipoArvore *t){
    tipoPalavra* palavra = (tipoPalavra*) malloc(sizeof(tipoPalavra));
    inicializaPalavra(palavra, valPalavra, idDoc);
    tipoArvore p;
    int i;
    if (*t == NULL) 
        return (criaNoExt(*palavra));
    else {
        p = *t;
        while (!eExterno(p)){
            if (letra(*palavra, p->no.nInterno.index) != p->no.nInterno.indexLetra)
            /* caso a letra de n° index da palavra a ser inserida for igual ao caractere de diferenciação
            a ação é direcionada à subárvore à direita do nó em questão */
                p = p->no.nInterno.esq;
            else
                p = p->no.nInterno.dir;
        }
        /* acha o primeiro bit diferente */
        i = 1;
        while ((i <= tamMaxPalavra) && (letra(*palavra, i) == letra(p->no.palavra, i))){
            i++;
        }
        tipoArvore* auxx = t;
        if (i > tamMaxPalavra){
            while (!eExterno(*auxx)){
            if (letra(*palavra, (*auxx)->no.nInterno.index) != (*auxx)->no.nInterno.indexLetra)
                auxx = &(*auxx)->no.nInterno.esq;
            else
                auxx = &(*auxx)->no.nInterno.dir;
            }
            if(buscaIdDoc(*p->no.palavra.listaPares, idDoc) == 1){
                aumentaQtdePar(&p->no.palavra, idDoc);
                (*auxx)->no.palavra.listaPares = p->no.palavra.listaPares;
            }
            else{
                insereNovoIdDoc(&p->no.palavra, idDoc);
                (*auxx)->no.palavra.listaPares = p->no.palavra.listaPares;
            }
            return (*t);
        } 
        else
            return (insereEntre(*palavra, t, i));
    }
}


tipoPalavra* pesquisaPatricia(char palavra[50], tipoArvore t){
    if (eExterno(t)){
        if (strcmp(palavra, t->no.palavra.valPalavra) == 0)
            return &t->no.palavra;
        else
            return NULL;
    }
    tipoPalavra* aux = (tipoPalavra*) malloc(sizeof(tipoPalavra));
    inicializaPalavra(aux, palavra, 0);
    if (letra(*aux, t->no.nInterno.index) == t->no.nInterno.indexLetra){
        pesquisaPatricia(palavra, t->no.nInterno.dir);
    }
    else{
        pesquisaPatricia(palavra, t->no.nInterno.esq);
    }
}

int quantasPalavrasPatricia(tipoArvore t, int idDoc){
    int num = 0;
    if (eExterno(t)){
        return getQtde(*t->no.palavra.listaPares, idDoc);
    }
    num += quantasPalavrasPatricia(t->no.nInterno.esq, idDoc);
    num += quantasPalavrasPatricia(t->no.nInterno.dir, idDoc);
    return num;
}

void imprimePatriciaI(tipoArvore t){
    /* função recursiva que imprime  */
    if (t == NULL)
        return;
    if (eExterno(t)){
        imprimePalavra(t->no.palavra);
        return;
    }
    imprimePatriciaI(t->no.nInterno.esq);
    imprimePatriciaI(t->no.nInterno.dir);
}

void imprimePatricia(tipoArvore t){
    printf("Árvore patricia:\n");
    imprimePatriciaI(t);
}