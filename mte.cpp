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
		Mte::coordinates();
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
	mvprintw(LINES -1, COLS - COLS, status.c_str() );
	attroff(A_REVERSE);
}

void Mte::coordinates()
{
	std::stringstream bufferLines, bufferCols;
	std::string l = "";
	std::string c = "";
	std::string & rl = l;
	std::string & rc = c;
	
	bufferLines << "Lines: ";
	bufferLines << textLineCaptured.size();
	l = bufferLines.str();

	bufferCols << "Cols: ";
	bufferCols << x;
	c = bufferCols.str();

	attron(A_REVERSE);
	mvprintw(LINES - 1, COLS - 20, rl.c_str());
	mvprintw(LINES - 1, COLS - 10, rc.c_str());
	attroff(A_REVERSE);
	clrtoeol();


	// Erase string stream
	// bufferLines.str("");
	// bufferCols.str("");
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
				case KEY_UP:
					Mte::setUp();
					break;
				case KEY_DOWN:
					Mte::setDown();
					break;;
				case KEY_LEFT:
					Mte::setLeft();
					break;
				case KEY_RIGHT:
					Mte::setRight();
					break;
				case KEY_DC:
					Mte::setDelete();
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

void Mte::setUp()
{
	if( y <= static_cast<size_t>(LINES) && y <= textLineCaptured.size() - 1 && y > 0)
	{
		--y;
	}
	if( x >= textLineCaptured[y].length())
	{
		x = textLineCaptured[y].length();
	}
	move(y,x);
}

void Mte::setDown()
{
	if(y <= static_cast<size_t>(LINES) && y <= textLineCaptured[y].length() - 1 && y < textLineCaptured.size() - 1)
	{
		++y;
	}
	if ( x > textLineCaptured[y].length())
	{
		x = textLineCaptured[y].length();
	}
	move (y, x);
}

void Mte::setLeft()
{
	if(x > 0)
	{
		--x;
	}
}

void Mte::setRight()
{
	if(x < textLineCaptured[y].length())
	{
		++x;
	}
}

void Mte::setDelete()
{
	if(x < textLineCaptured[y].length())
	{
		textLineCaptured[y].erase(x,1);
	}
	if(x == textLineCaptured[y].length() && y < textLineCaptured.size() - 1)
	{
		textLineCaptured[y] += textLineCaptured[y+1];
		textLineCaptured.erase(textLineCaptured.begin() + (y+1));
	}
}