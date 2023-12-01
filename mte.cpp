#include "mte.hpp"
#include <cstddef>
#include <ncurses.h>

Mte::Mte(const std::string& file)
{
	textLineCaptured.push_back("");

	if (file.empty())
	{
		filename = "untitled"; 
	}
	else
	{
		filename = file;
	}

	initscr();
	noecho();
	cbreak();
	keypad(stdscr, true);
}

Mte::~Mte()
{
	refresh();
	endwin();
}

void Mte::run()
{
	//printw(&filename[0]);
	//getch();
	
	while(getmaxyx(stdscr, LINES, COLS))
	{
		int intCaptured = getch();
		std::string caracterCaptured(1, static_cast<char>(intCaptured));
		textLineCaptured.push_back(caracterCaptured);

		for(size_t i{}; i < textLineCaptured.size(); ++i)
		{
			mvprintw(0, i, textLineCaptured[i].c_str());
		}
	}
}
