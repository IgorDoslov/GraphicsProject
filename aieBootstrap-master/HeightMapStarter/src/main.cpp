#include "HeightMapStarterApp.h"

int main() {
	
	auto app = new HeightMapStarterApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}