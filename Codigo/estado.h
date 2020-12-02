#ifndef PROJ_ESTADO_H
#define PROJ_ESTADO_H



/* estado.h
 * Definição do estado i.e. tabuleiro. Representação matricial do tabuleiro. */


/* Definição de valores possiveis no tabuleiro */

typedef enum {VAZIA, VALOR_X, VALOR_O} VALOR;


/* Estrutura que armazena o estado do jogo */

typedef struct estado {
    VALOR peca;                                  /* Peça do jogador que vai jogar! */
    VALOR grelha[8][8];                          /* Estado do tabuleiro. */
    int jogadasvalidas[8][8];                    /* Matriz com posicoes nas quais peca pode efetuar jogada. */
    int movimentovalido;                         /* Numero de jogadas validas de peca. */
    char modo;                                   /* Modo em que se está a jogar! 0-> manual, 1-> contra computador. */
    int nivelbot;                                /* Nivel do bot, se aplicavel. */
    VALOR PecaBot;                               /* Peca do bot, se aplicavel. */
    struct estado* next;                         /* Apontador para estado (jogada/novo jogo) seguinte. */
} ESTADO;

ESTADO* topo;



// --------------------------------------------------------- Manipulação Stack --------------------------------------------------------- \\

void inserir(ESTADO** x);
void remover(ESTADO** x);



// ------------------------------------------------------- Manipulação Ficheiros ------------------------------------------------------- \\

int CarregarJogo (char *str);
int GravarJogo (char *str);



// -------------------------------------------------------- Funções Acessórias --------------------------------------------------------- \\

VALOR PecaInversa(VALOR x);
int PodeJogar(ESTADO* x);
VALOR PecaToValor(char x);
char ValorToPeca(VALOR x);
void PrintEstado();
void PrintValidas();
void PrintSugestao(int x, int y);
void CopiarGrelha(ESTADO* x);
void GrelhaNovoJogo(ESTADO* x);
int Maximo(int x, int y);
int Minimo(int x, int y);



// ------------------------------------------------------ Análise/Edição Tabuleiro ----------------------------------------------------- \\

int PosicaoLivre(ESTADO* z, int x, int y);
int ContaPeca(ESTADO* x,VALOR y);
void CriarValidas(ESTADO* z);
int Virar(ESTADO* x, int linha, int coluna, int vetorlinha, int vetorcoluna);
void Jogar(ESTADO** z, int linha, int coluna);
void Passar(ESTADO** x);
void Vencedor();
void Fim();
int CasasVazias(ESTADO* x);



// ----------------------------------------------------------- Jogar Vs Bot ------------------------------------------------------------ \\

void jogadabot(int nivel);
int AlphaBeta(int nivel, int alpha, int beta, int bot);


#endif //PROJ_ESTADO_H