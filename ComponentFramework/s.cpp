#define _CRTDBG_MAP_ALLOC 

#include <stdlib.h>  
#include <crtdbg.h>
#include <memory>

#include <string>
#include "SceneManager.h"
#include "Debug.h"

  
int main(int argc, char* args[]) {
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	/// Comment out the line below if you really want to make a 64-bit build
	static_assert(sizeof(void*) == 4, "Are you ready for 64-bit build?");
	
	{
		Debug::DebugInit("GameEngineLog.txt");
		std::unique_ptr<SceneManager> game = std::make_unique<SceneManager>();
		if (game->Initialize("Game Engine", 1920 , 1080) == true) {
			game->Run();
		}
	}

	/// This writes out memory leaks to the output window not the console window
	_CrtDumpMemoryLeaks();
	exit(0);
}