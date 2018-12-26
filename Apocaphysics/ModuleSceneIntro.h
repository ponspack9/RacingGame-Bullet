#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;
class Plane;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	bool reStart();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	// Creates a city with random 3x3 blocks
	void CreateCity(float max_width, vec3 pos = { 0,0,0 },float buildings_offset = 5.0f);

	// Creates a block of maximum 9 buildings, randomly placed
	void CreateBlock3x3(const vec3 &pos, int num_buildings, const float &block_width, const float &block_height, const float &block_depth, const float &offset);

	// Creates a plane building, without sidewalk
	void CreateBuilding(const vec3 &Position, const float &w, const float &h, const float &d);
	void CreateStreetLight(const vec3& Position, const float&light_w,const float&light_h,const float &cube_w,const float &cube_h,const float &cube_d);
	int total_city_cubes;
	int cubes_destroyed;
	p2List<Cube*> Building_List;
	p2List<PhysBody3D*>BuildingPhys_List;

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	Cube Time;

	Cube plane_Ground;

	PhysBody3D*Ground;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	PhysBody3D* LastCollided;

	bool game_started;
	bool AbletoCameraShake;

	Timer main_timer;
};
