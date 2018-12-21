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



	CreateBuilding(vec3(10,0,6));

	CreateBuilding(vec3(20, 0, 6));

	CreateBuilding(vec3(15, 0, 20));
	
	CreateBuilding(vec3(40, 0, 20));
	//CreateBuilding(vec3(15, 0, 20));


	//CreateBuilding(vec3(55, 0, 30));
	//CreateBuilding(vec3(45, 0, 10));
	//CreateBuilding(vec3(15, 0, 20));
	return ret;
}
void ModuleSceneIntro::CreateBuilding(vec3 Position) {

	//Acera.SetPos();

	float w = 5;
	float h = 3;

	float x = w / 2;
	float y = h / 2;
	float z = w / 2;
	int HeightBuilding = rand() % 4 + 6;

	Acera.size = vec3(w * 2+2, 0.2f, w * 2+2);
	Acera.SetPos(w + Position.x, 0.2 / 2, w + Position.z);

	
	BuildingPhys_List.add(App->physics->AddBody(Acera, 500));
	Building_List.add(&Acera);
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

