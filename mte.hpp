#pragma once
#include <vector>
#pragma clang diagnostic ignored "-Wformat-security"

#include <ncurses.h>
#include <memory>
#include <string>

class Mte 
{
	protected: std::string filename;
	protected: std::vector<std::string> textLineCaptured;

	public:
		Mte(const std::string&);
		~Mte();
		void run();
};
