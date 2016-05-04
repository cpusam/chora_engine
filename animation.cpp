#include "animation.hpp"

void AnimationFrame::set_frame ( int d, SDL_Rect src )
{
	set_source(src);
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
	SDL_RendererFlip f = SDL_FLIP_NONE;
	
	if (hor == true)
	{
		f = SDL_FLIP_HORIZONTAL;
		if (ver == true)
			f = static_cast<SDL_RendererFlip>(static_cast<int>(f) | static_cast<int>(SDL_FLIP_VERTICAL));
	}
	
	if (ver == true)
	{
		f = SDL_FLIP_VERTICAL;
		if (hor == true)
			f = static_cast<SDL_RendererFlip>(static_cast<int>(f) | static_cast<int>(SDL_FLIP_HORIZONTAL));
	}

	flip = f;
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
	if (get_state() == ANIM_STOPED)
		set_state(ANIM_START);
	else
		set_state(ANIM_RUNNING);

	timer.start();
}

void Animation::pause (  )
{
	set_state(PAUSE_ANIM);
	timer.pause();
}

void Animation::reset (  )
{
	index = 0;
	set_state(ANIM_START);
	timer.reset();
}

void Animation::set_repeat ( bool r )
{
	repeat = r;
}

STimer Animation::get_timer (  )
{
	return timer;
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

void Animation::flip ( bool hor, bool ver )
{
	for (unsigned i = 0; i < frames.size(); i++)
		frames[i].set_flip(hor, ver);
}

void Animation::add_frame ( SDL_Texture * t, SDL_Rect src, int d )
{
	index = 0;
	texture.push_back(t);
	AnimationFrame f;

	f.set_source(src);
	f.set_destiny((SDL_Rect){0,0,src.w,src.h});
	f.set_delay(d);
	if (t)
		f.set_texture(t);

	frames.push_back(f);
}

void Animation::add_frame ( SDL_Texture * t, SDL_Rect src, SDL_Rect dst, int d )
{
	index = 0;
	texture.push_back(t);
	AnimationFrame f;

	f.set_source(src);
	f.set_destiny(dst);
	f.set_delay(d);
	if (t)
		f.set_texture(t);

	frames.push_back(f);
}

void Animation::add_frame ( SDL_Texture * t, AnimationFrame f )
{
	index = 0;
	texture.push_back(t);
	if (t)
		f.set_texture(t);
	frames.push_back(f);
}

SDL_Texture * Animation::get_texture ( int i )
{
	if (texture.size() > 0 && i < texture.size())
		return texture[i];

	return 0;
}

void Animation::destroy_textures (  )
{
	std::vector <SDL_Texture *> t;

	for (int i(0); i < texture.size(); i++)
	{
		SDL_Texture * aux = texture[i];
		int count = 0;
		for (int j(0); j < texture.size(); j++)
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
			for (int j(0); j < t.size(); j++)
				if (t.at(j) == aux)
					count++;

			if (count > 0)
				if (aux)
					t.push_back(aux);
		}
	}

	for (int i(0); i < t.size(); i++)
		if (t.at(i))
			SDL_DestroyTexture(t.at(i));

	for (int i = 0; i < frames.size(); i++)
		frames[i].destroy();

	texture.clear();
	t.clear();
}

bool Animation::has_texture ( SDL_Texture * t )
{
	if (!t)
		return false;

	for (int i(0); i < texture.size(); i++)
		if (t == texture.at(i))
			return true;

	return false;
}

bool Animation::set_index ( int i )
{
	if (i >= 0 && i < frames.size())
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

AnimationFrame Animation::get_frame ( int i )
{
	if (i > 0 && i <= frames.size())
		return frames[i];

	return AnimationFrame();
}

AnimationFrame Animation::get_curr_frame (  )
{
	return frames[index];
}

void Animation::draw ( SDL_Renderer * renderer, int x, int y )
{
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
			printf("Animation: tem que recortar a textura para o frame\n");
			SDL_RenderCopyEx(renderer, texture.at(index), &source, &dest, 0, 0, frames[index].get_flip());
		}
		else
		{
			SDL_Point center = {frames[index].get_source().w/2, frames[index].get_source().h/2};
			SDL_RenderCopyEx(renderer, texture.at(index), &source, &dest, TO_DEGREES(frames[index].get_angle()), &center, frames[index].get_flip());
		}
	}
}

void Animation::draw ( SDL_Renderer * renderer, Camera * cam, int x, int y )
{
	SDL_Rect dest, source;
	dest = frames.at(index).get_destiny();
	source = frames.at(index).get_source();

	Vect pos = cam->get_position();
	SDL_Rect dim = cam->get_dimension();

	dest.x += x + dim.x;
	dest.y += y + dim.y;

	if (use_center)
	{
		dest.x = dest.x - source.w / 2;
		dest.y = dest.y - source.h / 2;
	}
	
	#warning "Falta susbtituir essa parte pela função collision.hpp::rectIntersect\n"

	if (dest.x < dim.x + pos.x)
	{
		source.x += (dim.x + pos.x) - dest.x;
		if (((dim.x + pos.x) - dest.x) < source.w)
			source.w -= ((dim.x + pos.x) - dest.x);
		else
			//source.w = 0; // não pode ser 0 no emscriten
			return;

		dest.x = dim.x;
	}
	else if (dest.x + dest.w > pos.x + dim.x + dim.w)
	{
		if (dest.x + dest.w - (pos.x + dim.x + dim.w) < source.w)
			source.w -= dest.x + dest.w - (pos.x + dim.x + dim.w);
		else
			//source.w = 0; // não pode ser 0 no emscriten
			return;

		dest.x = dest.x - pos.x;
	}
	else
	{
		dest.x = dest.x - pos.x;
	}

	if (dest.y < dim.y + pos.y)
	{
		source.y += (dim.y + pos.y) - dest.y;
		if (((dim.y + pos.y) - dest.y) < source.h)
			source.h -= ((dim.y + pos.y) - dest.y);
		else
			//source.h = 0; // não pode ser 0 no emscriten
			return;

		dest.y = dim.y;
	}
	else if (dest.y + dest.h > pos.y + dim.y + dim.h)
	{
		if (dest.y + dest.h - (pos.y + dim.y + dim.h) < source.h)
			source.h -= dest.y + dest.h - (pos.y + dim.y + dim.h);
		else
			//source.h = 0;  // não pode ser 0 no emscriten
			return;

		dest.y = dest.y - pos.y;
	}
	else
	{
		dest.y = dest.y - pos.y;
	}

	if (texture.size() && texture.at(index))
	{
		if (use_rot == false)
			SDL_RenderCopyEx(renderer, texture.at(index), &source, &dest, 0, 0, frames[index].get_flip());
		else
		{
			SDL_Point center = {frames[index].get_source().w/2, frames[index].get_source().h/2};
			SDL_RenderCopyEx(renderer, texture.at(index), &source, &dest, TO_DEGREES(frames[index].get_angle()), &center, frames[index].get_flip());
		}
	}
}

int Animation::update (  )
{
	switch (get_state())
	{
		case ANIM_START:
		case ANIM_CHANGE_FRAME:
		case ANIM_RUNNING:
		case ANIM_FINISHED:
			if (frames.size() == 0)
				throw "Animation: animação sem frames\n";

			timer.update();
			if (timer.steps() >= frames[index].get_delay())
			{
				timer.reset();
				index++;
				if (index >= frames.size())
				{
					if (repeat)
					{
						index = 0;
						set_state(ANIM_FINISHED); // termina e repete a animação
						break;
					}
					else
					{
						index = int(frames.size() - 1);
						set_state(ANIM_STOPED); // terminou a animação e fica parado
						break;
					}
				}

				set_state(ANIM_CHANGE_FRAME); // novo frame
				break;
			}

			set_state(ANIM_RUNNING); // rodando
			break;

		default:
			break;
	}

	return get_state();
}


