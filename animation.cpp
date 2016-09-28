#include "animation.hpp"
#include "collision.hpp"

void AnimationFrame::set_frame ( int d, SDL_Rect src )
{
	set_source(src);
	set_destiny((SDL_Rect){0,0,src.w,src.h});
	set_delay(d);
}

bool AnimationFrame::set_delay ( int d )
{
	if (d > -1)
		delay = d;

	return (d > -1);
}

int AnimationFrame::get_delay (  )
{
	return delay;
}

void AnimationFrame::set_source ( SDL_Rect s )
{
	source = s;
}

SDL_Rect AnimationFrame::get_source (  )
{
	return source;
}

void AnimationFrame::set_destiny ( SDL_Rect d )
{
	x = d.x;
	y = d.y;
	destiny = d;
}

SDL_Rect AnimationFrame::get_destiny (  )
{
	return destiny;
}


Vect AnimationFrame::get_orientation (  )
{
	return orientation;
}

float AnimationFrame::get_angle (  )
{
	return angle;
}

void AnimationFrame::rotate ( float a )
{
	angle += a;
	orientation.rotate(a);
}

void AnimationFrame::set_flip ( bool hor, bool ver )
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

void AnimationFrame::set_texture ( SDL_Texture * t )
{
	texture = t;
}

SDL_Texture * AnimationFrame::get_texture (  )
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
	if (get_state() == STOPED)
		set_state(START);
	else
		set_state(RUNNING);

	timer.start();
}

void Animation::pause (  )
{
	set_state(PAUSED);
	timer.pause();
}

void Animation::reset (  )
{
	index = 0;
	set_state(START);
	timer.reset();
}

void Animation::set_repeat ( bool r )
{
	repeat = r;
}

void Animation::set_delay ( int f, int d )
{
	if (f >= 0 && f < int(frames.size()))
		frames[f].set_delay(d);
}

// seta todos os frames para o mesmo delay
void Animation::set_frames_delay ( int d )
{
	for (unsigned i = 0; i < frames.size(); i++)
		frames[i].set_delay(d);
}

int Animation::get_frames_size (  )
{
	return frames.size();
}

void Animation::clear_frames ( bool destroy )
{
	frames.clear();
	if (destroy)
		destroy_textures();
	texture.clear();
}

Vect Animation::get_orientation (  )
{
	return orientation;
}

float Animation::get_angle (  )
{
	return angle;
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

void Animation::set_use_rot ( bool u )
{
	use_rot = true;
}

bool Animation::get_use_rot (  )
{
	return use_rot;
}

void Animation::set_use_center ( bool u )
{
	use_center = true;
}

bool Animation::get_use_center (  )
{
	return use_center;
}

void Animation::flip ( SDL_RendererFlip f )
{
	bool hor, ver;
	
	hor = f & SDL_FLIP_HORIZONTAL;
	ver = f & SDL_FLIP_VERTICAL;
	
	for (unsigned i = 0; i < frames.size(); i++)
		frames[i].set_flip(hor, ver);
}

void Animation::flip ( bool hor, bool ver )
{
	for (unsigned i = 0; i < frames.size(); i++)
		frames[i].set_flip(hor, ver);
}

SDL_RendererFlip Animation::get_flip (  )
{
	return frames.at(0).get_flip();
}

void Animation::add_frame ( SDL_Texture * t, SDL_Rect src, int d )
{
	index = 0;
	texture.push_back(t);
	AnimationFrame f;

	f.set_source(src);
	f.set_destiny((SDL_Rect){0,0,src.w,src.h});
	f.set_delay(d);
	f.set_texture(t);

	frames.push_back(static_cast<AnimationFrame>(f));
}

void Animation::add_frame ( SDL_Texture * t, SDL_Rect src, SDL_Rect dst, int d )
{
	index = 0;
	texture.push_back(t);
	AnimationFrame f;

	f.set_source(src);
	f.set_destiny(dst);
	f.set_delay(d);
	f.set_texture(t);

	frames.push_back(f);
}

void Animation::add_frame ( SDL_Texture * t, AnimationFrame f )
{
	index = 0;
	texture.push_back(t);
	f.set_texture(t);
	frames.push_back(f);
}

SDL_Texture * Animation::get_texture ( int i )
{
	if (texture.size() > 0 && i < int(texture.size()))
		return texture[i];

	return 0;
}

void Animation::destroy_textures (  )
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

bool Animation::has_texture ( SDL_Texture * t )
{
	if (!t)
		return false;

	for (int i(0); i < int(texture.size()); i++)
		if (t == texture.at(i))
			return true;

	return false;
}

bool Animation::set_index ( int i )
{
	if (i >= 0 && i < int(frames.size()))
	{
		index = i;
		timer.reset();
		return true;
	}

	return false;
}

int Animation::get_index (  )
{
	return index;
}

void Animation::set_timer ( STimer t )
{
	timer = t;
}

STimer Animation::get_timer (  )
{
	return timer;
}

AnimationFrame Animation::get_frame ( int i )
{
	if (i > 0 && i <= int(frames.size()))
		return frames[i];

	return AnimationFrame();
}

AnimationFrame Animation::get_curr_frame (  )
{
	return frames[index];
}

int Animation::draw ( SDL_Renderer * renderer, int x, int y )
{
	int ret = 0;
	SDL_Rect dest, source;
	dest = frames.at(index).get_destiny();
	dest.x += x;
	dest.y += y;
	source = frames.at(index).get_source();

	if (use_center)
	{
		dest.x = dest.x - source.w / 2;
		dest.y = dest.y - source.h / 2;
	}

	if (texture.size() && texture.at(index))
	{
		if (use_rot == false)
		{
			ret = SDL_RenderCopyEx(renderer, texture.at(index), &source, &dest, 0, 0, frames[index].get_flip());
		}
		else
		{
			SDL_Point center = {frames[index].get_source().w/2, frames[index].get_source().h/2};
			ret = SDL_RenderCopyEx(renderer, texture.at(index), &source, &dest, TO_DEGREES(frames[index].get_angle()), &center, frames[index].get_flip());
		}
	}
	
	return ret;
}


int Animation::draw ( SDL_Renderer * renderer, Camera * cam, int x, int y )
{
	int ret = 0;
	SDL_Rect dest, source;
	dest = frames.at(index).get_destiny();
	source = frames.at(index).get_source();

	Vect pos = cam->get_position();
	SDL_Rect dim = cam->get_dimension();
	//SDL_Rect view = cam->get_view();

	dest.x += x;
	dest.y += y;

	if (use_center)
	{
		dest.x = dest.x - dest.w / 2;
		dest.y = dest.y - dest.h / 2;
	}
	
	
	dest.x = (dest.x - pos.x) + dim.x;
	dest.y = (dest.y - pos.y) + dim.y;
	
	/*
	SDL_Rect rect = rectIntersect(dest,dim);
	dest = rect;
	*/

	if (texture.size() && texture.at(index))
	{
		//SDL_Color color = {255,255,0,128};
		//fill_rect(renderer, color, rect);
		// atualiza o viewport para desenhar nele
		//cam->updateViewport(renderer);
		if (use_rot == false)
			ret = SDL_RenderCopyEx(renderer, frames.at(index).get_texture(), &source, &dest, 0, 0, frames[index].get_flip());
		else
		{
			SDL_Point center = {dest.w/2, dest.h/2};

			ret = SDL_RenderCopyEx(renderer, frames.at(index).get_texture(), &source, &dest, TO_DEGREES(frames[index].get_angle()), &center, frames[index].get_flip());
		}
	}
	
	return ret;
}

int Animation::update (  )
{
	switch (get_state())
	{
		case START:
		case CHANGE_FRAME:
		case RUNNING:
		case FINISHED:
			if (frames.size() == 0)
			{
				printf("Animation: animação sem frames\n");
			}

			timer.update();
			if (timer.steps() >= frames[index].get_delay())
			{
				timer.reset();
				index++;
				if (index >= int(frames.size()))
				{
					if (repeat)
					{
						index = 0;
						set_state(FINISHED); // termina e repete a animação
						break;
					}
					else
					{
						index = int(frames.size() - 1);
						set_state(STOPED); // terminou a animação e fica parado
						break;
					}
				}

				set_state(CHANGE_FRAME); // novo frame
				break;
			}

			set_state(RUNNING); // rodando
			break;

		default:
			break;
	}

	return get_state();
}

/*
int Animation::draw ( SDL_Renderer * renderer, Camera * cam, int x, int y )
{
	int ret = 0;
	SDL_Rect dest, source;
	dest = frames.at(index).get_destiny();
	source = frames.at(index).get_source();

	Vect pos = cam->get_position();
	SDL_Rect dim = cam->get_dimension();
	//SDL_Rect view = cam->get_view();

	dest.x += x;
	dest.y += y;

	if (use_center)
	{
		dest.x = dest.x - dest.w / 2;
		dest.y = dest.y - dest.h / 2;
	}
	
	
	dest.x = (dest.x - pos.x) + dim.x;
	dest.y = (dest.y - pos.y) + dim.y;
		
	SDL_Rect rect = rectIntersect(dest,dim);
	
	float s = 0;
	if (rect.w < dest.w && angle == 0)
	{
		s = (float)rect.w / dest.w;
		int w = source.w;
		
		std::string ifs;
		
		if (rect.x + dest.w > dim.x + dim.w)
		{
			source.w = w * s;
			//source.x = source.w;
			if (frames.at(index).get_flip() & SDL_FLIP_HORIZONTAL)
				source.x = source.x + (w - source.w);
		}
		else if (rect.x <= dim.x)
		{
			source.w = (w * s);
			if (frames.at(index).get_flip() & SDL_FLIP_HORIZONTAL)
				source.x = source.x;
			else
				source.x = source.x + (w - source.w);
		}
		//printf("animation::draw: src.x=%d,src.w=%d,rect.w=%d,s=%.2f,w=%d, %s\n",source.x,source.w,rect.w,s,w,ifs.c_str());
	}

	if (rect.h < dest.h && angle == 0)
	{
		s = (float)rect.h / dest.h;
		int h = source.h;
		
		std::string ifs;
		
		if (rect.y + dest.h > dim.y + dim.h)
		{
			source.h = h * s;
			//source.x = source.w;
			if (frames.at(index).get_flip() & SDL_FLIP_VERTICAL)
				source.y = source.y + (h - source.h);
		}
		else if (rect.y <= dim.y)
		{
			source.h = (h * s);
			if (frames.at(index).get_flip() & SDL_FLIP_VERTICAL)
				source.y = source.y;
			else
				source.y = source.y + (h - source.h);
		}
		//printf("animation::draw: src.y=%d,src.h=%d,rect.h=%d,s=%.2f,h=%d, %s\n",source.y,source.h,rect.h,s,h,ifs.c_str());
	}

	if (angle == 0)
		dest = rect;

	if (frames.at(index).get_texture())
	{
		//SDL_Color color = {255,255,0,128};
		//fill_rect(renderer, color, rect);
		if (use_rot == false)
			ret = SDL_RenderCopyEx(renderer, frames.at(index).get_texture(), &source, &dest, 0, 0, frames[index].get_flip());
		else
		{
			SDL_Point center = {dest.w/2, dest.h/2};

			ret = SDL_RenderCopyEx(renderer, frames.at(index).get_texture(), &source, &dest, TO_DEGREES(frames[index].get_angle()), &center, frames[index].get_flip());
		}
	}
	
	return ret;
}
*/


