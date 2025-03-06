#pragma once 

#include <cstdint>
#include <stdint.h>
#include <string>
#include "entity.hpp"
#include "tilemath.hpp"

// See 'tilemap.cpp' for function definitions

enum TILE_ID : char {
	TILE_EMPTY 	 	= '0',
	TILE_WALL		= '1',
	TILE_PLAYER		= '2',
	TILE_DOOR		= '4',
	TILE_ENEMY		= '5',
	TILE_KEY 		= '6'
};


// Tile class
typedef struct {
	bool traversible;			// Can an entity move through this tile?
	Entity *contents[2];		// Entities contained in this tile(max 2)
	// Note, we won't need to store the grid coordinates of tiles. 
	// The tile's index in the array will do that for us!
} Tile;

// Tilemap class
typedef struct {
	uint16_t cols;			// Width of map measured in tiles
	uint16_t rows;			// Height of map measured in tiles
	uint16_t tile_count;    // Surface area measured in tiles

	char *map_data;			// Pointer to data array
	// The array will be allocated at load time of the level, 
	// with the number of elements being equal to the total
	// number of tiles the loaded level contains
	// Static. This array will not change until a new level is loaded

	Tile *tile_pool; 		// Pointer to tile array
	// Also allocated at load time with element count
	// equal to map surface area
	// Dynamic! The contents of this array will be modified as the game is played,
	// reflecting movement, creation, and deletion of entities 
} Tilemap;

void MapLoad(std::string file_path, Tilemap *tilemap);
void MapUnload(Tilemap *tilemap);

uint16_t GetIndex(Coords coords, Tilemap *tilemap);
Coords GetCoords(uint16_t tile_index, Tilemap *tilemap);
Tile FetchTile(Coords coords, Tilemap *tilemap);

