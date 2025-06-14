#include <stdio.h>
#include <stdlib.h>

#define TAM 10
#define TAM_NAVIO 3
#define TAM_HAB 5 // Tamanho das matrizes de habilidade (5x5)

// Função para imprimir o tabuleiro com símbolos
void imprime_tabuleiro(int tabuleiro[TAM][TAM]) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == 0)
                printf("~ "); // Água
            else if (tabuleiro[i][j] == 3)
                printf("N "); // Navio
            else if (tabuleiro[i][j] == 5)
                printf("* "); // Área de habilidade
            else
                printf("? "); // Outro (não esperado)
        }
        printf("\n");
    }
}

// Função para construir matriz cone (ponta para baixo)
void matriz_cone(int mat[TAM_HAB][TAM_HAB]) {
    for (int i = 0; i < TAM_HAB; i++) {
        for (int j = 0; j < TAM_HAB; j++) {
            if (i >= abs(j - TAM_HAB/2))
                mat[i][j] = 1;
            else
                mat[i][j] = 0;
        }
    }
}

// Função para construir matriz cruz
void matriz_cruz(int mat[TAM_HAB][TAM_HAB]) {
    for (int i = 0; i < TAM_HAB; i++) {
        for (int j = 0; j < TAM_HAB; j++) {
            if (i == TAM_HAB/2 || j == TAM_HAB/2)
                mat[i][j] = 1;
            else
                mat[i][j] = 0;
        }
    }
}

// Função para construir matriz octaedro (losango)
void matriz_octaedro(int mat[TAM_HAB][TAM_HAB]) {
    for (int i = 0; i < TAM_HAB; i++) {
        for (int j = 0; j < TAM_HAB; j++) {
            if (abs(i - TAM_HAB/2) + abs(j - TAM_HAB/2) <= TAM_HAB/2)
                mat[i][j] = 1;
            else
                mat[i][j] = 0;
        }
    }
}

// Função para sobrepor matriz de habilidade ao tabuleiro
void aplica_habilidade(int tabuleiro[TAM][TAM], int mat[TAM_HAB][TAM_HAB], int origem_l, int origem_c) {
    int offset = TAM_HAB / 2;
    for (int i = 0; i < TAM_HAB; i++) {
        for (int j = 0; j < TAM_HAB; j++) {
            int l = origem_l + i - offset;
            int c = origem_c + j - offset;
            if (l >= 0 && l < TAM && c >= 0 && c < TAM) {
                if (mat[i][j] == 1 && tabuleiro[l][c] == 0)
                    tabuleiro[l][c] = 5;
            }
        }
    }
}

int main() {
    int tabuleiro[TAM][TAM] = {0};

    // Posicionamento dos navios (igual ao seu código)
    int l_h = 1, c_h = 2; // Horizontal
    int l_v = 5, c_v = 7; // Vertical
    int l_d1 = 0, c_d1 = 0; // Diagonal principal (↘)
    int l_d2 = 7, c_d2 = 2; // Diagonal secundária (↙)

    // Validação dos limites (igual ao seu código)
    if (l_h < 0 || l_h >= TAM || c_h < 0 || c_h + TAM_NAVIO - 1 >= TAM) {
        printf("Coordenadas inválidas para navio horizontal.\n");
        return 1;
    }
    if (l_v < 0 || l_v + TAM_NAVIO - 1 >= TAM || c_v < 0 || c_v >= TAM) {
        printf("Coordenadas inválidas para navio vertical.\n");
        return 1;
    }
    if (l_d1 < 0 || l_d1 + TAM_NAVIO - 1 >= TAM || c_d1 < 0 || c_d1 + TAM_NAVIO - 1 >= TAM) {
        printf("Coordenadas inválidas para navio diagonal principal.\n");
        return 1;
    }
    if (l_d2 < 0 || l_d2 + TAM_NAVIO - 1 >= TAM || c_d2 - (TAM_NAVIO - 1) < 0 || c_d2 >= TAM) {
        printf("Coordenadas inválidas para navio diagonal secundária.\n");
        return 1;
    }

    // Checagem de sobreposição (igual ao seu código)
    int ocupado[TAM][TAM] = {0};
    int sobreposicao = 0;
    for (int i = 0; i < TAM_NAVIO; i++) {
        if (ocupado[l_h][c_h + i]) sobreposicao = 1;
        ocupado[l_h][c_h + i] = 1;
    }
    for (int i = 0; i < TAM_NAVIO; i++) {
        if (ocupado[l_v + i][c_v]) sobreposicao = 1;
        ocupado[l_v + i][c_v] = 1;
    }
    for (int i = 0; i < TAM_NAVIO; i++) {
        if (ocupado[l_d1 + i][c_d1 + i]) sobreposicao = 1;
        ocupado[l_d1 + i][c_d1 + i] = 1;
    }
    for (int i = 0; i < TAM_NAVIO; i++) {
        if (ocupado[l_d2 + i][c_d2 - i]) sobreposicao = 1;
        ocupado[l_d2 + i][c_d2 - i] = 1;
    }
    if (sobreposicao) {
        printf("Os navios se sobrepõem. Escolha outras coordenadas.\n");
        return 1;
    }

    // Posiciona navios no tabuleiro
    for (int i = 0; i < TAM_NAVIO; i++) {
        tabuleiro[l_h][c_h + i] = 3; // Horizontal
        tabuleiro[l_v + i][c_v] = 3; // Vertical
        tabuleiro[l_d1 + i][c_d1 + i] = 3; // Diagonal principal
        tabuleiro[l_d2 + i][c_d2 - i] = 3; // Diagonal secundária
    }

    // Matrizes de habilidade
    int cone[TAM_HAB][TAM_HAB], cruz[TAM_HAB][TAM_HAB], octaedro[TAM_HAB][TAM_HAB];
    matriz_cone(cone);
    matriz_cruz(cruz);
    matriz_octaedro(octaedro);

    // Pontos de origem das habilidades no tabuleiro
    int origem_cone_l = 2, origem_cone_c = 2;
    int origem_cruz_l = 7, origem_cruz_c = 7;
    int origem_octaedro_l = 5, origem_octaedro_c = 3;

    // Aplica habilidades ao tabuleiro
    aplica_habilidade(tabuleiro, cone, origem_cone_l, origem_cone_c);
    aplica_habilidade(tabuleiro, cruz, origem_cruz_l, origem_cruz_c);
    aplica_habilidade(tabuleiro, octaedro, origem_octaedro_l, origem_octaedro_c);

    // Exibe o tabuleiro
    printf("Tabuleiro:\n");
    imprime_tabuleiro(tabuleiro);

    return 0;
}
