#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

void trocarCor(int cor, WORD* Attributes) {
    CONSOLE_SCREEN_BUFFER_INFO Info;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &Info);
    *Attributes = Info.wAttributes;

    switch (cor) {
    case 0:
        SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY);
        break;
    case 1:
        SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
        break;
    case 2:
        SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE);
        break;
    default:
        break;
    }
}

void resetarCor(WORD Attributes) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}

void exibir(char *tent, int res[5]) {
    WORD Attributes = 0;
    trocarCor(3, &Attributes);

    int i;

    for (i=0;i<5;i++) {
        trocarCor(res[i], &Attributes);
        printf("[%c]", tent[i]);
        resetarCor(Attributes);
    }

    printf("\n");
}

int logica(char * tent, char * word) {
    int res[5];
    char wordCopia[6];
    int i, j;

    strcpy(wordCopia, word);

    for (i=0;i<5;i++) {
        res[i] = 0;
    }

    for (i=0;i<5;i++) {
        if (tent[i] == word[i]) {
            res[i] = 1;
        }
    }

    for (i=0;i<5;i++) {
        for (j=0;j<5;j++) {
            if (j != i && tent[i] == wordCopia[j] && res[j] != 1 && res[i] == 0) {
                wordCopia[j] = 0;
                res[i] = 2;
                break;
            }
        }
    }

    exibir(tent, res);

    if (strcmp(tent, word) == 0) {
        return 1;
    }

    return 0;
}

int abrirArquivo(char palavras[5481][6]) {
    FILE *arq;
    char buffer[32];
    int i=0;

    if ((arq = fopen("palavras.txt", "r")) == NULL) {
        printf("Nao foi possivel abrir o arquivo");
        return 0;
    }

    while (fgets(buffer, 32, arq) != NULL) {
        strcpy(palavras[i], buffer);
        palavras[i][5] = '\0';
        i++;
    }

    return 1;
}

int conferir(char tent[128], char palavras[5481][6]) {
    int i;

    if (strlen(tent) != 5) {
        printf("A palavra digitada deve conter 5 letras!\n");
        return 0;
    }

    for (i=0;i<5481;i++) {
        if (strcmp(tent, palavras[i]) == 0) {
            return 1;
        }
    }

    printf("Palavra invalida!\n");

    return 0;
}

void gerarAleatoria(char word[6], char palavras[5481][6]) {
    srand(time(NULL));
    strcpy(word, palavras[rand() % 5481]);
}

int main() {
    char palavras[5481][6];
    char word[6];
    char tent[128];
    int count = 0;

    if (abrirArquivo(palavras) == 0) {
        return 1;
    };

    gerarAleatoria(word, palavras);

    printf("Termo\n\n");
    printf("Uma palavra aleatoria de 5 letras foi gerada! Tente adivinha-la, voce tem 6 tentativas\n");

    while (count < 6) {
        do {
            printf("\nDigite uma palavra: ");
            scanf("%s", tent);
        } while (conferir(tent, palavras) != 1);

        count++;

        if (logica(tent, word) == 1) {
            printf("\nParabens! Voce descobriu a palavra!\n");
            printf("Pressione qualquer tecla para sair\n");
            getch();
            return 0;
        };
    }

    printf("\nTentativas esgotadas! A palavra era: %s\n", word);
    printf("Pressione qualquer tecla para sair\n");
    getch();

    return 0;
}
