#include "mte.hpp"
#include <ncurses.h>

Mte::Mte()
{
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, true);
}

Mte::-Mte()
{
	refresh();
	endwin();
}

void Mte::run()
{

}
