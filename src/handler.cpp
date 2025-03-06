#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <stdint.h>
#include <sys/types.h>
#include "handler.hpp"
#include "tilemap.hpp"
#include "entity.hpp"
#include "tilemath.hpp"

/* ------------------------------------------------------------------------------------------------------------------------------------- */
// v [SETUP VARIABLES] v

// Not an enum technically, but functionally the same 
Coords FACING_ENUM[4] = {
	NORTH,		// 0, {  0, -1 }
	EAST,		// 1, {  1,  0 }
	SOUTH,		// 2, {  0,  1 }
	WEST		// 3, { -1,  0 }
};

bool CAMERA_ROTATING = false;	// Is the camera rotating 
bool CAMERA_MOVING  = false;	// Is the camera moving

Vector3 camera_target_destination;		// Where the camera target should move towards
Vector3 camera_position_destination;	// Where the camera position should move towards

/* ------------------------------------------------------------------------------------------------------------------------------------- */
// v [GAME SETUP FUNCTIONS] v

// Initialize and entity instance
Entity EntityCreate(uint8_t id, Coords coords) {
	return (Entity) {
		.is_active = true,
		.id = id,
		.coords = coords,
		.facing = SOUTH,
		.draw_position = {0, 1, 0}
	};
}

// Generate all objects after loading map
void GenerateObjects(Handler *handler) {
	// start count at 0, then count number of entities to be allocated
	handler->entity_count = 0;

	// Loop through map data array to count entities,
	// We'll create them later!
	for(uint16_t i = 0; i < handler->tilemap->tile_count; i++) {
		switch(handler->tilemap->map_data[i]) {
			case TILE_PLAYER:
			case TILE_ENEMY:
			case TILE_KEY:
				handler->entity_count++;
				break;
		}
	}
	
	// Allocate memory for entities, now that we know how many should exist  
	handler->entities = (Entity*)malloc(sizeof(Entity) * handler->entity_count);

	// And, now we make them!
	uint8_t local_entity_count = 0;	// We'll have to keep track of where in the array to place the entites we create,
	// we'll increment this variable by 1 each time we create a new entity

	// Loop through map data array, instance and index entities corresponding with tilemap data
	for(uint16_t i = 0; i < handler->tilemap->tile_count; i++) {
		Coords coords = GetCoords(i, handler->tilemap);	// Get coordinates from index to know where to place the entity
	
		// We will also initialize our tile class instances
		handler->tilemap->tile_pool[i] = (Tile) {
			.traversible = true,
			.contents = {0, 0}
			// By default a tile will be traversible and contain no entities
		};

		switch(handler->tilemap->map_data[i]) {
			case TILE_WALL:
				handler->tilemap->tile_pool[i].traversible = false;
				break;

			case TILE_PLAYER:
				handler->player_index = local_entity_count;	// Store player index so we don't have to search through the array for it too often
				handler->entities[local_entity_count] = EntityCreate(ENT_PLAYER, coords); // Add entity to array 
				local_entity_count++;						// Increment local count
				printf("Player created! index[%d]\n", handler->player_index);
				break;

			case TILE_ENEMY:
				handler->entities[local_entity_count] = EntityCreate(ENT_ENEMY, coords);
				local_entity_count++;
				break;

			case TILE_KEY:
				handler->entities[local_entity_count] = EntityCreate(ENT_KEY, coords);
				local_entity_count++;
				break;
		}
	}
}

/* ------------------------------------------------------------------------------------------------------------------------------------- */
// v [UPDATE LOGIC FUNCTIONS] v

void HandlerUpdate(Handler *handler) {
	Entity &player = handler->entities[handler->player_index];	// Reference to player

	if(CAMERA_ROTATING) {
		// Lerp camera target towards destination
		handler->cam_3d->target = Vector3Lerp(handler->cam_3d->target, camera_target_destination, 0.25f);

		// If close enough to destination, end rotation
		if(Vector3Distance(handler->cam_3d->target, camera_target_destination) <= 0.01f) CAMERA_ROTATING = false;
	} else {
		// Snap camera target to destination
		handler->cam_3d->target = (Vector3) {
			handler->cam_3d->position.x + player.facing.col, 0.0f,
			handler->cam_3d->position.z + player.facing.row
		};
	}
	
	if(CAMERA_MOVING) {
		// Lerp camera position towards destination
		handler->cam_3d->position = Vector3Lerp(handler->cam_3d->position, camera_position_destination, 0.5f);

		// If close enough to destination, end movement
		if(Vector3Distance(handler->cam_3d->position, camera_position_destination) <= 0.01f) CAMERA_MOVING = false;
	} else {
		// Snap camera position to destination
		handler->cam_3d->position = GridToVec3(player.coords, 1);
	}
	
	// Only allow player input while camera is't transforming/rotating
	if(!CAMERA_ROTATING && !CAMERA_MOVING) {
		PlayerInput(handler);
	}
}

void PlayerInput(Handler *handler) {
	Entity &player = handler->entities[handler->player_index];

	bool ACTION_VALID = false;

	// Turn left
	if(IsKeyPressed(KEY_LEFT)) {
		player.facing_enum--;
		if(player.facing_enum < 0) player.facing_enum = 3;
		player.facing = FACING_ENUM[player.facing_enum];

		camera_target_destination = (Vector3) {
			handler->cam_3d->position.x + player.facing.col, 0.0f,
			handler->cam_3d->position.z + player.facing.row
		};

		CAMERA_ROTATING = true;
		ACTION_VALID = true;
	}
	
	// Turn right
	if(IsKeyPressed(KEY_RIGHT)) {
		player.facing_enum++;
		if(player.facing_enum > 3) player.facing_enum = 0;
		player.facing = FACING_ENUM[player.facing_enum];

		camera_target_destination = (Vector3) {
			handler->cam_3d->position.x + player.facing.col, 0.0f,
			handler->cam_3d->position.z + player.facing.row
		};

		CAMERA_ROTATING = true;
		ACTION_VALID = true;
	}

	// Move forward
	if(IsKeyPressed(KEY_UP)) {
		Coords new_position = CoordsAdd(player.coords, player.facing); // Add player position and direction to get new player position 
		uint16_t new_position_index = GetIndex(new_position, handler->tilemap); // Get tile array index of position we want to move to	
		
		// If the new tile doesn't have a wall, move to it
		if(handler->tilemap->tile_pool[new_position_index].traversible) {
			player.coords = new_position;

			camera_position_destination = GridToVec3(new_position, 1);

			CAMERA_MOVING = true;
			ACTION_VALID = true;
		}
	}

	//if(ACTION_VALID) ProcessTurn(handler);
}

/* ------------------------------------------------------------------------------------------------------------------------------------- */
// v [DRAWING FUNCTIONS] v

// Draw minimap
void HandlerDraw2D(Handler *handler) {
	Entity &player = handler->entities[handler->player_index];	// Reference to player

	// Draw black rectangle as a background
	DrawRectangleV(Vector2Zero(), (Vector2){
			(float)(handler->tilemap->cols * 8),
			(float)(handler->tilemap->rows * 8)
			}, ColorAlpha(BLACK, 1.0f));
	
	// Draw tiles/entities 
	for(uint16_t i = 0; i < handler->tilemap->tile_count; i++) { 
		Coords coords = GetCoords(i, handler->tilemap);
		Vector2 draw_pos = (Vector2){(float)coords.col * 8, (float)coords.row * 8 };	
		
		if(handler->tilemap->map_data[i] == TILE_WALL) {
			DrawRectangleV(draw_pos, (Vector2){8, 8}, LIGHTGRAY);
		}
		
		if(CoordsEqual(coords, player.coords)) {
			Vector2 center = Vector2Add(GridToVec2(coords, 8), {4, 4});
			Vector2 line_end_center = Vector2Add(GridToVec2(CoordsAdd(coords, player.facing), 8), {4, 4});
			DrawLineEx(center, line_end_center, 4, RED);

			DrawRectangleV(draw_pos, (Vector2){8, 8}, GREEN);
		}
	}
}

// Draw 3D view
void HandlerDraw3D(Handler *handler) {
	Entity &player = handler->entities[handler->player_index];	// Reference to player

	// Draw walls
	for(uint16_t i = 0; i < handler->tilemap->tile_count; i++) {
		if(handler->tilemap->map_data[i] == TILE_WALL) {
			Coords coords = GetCoords(i, handler->tilemap);
			Vector3 draw_pos = GridToVec3(coords, 1);
			draw_pos.y = 0.0f;
			Vector3 size = { 1, 1, 1 };
			
			// We can add some depth to the scene with some simple lighting
			Color color = DARKGRAY;
			
			// Modifier for our color, calculated by distance from camera/player
			float color_mod = 1 - (CoordsDistance(player.coords, coords) * 0.05f);

			if(color_mod < 0) color_mod = 0; 	// An unsigned char will loop to 255 if lower than 0, so we limit it
			
			// Muliply color by the modifier
			color = (Color) {
				(u_char)(color.r * color_mod),
				(u_char)(color.g * color_mod),
				(u_char)(color.b * color_mod),
				color.a	// Keep alpha the same, we don't want transperancy!
			};
			
			// Draw the wall using the new color
			DrawCubeV(draw_pos, size, color);
		}
	}
}
/* ------------------------------------------------------------------------------------------------------------------------------------- */

