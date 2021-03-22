/*
	-Programa para criar fontes para os programa que usam a chora engine java
	02/12/2015 às 20:03
	./fonter font=20db.otf size=15 dim=512 color=0/0/0/255 text=sacii fileName=fontOut.txt
*/

#include "Chora.hpp"
#include <cstring>
#include <string>
#include "../fonter/SDL_SavePNG/savepng.h"

int main ( int argc, char ** argv )
{
	if (argc < 5)
	{
		printf("Usage: %s image=\"image file[png]\" size=N dim=N color=R/G/B/A text=AbCdE...|ascii fileName=fileOut.txt \n", argv[0]);
		return 1;
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return 1;
	}
	
	if (TTF_Init() < 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_Window * window = SDL_CreateWindow("Fonter", 0,0, 512,512, 0);
	if (!window)
	{
		printf("Error: %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_Renderer * renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		printf("Error: %s\n", SDL_GetError());
		return 1;
	}
	
	char image[256];
	int size = 50;
	char text[512] = "\0";
	int dim = 512;
	char name[256];
	memset(name, 0, sizeof(name));
	SDL_Color color = {255,255,255,255};
	int count = 0;
	for (int i = 0; i < argc; i++)
	{
		if (strstr(argv[i], "image="))
		{
			sscanf(argv[i],"image=%s",image);
			count++;
		}
		
		if (strstr(argv[i],"size="))
		{
			sscanf(argv[i],"size=%d",&size);
			count++;
		}
		
		if (strstr(argv[i],"text="))
		{
			sscanf(argv[i],"text=%s",text);
			count++;
		}
		
		if (strstr(argv[i],"color="))
		{
			sscanf(argv[i],"color=%hhu/%hhu/%hhu/%hhu", &color.r, &color.g,&color.b,&color.a);
			count++;
		}
		
		if (strstr(argv[i],"dim="))
		{
			sscanf(argv[i],"dim=%d", &dim);
			count++;
		}
		
		if (strstr(argv[i],"fileName="))
		{
			sscanf(argv[i],"fileName=%s", name);
			count++;
		}
	}
	
	if (strstr(text, "ascii"))
	{
		memset(text, 0x0,sizeof(text));
		for (int i = 0; i < 256; i++)
			text[i] = (char)(i + ' ');
	}
	
	if (count < 6)
	{
		printf("faltou parametros do spriter\n");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
	
		SDL_Quit();
		return 1;
	}
	
  Entity * entityImage = new Entity();

  
  
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	
	SDL_Quit();
	
	return 0;
	
}


