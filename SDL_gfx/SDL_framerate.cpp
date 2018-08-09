#include "SDL_framerate.hpp"

std::atomic<FPSManager *> FPSManager::singleton {nullptr};
std::mutex FPSManager::myMutex;

