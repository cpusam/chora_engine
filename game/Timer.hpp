#ifndef CHORA_TIMER_HPP
#define CHORA_TIMER_HPP

#include "Node.hpp"

//TODO: terminar classe
class Timer: public Node
{
	public:	
		Timer ( std::string name, int time, std::function<void(void *)> callback );
		
		int update (  ) override;
		
	protected:
		STimer timer;
		int time;
		std::function<void(void *)> callback;
};

#endif
