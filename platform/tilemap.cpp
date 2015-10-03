#include "tilemap.hpp"

void CTileMap::set_tilesize ( int ts )
{
	tilesize = ts;
}

bool CTileMap::set_tile ( int x, int y, int t )
{
	if (x < 0 || y < 0)
		throw "CTileMap: get_tile posições negativas!\n";

	x = x / tilesize;
	y = y / tilesize;

	int p = y*width + x;
	if (p >= int(tileset.size()))
		return false;
	else
		tileset[p] = t;

	return true;
}

void CTileMap::set_tile ( int i, int t )
{
	if (i > -1 && i < int(tileset.size()))
	{
		tileset[i] = t;
		if (!has_tile(t))
			tiles.push_back(t);
	}
}

int CTileMap::get_tile ( int x, int y )
{
	if (x < 0 || y < 0)
	{
		printf("CTilaMap: get_tile posições negativas x = %d, y = %d\n", x, y);
		//throw "CTileMap: get_tile posições negativas!\n";
		return -1;
	}

	x = int(x / tilesize);
	y = int(y / tilesize);

	if (y * width + x < int(tileset.size()))
	{
		//if (x < width && y < height)
		return tileset.at(y * width + x);
	}

	return -1;
}

int CTileMap::get_tile ( int i )
{
	if (i > -1 && i < int(tileset.size()))
		return tileset.at(i);

	return -1;
}

int CTileMap::find_first_pos ( int t )
{
	for (unsigned int i = 0, end = tileset.size(); i < end; i++)
		if (t == tileset[i])
			return i;

	return -1; // sem posição ou não encontrado
}

int CTileMap::get_tilesize (  )
{
	return tilesize;
}

int CTileMap::get_width (  )
{
	return width;
}

int CTileMap::get_height (  )
{
	return height;
}

SDL_Rect CTileMap::get_dimension (  )
{
	return dimension;
}

bool CTileMap::has_tile ( int t )
{
	for (unsigned int i = 0, end = tiles.size(); i < end; i++)
		if (t == tiles[i])
			return true;

	return false;
}

bool CTileMap::add_tile ( int t )
{
	if (has_tile(t))
		return false;

	tiles.push_back(t);
	return true;
}

void CTileMap::remove_tile ( int t )
{
	for (std::vector <int>::iterator it = tiles.begin(), end = tiles.end(); it != end; it++)
		if (t == *it)
		{
			tiles.erase(it);
			break;
		}
}

/*
	Lẽ todo o vetor e se encontra um -1 soma uma linha
*/
int CTileMap::read ( std::vector <int> & t )
{
	tileset.clear();
	width = height = 0;
	for (std::vector <int>::iterator i = t.begin(), end = t.end(); i < end; i++)
	{
		if (*i <= -1)
		{
			height++;
			if (width == 0)
				width = int(i - t.begin());
		}
		else
		{
			tileset.push_back(*i);
			if (!has_tile(*i))
				tiles.push_back(*i);
		}
	}

	if (t.back() != -1)
		height++;

	dimension = (SDL_Rect){0,0, width * tilesize, height * tilesize};
	
	return 1;
}
/*
	cada linha de tileset está dividida com -1, ou seja,
	a cada tile sendo -1 é uma linha.
*/
int CTileMap::read ( int * t, int size )
{
	tileset.clear();
	width = height = 0;
	for (int i(0); i < size; i++)
	{
		if (t[i] == -1)
		{
			height++;
			if (width == 0)
				width = i;
		}
		else if (t[i] >= ' ')
		{
			tileset.push_back(t[i]);
			if (!has_tile(t[i]))
				tiles.push_back(t[i]);
		}
	}

	if (t[size - 1] != -1)
		height++;

	dimension = (SDL_Rect){0,0, width * tilesize, height * tilesize};
	return 1;
}

/*
	Procurar ler um mapa melhor que isso.
*/
int CTileMap::read ( std::string filename )
{
	std::ifstream file(filename.c_str(), std::ifstream::in | std::ifstream::binary);
	if (!file)
		return 0;

	tileset.clear();
	width = height = 0;
	while (file)
	{
		char tile;
		file.get(tile);

		if (tile == '\n')
		{
			if (width == 0)
			{
				width = int(tileset.size());
			}
		}
		else if (file && tile >= ' ')
		{
			tileset.push_back(tile);
			if (!has_tile(tile))
				tiles.push_back(tile);
		}
	}
	height = tileset.size() / width;
	dimension = (SDL_Rect){0,0, width * tilesize, height * tilesize};
	file.close();

	return 1;
}


