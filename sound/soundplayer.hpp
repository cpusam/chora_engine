/*
 Copyright (C) 2014 Samuel Leonardo

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

#ifndef CHORA_SOUNDPLAYER_HPP
#define CHORA_SOUNDPLAYER_HPP

#include <string>
#include <vector>

#include "../sdl.hpp"
#include "../include/statemachine.hpp"


class SoundFX: public StateMachine
{
	public:
		enum Type
		{
			UNDEF=-1,
			CHUNK,
			MUSIC
		};

		enum State
		{
			UNLOADED, // som ainda n�o carregado
			INACTIVE, // som carregado e pronto para tocar
			PAUSED, // som pausado
			PLAYING // tocando o som
		};

	public:
		SoundFX (  )
		{
			type = UNDEF;
			chunk = nullptr;
			music = nullptr;
			setState(UNLOADED);
			channel = -1;
		}

		int getType (  );

		std::string getID (  );

		std::string getPath (  );

		Mix_Chunk * getMixChunk (  );

		Mix_Music * getMixMusic (  );

		int getChannel (  );

		bool play ( int ch, int loops );

		bool pause (  );

		bool resume (  );

		void destroy (  );

		void loadChunk ( std::string path, std::string id );

		void loadMusic ( std::string p );

	protected:
		int type;
		int channel; // canal usado para tocar o som
		std::string id;
		std::string path;
		Mix_Chunk * chunk;
		Mix_Music * music;

		virtual int update (  )
		{
			return getState();
		}
};

class SoundPlayer: public StateMachine
{
	private:
		static SoundPlayer * singleton;
		std::vector <SoundFX> sound;

	protected:
		SoundPlayer (  )
		{
			
		}

	public:
		~SoundPlayer (  )
		{
			freeSounds();
		}

		static SoundPlayer * instance (  )
		{
			if (singleton)
				return singleton;
			
			singleton = new SoundPlayer();
			return singleton;
		}

		static void destroy (  )
		{
			if (singleton)
			{
				delete singleton;
				singleton = nullptr;
			}
		}

		void freeSounds (  );

		bool hasSound ( std::string id );

		bool addSound ( SoundFX & s );

		bool playing ( std::string id );

		bool pauseSound ( std::string id );

		bool resumeSound ( std::string id );

		bool playSound ( std::string id, int channel=-1, int loops=0 );

		bool haltSound ( std::string id );

		int update (  );
};

#endif
