#include "SDL_framerate.hpp"

#if defined(WIN32) || defined(WIN64) || defined(NO_ATOMIC)
	FPSManager * FPSManager::singleton = nullptr;
#else
	std::atomic<FPSManager *> FPSManager::singleton {nullptr};
	std::mutex FPSManager::myMutex;
#endif

