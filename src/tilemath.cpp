#include <cmath>
#include <cstdint>
#include <stdint.h>
#include "raylib.h"
#include "raymath.h"
#include "tilemath.hpp"

// Convert coordinates to Vector2
Vector2 GridToVec2(Coords coords, float tile_size) {
	return (Vector2) {
		coords.col * tile_size,
		coords.row * tile_size
	};
};

// Convert coordinates to Vector3
Vector3 GridToVec3(Coords coords, float tile_size) {
	Vector3 vec3;
	// Calculate X and Z, Y is ignored
	vec3.x = (float)(coords.col * tile_size);
	vec3.z = (float)(coords.row * tile_size);
	return  vec3;
};

// Convert Vector2 to coordinates
Coords Vec2ToGrid(Vector2 vec2, float tile_size) {
	return (Coords) {
		(uint16_t)(vec2.x / tile_size),
		(uint16_t)(vec2.y / tile_size)
	};
}

// Convert Vector3 to coordinates
Coords Vec3ToGrid(Vector3 vec3, float tile_size) {
	return (Coords) {
		(uint16_t)(vec3.x / tile_size),
		(uint16_t)(vec3.y / tile_size)
	};
}

// Returns sum of to sets of coordinates
Coords CoordsAdd(Coords a, Coords b) {
	return (Coords) {
		(uint16_t)(a.col + b.col),
		(uint16_t)(a.row + b.row)
	};	
};

// Returns distance of two sets of coordinates
short CoordsDistance(Coords a, Coords b) {
	return (short) {
		(short)sqrt(
			((b.col - a.col) * (b.col - a.col)) + 
			((b.row - a.row) * (b.row - a.row))
		)
	};
}

// Check if two sets of coordinates are equal 
bool CoordsEqual(Coords a, Coords b) {
	if(a.col - b.col == 0 && a.row - b.row == 0) return true;
	else return false;
}

}
