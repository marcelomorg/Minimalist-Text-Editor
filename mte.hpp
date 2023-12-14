#pragma once
#pragma clang diagnostic ignored "-Wformat-security"
#include <vector>
#include <ncurses.h>
#include <memory>
#include <string>
#include <sstream>

class Mte 
{
	protected: size_t x, y;
	protected: char way;
	protected: std::string filename;
	protected: std::string status;
	protected: std::vector<std::string> textLineCaptured;

	public:
		Mte(const std::string&);
		~Mte();
		void run(); 			//Do loop
		void choose();			//Choose way
		void coordinates();
		void statusline();		//Print screen way status
		void inputText(int &);
		void print();
		
		void setBackspace();
		void setEnter();
		void setUp();
		void setDown();
		void setLeft();
		void setRight();
		void setDelete();
		void setTab();
};
