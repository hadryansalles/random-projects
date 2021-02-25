/*

        Hadryan Salles 2026961
        
        https://youtu.be/uwxLOZvVvNI

*/

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define NOME "COLISÕES"
#define CAMINHO_IMAGEM "bola50.png" //use bola25.png ou bola50.png para a bola com RAIO 25/50 pixeis

/* ------------------------------------------------------------- funções ---------------------------------------------------------*/
typedef struct {

    double x;
    double y;

    double vx;
    double vy;

    int r;

}TipoBola;

void recebeValores(int* LARGURA_TELA, int* ALTURA_TELA, int* N_BOLAS, int* V_MAX);

void modoPendulo(TipoBola* bola, int LARGURA_TELA, int ALTURA_TELA);

void criaJanela( int LARGURA_TELA, int ALTURA_TELA);

void criaBolas(TipoBola* bola, int N_BOLAS, int V_MAX, int V_MIN, int LARGURA_TELA, int ALTURA_TELA);

void texturaBolas();

void renderizaBolas(TipoBola* bola, int N_BOLAS);

void colisaoParede(TipoBola* bola, int N_BOLAS, int LARGURA_TELA, int ALTURA_TELA);

void colisaoBolas(TipoBola* bola, int N_BOLAS);

void atualizaVelocidade(TipoBola* bola, int i, int j);

void atualizaPosicao(TipoBola* bola, int N_BOLAS);

int nasceramJuntas(TipoBola* bola, int i, int j);

int centroDa(int b);

/* --------------------------------------------------------------- variaveis globais -------------------------------------------------*/

int raio;

SDL_Window* janela = NULL;

SDL_Renderer* renderizador = NULL;

SDL_Surface* imagem_bola = NULL;

SDL_Texture* textura_bola = NULL;

SDL_Rect bola_atual;

/* ---------------------------------------------------------------- funçao principal ----------------------------------------------------*/

int main(int argc, char** argv){

    int LARGURA_TELA;
    int ALTURA_TELA;
    int N_BOLAS;
    int V_MAX;
    int V_MIN;

    int rodando = 1;

    SDL_Event fechou;

    srand(time(NULL));

    switch(argc){

        case 1 :
            LARGURA_TELA = 800;
            ALTURA_TELA = 600;
            N_BOLAS = 4;
            V_MAX = 5;
            printf("Primeiro argumento é a LARGURA.\nSegundo argumento é a ALTURA.\nTerceiro argumento é o NUMERO de BOLAS.\nQuarto argumento é a velocidade MAXIMA das BOLAS.\n");
            break;

        case 2 :
            LARGURA_TELA = atoi(argv[1]);
            ALTURA_TELA = 600;
            N_BOLAS = 4;
            V_MAX = 5;
            printf("Primeiro argumento é a LARGURA.\nSegundo argumento é a ALTURA.\nTerceiro argumento é o NUMERO de BOLAS.\nQuarto argumento é a velocidade MAXIMA das BOLAS.\n");
            break;

        case 3 :
            LARGURA_TELA = atoi(argv[1]);
            ALTURA_TELA = atoi(argv[2]);
            N_BOLAS = 4;
            V_MAX = 5;
            printf("Primeiro argumento é a LARGURA.\nSegundo argumento é a ALTURA.\nTerceiro argumento é o NUMERO de BOLAS.\nQuarto argumento é a velocidade MAXIMA das BOLAS.\n");
            break;

        case 4 :
            LARGURA_TELA = atoi(argv[1]);
            ALTURA_TELA = atoi(argv[2]);
            N_BOLAS = atoi(argv[3]);
            V_MAX = 5;
            printf("Primeiro argumento é a LARGURA.\nSegundo argumento é a ALTURA.\nTerceiro argumento é o NUMERO de BOLAS.\nQuarto argumento é a velocidade MAXIMA das BOLAS.\n");
            break;

        default :
            LARGURA_TELA = atoi(argv[1]);
            ALTURA_TELA = atoi(argv[2]);
            N_BOLAS = atoi(argv[3]);
            V_MAX = atoi(argv[4]);
            break;
    }

    if( ALTURA_TELA < 100 || ALTURA_TELA > 3000 ||
        LARGURA_TELA < 100 || LARGURA_TELA > 3000 ||
        N_BOLAS < 1 || N_BOLAS > 10 ||
        V_MAX < 1 || V_MAX > 10 ){

            printf("\nDados Invalidos.\nUtilize valores de\nLARGURA e ALTURA entre 100 e 3000.\nNumero de bolas e velocidade maxima entre 1 e 10.\n");
            return 1;
    }

    V_MIN = V_MAX * (-1);

    TipoBola* bola = (TipoBola*) malloc (sizeof (TipoBola) * N_BOLAS);

    criaJanela(LARGURA_TELA, ALTURA_TELA);

    texturaBolas();

    criaBolas(bola, N_BOLAS, V_MAX, V_MIN, LARGURA_TELA, ALTURA_TELA);

    printf("\nPressione a tecla \"q\" para fechar o programa.\n\n");

    //alma do programa
    while(rodando){

        while(SDL_PollEvent(&fechou) != 0){
            // fecha quando pressiona tecla Q
            if(fechou.key.keysym.sym == SDLK_q)
                rodando = 0;
        }


        // limpa a tela
        SDL_RenderClear (renderizador);

        // define a tela como branco
        SDL_SetRenderDrawColor ( renderizador, 0xFF, 0xFF, 0xFF, 0xFF);

        // imprime bola por bola
        renderizaBolas(bola, N_BOLAS);

        // soma velocidade na posiçao das bolas
        atualizaPosicao(bola, N_BOLAS);

        // checa se tem alguma colisao com parede
        colisaoParede(bola, N_BOLAS, LARGURA_TELA, ALTURA_TELA);

        // checa colisao entre bolas
        colisaoBolas(bola, N_BOLAS);

        // imprime novo render
        SDL_RenderPresent (renderizador);

        double energia_cinetica = 0;

        for(int i = 0; i < N_BOLAS; i++)
            energia_cinetica += (bola[i].vx*bola[i].vx) + (bola[i].vy*bola[i].vy);

        printf("Energia Cinética Total: %.2f\r", energia_cinetica/2);
    }

    //desaloca variaveis
    free(bola);
    SDL_DestroyTexture(textura_bola);
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);

    return (0);
}

/* -------------------------------------------------------------------- funções ------------------------------------------------------- */

//atualiza velocidade apos colisao entre bolas
void atualizaVelocidade (TipoBola* bola, int i, int j) {

    //eixo de colisao
    double colisao_x = centroDa(bola[i].x) - centroDa(bola[j].x);
    double colisao_y = centroDa(bola[i].y) - centroDa(bola[j].y);

    double colisao = colisao_x*colisao_x + colisao_y*colisao_y;

    //velocidades projetadas
    double pvx1 = ((bola[i].vx * colisao_x) + (bola[i].vy * colisao_y)) * colisao_x / colisao;
    double pvy1 = ((bola[i].vx * colisao_x) + (bola[i].vy * colisao_y)) * colisao_y / colisao;
    double pvx2 = ((bola[j].vx * colisao_x) + (bola[j].vy * colisao_y)) * colisao_x / colisao;
    double pvy2 = ((bola[j].vx * colisao_x) + (bola[j].vy * colisao_y)) * colisao_y / colisao;

    //inverte as velocidades que estao no EIXO de colisao
    bola[i].vx -= (pvx1 - pvx2);
    bola[i].vy -= (pvy1 - pvy2);
    bola[j].vx -= (pvx2 - pvx1);
    bola[j].vy -= (pvy2 - pvy1);

    //separa bolas grudadas
    if(colisao_x != 0 && colisao_y != 0){

        //soma ou diminui um pixel na posiçao das bolas
        bola[i].x += colisao_x / abs(colisao_x);
        bola[i].y += colisao_y / abs(colisao_y);

        bola[j].x -= colisao_x / abs(colisao_x);
        bola[j].y -= colisao_y / abs(colisao_y);
    }

}

//confere colisao entre bolas
void colisaoBolas(TipoBola* bola, int N_BOLAS){

    double distancia_centros;
    double x, y;

    for(int i = 0; i < N_BOLAS; i++){

        for(int j = i + 1; j < N_BOLAS; j++){

            x = centroDa(bola[i].x) - centroDa(bola[j].x);
            y = centroDa(bola[i].y) - centroDa(bola[j].y);
            distancia_centros = x*x + y*y;

            if(distancia_centros <= raio*raio*4)
                atualizaVelocidade(bola, i, j);

        }
    }
}

//confere colisao contra a parede
void colisaoParede(TipoBola* bola, int N_BOLAS, int LARGURA_TELA, int ALTURA_TELA){

    for(int i = 0; i < N_BOLAS; i++){

        if((bola[i].x <= 0 && bola[i].vx <= 0 )|| (bola[i].x  + 2 * raio >= LARGURA_TELA && bola[i].vx >= 0))
            bola[i].vx *= -1;

        if((bola[i].y <= 0 && bola[i].vy <= 0 )||( bola[i].y + 2 * raio >= ALTURA_TELA && bola[i].vy >= 0))
            bola[i].vy *= -1;
    }
}

//cria janela e primeira superficie branca
void criaJanela(int LARGURA_TELA, int ALTURA_TELA){

    if(SDL_Init(SDL_INIT_VIDEO) < 0){

        printf("erro abrindo video\n");
    }
    else{

        janela = SDL_CreateWindow(NOME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN);

        if(janela == NULL){

            printf("erro criando a janela\n");
        }
        else{

            renderizador = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            if(renderizador == NULL){

                printf("erro criando renderizador\n");
            }
            else{

                SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }
}

//cria a textura da bola
void texturaBolas(){

    int imgFlags = IMG_INIT_PNG;

    if(!(IMG_Init(imgFlags) & imgFlags)){

        printf( "erro abrindo header imagem\n");
    }
    else{

        imagem_bola = IMG_Load(CAMINHO_IMAGEM);

        if(imagem_bola == NULL){

            printf("erro abrindo imagem\n");
        }
        else{

            raio = imagem_bola->h /2;

            textura_bola = SDL_CreateTextureFromSurface(renderizador, imagem_bola);

            if(textura_bola == NULL){

                printf("erro carregando textura da bola\n");
            }
        }
    }

    SDL_FreeSurface(imagem_bola);
}

// renderiza as bolas em suas respectivas posiçoes
void renderizaBolas(TipoBola* bola, int N_BOLAS){

    for(int i = 0; i < N_BOLAS; i++){

        bola_atual.h = bola[i].r * 2;
        bola_atual.w = bola[i].r * 2;
        bola_atual.x = bola[i].x;
        bola_atual.y = bola[i].y;

        SDL_RenderCopy(renderizador, textura_bola, NULL, &bola_atual);
    }
}

// atualiza posicao para cada loop
void atualizaPosicao(TipoBola* bola, int N_BOLAS){

    for(int i = 0; i < N_BOLAS; i++){

        bola[i].x += bola[i].vx;
        bola[i].y += bola[i].vy;
    }
}

// retorna o centro da bola
int centroDa(int b){

    return (b + raio);
}

//cria as bolas com valores aleatorios
void criaBolas(TipoBola* bola, int N_BOLAS, int V_MAX, int V_MIN, int LARGURA_TELA, int ALTURA_TELA){

    int i, j = 0;

    for(i = 0; i < N_BOLAS; i++){

        bola[i].r = raio;

        bola[i].vx = V_MIN + (rand() % (V_MAX - V_MIN));
        bola[i].vy = V_MIN + (rand() % (V_MAX - V_MIN));

        bola[i].x = 1 + (rand() % (LARGURA_TELA - (raio * 2)));
        bola[i].y = 1 + (rand() % (ALTURA_TELA - (raio * 2)));
    }

    for(i = 0; i  < N_BOLAS; i++){

        for(j = i + 1; j < N_BOLAS; j++){

            if(nasceramJuntas(bola, i, j)){

                bola[i].x = 1 + (rand() % (LARGURA_TELA - (raio * 2)));
                bola[i].y = 1 + (rand() % (ALTURA_TELA - (raio * 2)));

                i = 0;
                j = 1;
            }
        }
    }
}

// checa se as bolas nasceram juntas
int nasceramJuntas(TipoBola* bola, int i, int j){

    double distancia_centros, x, y;

    x = centroDa(bola[i].x) - centroDa(bola[j].x);
    y = centroDa(bola[i].y) - centroDa(bola[j].y);
    distancia_centros = x*x + y*y;

    if(distancia_centros <= 5 + (raio*raio*4))
        return 1;

    return 0;
}

