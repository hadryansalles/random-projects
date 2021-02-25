#include <stdio.h>
#include <stdlib.h>

#define LARGURA 4
#define ALTURA 4
#define VALOR_MAXIMO 2048

#define SETA_CIMA 'w'
#define SETA_BAIXO 's'
#define SETA_ESQUERDA 'a'
#define SETA_DIREITA 'd'



void atualizaMatriz (int ** matriz, char pressionou);
//move todos os valores na direçao do movimento, se forem dois valores iguais soma 

int novoNumero(int **matriz);
//gera um novo numero em uma posicao aleatoria a cada repitição do while

int** geraMatriz();
//cria a matriz com 2 valores aleatorios em posicoes aleatorias

char recebeTecla();
//recebe a tecla do movimento

int daPraMexer(int** matriz);
//simula possiveis tecladas do usuario para nao acabar o jogo com movimentos possiveis

int posicaoVazia(int** matriz,int* i,int* j);
//retorna uma posiçao vazia aleatoria

void printaMatriz(int** matriz);
//printa a matriz

int ganhou(int** matriz);
//verifica se algum valor é VALOR_MAXIMO

void destroiMatriz(int** matriz);
//da free na matriz


int main()
{

	int over = 0;
	int** matriz = geraMatriz();
	char pressionou;
	int movimentos = 0;

	while (!over)
	{
		printaMatriz(matriz);

		printf("\nNumero de movimentos: %d.\nUse W, A, S, D para mover os numeros.\n", movimentos);
	
		pressionou = NULL;
		while (pressionou == NULL) {
			pressionou = recebeTecla();
		}
		movimentos++;

		atualizaMatriz(matriz, pressionou);

		if (!novoNumero(matriz))
			if (!daPraMexer(matriz))
				over = 1;
		if (ganhou(matriz))
			over = 2;
	}

	destroiMatriz(matriz);

	if (over == 1)
		printf("O jogo acabou, voce perdeu com %d movimentos.\n", movimentos);
	else
		printf("Parabens, voce ganhou com %d movimentos.\n\n", movimentos);
	
	system("pause");
	//esse segundo pause é pra caso o jogador esteja apertando qualquer tecla igual doido
	system("pause");

	return 0;
}

int** geraMatriz()
{
    int i,j,k;
    int** t;
    t = (int**) malloc(sizeof(int*)*LARGURA);
    for (i=0; i<LARGURA; i++)
        t[i] = (int*) malloc(sizeof(int)*LARGURA);
    for (i=0; i<LARGURA; i++)
    {
        for(j=0; j<LARGURA; j++)
            t[i][j]=0;
    }
    for (k=0; k < 2; k++)
    {
        i = rand()%4;
        j = rand()%4;
        t[i][j] = (rand() % 3)*2;
        if (t[i][j] == 0)
            t[i][j]=2;
    }
    return (t);
}

char recebeTecla(){

    char tecla;
	tecla = getch();
    if ( tecla != 'w' && tecla != 's' && tecla != 'a' && tecla != 'd')
        return(NULL);
    return(tecla);
}

int daPraMexer(int** matriz){

   int i, j;


    for(i = 1; i < ALTURA; i++)
    {
        for(j = 0; j < LARGURA; j++)
        {
            if(matriz[i][j] == matriz[i-1][j] && matriz[i][j] != 0)
            {
                return 1;
            }
        }
    }

    for(i = 0; i < ALTURA - 1; i++)
    {
        for(j = 0; j < LARGURA; j++)
        {
            if(matriz[i][j] == matriz[i+1][j] && matriz[i][j] != 0)
            {
                return 1;
            }
        }
    }

    for(i = 0; i < LARGURA - 1; i++)
    {
        for(j = 0; j < ALTURA; j++)
        {
            if(matriz[j][i] == matriz[j][i + 1] && matriz[j][i] != 0)
            {
                return 1;
            }
        }
    }

    for(i = 1; i < LARGURA; i++)
    {
        for(j = 0; j < ALTURA; j++)
        {
            if(matriz[j][i] == matriz[j][i - 1] && matriz[j][i] != 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

int posicaoVazia(int** matriz, int* i, int* j){

	int t[16][2],k,l,aux=0;

	for (k = 0; k < LARGURA; k++) {
		for (l = 0; l < LARGURA; l++) {

			if (matriz[k][l] == 0) {
				t[aux][0] = k;
				t[aux][1] = l;
				aux++;
			}
		}
	}
	
	if(aux==0)
		return(0);

	k = rand() % aux ;

	*i = t[k][0];

	*j = t[k][1];

	return(1);
}

void atualizaMatriz(int ** matriz, char pressionou)
{
	system("cls");

	int i, j;
	int movimentos = 1;

	while (movimentos)
	{
		movimentos = 0;
		if (pressionou == SETA_CIMA)
		{
			for (i = 1; i < ALTURA; i++)
			{
				for (j = 0; j < LARGURA; j++)
				{
					if (matriz[i][j] == matriz[i - 1][j] && matriz[i][j] != 0)
					{
						matriz[i - 1][j] *= 2;
						matriz[i][j] = 0;
						movimentos++;
					}
					if(matriz[i - 1][j] == 0 && matriz[i][j] != 0){
						
						matriz[i - 1][j] = matriz[i][j];
						matriz[i][j] = 0;
						movimentos++;
					}
				}
			}
		}
		else if (pressionou == SETA_BAIXO)
		{
			for (i = 0; i < ALTURA - 1; i++)
			{
				for (j = 0; j < LARGURA; j++)
				{
					if (matriz[i][j] == matriz[i + 1][j] && matriz[i][j] != 0)
					{
						matriz[i + 1][j] *= 2;
						matriz[i][j] = 0;
						movimentos++;
					}
					if (matriz[i + 1][j] == 0 && matriz[i][j] != 0) {

						matriz[i + 1][j] = matriz[i][j];
						matriz[i][j] = 0;
						movimentos++;
					}
				}
			}
		}
		else if (pressionou == SETA_DIREITA)
		{
			for (i = 0; i < LARGURA - 1; i++)
			{
				for (j = 0; j < ALTURA; j++)
				{
					if (matriz[j][i] == matriz[j][i + 1] && matriz[j][i] != 0)
					{
						matriz[j][i + 1] *= 2;
						matriz[j][i] = 0;
						movimentos++;
					}
					if (matriz[j][i + 1] == 0 && matriz[j][i] != 0) {

						matriz[j][i + 1] = matriz[j][i];
						matriz[j][i] = 0;
						movimentos++;
					}
				}
			}
		}
		else if (pressionou == SETA_ESQUERDA)
		{
			for (i = 1; i < LARGURA; i++)
			{
				for (j = 0; j < ALTURA; j++)
				{
					if (matriz[j][i] == matriz[j][i - 1] && matriz[j][i] != 0)
					{
						matriz[j][i - 1] *= 2;
						matriz[j][i] = 0;
						movimentos++;
					}
					if (matriz[j][i - 1] == 0 && matriz[j][i] != 0) {

						matriz[j][i - 1] = matriz[j][i];
						matriz[j][i] = 0;
						movimentos++;
					}
				}
			}
		}
	}
}

void printaMatriz(int** matriz)
{

	int i, j;
	for (i = 0; i < LARGURA; i++)
	{
		for (j = 0; j < ALTURA; j++)
		{
			if (matriz[i][j] != 0)
				printf("%d\t", matriz[i][j]);
			else
				printf("-\t");
		}
		printf("\n");
	}
}

int novoNumero(int **matriz)
{

	int i, j;
	if (!posicaoVazia(matriz, &i, &j))
		return 0;

	matriz[i][j] = 2 * (1 + rand() % 2);
	return (1);
}

int ganhou(int** matriz) {

	int i, j;

	for (i = 0; i < LARGURA; i++)
		for (j = 0; j < ALTURA; j++)
			if (matriz[i][j] == VALOR_MAXIMO)
				return 1;
	return 0;
}

void destroiMatriz(int** matriz) {
	
	int i;
	for (i = 0; i < LARGURA; i++)
		free(matriz[i]);
	free(matriz);
}