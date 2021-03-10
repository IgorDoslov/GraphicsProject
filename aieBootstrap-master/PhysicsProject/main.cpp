#include "PhysicsProjectApp.h"

int main() {
	
	// allocation
	auto app = new PhysicsProjectApp();

	// initialise and loop
	app->run("AIE", 1024, 768, false);

	// deallocation
	delete app;

	return 0;
}