#include "handler.hpp"
#include "raylib.h"
#include "raymath.h"
#include "include/resource_dir.h"	

#include "tilemap.hpp"

int main () {
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(1600, 900, "Blobber Demo");
	SetTargetFPS(GetMonitorRefreshRate(0));
	DisableCursor();

	SearchAndSetResourceDir("resources");

	// Setup camera for 3d view
	Camera3D cam_3d {
		.position 		= 	(Vector3) { 0.0f,  1.0f, 0.0f },
		.target   		= 	(Vector3) { 0.0f,  0.0f, 0.0f },
		.up 	  		= 	(Vector3) { 0.0f,  1.0f, 0.0f },
		.fovy	  		= 	60.0f,
		.projection 	= 	CAMERA_PERSPECTIVE	
	};
	
	// Setup camera for minimap
	Camera2D cam_2d = {
		.offset = Vector2Zero(),
		.target = Vector2Zero(),
		.rotation = 0.0f,
		.zoom = 1.0f
	};

	// Declare and initialize tilemap class
	// See 'tilemap.hpp' or 'tilemap.cpp'
	Tilemap tilemap = {0};
	
	// Declare and initialize handler class
	// See 'handler.hpp' or 'handler.cpp'
	Handler handler = {
		.entity_count = 0,
		.player_index = 0,
		.cam_3d = &cam_3d,
		.tilemap = &tilemap
	};

	MapLoad("level.mlf", &tilemap);
	GenerateObjects(&handler);
	
	// Main loop
	while (!WindowShouldClose()) {
		// Update...
		HandlerUpdate(&handler);

		// Render...
		BeginDrawing();
			ClearBackground(BLACK);

			BeginMode3D(cam_3d);
			HandlerDraw3D(&handler);
			EndMode3D();

			BeginMode2D(cam_2d);
			HandlerDraw2D(&handler);	
			EndMode2D();

		EndDrawing();
	}
	
	// Cleanup
	CloseWindow();
	MapUnload(&tilemap);
	
	return 0;
}

