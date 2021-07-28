/*
*  Thank you for staying with us.
*  (c) Yume Games 2020 - 2021
*/

#include <TLEBCApp.h>

int main()
{
	try
	{
		TLEBCApp* app = new TLEBCApp;

		app->Run();

		delete app;
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << '\n';
		spdlog::get("log")->error(ex.what());

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}