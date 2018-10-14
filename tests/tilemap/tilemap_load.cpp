#include "Chora.hpp"

int main (int argc, char **argv )
{
	TileMap * tileMap = new TileMap(32);

	if (tileMap->read_from_xpm("imagem.xpm") == 0)
		printf("Não abriu imagem.xpm\n");
	
	delete tileMap;
	return 0;
}
