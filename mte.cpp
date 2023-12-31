#include "mte.hpp"
#include <cstddef>
#include <cstdlib>
#include <curses.h>
#include <filesystem>
#include <fstream>
#include <ncurses.h>
#include <string>

Mte::Mte(const std::string& file)
{
	textLineCaptured.push_back("");
	way ='n';
	status = "NORMAL";
	if (file.empty())
	{
		filename = "new file";
		// TODO
		// Create function to ask if user wants create a new file or not. 
	}
	else
	{
		filename = file;
	}

	Mte::openfile();

	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	use_default_colors();
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
			status = "MODE: NORMAL\t\t| (i) INSERT MODE | (w) SAVE FILE | (q) EXIT |";
			break;
		case 'i':
			status = "MODE: INSERT\t\t| (ESC) NORMAL MODE | ('ESC' 'w') SAVE FILE |";
			break;
		case 'w':
			status = "MODE: NORMAL - SAVED FILE!\t\t| (i) INSERT MODE |(q) EXIT |";
		case 'q':
			break;
	};
}

void Mte::statusline()
{

	start_color(); //Creating color profile.
	if(way == 'n')
	{
		init_pair(1, COLOR_CYAN, COLOR_BLACK);
	}
	else if(way == 'w')
	{
		init_pair(1, COLOR_GREEN, COLOR_BLACK);
	}
	else if(way == 'i')
	{
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
	}
	else
	{
		init_pair(1, COLOR_RED, COLOR_BLACK);
		way = 'n';
	}

	// ATTR turn on
	attron(A_REVERSE);
	attron(A_BOLD);
	attron(COLOR_PAIR(1));

	for(int i{0}; i < COLS; i++)
	{
		mvprintw(LINES-1, i, " ");
	}
	mvprintw(LINES -1, COLS - COLS, status.c_str() );

	// ATTR turn off
	attroff(COLOR_PAIR(1));
	attroff(A_BOLD);
	attroff(A_REVERSE);
;
}

void Mte::coordinates()
{
	std::stringstream bufferLines, bufferCols;
	std::string l = "";
	std::string c = "";
	std::string panel;
	std::string & rl = l;
	std::string & rc = c;
	
	
	bufferLines << "Lines: ";
	bufferLines << textLineCaptured.size();
	l = bufferLines.str();

	bufferCols << "Cols: ";
	bufferCols << x;
	c = bufferCols.str();

	panel = "| "+ rl + " | " + rc + " | FILE: " + filename + " |";

	attron(A_REVERSE);
	attron(A_BOLD);
	attron(COLOR_PAIR(1));

	// mvprintw(LINES - 1, COLS - 20, rl.c_str());
	// mvprintw(LINES - 1, COLS - 10, rc.c_str());
	mvprintw(LINES -1, COLS - panel.length(), panel.c_str());

	attroff(COLOR_PAIR(1));
	attroff(A_BOLD);
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
		case 27: //ESC key
		case 'n':
			switch (ctr)
			{
				case 'q':
					way = 'q';					
					break;
				case 'i':
					way = 'i';
					break;
				case 'w':
					way = 'w';
					Mte::savefile();
					break;
				case  'z':	//TEST KEY

				//Window alert
				WINDOW * win1 = newwin(6, 40, LINES - (LINES / 2 + 3), COLS - (COLS / 2 + 20));
				start_color();
				init_pair(5, COLOR_BLACK, COLOR_WHITE);
				wbkgd(win1, COLOR_PAIR(5));
				mvwprintw( win1, 2, 10, "Press any key to exit");
				wgetch(win1);
				wrefresh(win1);

				//system("sleep 3");

				delwin(win1);

				break;

				//TODO
				// Create navagation in normal mode.				
				// case KEY_DOWN:
				// 	break;
				// case KEY_UP:
				// 	break;
			}
			break;

		case 'i':
			switch (ctr)
			{
				case 27: //ESC key
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
				case KEY_BTAB:
				case KEY_CTAB:
				case KEY_STAB:
				case 9:
					Mte::setTab();
					break;
				default:
					textLineCaptured[y].insert(x, 1, ctr);
					++x;
					break;
			}
		case 'w':
			switch (ctr) 
			{
				case 'q':
					way = 'q';
					break;
				case 'i':
					way = 'i';
					break;
			}

	}
}

//TODO
//Create function of the pagination.
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

void Mte::setTab()
{
	int rest = textLineCaptured[y].length();
	if( rest % 4 == 0 )
	{
		textLineCaptured[y].insert(x, "    "); //4 spaces
		x += 4;
	} else
	{
		std::string & text  = textLineCaptured[y];
		for(int i = text.length(); i % 4 != 0; i++)
		{
			text.insert(x, " "); //1 space
			x += 1;
		}
	}
}

void Mte::openfile()
{
	if(std::filesystem::exists(filename))
	{
		//in from a file
		std::ifstream currentFileStream (filename);
		if(currentFileStream.is_open())
		{
			textLineCaptured.pop_back(); //remove linha used in vector inicialization
			while(!currentFileStream.eof())
			{
				std::string fileBuffer;
				std::getline(currentFileStream , fileBuffer);
				textLineCaptured.push_back(fileBuffer);
			}
			textLineCaptured.pop_back(); //remove last line not writed.
			move(x, 0);
			move(y, 0);

		}
		else
		{
			throw std::runtime_error("Cannot open file. Permission denied.");
		}
	}
}

void Mte::savefile()
{
	if(std::filesystem::exists(filename))
	{
		//out to a file
		std::ofstream currentFileStream(filename);
		if(currentFileStream.is_open())
		{
			for(size_t i{0}; i < textLineCaptured.size(); i++)
			{
				currentFileStream << textLineCaptured[i] + "\n";
			}

			currentFileStream.close();
		}
	}
	else
	{
		Mte::warning("FILE NOT FOUND TO SAVE.");
		// TODO
		// Create function to ask if user wants create new file with content.
	}
}

void Mte::warning(std::string information)
{
	status = information;
	way = ' ';
}