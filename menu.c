#include "menu.h"
#include "game_logic.h"

char * choices[] = {
    "New Game",
    "Resume Game",
    "Beast Mode",
    "Exit",
};

int print_menu() {
    ITEM * * items;
    int c;
    MENU * gameMenu;
    int NRchoices, i;
    ITEM * current_item;
    WINDOW * gameMenuWindow;

    int currentItem = 0;
    start_color();
    curs_set(0);
    init_pair(1, COLOR_RED, COLOR_GREEN);
    attron(COLOR_PAIR(1));
    mvprintw(4, 20, "2048 MENU");
    attroff(COLOR_PAIR(1));

    /*items and menu */
    NRchoices = ARRAYSIZE(choices);
    items = calloc(NRchoices + 1, sizeof(ITEM * ));

    for (i = 0; i < NRchoices; ++i)
        items[i] = new_item(choices[i], "");
    items[NRchoices] = (ITEM * ) NULL;
    gameMenu = new_menu((ITEM * * ) items);
    set_menu_mark(gameMenu, " -> ");
    clear();

    /* WINDOW */
    gameMenuWindow = newwin(10, 40, 8, 4);
    keypad(gameMenuWindow, TRUE);

    /* Ascii art */
    set_menu_win(gameMenu, gameMenuWindow);
    set_menu_sub(gameMenu, derwin(gameMenuWindow, 5, 38, 3, 1));
    box(gameMenuWindow, 0, 0);
    mvprintw(0, 4, " _______  _______     ___    _____  ");
    mvprintw(1, 4, "/ ___   )(  __   )   /   )  / ___ \\ ");
    mvprintw(2, 4, "\\/   )  || (  )  |  / /) | ( (___) )");
    mvprintw(3, 4, "    /   )| | /   | / (_) (_ \\     / ");
    mvprintw(4, 4, "  _/   / | (/ /) |(____   _)/ ___ \\ ");
    mvprintw(5, 4, " /   _/  |   / | |     ) ( ( (   ) )");
    mvprintw(6, 4, "(   (__/\\|  (__) |     | | ( (___) )");
    mvprintw(7, 4, "\\_______/(_______)     (_)  \\_____/");
    mvprintw(20, 4, "Apasa <ENTER> pentru a selecta optiunea");
    mvprintw(21, 4, "Sagetile sus/jos pentru a naviga in meniu");
    mvprintw(22, 4, "** Ungureanu Ricardo - 311 CC Tema PC **");

    /* GUI stuff si afisarea meniului */
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    set_menu_back(gameMenu, COLOR_PAIR(2));
    init_pair(1, COLOR_RED, COLOR_BLACK);
    set_menu_fore(gameMenu, COLOR_PAIR(1) | A_REVERSE);
    post_menu(gameMenu);
    refresh();
    wrefresh(gameMenuWindow);

    /* mutam optiunea principala in functie de tastele apasate
       si determinam functionarea acesteia in functie de statusul
       in care se afla jocul */
    while (c != 'q' && (c = wgetch(gameMenuWindow))) {
        switch (c) {
        case KEY_DOWN:
            menu_driver(gameMenu, REQ_DOWN_ITEM);
            if (currentItem < 3)
                currentItem++;
            break;

        case KEY_UP:
            menu_driver(gameMenu, REQ_UP_ITEM);
            if (currentItem > 0)
                currentItem--;
            break;

        case 10:
            if (currentItem == 0) {
                allocBoard(4);
                newGame();
            } else if (currentItem == 1)
                resumeGame();
            else if (currentItem == 2) {
                allocBoard(10);
                newGame();
            } else
                gameIsOn = -1; // exit
            c = 'q';
            break;

        case 'q':
            gameIsOn = -1;
            c = 'q';
            break;
        }
    }

    /* Free pentru obiectele alocate dinamic */
    free_item(items[0]);
    free_item(items[1]);
    free_item(items[2]);
    free_menu(gameMenu);
    clear();
    endwin();
    return 0;
}
