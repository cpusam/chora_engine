#include "animation.hpp"
#include "collision.hpp"

void AnimationFrame::setFrame ( int d, SDL_Rect src )
{
	setSourceRect(src);
	setDestinyRect((SDL_Rect){0,0,src.w,src.h});
	setDelay(d);
}

bool AnimationFrame::setDelay ( int d )
{
	if (d > -1)
		delay = d;

	return (d > -1);
}

int AnimationFrame::getDelay (  )
{
	return delay;
}

void AnimationFrame::setSourceRect ( const SDL_Rect & s )
{
	source = s;
}

SDL_Rect AnimationFrame::getSourceRect (  )
{
	return source;
}

void AnimationFrame::setDestinyRect ( const SDL_Rect & d )
{
	x = d.x;
	y = d.y;
	destiny = d;
}

SDL_Rect AnimationFrame::getDestinyRect (  )
{
	return destiny;
}


Vect AnimationFrame::getOrientation (  )
{
	return orientation;
}

float AnimationFrame::getAngle (  )
{
	return angle;
}

void AnimationFrame::setAngle ( float rad )
{
	angle = rad;
}

void AnimationFrame::rotate ( float a )
{
	angle += a;
	orientation.rotate(a);
}

void AnimationFrame::setFlip ( bool hor, bool ver )
{
	unsigned int f = static_cast<unsigned int>(flip);
	
	if (hor == true)
		f = f ^ static_cast<unsigned int>(SDL_FLIP_HORIZONTAL);
	
	if (ver == true)
		f = f ^ static_cast<unsigned int>(SDL_FLIP_VERTICAL);

	flip = static_cast<SDL_RendererFlip>(f);
}

SDL_RendererFlip AnimationFrame::get_flip (  )
{
	return flip;
}

void AnimationFrame::setTexture ( SDL_Texture * t )
{
	texture = t;
}

SDL_Texture * AnimationFrame::getTexture (  )
{
	return texture;
}

bool AnimationFrame::destroy (  )
{
	bool ret = false;

	if (texture)
	{
		//SDL_DestroyTexture(texture);
		texture = 0;
		ret = true;
	}

	return ret;
}

//////////////////////////////////////////////////////////////////

void Animation::play (  )
{
	if (getState() == STOPPED)
		setState(START);
	else
		setState(RUNNING);

	timer.start();
}

void Animation::pause (  )
{
	setState(PAUSED);
	timer.pause();
}

void Animation::reset (  )
{
	index = 0;
	setState(START);
	timer.reset();
}

void Animation::setRepeat ( bool r )
{
	repeat = r;
}

void Animation::setFrameDelay ( int f, int d )
{
	if (f >= 0 && f < int(frames.size()))
		frames[f].setDelay(d);
}

int Animation::getTotalTime (  )
{
	int total = 0;
	for (auto & frame: frames)
		total += frame.getDelay();
	return total;
}

// seta todos os frames para o mesmo delay
void Animation::setFramesDelay ( int d )
{
	for (unsigned i = 0; i < frames.size(); i++)
		frames[i].setDelay(d);
}

int Animation::getFramesSize (  )
{
	return frames.size();
}

void Animation::clearFrames ( bool destroy )
{
	frames.clear();
	if (destroy)
		destroyTextures();
	texture.clear();
}

Vect Animation::getOrientation (  )
{
	return orientation;
}

float Animation::getAngle (  )
{
	return angle;
}

void Animation::setAngle ( float rad )
{
	for (auto & frame: frames)
	{
		frame.setAngle(rad);
	}
}


void Animation::rotate ( float a )
{
	if (use_rot == false)
		return;

	angle += a;
	orientation.rotate(a);
	for (unsigned i = 0; i < frames.size(); i++)
		frames[i].rotate(a);
}

void Animation::setUseRotation ( bool u )
{
	use_rot = true;
}

bool Animation::getUseRotation (  )
{
	return use_rot;
}

void Animation::setUseCenter ( bool u )
{
	use_center = true;
}

bool Animation::getUseCenter (  )
{
	return use_center;
}

void Animation::setCenter ( Vect center )
{
	this->center = center;
}

void Animation::flip ( SDL_RendererFlip f )
{
	bool hor, ver;
	
	hor = f & SDL_FLIP_HORIZONTAL;
	ver = f & SDL_FLIP_VERTICAL;
	
	for (unsigned i = 0; i < frames.size(); i++)
		frames[i].setFlip(hor, ver);
}

void Animation::flip ( bool hor, bool ver )
{
	for (unsigned i = 0; i < frames.size(); i++)
		frames[i].setFlip(hor, ver);
}

SDL_RendererFlip Animation::get_flip (  )
{
	return frames.at(0).get_flip();
}

void Animation::addFrame ( SDL_Texture * t, SDL_Rect const & src, int d )
{
	index = 0;
	texture.push_back(t);
	AnimationFrame f;

	f.setSourceRect(src);
	f.setDestinyRect((SDL_Rect){0,0,src.w,src.h});
	f.setDelay(d);
	f.setTexture(t);

	frames.push_back(static_cast<AnimationFrame>(f));
}

void Animation::addFrame ( SDL_Texture * t, SDL_Rect const & src, SDL_Rect const & dst, int d )
{
	index = 0;
	texture.push_back(t);
	AnimationFrame f;

	f.setSourceRect(src);
	f.setDestinyRect(dst);
	f.setDelay(d);
	f.setTexture(t);

	frames.push_back(f);
}

void Animation::addFrame ( SDL_Texture * t, AnimationFrame & f )
{
	index = 0;
	texture.push_back(t);
	f.setTexture(t);
	frames.push_back(f);
}

SDL_Texture * Animation::getTexture ( int i )
{
	if (texture.size() > 0 && i < int(texture.size()))
		return texture[i];

	return 0;
}

void Animation::destroyTextures (  )
{
	
	std::vector <SDL_Texture *> t;

	for (unsigned int i(0); i < texture.size(); i++)
	{
		SDL_Texture * aux = texture[i];
		int count = 0;
		for (unsigned int j(0); j < texture.size(); j++)
			if (j != i && aux == texture[j])
				count++;

		if (count == 0)
		{
			if (aux)
				t.push_back(aux);
		}
		else
		{
			count = 0;
			for (unsigned int j(0); j < t.size(); j++)
				if (t.at(j) == aux)
					count++;

			if (count > 0)
				if (aux)
					t.push_back(aux);
		}
	}

	for (unsigned int i(0); i < t.size(); i++)
		if (t.at(i))
			SDL_DestroyTexture(t.at(i));

	for (unsigned int i = 0; i < frames.size(); i++)
		frames[i].destroy();
	
	texture.clear();
	//t.clear();
}

bool Animation::hasTexture ( SDL_Texture * t )
{
	if (!t)
		return false;

	for (size_t i(0), size = texture.size(); i < size; ++i)
		if (t == texture[i])
			return true;

	return false;
}

bool Animation::setIndex ( int i )
{
	if (i >= 0 && i < int(frames.size()))
	{
		index = i;
		timer.reset();
		return true;
	}

	return false;
}

int Animation::getIndex (  )
{
	return index;
}

void Animation::setTimer ( STimer t )
{
	timer = t;
}

STimer Animation::getTimer (  )
{
	return timer;
}

void Animation::setName ( std::string n )
{
	name = n;
}

std::string Animation::getName (  )
{
	return name;
}

std::vector<AnimationFrame> & Animation::getFrames (  )
{
	return frames;
}

AnimationFrame Animation::getFrame ( int i )
{
	if (i > 0 && i <= int(frames.size()))
		return frames[i];

	return AnimationFrame();
}

AnimationFrame Animation::getCurrentFrame (  )
{
	return frames[index];
}

int Animation::draw ( SDL_Renderer * renderer, int x, int y )
{
	if (frames.size() == 0 || size_t(index) >= frames.size())
		return -1;
	
	int ret = 0;
	static SDL_Rect dest, source;

	dest = frames.at(index).getDestinyRect();
	source = frames.at(index).getSourceRect();

	dest.x = dest.x + x;
	dest.y = dest.y + y;

	if (texture.size() && texture.at(index))
	{
		if (use_rot == false)
		{
			ret = SDL_RenderCopyEx(renderer, texture.at(index), &source, &dest, 0, 0, frames[index].get_flip());
		}
		else
		{
			static SDL_Point center;
			if (use_center)
				center = {frames[index].getSourceRect().w/2, frames[index].getSourceRect().h/2};
			else if (use_rot)
			{
				center.x = this->center.x;
				center.y = this->center.y;
			}
			ret = SDL_RenderCopyEx(renderer, texture.at(index), &source, &dest, TO_DEGREES(frames[index].getAngle()), &center, frames[index].get_flip());
		}
	}
	
	return ret;
}


int Animation::draw ( SDL_Renderer * renderer, Camera * cam, int x, int y, int destW, int destH )
{
	if (frames.size() == 0 || size_t(index) >= frames.size())
		return -1;
	
	int ret = 0;
	SDL_Rect dest, source;
	//if (getState() == CHANGE_FRAME)
	{
		dest = frames.at(index).getDestinyRect();
		source = frames.at(index).getSourceRect();
	}

	dest.w = destW;
	dest.h = destH;

	Vect position = cam->getPosition();
	SDL_Rect dim = cam->getDimension();
	//SDL_Rect view = cam->getView();

	dest.x = dest.x + x;
	dest.y = dest.y + y;

	
	dest.x = (dest.x - position.x) + dim.x;
	dest.y = (dest.y - position.y) + dim.y;
	
	/*
	SDL_Rect rect = rectIntersect(dest,dim);
	dest = rect;
	*/

	if (texture.size() && texture.at(index))
	{
		//SDL_Color color = {255,255,0,128};
		//fillRect(renderer, color, rect);
		// atualiza o viewport para desenhar nele
		//cam->updateViewport(renderer);
		if (use_rot == false)
			ret = SDL_RenderCopyEx(renderer, frames.at(index).getTexture(), &source, &dest, 0, 0, frames[index].get_flip());
		else
		{
			static SDL_Point center = {dest.w/2, dest.h/2};
			if (use_center == false && use_rot)
			{
				center.x = this->center.x;
				center.y = this->center.y;
			}

			ret = SDL_RenderCopyEx(renderer, frames.at(index).getTexture(), &source, &dest, TO_DEGREES(frames[index].getAngle()), &center, frames[index].get_flip());
		}
	}
	
	return ret;
}

int Animation::draw ( SDL_Renderer * renderer, Camera * cam, int x, int y )
{
	if (frames.size() == 0 || size_t(index) >= frames.size())
		return -1;
	
	int ret = 0;
	SDL_Rect dest, source;
	dest = frames.at(index).getDestinyRect();
	source = frames.at(index).getSourceRect();

	Vect position = cam->getPosition();
	SDL_Rect dim = cam->getDimension();
	//SDL_Rect view = cam->getView();

	dest.x += x;
	dest.y += y;

	
	dest.x = (dest.x - position.x) + dim.x;
	dest.y = (dest.y - position.y) + dim.y;
	
	
	/*
	SDL_Rect rect = rectIntersect(dest,dim);
	dest = rect;
	*/

	if (texture.size() && texture.at(index))
	{
		//SDL_Color color = {255,255,0,128};
		//fillRect(renderer, color, rect);
		// atualiza o viewport para desenhar nele
		//cam->updateViewport(renderer);
		if (use_rot == false)
			ret = SDL_RenderCopyEx(renderer, frames.at(index).getTexture(), &source, &dest, 0, 0, frames[index].get_flip());
		else
		{
			SDL_Point center = {dest.w/2, dest.h/2};
			if (use_center == false && use_rot)
			{
				center.x = this->center.x;
				center.y = this->center.y;
			}

			ret = SDL_RenderCopyEx(renderer, frames.at(index).getTexture(), &source, &dest, TO_DEGREES(frames[index].getAngle()), &center, frames[index].get_flip());
		}
	}
	
	return ret;
}

int Animation::update (  )
{
	switch (getState())
	{
		case START:
		case CHANGE_FRAME:
		case RUNNING:
		case FINISHED:
			if (frames.size() == 0)
			{
				printf("Animation: animação sem frames\n");
				return -1;
			}

			timer.update();
			if (timer.steps() >= frames[index].getDelay())
			{
				timer.reset();
				index++;
				if (index >= int(frames.size()))
				{
					if (repeat)
					{
						index = 0;
						setState(FINISHED); // termina e repete a animação
						break;
					}
					else
					{
						index = int(frames.size() - 1);
						setState(STOPPED); // terminou a animação e fica parado
						break;
					}
				}

				setState(CHANGE_FRAME); // novo frame
				break;
			}

			setState(RUNNING); // rodando
			break;

		default:
			break;
	}

	return getState();
}

