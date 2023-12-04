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
		int caracter = getch();
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

void Mte::inputText(int & ctr)
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
					break;
				case 127:
				case KEY_BACKSPACE:
					Mte::setBackspace();
					break;
				
				case 10: 		//line feed
				case 13:		//Carriage return
				case KEY_ENTER:
					Mte::setEnter();
					ctr = 0;
					break;

				default:
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
			mvprintw(i, 0, textLineCaptured[i].c_str());
			clrtoeol();
		}
	}	
	move(y,x); // move cursor of the final from 'Y' to 'X' axis begin.
}

void Mte::setBackspace()
{
	if(x == 0 && y > 0)
	{
		textLineCaptured.pop_back();
		x = textLineCaptured[y - 1].length();
		--y;
	}
	else if (x > 0)
	{
		textLineCaptured[y].erase(--x, 1);
	}
	
}

void Mte::setEnter()
{
	textLineCaptured.push_back(textLineCaptured[y].c_str());
	++y;
	x = 0;
	textLineCaptured[y].erase();
}
