#pragma once 

#include <cstdint>
#include <raylib.h>
#include <stdint.h>
#include "entity.hpp"
#include "tilemap.hpp"

// Handler class
// See 'handler.cpp' for function definitions
typedef struct {
	uint8_t entity_count;	// Number of entities 
	// Note: I like writing in C style and this is not at all necessary if you're using a std::vector or list  
	
	uint8_t player_index;	// Element in entities array which contains player character 	
	Entity *entities; 		// Entity array

	Camera3D *cam_3d;  		// Pointer to camera class
	Tilemap *tilemap; 		// Pointer to tilemap class
} Handler;

void GenerateObjects(Handler *handler);		// Generates tiles and entities, called once after loading level
void ProcessTurn(Handler *handler);			// Updates entities, called once after every player input

void HandlerUpdate(Handler *handler);		// Update objects

void HandlerDraw2D(Handler *handler);		// Draw minimap
void HandlerDraw3D(Handler *handler);		// Draw scene

void PlayerInput(Handler *handler);			// Take player input

void HanlderClose(Handler *handler);		// Free allocated memory
