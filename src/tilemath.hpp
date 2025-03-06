#pragma once

#include <stdint.h>
#include "raylib.h"

// Basic unit of measurement in a 2D grid
typedef struct {
	int col, row;
} Coords;

// Helper functions for dealing with grid manipulations
// See 'tilemath.cpp' for function definitions

Vector2 GridToVec2(Coords coords, float tile_size);		// Convert grid coordinates to Vector2
Vector3 GridToVec3(Coords coords, float tile_size);		// Convert grid coordinates to Vector3
Coords Vec2ToGrid(Vector3 vec3, float tile_size);		// Convert a Vector2 to grid coordinates
Coords Vec3ToGrid(Vector3 vec3, float tile_size);		// Convert a Vector3 to grid coordinates
Coords CoordsAdd(Coords a, Coords b);					// Returns sum of two sets of coordinates
short CoordsDistance(Coords a, Coords b);				// Returns distance of two sets of coordinates
bool CoordsEqual(Coords a, Coords b);					// Returns true if two sets of coords are equivalent
