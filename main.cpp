#include "mte.hpp"

int main(int argc, char** argv)
{
	auto app_mte = std::make_shared<Mte>();
	app_mte->run();

	return 0;
}
