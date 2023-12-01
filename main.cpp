#include "mte.hpp"
#include <memory>

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		auto app_mte = std::make_shared<Mte>(argv[1]);

		app_mte->run();
	}
	else
	{
		auto app_mte = std::make_shared<Mte>("");
		app_mte->run();
	}

	return 0;
}
