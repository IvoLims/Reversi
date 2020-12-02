#include <stdio.h>
#include <ctype.h>
#include "estado.h"


int main()
{
    /* Estado inicial do tabuleiro. Inicio do jogo! */

    ESTADO e = {0};
    int i,j;
    for (i=0;i<8;i++) {
        for (j=0;j<8;j++)
            e.grelha[i][j] = VAZIA;
    }
    e.grelha[3][4] = VALOR_X;
    e.grelha[4][3] = VALOR_X;
    e.grelha[3][3] = VALOR_O;
    e.grelha[4][4] = VALOR_O;
    e.movimentovalido=1;
    e.peca=VAZIA;
    topo = &e;

    /* Menu do jogo que vai encarregar-se de executar as ações desejadas como novo jogo, guardar jogo, ... */

    char comando[20];
    int linha, coluna,FlagValidas=1,flag,hint,m,ct1,ct2;
    do
    {
        if (topo->modo=='A' && topo->peca==topo->PecaBot) {
            if (topo->movimentovalido !=0){
                jogadabot(topo->nivelbot);
                PrintEstado();
                continue;
            }
            Passar(&topo);
            if (topo->movimentovalido !=0){
                printf("O Bot vai passar. Prima Enter para continuar.\n");
                getchar();
                PrintEstado();
                continue;
            }
            else {
                printf("Não há mais movimentos possíveis. Fim de jogo!\n");
                Vencedor();
            }
        }
        flag=0;
        printf("Insira comando (Info: insira I): ");
        gets(comando);
        printf("\n");
        switch (toupper(comando[0])) {
            case 'N':
                if (toupper(comando[2]) != 'X' && toupper(comando[2]) != 'O') {
                    printf("Comando invalido. Prima Enter para continuar.\n\n");
                    getchar();
                    if (topo->peca != VAZIA) PrintEstado();
                    break;
                }
                inserir(&topo);
                topo->peca = PecaToValor(toupper(comando[2]));
                topo->modo = 'M';
                GrelhaNovoJogo(topo);
                CriarValidas(topo);
                topo->movimentovalido = PodeJogar(topo);
                printf("Jogo iniciado com sucesso. Prima Enter para continuar.\n\n");
                getchar();
                PrintEstado();
                break;
            case 'L':
                if (CarregarJogo(&comando[2]) == 1) {
                    printf("Jogo carregado com sucesso. Prima Enter para continuar.\n\n");
                    getchar();
                    PrintEstado();
                    break;
                }
                else {
                    printf("Sem sucesso. Prima Enter para continuar.\n\n");
                    getchar();
                    if (topo->peca != VAZIA)
                        PrintEstado();
                    break;
                }
            case 'E':
                if (topo->peca == VAZIA) {
                    printf("Ainda nao iniciou nenhum jogo.\n\n");
                    break;
                }
                if (GravarJogo(&comando[2]) == 1) {
                    printf("Jogo gravado com sucesso. Prima Enter para continuar.\n\n");
                    getchar();
                    PrintEstado();
                    break;
                }
                else {
                    printf("Sem sucesso. Prima Enter para continuar.\n\n");
                    getchar();
                    PrintEstado();
                    break;
                }
            case 'J':
                if (topo->peca==VAZIA) {
                    printf("Ainda nao iniciou nenhum jogo.\n\n");
                    break;
                }
                linha = comando[2] - '0' - 1;
                coluna = comando[4] - '0' - 1;
                if (linha < 0 || linha >= 8 || coluna < 0 || coluna >= 8 || topo->jogadasvalidas[linha][coluna] == 0){
                    printf("Jogada invalida! Prima Enter para continuar.\n\n");
                    getchar();
                }
                else Jogar(&topo, linha, coluna);
                PrintEstado();
                break;
            case 'P':
                if (topo->peca==VAZIA) {
                    printf("Ainda nao iniciou nenhum jogo.\n\n");
                    break;
                }
                if (topo->movimentovalido == 0) Passar(&topo);
                else {
                    printf("Existem jogadas validas. Nao pode passar! Prima Enter para continuar.\n\n");
                    getchar();
                }
                PrintEstado();
                break;
            case 'S':
                if (topo->peca==VAZIA) {
                    printf("Ainda nao iniciou nenhum jogo.\n\n");
                    break;
                }
                PrintValidas();
                break;
            case 'H':
                if (topo->peca==VAZIA) {
                    printf("Ainda nao iniciou nenhum jogo.\n\n");
                    break;
                }
                hint=1;
                m=PodeJogar(topo);
                if (m==0) break;
                if (FlagValidas>m) FlagValidas=1;
                for(ct1=0;(ct1<8 && hint<=m && flag==0);ct1++){
                    for(ct2=0;(ct2<8 && hint<=m && flag==0);ct2++){
                        if (topo->jogadasvalidas[ct1][ct2]==1) {
                            if (hint==FlagValidas) {
                                PrintSugestao(ct1,ct2);
                                FlagValidas++;
                                flag=1;
                            }
                            else hint++;
                        }
                    }
                }
                break;
            case 'U':
                if (topo->peca==VAZIA) {
                    printf("Ainda nao iniciou nenhum jogo.\n\n");
                    break;
                }
                if (topo->next->next == NULL) {
                    printf("Ja chegou ao inicio da sessao. Prima Enter para continuar.\n\n");
                    getchar();
                }
                else do remover(&topo); while ((topo->movimentovalido == 0 || topo->peca == topo->PecaBot) && topo->next->next != NULL);
                PrintEstado();
                break;
            case 'A':
                if ((toupper(comando[2])!='X' && toupper(comando[2])!='O') || ((comando[4]-'0')<1 || (comando[4]-'0')>3)) {
                    printf("Comando invalido. Prima Enter para continuar.\n\n");
                    getchar();
                    if (topo->peca != VAZIA) PrintEstado();
                    break;
                }
                else {
                    inserir(&topo);
                    topo->peca = VALOR_X;
                    topo->modo = 'A';
                    topo->nivelbot = comando[4] - '0';
                    topo->PecaBot = PecaToValor(toupper(comando[2]));
                    GrelhaNovoJogo(topo);
                    CriarValidas(topo);
                    topo->movimentovalido = PodeJogar(topo);
                    printf("Jogo iniciado com sucesso. Prima Enter para continuar.\n\n");
                    getchar();
                    PrintEstado();
                }
                break;
            case 'I':
                printf("N <Peca> - Novo jogo (Peca e o primeiro a jogar)\n");
                printf("L - Carregar jogo de ficheiro\n");
                printf("E - Guardar estado do jogo em ficheiro\n");
                printf("J <Linha> <Coluna> - Jogar peca na posicao (Linha,Coluna):\n");
                printf("S - Mostrar jogadas validas\n");
                printf("H - Sugerir jogada\n");
                printf("U - Desfazer jogada\n");
                printf("A <Peca> <Nivel> - Novo jogo VS Bot (Peca - peca do Bot; Nivel - grau de dificuldade; Peca X e o primeiro a jogar)\n");
                printf("Q - Sair do jogo\n\n");
                printf("Prima Enter para continuar.");
                getchar();
                printf("\n");
                if (topo->peca != VAZIA) PrintEstado();
                break;
            case 'Q':
                printf("A Sair Do Jogo ...");
                while (topo->next != NULL) remover(&topo);
                break;
            default:
                printf("Comando invalido! Prima Enter para continuar.\n\n");
                getchar();
                if (topo->peca != VAZIA) PrintEstado();
                break;
        }
        if(flag==0) FlagValidas=1;
    }
    while(toupper(comando[0]) != 'Q');


    printf("\n");

    return 0;
}