#include <stdlib.h>
#include <time.h>
#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "memleaks.h"

// This is needed here because SDL redefines main function
// do not add any other libraries here, instead put them in their modules
#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

//
//#include "Brofiler.h"

enum MainState
{
	CREATE = 1,
	awake,
	start,
	LOOP,
	CLEAN,
	FAIL,
	EXIT
};

App* app = NULL;

int main(int argc, char* args[])
{
	// NOTE : For now brofiler puts the same error as with when i tried to put SDL on the new project, need his help.
	//NOTE : the memory leaks detector doesn't work either
	//This makes brofiler to work
	//BROFILER_FRAME("Hello World")
	//ReportMemoryLeaks();

	LOG("Engine starting ... %d");

	srand(time(NULL));

	MainState state = MainState::CREATE;
	int result = EXIT_FAILURE;

	while(state != EXIT)
	{
		switch(state)
		{

			// Allocate the engine --------------------------------------------
			case CREATE:
			LOG("CREATION PHASE ===============================");

			app = new App(argc, args);

			if(app != NULL)
				state = awake;
			else
				state = FAIL;

			break;

			// awake all modules -----------------------------------------------
			case awake:
			LOG("awake PHASE ===============================");
			if(app->awake() == true)
				state = start;
			else
			{
				LOG("ERROR: awake failed");
				state = FAIL;
			}

			break;

			// Call all modules before first frame  ----------------------------
			case start:
			LOG("start PHASE ===============================");
			if(app->start() == true)
			{
				state = LOOP;
				LOG("update PHASE ===============================");
			}
			else
			{
				state = FAIL;
				LOG("ERROR: start failed");
			}
			break;

			// Loop all modules until we are asked to leave ---------------------
			case LOOP:
			if(app->update() == false)
				state = CLEAN;
			break;

			// cleanUp allocated memory -----------------------------------------
			case CLEAN:
			LOG("cleanUp PHASE ===============================");
			if(app->cleanUp() == true)
			{
				RELEASE(app);
				result = EXIT_SUCCESS;
				state = EXIT;
			}
			else
				state = FAIL;

			break;

			// Exit with errors and shame ---------------------------------------
			case FAIL:
			LOG("Exiting with errors :(");
			result = EXIT_FAILURE;
			state = EXIT;
			break;
		}
	}

	LOG("... Bye! :)\n");

	
	// Dump memory leaks
	/*ReportMemoryLeaks();
	char* str;

	str = new char[200];*/


	return result;
}