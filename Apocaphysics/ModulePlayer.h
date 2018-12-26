#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Shoot();
	bool isShooting = false;

	bool shootLeft = true;
	bool ShootRight = false;

	p2List<Sphere*> Shooot_List;
	p2List<PhysBody3D*>ShootPhys_List;


public:

	float ScaleTime;
	PhysVehicle3D* vehicle;

	float* ActualColorGreen = nullptr;
	float turn;
	float acceleration;
	float brake;

	Timer no_velocity_timer;
};