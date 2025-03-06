#pragma once 

#include <cstdint>
#include <stdint.h>
#include "raylib.h"
#include "tilemath.hpp"

enum ENTITY_ID : uint8_t {
	ENT_PLAYER,
	ENT_ENEMY,
	ENT_KEY
};

// Definitions for orientation on a grid
#define NORTH (Coords) {  0, -1  }	// Enum value, 0
#define EAST  (Coords) {  1,  0  }	// Enum value, 1
#define SOUTH (Coords) {  0,  1  }	// Enum value, 2
#define WEST  (Coords) { -1,  0  }	// Enum value, 3

/* ------------------------------------------------- */ 
// [Entity class]
// See 'handler.cpp' for function definitions
typedef struct {
	bool is_active;	// Only active entities will be upated/drawn 	
	uint8_t id;		// Entity type

	Coords coords;	// Grid position, used for all game logic, movement, collision checking, etc.
	Coords facing; 	// Which direction the entity is facing towards

	short facing_enum; 	// Reference to 'facing' but, using an int, useful for continuous rotation

	Vector3 draw_position;  // Position in screen space, only used for rendering!
} Entity;
/* ------------------------------------------------- */ 

Entity EntityCreate(uint8_t id, Coords coords);

