#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	//Cube a;


	//float w = 3;
	//float h = 3;

	//int x = w/2;
	//float y = h/2;
	//int z = w/2;
	//int size = rand()%4+6;

	////a.transform.scale(w,h,w);
	//a.size = (w, h, w);
	//a.wire = true;
	//for (z; z <= w * 2; z = z + w) {
	//	for (x; x <= w * 2; x = x + w) {
	//		for (y; y <= h * size; y = y + h) {
	//			a.SetPos(x, y, z);
	//			App->physics->AddBody(a, 5);
	//		}
	//		y = h / 2;
	//	}
	//	x = w / 2;
	//}


	//CreateBuilding(vec3(10,0,6));

	//CreateBuilding(vec3(20, 0, 6));

	//CreateBuilding(vec3(15, 0, 20));
	//
	//CreateBuilding(vec3(40, 0, 20));
	//CreateBuilding(vec3(15, 0, 20));


	//CreateBuilding(vec3(55, 0, 30));
	//CreateBuilding(vec3(45, 0, 10));
	//CreateBuilding(vec3(15, 0, 20));

	//CreateBlock(8, 6, 4);
	CreateBlock3x3(9, 6, 4);

	return ret;
}

/* *Creates a block of buildings in a 3x3 grid
   *Maximum 9 buildings
   *Places it randomly */
void ModuleSceneIntro::CreateBlock3x3(int num_buildings, float width, float height)
{
	if (num_buildings > 9) num_buildings = 9;

	int margin = 9 - num_buildings;
	int placed = 0;

	float offset = 5;

	vec3 pos = { 20,0,30 };
	vec3 aux_pos = pos;

	// Creating sidewalk
	sidewalk.size = vec3(width * 9 + 2, 0.2f, width * 9  + 2);
	sidewalk.SetPos(width*4 + pos.x, 0.2 / 2, width*4 + pos.z);


	BuildingPhys_List.add(App->physics->AddBody(sidewalk, 500));
	Building_List.add(&sidewalk);

	// Randomly fills a imaginary 3x3 boolean matrix to place the buildings
	for (int i = 0; i < 3; i++) {
		pos.z = aux_pos.z + i * (width * 2 + offset);

		for (int j = 0; j < 3; j++) {

			if (placed >= num_buildings) break;
			pos.x = aux_pos.x + j * (width * 2 + offset);

			if (margin > 1) {
				if ((rand() + SDL_GetTicks()) % 2) margin--;
				else {
					CreateBuilding(pos, width, height);
					placed++;
				}
			}
			else {
				CreateBuilding(pos, width, height);
				placed++;
			}
		}
	}

}


void ModuleSceneIntro::CreateBuilding(vec3 Position, float w, float h) 
{

	float x = w / 2;
	float y = h / 2;
	float z = w / 2;
	int HeightBuilding = rand() % 4 + 2;

	a.size = vec3(w, h, w);

	for (z; z <= w * 2; z = z + w) {
		for (x; x <= w * 2; x = x + w) {
			for (y; y <= h * HeightBuilding; y = y + h) {
				a.SetPos(x + Position.x, y + Position.y, z + Position.z);
				a.wire = false;
				a.color = Red;
				BuildingPhys_List.add(App->physics->AddBody(a, 50));
				Building_List.add(&a);
			}
			y = h / 2;
		}
		x = w / 2;
	}

}

void ModuleSceneIntro::CreateBuilding(vec3 Position) {

	//sidewalk.SetPos();

	float w = 5;
	float h = 3;

	float x = w / 2;
	float y = h / 2;
	float z = w / 2;
	int HeightBuilding = rand() % 4 + 6;

	sidewalk.size = vec3(w * 2+2, 0.2f, w * 2+2);
	sidewalk.SetPos(w + Position.x, 0.2 / 2, w + Position.z);

	
	BuildingPhys_List.add(App->physics->AddBody(sidewalk, 500));
	Building_List.add(&sidewalk);
	//a.transform.scale(w,h,w);
	a.size = vec3(w, h , w);

	for (z; z <= w * 2; z = z + w) {
		for (x; x <= w * 2; x = x + w) {
			for (y; y <= h * HeightBuilding; y = y + h) {
				a.SetPos(x+Position.x, y+Position.y, z+Position.z);
				a.wire = false;
				a.color = Red;
				BuildingPhys_List.add(App->physics->AddBody(a, 50));
				Building_List.add(&a);
			}
			y = h / 2;
		}
		x = w / 2;
	}

}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	p2List_item<Cube*>*Buildings=Building_List.getFirst();
	p2List_item<PhysBody3D*>*BuildingsPhys = BuildingPhys_List.getFirst();

	for (; Buildings != nullptr; Buildings = Buildings->next) {
		BuildingsPhys->data->GetTransform(&Buildings->data->transform);
		Buildings->data->Render();
		BuildingsPhys = BuildingsPhys->next;
	}

	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

