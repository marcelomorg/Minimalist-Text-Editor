#pragma once
#pragma clang diagnostic ignored "-Wformat-security"
#include <vector>
#include <ncurses.h>
#include <memory>
#include <string>

class Mte 
{
	protected: char x,y;
	protected: char way;
	protected: std::string filename;
	protected: std::string status;
	protected: std::vector<std::string> textLineCaptured;

	public:
		Mte(const std::string&);
		~Mte();
		void run(); 			//Do loop
		void choose();			//Choose way
		void statusline();		//Print screen way status
		void inputText(int &);
		void print();
		
		void setBackspace();
		void setEnter();
		

};
