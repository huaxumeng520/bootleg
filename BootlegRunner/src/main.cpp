
#include <KIT/Engine.hpp>

#include <csignal>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>


namespace 
{
	kit::Engine* engine = nullptr;

	void handleInterrupt(int s)
	{
		engine->shutdown();
	}

}

int main(int argc, char**argv)
{
	::engine = new kit::Engine();

	struct sigaction interruptHandler;
	sigemptyset(&interruptHandler.sa_mask);
	interruptHandler.sa_handler = ::handleInterrupt;
	interruptHandler.sa_flags = 0;

	sigaction(SIGINT, &interruptHandler, NULL);

	::engine->run();
	
	delete ::engine;

	return 0;
}
