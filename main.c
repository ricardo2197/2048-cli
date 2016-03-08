#include "menu.h"
#include "game_logic.h"

int main() {

    /* initializing curse */
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    srand(time(NULL));
    N=0;
    allocBoard(4);
    gameIsOn = -1;
    timeLeft = TIME_LEFT;

    /* initializare culori si printare meniu */
    start_color();
    print_menu();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    attron(COLOR_PAIR(1));
    int nfds, sel;
    fd_set read_descriptors;
    struct timeval timeout;

    nfds = 1;
    FD_ZERO( & read_descriptors);
    copyBoard(0);

    /* while-ul principal al jocului */
    while (gameIsOn >= 0) {
        clear();
        printBoard();
        refresh();
        fflush(stdin);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        /* waiting for input or time to run out */
        FD_SET(0, & read_descriptors);
        int c;
        sel = select(nfds, & read_descriptors, NULL, NULL, & timeout);
        switch (sel) {
        case 1: // KEYBOARD EVENT
            c = wgetch(stdscr);
            switch (c) {
            case KEY_DOWN:
                moveDirection('s');
                break;
            case KEY_UP:
                moveDirection('w');
                break;
            case KEY_LEFT:
                moveDirection('a');
                break;
            case KEY_RIGHT:
                moveDirection('d');
                break;
            default:
                moveDirection(c);
            }
            clear();
            printBoard();

            /* intarziem casutei random pentru a produce o animatie */
            if (isMoveValid == 1) {
                usleep(150000);
                fillEmptyField(generateRandomNumber(), 0);
                clear();
                printBoard();
            }
            timeLeft = TIME_LEFT;
            break;

        case 0: // TIMEOUT
            clear();
            printBoard();
            refresh();

            /*implementam miscarea automata */
            if (timeLeft) timeLeft--;
            else {
                timeLeft = TIME_LEFT;
                getEfficientMove();
                clear();
                printBoard();
                refresh();
                if (isMoveValid == 1) {
                    usleep(150000);
                    fillEmptyField(generateRandomNumber(), 0);
                    clear();
                    printBoard();
                }
            }
            break;
        }

        /* In cazul in care game is over, afisam mesajul de END */
        if (gameIsOn == 0) {
            attron(A_BLINK | A_BOLD | A_STANDOUT | COLOR_PAIR(2));
            mvprintw(LINES / 2, 5, "GAME OVER - Your score: %d\n     Press any key...", score);
            refresh();
            fflush(stdin);
            sleep(3);
            attron(COLOR_PAIR(1));
            attroff(A_BLINK | A_BOLD | A_STANDOUT);
            getch();
            clear();
            print_menu();
        }
    }
    endwin();
    return 0;
}
