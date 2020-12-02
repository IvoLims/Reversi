#include <stdio.h>
#include "estado.h"
#include <stdlib.h>





// --------------------------------------------------------- Manipulação Stack --------------------------------------------------------- \\

/* Aloca memória para criar estados de jogo novos, sempre que os
 * jogadores formalizem uma jogada válida ou iniciem novo jogo. */

void inserir(ESTADO** x) {
    ESTADO* temp;
    temp = (ESTADO*)malloc(sizeof(ESTADO));
    temp->next = (*x);
    (*x) = temp;
}


/* Utilizada pela funcionalidade Undo, pois vai fazer o inverso da inserir, apagando o estado atual do jogo.
 * Permite eliminar jogadas e inícios de novos jogos até ao iníco da sessão. */

void remover(ESTADO** x) {
    ESTADO* temp;
    if ((*x)->next == NULL) printf("Ja chegou ao inicio do jogo.\n\n");
    else {
        temp = *x;
        *x = (*x)->next;
        free(temp);
    }
}

// --------------------------------------------------------- Manipulação Stack --------------------------------------------------------- \\





// ------------------------------------------------------- Manipulação Ficheiros ------------------------------------------------------- \\

/* Carrega um (estado de) jogo de um ficheiro e permite continuar esse jogo. */

int CarregarJogo (char *str) {
    FILE *f;
    f = fopen(str,"r");
    if(f == NULL)
        return 0;

    inserir(&topo);
    char t;
    fscanf(f,"%c %c", &(topo->modo), &t);
    topo->peca = PecaToValor(t);

    if (topo->modo == 'A') {
        topo->PecaBot=PecaInversa(topo->peca);
        fscanf(f," %c\n",&t);
        topo->nivelbot= t - '0';
    }
    else fscanf(f,"\n");

    fscanf(f, " %c", &t);
    int i,j;
    for(i=0 ; i<8 ; i++) {
        for(j=0 ; j<8 ; j++) {
            topo->grelha[i][j]=PecaToValor(t);
            fscanf(f, " %c", &t);
        }
    }
    fclose(f);

    CriarValidas(topo);
    topo->movimentovalido = PodeJogar(topo);
    return 1;
}


/* Guarda um (estado de) jogo num ficheiro. */

int GravarJogo (char *str) {
    FILE *f;
    f = fopen(str,"w");
    if(f == NULL)
        return 0;

    if (topo->modo == 'A') fprintf(f,"%c %c %d\n",topo->modo,ValorToPeca(topo->peca),topo->nivelbot);
    else fprintf(f,"%c %c\n",topo->modo,ValorToPeca(topo->peca));

    int i,j;
    for (i=0;i<8;i++) {
        for (j=0; j<8; j++) {
            fprintf(f, "%c ", ValorToPeca(topo->grelha[i][j]));
        }
        fprintf(f,"\n");
    }
    fclose(f);
    return 1;
}

// ------------------------------------------------------- Manipulação Ficheiros ------------------------------------------------------- \\





// -------------------------------------------------------- Funções Acessórias --------------------------------------------------------- \\

/* Determina a peça que está à espera do seu turno. */

VALOR PecaInversa(VALOR x) {
    if (x == VALOR_X)
        return VALOR_O;
    else
        return VALOR_X;
}


/* Determina se o jogador tem (e quantos) movimentos válidos, retornando esse valor. */

int PodeJogar(ESTADO* x) {
    int i,j,t=0;
    for (i=0;i<8;i++) {
        for (j=0;j<8;j++)
            if (x->jogadasvalidas[i][j]>0) t++;
    }
    return t;
}


/* Converte as peças (símbolos) nos valores definidos (VALOR_0, ...). */

VALOR PecaToValor(char x) {
    if (x=='O') return VALOR_O;
    if (x=='X') return VALOR_X;
    if (x=='-') return VAZIA;
}


/* Converte valores definidos (VALOR_0, ...) nas peças (símbolos). */

char ValorToPeca(VALOR x) {
    if (x==VALOR_O) return 'O';
    if (x==VALOR_X) return 'X';
    if (x==VAZIA) return '-';
}


/*  Imprime o estado atual do tabuleiro. */

void PrintEstado() {
    char c;
    printf("  1 2 3 4 5 6 7 8\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ",i+1);
        for (int j = 0; j < 8; j++) {
            c = ValorToPeca(topo->grelha[i][j]);
            printf("%c ", c);
        }
        printf("%d ",i+1);
        printf("\n");
    }
    printf("  1 2 3 4 5 6 7 8\n\n");
    printf("X: %d \t O:%d \n\n",ContaPeca(topo,VALOR_X),ContaPeca(topo,VALOR_O));
    Fim();
}


/*  Imprime o estado atual do tabuleiro incluindo as jogadas válidas para o jogador com a vez. */

void PrintValidas() {
    char c;
    printf("  1 2 3 4 5 6 7 8\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ",i+1);
        for (int j = 0; j < 8; j++) {
            if (topo->jogadasvalidas[i][j]==1) c = '.';
            else c = ValorToPeca(topo->grelha[i][j]);
            printf("%c ", c);
        }
        printf("%d ",i+1);
        printf("\n");
    }
    printf("  1 2 3 4 5 6 7 8\n\n");
    printf("X: %d \t O: %d \n\n",ContaPeca(topo,VALOR_X),ContaPeca(topo,VALOR_O));
    Fim();
}


/*  Imprime o estado atual do tabuleiro incluindo uma sugestão de jogada. */

void PrintSugestao(int x, int y) {
    char c;
    printf("  1 2 3 4 5 6 7 8\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ",i+1);
        for (int j = 0; j < 8; j++) {
            if (i==x && j==y) c='?';
            else c = ValorToPeca(topo->grelha[i][j]);
            printf("%c ", c);
        }
        printf("%d ",i+1);
        printf("\n");
    }
    printf("  1 2 3 4 5 6 7 8\n\n");
    printf("X: %d \t O:%d \n\n",ContaPeca(topo,VALOR_X),ContaPeca(topo,VALOR_O));
    Fim();
}


/*  Copia o tabuleiro do estado de jogo anterior para o estado de jogo atual.
 *  Utilizada nas funções passar e jogar, para posteriormente serem efetuadas as devidas alterações. */

void CopiarGrelha(ESTADO* x) {
    int i,j;
    for (i=0;i<8;++i) {
        for (j=0;j<8;++j) {
            x->grelha[i][j]=x->next->grelha[i][j];
        }

    }
}


/* Cria um tabuleiro com o estado inicial de um jogo novo. */

void GrelhaNovoJogo(ESTADO* x){
    int i,j;
    for (i=0;i<8;i++) {
        for (j=0;j<8;j++)
            x->grelha[i][j] = VAZIA;
    }
    x->grelha[3][4] = VALOR_X;
    x->grelha[4][3] = VALOR_X;
    x->grelha[3][3] = VALOR_O;
    x->grelha[4][4] = VALOR_O;
}



int Maximo(int x, int y){
    if (x>y) return x;
    else return y;
}



int Minimo(int x, int y){
    if (x<y) return x;
    else return y;
}

// -------------------------------------------------------- Funções Acessórias --------------------------------------------------------- \\





// ------------------------------------------------------ Análise/Edição Tabuleiro ----------------------------------------------------- \\

/* Determina se uma determinada posição no tabuleiro está livre. */

int PosicaoLivre(ESTADO* z, int x, int y) {
    if (z->grelha[x][y]== VAZIA) return 1;
    else return 0;
}


/* Determina o nº de ocorrências de determinada peça no tabuleiro. */

int ContaPeca(ESTADO* x,VALOR y) {
    int i,j,t=0;
    for (i=0;i<8;i++) {
        for (j=0;j<8;j++)
            if (x->grelha[i][j]==y) t++;
    }
    return t;
}


/* Calcula uma matriz com as posições nas quais o jogador
 * com a vez de jogar pode realizar uma jogada válida. */

void CriarValidas(ESTADO* z) {
    int i,j;
    for (i=0;i<8;i++) {
        for (j=0;j<8;j++)
            z->jogadasvalidas[i][j]=0;
    }

    for (i=0;i<8;i++) {
        for (j=0;j<8;j++) {
            if (PosicaoLivre(z,i,j)==0) continue;
            int x,y,k;
            for (x=-1;x<=1;x++){
                for (y=-1;y<=1;y++) {
                    if (i+x<0 || i+x>=8 || j+y<0 || j+y>=8) continue;
                    if (z->grelha[i+x][j+y]==PecaInversa(z->peca)) {
                        for (k=2; (i+k*x>=0 && i+k*x<8 && j+k*y>=0 && j+k*y<8);k++) {
                            if (z->grelha[i+k*x][j+k*y]==z->peca) {
                                z->jogadasvalidas[i][j]=1;
                                break;
                            }
                            else
                                if (z->grelha[i+k*x][j+k*y]==PecaInversa(z->peca)) continue;
                                else break;
                        }
                    }
                    if (z->jogadasvalidas[i][j]==1) break;
                }
                if (z->jogadasvalidas[i][j]==1) break;
            }
        }
    }
}


/* Vira as peças do adversário numa determinada direção.
 * Função recursiva que é utilizada na função Jogar. */

int Virar(ESTADO* x, int linha, int coluna, int vetorlinha, int vetorcoluna) {
    int a,b;
    a=linha+vetorlinha;
    b=coluna+vetorcoluna;
    if (a < 0 || a >= 8 || b < 0 || b >= 8) return 0;
    else
        if (x->grelha[a][b]==VAZIA) return 0;
        else
            if (x->grelha[a][b]==x->peca) return 1;
            else
                if (x->grelha[a][b]==PecaInversa(x->peca)) {
                    if (Virar(x,a,b,vetorlinha,vetorcoluna)==1) {
                        x->grelha[a][b]=x->peca;
                        return 1;
                    }
                    else return 0;
                }
}


/* Efetua uma jogada válida, colocando a peça no posição escolhida
 * e virando as peças do adversário com o auxílio da função Virar. */

void Jogar(ESTADO** z, int linha, int coluna) {
    inserir(z);
    (*z)->peca = (*z)->next->peca;
    (*z)->modo = (*z)->next->modo;
    (*z)->PecaBot = (*z)->next->PecaBot;
    (*z)->nivelbot=(*z)->next->nivelbot;
    CopiarGrelha(*z);
    (*z)->grelha[linha][coluna]=(*z)->peca;
    int x, y;
    for (x = -1; x <= 1; x++) {
        for (y = -1; y <= 1; y++) {
            Virar((*z),linha,coluna,x,y);
        }
    }
    (*z)->peca = PecaInversa((*z)->peca);
    CriarValidas(*z);
    (*z)->movimentovalido=PodeJogar(*z);
}


/* Permite passar a vez ao adversário. */

void Passar(ESTADO** x) {
    inserir(x);
    (*x)->peca = PecaInversa((*x)->next->peca);
    (*x)->modo = (*x)->next->modo;
    (*x)->PecaBot = (*x)->next->PecaBot;
    (*x)->nivelbot = (*x)->next->nivelbot;
    CopiarGrelha(*x);
    CriarValidas(*x);
    (*x)->movimentovalido=PodeJogar(*x);
}


/* Com o auxílio da função ContaPeca permite decidir qual o vencedor do jogo. */

void Vencedor() {
    int x;
    x = ContaPeca(topo,VALOR_O)-ContaPeca(topo,VALOR_X);
    if (x==0) printf("Empate!\n\n");
    if (x<0) printf("O jogador com a peca X venceu!\n\n");
    if (x>0) printf("O jogador com a peca O venceu!\n\n");
}


/* Avalia se estamos na presença de uma situação de fim de jogo,
 * verificando se cada jogador tem jogadas válidas. */

void Fim() {
    if (topo->movimentovalido==0 && topo->next!=NULL) {
        Passar(&topo);
        if (topo->movimentovalido!=0) {
            remover(&topo);
            printf("E a vez do jogador com a peca %c.\nO jogador nao tem jogadas possiveis. Insira P para passar.\n\n",ValorToPeca(topo->peca));
        }
        else
            if (topo->movimentovalido==0) {
                remover(&topo);
                printf("Nao ha mais movimentos possiveis. Fim de jogo!\n");
                Vencedor();
            }
            else {
                remover(&topo);
            }
    } else {
        printf("E a vez do jogador com a peca %c.\n\n",ValorToPeca(topo->peca));
    }
}

/* Conta o numero de casas vazias no tabuleiro. */

int CasasVazias(ESTADO* x){
    int i,j,k=0;
    for (i=0;i<8;i++) {
        for (j=0;j<8;j++)
            if (x->grelha[i][j]==VAZIA) k++;
    }
    return k;
}

// ------------------------------------------------------ Análise/Edição Tabuleiro ----------------------------------------------------- \\





// ----------------------------------------------------------- Jogar Vs Bot ------------------------------------------------------------ \\

void jogadabot(int nivel) {
    int i, j, x, y, k, heur, Alpha, Beta;
    heur = -100;
    Alpha = -100;
    Beta = 100;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (topo->jogadasvalidas[i][j] == 1) {                  /* Verificar se existem jogadas válidas */
                Jogar(&topo, i, j);
                k = AlphaBeta(nivel*(nivel - 1), Alpha, Beta, 0);
                remover(&topo);

                /* Nesta parte o tabuleiro foi dividido em 4 regiões tal como a figura apresenta: http://mnemstudio.org/ai/game/images/reversi_zones1.gif .
                 * Atribuindo um valor numérico a cada região exceto à verde*/

                if (CasasVazias(topo)>((nivel*(nivel-1))+1)) {
                    if ((i==0 && j==0)||(i==0 && j==7)||(i==7 && j==0)||(i==7 && j==7))                                     /* Região amarela */
                        k=k+(nivel*nivel*2);                                                                                    /* Valor heuristico da posição = Valor heuristico + (Nível do Bot * Nível do Bot *2 ) */
                    if ((i==0 && j==1)||(i==0 && j==6)||(i==1 && j==0)||(i==1 && j==1)||(i==1 && j==6)||(i==1 && j==7)      /* Região vermelha */
                      ||(i==7 && j==1)||(i==7 && j==6)||(i==6 && j==0)||(i==6 && j==1)||(i==6 && j==6)||(i==6 && j==7))
                        k=k-(nivel*nivel*2);                                                                                    /* = ao anterior mas subtrai em vez de somar */
                    if ((i==0 && (j>1 && j<6))||(i==7 && (j>1 && j<6))||(j==0 && (i>1 && i<6))||(j==7 && (i>1 && i<6)))     /* Região azul claro */
                        k=k+(nivel*nivel);                                                                                      /* Valor heuristico da posição = Valor heuristico + (Nível do Bot * Nível do Bot) */
                    if ((i==1 && (j>1 && j<6))||(i==6 && (j>1 && j<6))||(j==1 && (i>1 && i<6))||(j==6 && (i>1 && i<6)))     /* Região azul escuro */
                        k=k-(nivel*nivel);                                                                                      /* = ao anterior mas subtrai em vez de somar */
                }

                /* Atualização do valor da heuristica, se aplicavel */

                if (k > heur) {
                    heur = k;
                    x = i;
                    y = j;
                }
            }
        }
    }
    printf("O Bot vai jogar na posicao %d:%d. Prima Enter para continuar.\n",x+1,y+1);
    getchar();
    Jogar(&topo,x,y);
}

/* Função que se vai encarregar de calcular a jogada que trará maior vantagem ao bot.
 * Aplicação direta da função clássica AlphaBeta Pruning */

int AlphaBeta(int nivel, int alpha, int beta, int bot){
    if (nivel==0)                                                                                    // Como é nível 0 estamos no ultimo nivel da arvore, logo tem de se retornar
        return ((ContaPeca(topo,topo->PecaBot))-(ContaPeca(topo,PecaInversa(topo->PecaBot))));       // o valor heuristico desse nodo. (Num Pecas Bot - Num Pecas Jogador)
    int valor,i,j,k;
    if (bot==1) {                                      // Se for o jogador a maximizar pontuaçao, neste caso o bot
        valor=-100;
        if (topo->movimentovalido==0){                 // Detetar se há jogadas válidas. 0 == Não existem
            Passar(&topo);                             // Se não existir tem de passar.
            k = AlphaBeta(nivel-1,alpha,beta,0);
            valor = Maximo(valor,k);                   // Uma vez que tem de passar, o codigo poderia ser substituido por valor = k
            remover(&topo);
        }
        else {
            for (i = 0; i < 8; i++) {                 // Movimentação entre os nodos do mesmo nivel
                for (j = 0; j < 8; j++) {
                    if (topo->jogadasvalidas[i][j] == 1) {
                        Jogar(&topo, i, j);
                        k = AlphaBeta(nivel - 1, alpha, beta, 0);
                        valor = Maximo(valor, k);     // A variavel "valor" será o maior valor entre cada chamada da funçao alphabeta para os nodos filhos.
                        alpha = Maximo(alpha, valor); // Atualização do alpha
                        remover(&topo);
                        if (alpha >= beta) break;      // Se tiver encontrado um alpha >= beta ja não é necessário considerar mais nodos filhos
                    }                                  // Caso contrário vai ter de continuar a avaliar //
                }
                if (alpha >= beta) break;
            }
        }
        return valor;                                // Retorna o melhor valor encontrado
    }
    else {                                          // O mesmo principio que em cima mas para o jogador a minimizar, ou seja, para o adversario do bot //
        valor=100;
        if (topo->movimentovalido==0){
            Passar(&topo);
            k = AlphaBeta(nivel-1,alpha,beta,1);
            valor = Minimo(valor,k);
            remover(&topo);
        }
        else {
            for (i = 0; i < 8; i++) {
                for (j = 0; j < 8; j++) {
                    if(topo->jogadasvalidas[i][j]==1){
                        Jogar(&topo,i,j);
                        k = AlphaBeta(nivel-1,alpha,beta,1);
                        valor = Minimo(valor,k);    // A variavel "valor" será o menor valor entre cada chamada da funçao alphabeta para os nodos filhos.
                        beta = Minimo(beta,valor);  // Atualização do beta
                        remover(&topo);
                        if (alpha>=beta) break;
                    }
                }
                if (alpha>=beta) break;
            }
        }
        return valor;
    }
}

// ----------------------------------------------------------- Jogar Vs Bot ------------------------------------------------------------ \\