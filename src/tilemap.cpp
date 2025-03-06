#include "tilemap.hpp"
#include "tilemath.hpp"
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <string>
#include <strings.h>

/* ------------------------------------------------- */ 
/*  		   [ HELPER FUNCTIONS ]					 */  	
uint16_t GetIndex(Coords coords, Tilemap *tilemap) {
	// column + row * map width will always equal index
	return coords.col + coords.row * tilemap->cols;
};

Coords GetCoords(uint16_t tile_index, Tilemap *tilemap) {
	// mod of index by map width will always equal column
	// division of index and map width will always equal row
	return (Coords) {
		(tile_index % tilemap->cols),   // Column
		(tile_index / tilemap->cols)	// Row
	};
}
/* ------------------------------------------------- */ 

void MapLoad(std::string file_path, Tilemap *tilemap) {
	std::ifstream file(file_path);	

	if(file.is_open()) {
		std::string w;
		std::string h;

		std::getline(file, w);
		std::getline(file, h);

		tilemap->cols = std::stoi(w);
		tilemap->rows = std::stoi(h);
		tilemap->tile_count = tilemap->cols * tilemap->rows;

		tilemap->map_data = (char*)malloc(sizeof(char) * tilemap->tile_count);
		tilemap->tile_pool = (Tile*)malloc(sizeof(Tile) * tilemap->tile_count);

		for(uint16_t r = 0; r < tilemap->rows; r++) {
			std::string line; 
			std::getline(file, line);

			for(uint16_t c = 0; c < tilemap->cols; c++) {
				uint16_t tile_index = GetIndex((Coords){c, r}, tilemap);
				char ch = line[c];
				tilemap->map_data[tile_index] = ch;
			}
		}

		file.close();
	} else puts("ERROR: Could not locate level file");
}

void MapUnload(Tilemap *tilemap) {
	free(tilemap->map_data);
	free(tilemap->tile_pool);
}

