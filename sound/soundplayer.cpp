#include "soundplayer.hpp"

int SoundFX::get_type (  )
{
	return type;
}

std::string SoundFX::getID (  )
{
	return id;
}

std::string SoundFX::get_path (  )
{
	return path;
}

Mix_Chunk * SoundFX::get_chunk (  )
{
	return chunk;
}

Mix_Music * SoundFX::get_music (  )
{
	return music;
}

int SoundFX::get_channel (  )
{
	return channel;
}

bool SoundFX::play ( int ch, int loops )
{
	bool ret = false;

	switch (type)
	{
		case CHUNK_SOUND:
			ch = Mix_PlayChannel(channel, chunk, loops);
			if (channel == -1)
			{
				channel = ch;
				ret = true;
			}
			break;

		case MUSIC_SOUND:
			ch = Mix_PlayMusic(music, loops);
			if (channel == -1)
			{
				channel = ch;
				ret = true;
			}
			break;

		case UNDEF_SOUND:
			if (chunk || music)
				setState(INACTIVE_SOUND);
			else
				setState(UNLOADED_SOUND);
			return false;

		default:
			break;
	}

	if (ret)
		setState(PLAYING_SOUND);
	else
		setState(PAUSED_SOUND);

	return ret;
}

bool SoundFX::pause (  )
{
	bool ret = false;

	switch (type)
	{
	case CHUNK_SOUND:
		if (channel > -1)
		{
			Mix_Pause(channel);
			ret = true;
		}
		break;

	case MUSIC_SOUND:
		Mix_PauseMusic();
		ret = true;
		break;

	case UNDEF_SOUND:
		if (chunk || music)
			setState(INACTIVE_SOUND);
		else
			setState(UNLOADED_SOUND);
		return false;

	default:
		break;
	}

	if (ret)
		setState(PAUSED_SOUND);
	else
		setState(PLAYING_SOUND);

	return ret;
}

bool SoundFX::resume (  )
{
	bool ret = false;

	switch (type)
	{
	case CHUNK_SOUND:
		if (channel > -1)
		{
			Mix_Resume(channel);
			ret = true;
		}
		break;

	case MUSIC_SOUND:

		break;

	case UNDEF_SOUND:
		if (chunk || music)
			setState(INACTIVE_SOUND);
		else
			setState(UNLOADED_SOUND);
		ret = false;
	}

	return ret;
}

void SoundFX::destroy (  )
{
	if (chunk)
		Mix_FreeChunk(chunk);

	if (music)
		Mix_FreeMusic(music);

	chunk = 0;
	music = 0;

	setState(UNLOADED_SOUND);
	type = UNDEF_SOUND;
}

void SoundFX::set_chunk ( std::string p )
{
	Mix_Chunk * c = Mix_LoadWAV(p.c_str());
	if (c)
	{
		unsigned int f = p.find_last_of("/\\");
		path = p.substr(0, f);
		id = p.substr(f + 1);

		chunk = c;
		type = CHUNK_SOUND;
	}
	else
	{
		std::string t;
		t = "SoundFX: não conseguiu abrir efeito ";
		t.append(p);
		throw t.c_str();
	}
}

void SoundFX::set_music ( std::string p )
{
	Mix_Music * m = Mix_LoadMUS(p.c_str());
	if (m)
	{
		unsigned int f = p.find_last_of("/\\");
		path = p.substr(0, f);
		id = p.substr(f + 1);

		music = m;
		type = MUSIC_SOUND;
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

void SoundPlayer::free_sounds (  )
{
	for (std::vector <SoundFX>::iterator it = sound.begin(); it != sound.end(); it++)
		it->destroy();

	sound.clear();
}

bool SoundPlayer::has_sound ( std::string id )
{
	for (SoundFX & it: sound)
		if (id == it.getID())
			return true;

	return false;
}

bool SoundPlayer::add_sound ( SoundFX & s )
{
	if (has_sound(s.getID()))
		return false;

	sound.push_back(s);
	return true;
}

bool SoundPlayer::playing ( std::string id )
{
	for (SoundFX & it: sound)
		if (it.getID() == id && it.getState() == PLAYING_SOUND)
			return true;

	return false;
}

bool SoundPlayer::pause_sound ( std::string id )
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

bool SoundPlayer::resume_sound ( std::string id )
{
	bool ret = false;

	for (SoundFX & it: sound)
		if (id == it.getID())
		{
			switch (it.get_type())
			{
			case CHUNK_SOUND:
				Mix_Resume(it.get_channel());
				ret = true;
				break;
			case MUSIC_SOUND:
				Mix_ResumeMusic();
				ret = true;
				break;
			}

			if (ret)
			{
				it.setState(PLAYING_SOUND);
				break;
			}
		}

	return ret;
}

bool SoundPlayer::play_sound ( std::string id, int channel, int loops )
{
	bool ret = false;
	for (SoundFX & it: sound)
		if (id == it.getID())
		{
			if (it.getState() == PAUSED_SOUND)
			{
				it.resume();
				ret = true;
			}
			else
				ret = it.play(channel, loops);
			it.setState(PLAYING_SOUND);

			if (ret)
				return true;
		}

	return ret;
}

bool SoundPlayer::halt_sound ( std::string id )
{
	bool ret = false;

	for (SoundFX & it: sound)
		if (id == it.getID() || id == "all")
		{
			switch (it.get_type())
			{
			case CHUNK_SOUND:
				Mix_HaltChannel(it.get_channel());
				ret = true;
				break;
			case MUSIC_SOUND:
				Mix_HaltMusic();
				ret = true;
				break;
			}

			if (ret)
			{
				it.setState(INACTIVE_SOUND);
				break;
			}
		}

	return ret;
}

int SoundPlayer::update (  )
{
	for (SoundFX & it: sound)
		switch (it.get_type())
		{
			case CHUNK_SOUND:
				if (Mix_Playing(it.get_channel()))
					it.setState(PLAYING_SOUND);
				else if (it.getState() != PAUSED_SOUND)
					it.setState(INACTIVE_SOUND);
				break;

			case MUSIC_SOUND:
				if (Mix_PlayingMusic())
					it.setState(PLAYING_SOUND);
				else if (it.getState() != PAUSED_SOUND)
					it.setState(INACTIVE_SOUND);
				break;
			default:
				break;
		}

	return getState();
}


