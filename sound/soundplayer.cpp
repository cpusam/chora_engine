#include "soundplayer.hpp"

int SoundFX::getType (  )
{
	return type;
}

std::string SoundFX::getID (  )
{
	return id;
}

std::string SoundFX::getPath (  )
{
	return path;
}

Mix_Chunk * SoundFX::getMixChunk (  )
{
	return chunk;
}

Mix_Music * SoundFX::getMixMusic (  )
{
	return music;
}

int SoundFX::getChannel (  )
{
	return channel;
}

bool SoundFX::play ( int ch, int loops )
{
	bool ret = false;

	switch (type)
	{
		case SoundFX::Type::CHUNK:
			ch = Mix_PlayChannel(channel, chunk, loops);
			if (channel == -1)
			{
				channel = ch;
				ret = true;
			}
			break;

		case SoundFX::Type::MUSIC:
			ch = Mix_PlayMusic(music, loops);
			if (channel == -1)
			{
				channel = ch;
				ret = true;
			}
			break;

		case SoundFX::Type::UNDEF:
			if (chunk || music)
				setState(SoundFX::State::INACTIVE);
			else
				setState(SoundFX::State::UNLOADED);
			return false;

		default:
			break;
	}

	if (ret)
		setState(SoundFX::State::PLAYING);
	else
		setState(SoundFX::State::PAUSED);

	return ret;
}

bool SoundFX::pause (  )
{
	bool ret = false;

	switch (type)
	{
		case SoundFX::Type::CHUNK:
			if (channel > -1)
			{
				Mix_Pause(channel);
				ret = true;
			}
			break;

		case SoundFX::Type::MUSIC:
			Mix_PauseMusic();
			ret = true;
			break;

		case SoundFX::Type::UNDEF:
			if (chunk || music)
				setState(SoundFX::State::INACTIVE);
			else
				setState(SoundFX::State::UNLOADED);
			return false;

		default:
			break;
	}

	if (ret)
		setState(SoundFX::State::PAUSED);
	else
		setState(SoundFX::State::PLAYING);

	return ret;
}

bool SoundFX::resume (  )
{
	bool ret = false;

	switch (type)
	{
		case SoundFX::Type::CHUNK:
			if (channel > -1)
			{
				Mix_Resume(channel);
				ret = true;
			}
			break;

		case SoundFX::Type::MUSIC:

			break;

		case SoundFX::Type::UNDEF:
			if (chunk || music)
				setState(SoundFX::State::INACTIVE);
			else
				setState(SoundFX::State::UNLOADED);
			ret = false;
		default:
			break;
	}

	return ret;
}

void SoundFX::destroy (  )
{
	if (chunk)
		Mix_FreeChunk(chunk);

	if (music)
		Mix_FreeMusic(music);

	chunk = nullptr;
	music = nullptr;

	setState(SoundFX::State::UNLOADED);
	type = SoundFX::Type::UNDEF;
}

void SoundFX::loadChunk ( std::string p )
{
	Mix_Chunk * c = Mix_LoadWAV(p.c_str());
	if (c)
	{
		unsigned int f = p.find_last_of("/\\");
		path = p.substr(0, f);
		id = p.substr(f + 1);

		chunk = c;
		type = SoundFX::Type::CHUNK;
	}
	else
	{
		std::string t;
		t = "SoundFX: não conseguiu abrir efeito ";
		t.append(p);
		throw t.c_str();
	}
}

void SoundFX::loadMusic ( std::string p )
{
	Mix_Music * m = Mix_LoadMUS(p.c_str());
	if (m)
	{
		unsigned int f = p.find_last_of("/\\");
		path = p.substr(0, f);
		id = p.substr(f + 1);

		music = m;
		type = SoundFX::Type::MUSIC;
	}
	else
	{
		std::string t;
		t = "SoundFX: não conseguiu abrir musica ";
		t.append(p);
		throw t.c_str();
	}
}


/////////////////////////////////////////////////////////////////

SoundPlayer * SoundPlayer::singleton = nullptr;

void SoundPlayer::freeSounds (  )
{
	for (std::vector <SoundFX>::iterator it = sound.begin(); it != sound.end(); it++)
		it->destroy();

	sound.clear();
}

bool SoundPlayer::hasSound ( std::string id )
{
	for (SoundFX & it: sound)
		if (id == it.getID())
			return true;

	return false;
}

bool SoundPlayer::addSound ( SoundFX & s )
{
	if (hasSound(s.getID()))
		return false;

	sound.push_back(s);
	return true;
}

bool SoundPlayer::playing ( std::string id )
{
	for (SoundFX & it: sound)
		if (it.getID() == id && it.getState() == SoundFX::State::PLAYING)
			return true;

	return false;
}

bool SoundPlayer::pauseSound ( std::string id )
{
	bool ret = false;

	for (SoundFX & it: sound)
		if (id == it.getID())
		{
			it.pause();
			ret = true;
			break;
		}

	return ret;
}

bool SoundPlayer::resumeSound ( std::string id )
{
	bool ret = false;

	for (SoundFX & it: sound)
		if (id == it.getID())
		{
			switch (it.getType())
			{
			case SoundFX::Type::CHUNK:
				Mix_Resume(it.getChannel());
				ret = true;
				break;
			case SoundFX::Type::MUSIC:
				Mix_ResumeMusic();
				ret = true;
				break;
			}

			if (ret)
			{
				it.setState(SoundFX::State::PLAYING);
				break;
			}
		}

	return ret;
}

bool SoundPlayer::playSound ( std::string id, int channel, int loops )
{
	bool ret = false;
	for (SoundFX & it: sound)
		if (id == it.getID())
		{
			if (it.getState() == SoundFX::State::PAUSED)
			{
				it.resume();
				ret = true;
			}
			else
				ret = it.play(channel, loops);
			it.setState(SoundFX::State::PLAYING);

			if (ret)
				return true;
		}

	return ret;
}

bool SoundPlayer::haltSound ( std::string id )
{
	bool ret = false;

	for (SoundFX & it: sound)
		if (id == it.getID() || id == "all")
		{
			switch (it.getType())
			{
			case SoundFX::Type::CHUNK:
				Mix_HaltChannel(it.getChannel());
				ret = true;
				break;
			case SoundFX::Type::MUSIC:
				Mix_HaltMusic();
				ret = true;
				break;
			}

			if (ret)
			{
				it.setState(SoundFX::State::INACTIVE);
				break;
			}
		}

	return ret;
}

int SoundPlayer::update (  )
{
	for (SoundFX & it: sound)
		switch (it.getType())
		{
			case SoundFX::Type::CHUNK:
				if (Mix_Playing(it.getChannel()))
					it.setState(SoundFX::State::PLAYING);
				else if (it.getState() != SoundFX::State::PAUSED)
					it.setState(SoundFX::State::INACTIVE);
				break;

			case SoundFX::Type::MUSIC:
				if (Mix_PlayingMusic())
					it.setState(SoundFX::State::PLAYING);
				else if (it.getState() != SoundFX::State::PAUSED)
					it.setState(SoundFX::State::INACTIVE);
				break;
			default:
				break;
		}

	return getState();
}


