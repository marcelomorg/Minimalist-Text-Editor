#include "mte.hpp"
#include <cstddef>
#include <ncurses.h>

Mte::Mte(const std::string& file)
{
	textLineCaptured.push_back("");
	way ='n';
	status = "NORMAL";

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
};

Mte::~Mte()
{
	refresh();
	endwin();
};

void Mte::run()
{
	while(way != 'q')
	{
		Mte::choose();
		Mte::statusline();
		Mte::print();
		int caracter  = getch();
		Mte::inputText(caracter);
	};

	/*
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
	*/ 

	/*
	printw(&filename[0]);
	getch();
	*/ 	
};

void Mte::choose()
{
	switch (way) 
	{
		case 27: //KEY "Esc"
		case 'n':
			status = "NORMAL";
			break;
		case 'i':
			status = "INSERT";
			break;
		case 'q':
			break;
	};
}

void Mte::statusline()
{
	attron(A_REVERSE);
	mvprintw(LINES -1, COLS - (status.length() + 3) , status.c_str() );
	attroff(A_REVERSE);
}

void Mte::inputText(int ctr)
{
	switch (way)
	{
		case 27:
		case 'n':
			switch (ctr)
			{
				case 'q':
					way = 'q';
					break;
				case 'i':
					way = 'i';
					break;
			}
			break;
		case 'i':
			switch (ctr)
			{
				case 27:
					way = 'n';
				default:
					std::string s(1, ctr);
					//textLineCaptured.push_back(s);
					textLineCaptured[y].insert(x, 1, ctr);
					++x;
					break;
			}
	}
}

void Mte::print()
{
	for(size_t i {}; i < static_cast<size_t>(LINES - 1); ++i)
	{
		if (i >= textLineCaptured.size())
		{
			move(i, 0);
			clrtoeol();
		}
		else
		{
			mvprintw(0, i, textLineCaptured[i].c_str());

		}
		clrtoeol();
	}	
	move(y,x); // move cursor of the final from 'Y' to 'X' axis begin.
}
