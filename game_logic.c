#include "game_logic.h"

/*
    Functie ce generaza un numar
    aleatoriu din multimea {2,4}
*/
int generateRandomNumber() {
    int random_number;
    random_number = rand() % 4 + 1;
    if (random_number % 2 == 1)
        random_number++;
    return random_number;

}

/*
    Functia genereaza si afiseaza tabla de joc.
    Este o functie strict de printare, nimic
    interesant de vazut.
    Aceasta functie este apelata dupa fiecare miscare.
*/
void printBoard() {
    int i, j, z;
    attron(COLOR_PAIR(1));
    time_t currentDate;
    char * c;
    currentDate = time(NULL);
    c = ctime( & currentDate);
    move(0, 0);
    attron(COLOR_PAIR(13));
    printw("Score: %d\n"
        "TIME: %d", score, timeLeft);
    move(0, 20);
    printw("|   Arrow keys / ASDW - to play\n");
    move(1, 20);
    printw("|   U to UNDO\n");
    move(2, 20);
    printw("|   Q to PAUSE\n");
    move(2, 0);
    printw("Press X to automove");
    move(4, 0);
    initColors();
    /* Printeaza casutele si numerele corespunzatoare */
    for (i = 0; i < N; i++) {
        for (z = 0; z < H; z++) {
            for (j = 0; j < N; j++) {
                attron(COLOR_PAIR(getColorCode(BOARD[i][j])));
                if (z == H / 2) {
                    char s[8];
                    snprintf(s, 8, "%d", BOARD[i][j]);
                    int t = L - strlen(s);
                    printw("%*s%s%*s", t - t / 2, "", s, t / 2, "");
                } else
                    printw("       ");
                attroff(COLOR_PAIR(getColorCode(BOARD[i][j])));
                printw("  ");

            }
            printw("\n");
        }
        printw("\n");
    }
    attron(COLOR_PAIR(13));
    printw("Ungureanu Ricardo | 2015\nTime and Date: %s", c);
    refresh();
}

/*
    Functia cauta casute goale in tabla de joc si le numara.
    In cazul in care al 2-lea parametru este 0,
    se alege o casute aleatoriu si aceasta se umple
    cu o valoare returnata de functia generateRandomNumber.
*/
int fillEmptyField(int a, int getOnlySpaces) {
    int i, j;
    int nrOfFreeSpaces = 0;
    int m[10 * 10][2] = {-1,
        -1
    };

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            if (BOARD[i][j] == 0) {
                m[nrOfFreeSpaces][0] = i;
                m[nrOfFreeSpaces][1] = j;
                nrOfFreeSpaces++;
            }

    if (nrOfFreeSpaces == 0) {

        isGameOver();
    }

    if (getOnlySpaces == 1) {

        return nrOfFreeSpaces;
    }

    /* Umple o casuta libera cu valoarea transmisa ca parametru */
    int x = 2;
    x = rand() % nrOfFreeSpaces;

    if (BOARD[m[x][0]][m[x][1]] == 0)
        BOARD[m[x][0]][m[x][1]] = a;

    if (nrOfFreeSpaces == 1)
        isGameOver();

    return;
}

    /*
        Aceasta functie muta toate casutele din
        tabla de joc in sus si face schimbarile
        necesare jocului de 2048
    */
void moveVertically() {
    int i, j, k;
    int stop;
    isMoveValid = 0;
    for (j = 0; j < N; j++) {
        stop = 0;
        for (i = 0; i < N; i++) {
            if (i == 0 || BOARD[i][j] == 0)
                continue;
            else {
                for (k = i - 1; k >= stop; k--) {
                    if (BOARD[i][j] == BOARD[k][j]) {
                        /* Daca 2 casute egale se suprapun
                            ele se aduna */
                        BOARD[k][j] *= 2;
                        score += BOARD[k][j];
                        stop = k + 1;
                        BOARD[i][j] = 0;
                        isMoveValid = 1;
                        break;
                    } else if (BOARD[k][j] == 0 && stop == k) {
                        /*  Daca o casuta este libera
                            ea este ocupata de orice alta
                            casuta nelibera aflata in miscare */
                        BOARD[k][j] = BOARD[i][j];
                        BOARD[i][j] = 0;
                        isMoveValid = 1;
                        break;
                    } else if (BOARD[i][j] != BOARD[k][j] && BOARD[k][j] != 0) {
                        /*  Daca 2 casute contin numere diferite
                            ele nu se pot suprapune */
                        int aux;
                        aux = BOARD[i][j];
                        BOARD[i][j] = 0;
                        BOARD[k + 1][j] = aux;
                        stop = k + 1;
                        if (BOARD[i][j] != aux)
                            isMoveValid = 1;

                    }
                }
            }
        }
    }
}

/*
    Functia roteste tabla de joc in sens orar.
    Functia este folosita pentru a duce tabla
    in pozitie de mutare, iar dupa efectuarea mutarii
    tabla este rotita din nou in starea initiala
*/
void rotateBoard() {
    int i, j;
    for (i = 0; i < N / 2; i++) {
        for (j = i; j < N - i - 1; j++) {
            int aux = BOARD[i][j];
            BOARD[i][j] = BOARD[N - j - 1][i];
            BOARD[N - j - 1][i] = BOARD[N - 1 - i][N - 1 - j];
            BOARD[N - 1 - i][N - 1 - j] = BOARD[j][N - 1 - i];
            BOARD[j][N - 1 - i] = aux;
        }
    }
}

/*
    Primeste ca argument un caracter citit de la tastatura
    si in functie de acesta efectueaza miscarea intr-o
    anumita directie sau afiseaza meniul principal.
    Acesta functie implementeaza contrulul programului cu
    tastatura
*/
void moveDirection(char c) {

    switch (c) {
    case 'w': //UP
        copyBoard(0);
        undoScore = score;
        moveVertically();
        break;
    case 'd': //RIGHT
        copyBoard(0);
        undoScore = score;
        rotateBoard();
        rotateBoard();
        rotateBoard();
        moveVertically();
        rotateBoard();
        break;
    case 's': //DOWN
        copyBoard(0);
        undoScore = score;
        rotateBoard();
        rotateBoard();
        moveVertically();
        rotateBoard();
        rotateBoard();
        break;
    case 'a': //LEFT
        copyBoard(0);
        undoScore = score;
        rotateBoard();
        moveVertically();
        rotateBoard();
        rotateBoard();
        rotateBoard();
        break;
    case 'q': //PAUSE
        pauseGame();
        gameIsOn = 2;
        print_menu();
        isMoveValid = 0;
        break;
    case 'x': //AUTOMOVE
        getEfficientMove();
        break;
    case 'u': //UNDO
        undoMove();
        isMoveValid = 0;
        break;
    default:
        isMoveValid = 0;
    }

}

/*
    Initializeaza paleta de culori ce
    urmeaza a fi folosita pe tot parcursul
    programului pentru afisarea GUI-ului.
*/
void initColors() {

    if (can_change_color()) {
        init_pair(1, 235, 235);
        init_pair(2, COLOR_BLACK, 221);
        init_pair(3, COLOR_BLACK, 226);
        init_pair(4, COLOR_BLACK, 208);
        init_pair(5, COLOR_WHITE, 27);
        init_pair(6, COLOR_WHITE, 19);
        init_pair(7, COLOR_BLACK, 10);
        init_pair(8, COLOR_BLACK, 28);
        init_pair(9, COLOR_WHITE, 125);
        init_pair(10, COLOR_WHITE, 88);
        init_pair(11, COLOR_WHITE, 1);
        init_pair(12, COLOR_WHITE, 3);
        init_pair(13, COLOR_WHITE, COLOR_BLACK);
        init_pair(14, COLOR_WHITE, 2);

    } else {

        init_pair(1, COLOR_WHITE, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_GREEN);
        init_pair(3, COLOR_WHITE, COLOR_YELLOW);
        init_pair(4, COLOR_WHITE, COLOR_BLUE);
        init_pair(5, COLOR_WHITE, COLOR_MAGENTA);
        init_pair(6, COLOR_WHITE, COLOR_CYAN);
        init_pair(7, COLOR_WHITE, COLOR_RED);
        init_pair(8, COLOR_WHITE, COLOR_YELLOW);
        init_pair(9, COLOR_WHITE, COLOR_YELLOW);
        init_pair(10, COLOR_WHITE, COLOR_YELLOW);
        init_pair(11, COLOR_WHITE, COLOR_YELLOW);
        init_pair(12, COLOR_WHITE, COLOR_YELLOW);
        init_pair(13, COLOR_WHITE, COLOR_BLACK);
        init_pair(14, COLOR_WHITE, 2);
    }
}

/*
    Functie ce determina codul culorii
    ce urmeaza a fi folosita pentru desenarea
    casutei, in functie de numarul ce va ocupa
    aceasta casute pe tabla de joc.
*/
int getColorCode(int n) {
    int pow = 1;
    while (n > 1) {
        pow++;
        n /= 2;
    }

    if (pow > 12)
        return 14;
    return pow;
}

/*
    Functie apelata dupa fiecare miscare
    pentru a determina daca mai exista miscari
    valide posibile, sau jocul este terminat
*/
void isGameOver() {
    int i, j;

    for (j = 0; j < N; j++)
        for (i = 0; i < N - 1; i++) {
            if (BOARD[i][j] == BOARD[i + 1][j] || BOARD[j][i] == BOARD[j][i + 1])
                return;
        }
    gameIsOn = 0;
}

/*
    Functie ce initializeaza un nou joc.
    Reseteaza tabla si scorul.
*/
void newGame() {
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            BOARD[i][j] = 0;

    score = 0;
    gameIsOn = 1;
    timeLeft = TIME_LEFT;
    fillEmptyField(generateRandomNumber(), 0);
    fillEmptyField(generateRandomNumber(), 0);
}

/*
    Functie ce pune pauza unui joc.
    Salveaza tabla de joc si score-ul
    intr-un fisier, astfel ca jocul sa poate fi
    reluat si dupa iesirea din program.
    Dupa ce face salvarile necesare, va afisa meniul
*/
void pauseGame() {
    FILE * saveFile;
    int i, j;
    saveFile = fopen(".2048_saved", "w+");
    if (saveFile != NULL) {
        fprintf(saveFile, "%d ", N);
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                fprintf(saveFile, "%d ", BOARD[i][j]);
        fprintf(saveFile, "%d", score);
        gameIsOn = 2;
        fclose(saveFile);
    }
}

/*
    Functia continua un joc inceput anterior.
    Aceasta citeste din fisierul de salvare datele
    necesare si le incarca in variabilele corespunzatoare
    dupa care afiseaza ecranul de joc.
*/
void resumeGame() {

    FILE * saveFile;
    int i, j, k;
    saveFile = fopen(".2048_saved", "r+");
    if (saveFile != NULL && gameIsOn != 0) {
        fscanf(saveFile, "%d", & k);
        allocBoard(k);
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                fscanf(saveFile, "%d", & BOARD[i][j]);
        fscanf(saveFile, "%d", & score);
        gameIsOn = 1;
        fclose(saveFile);
        timeLeft = TIME_LEFT;

    } else
        newGame();
}

/*
    Functia determina miscarea optima
    ce urmeaza a fi facuta automat dupa
    TIME_LEFT de inactivitate sau la cererea
    jucatorului. Functia incearca o miscare
    in toate cele 4 directii si o gaseste pe aceea
    ce elibereaza cat mai multe casute
*/
void getEfficientMove() {
    int max = 0, i = 0;
    char bestMove;
    char directionChars[] = {
        'a',
        's',
        'd',
        'w'
    };

    for (i = 0; i < 4; i++) {
        moveDirection(directionChars[i]);
        if (isMoveValid == 0) continue;
        int b = fillEmptyField(0, 1);
        if (b > max)
            max = b, bestMove = directionChars[i];
        undoMove();
    }

    moveDirection(bestMove);
}

/*
    Functia face undo la o miscare realizata
    si revine la miscarea anterioara
*/
void undoMove() {
    copyBoard(1);
    score = undoScore;
    timeLeft = TIME_LEFT;
    printBoard();
}

/*
    Functia copiaza matricea de joc in alta.
    Acesta functie este apelata inaintea executarii oricarei
    miscari, astfel ca jucatorul sa poate facea reveni la
    configuratia anterioara a tablei de joc.
*/
void copyBoard(int reverse) {
    int i, j;
    for (j = 0; j < N; j++)
        for (i = 0; i < N; i++)
            if (!reverse)
                UNDO_BOARD[i][j] = BOARD[i][j];
            else
                BOARD[i][j] = UNDO_BOARD[i][j];
}

/*
    Aloca memorie dinamica pentru variabila
    matricei de joc. Parametru primit reprezinta
    dimensiunea dorita a matricei (Default: 4)
*/
void allocBoard(int a) {
    if (N != 0 ) freeBoard();
    N = a;
    BOARD = calloc(N, sizeof(unsigned int * ));
    UNDO_BOARD = calloc(N, sizeof(unsigned int * ));
    int i = 0;
    for (i = 0; i < N; i++) {
        BOARD[i] = calloc(N, sizeof(unsigned int));
        UNDO_BOARD[i] = calloc(N, sizeof(unsigned int));
    }
}

/*
    Elibereaza zonele de memorie unde este matricea
    de joc alocata pentru a evita eventuale
    memory leaks
*/
void freeBoard() {
    int i = 0;
    for (i = 0; i < N; i++) {
        free(BOARD[i]);
        free(UNDO_BOARD[i]);
    }

    free(BOARD);
    free(UNDO_BOARD);

}
