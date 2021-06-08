#include "SDL_framerate.hpp"

FPSManager * FPSManager::singleton = nullptr;

FPSManager::FPSManager (  )
{
	time_passed = 0;
	initFramerate();
}

void FPSManager::initFramerate (  )
{
	lastticks = baseticks = getTicks();
}


FPSManager * FPSManager::instance (  )
{
	if (!singleton)
	{
		singleton = new FPSManager();
	}

	return singleton;
}

void FPSManager::destroy (  )
{
	if (singleton)
	{
		delete singleton;
		singleton = nullptr;
	}
}

Uint32 FPSManager::getTicks (  )
{
	Uint32 ticks = SDL_GetTicks();

	/*
	* Since baseticks!=0 is used to track initialization
	* we need to ensure that the tick count is always >0
	* since SDL_GetTicks may not have incremented yet and
	* return 0 depending on the timing of the calls.
	*/
	if (ticks == 0)
	{
		return 1;
	}
	else
	{
		return ticks;
	}
}

int FPSManager::setFramerate ( Uint32 rate )
{
	if ((rate >= FPS_LOWER_LIMIT) && (rate <= FPS_UPPER_LIMIT))
	{
		fpsdef.framecount = 0;
		fpsdef.rate = rate;
		fpsdef.rateticks = (1000.0f / (float) rate);
		return (0);
	}

	return (-1);
}

FPSDef FPSManager::getFPSDef (  )
{
	return fpsdef;
}

bool FPSManager::setFPSDef ( FPSDef d )
{
	if (setFramerate(d.rate) > -1)
	{
		fpsdef = d;
		return true;
	}

	return false;
}

Uint32 FPSManager::getDelta (  )
{
	if (fpsdef.fixedDelta)
		return double(fpsdef.rateticks);
	return time_passed;
}

double FPSManager::getDeltaSeconds (  )
{
	if (fpsdef.fixedDelta)
		return double(fpsdef.rateticks / 1000.0);
	return double(time_passed)/1000.0;
}

void FPSManager::setFixedDelta (bool fixed) {
	fpsdef.fixedDelta = fixed;
}

Uint32 FPSManager::update (  )
{
	Uint32 current_ticks;
	Uint32 target_ticks;
	Uint32 the_delay;
	Uint32 delta_time = 0;

	/*
	* Initialize uninitialized manager
	*/
	if (baseticks == 0)
	{
		initFramerate();
	}

	/*
	* Next frame
	*/
	fpsdef.framecount++;

	/*
	* Get/calc ticks
	*/
	current_ticks = getTicks();
	delta_time = current_ticks - lastticks;
	lastticks = current_ticks;
	target_ticks = baseticks + (Uint32) ((float) fpsdef.framecount * fpsdef.rateticks);

	if (current_ticks <= target_ticks)
	{
		the_delay = target_ticks - current_ticks;
		SDL_Delay(the_delay);
	}
	else
	{
		fpsdef.framecount = 0;
		baseticks = getTicks();
	}

	time_passed = delta_time;

	return time_passed;
}

