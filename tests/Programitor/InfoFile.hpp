/*
 * infofile.hpp
 * 
 * Copyright 2015 Thiago <thiago@thihup-pc>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#ifndef INFOFILE_HPP
#define INFOFILE_HPP

#include "Chora.hpp"

class InfoFile{
	
	static InfoFile *singleton;
	
	std::vector<std::string> name;
	std::vector<SDL_Rect> position;
	
	SDL_Surface *auxSurface;
	SDL_Texture *auxTexture;
	
	InfoFile();
	
	~InfoFile();
	
	public:
	
	static InfoFile *instance();
	
	void add(std::string n, SDL_Renderer *r);
	
	SDL_Texture *getTexture(std::string n);
	
	SDL_Rect *getPos(std::string n);
	
	void setPos(std::string n, SDL_Rect p);

	void drawAll(SDL_Renderer *r);
	
	void destroy();
	
	int getSize();
};


#endif /* INFOFILE_HPP */ 
