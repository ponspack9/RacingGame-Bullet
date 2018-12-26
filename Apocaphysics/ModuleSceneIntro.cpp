#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "ModulePhysics3D.h"
#include "PhysVehicle3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	total_city_cubes = 0;
	cubes_destroyed = 0;
	game_started = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	AbletoCameraShake = true;

	LOG("Loading Intro assets");
	bool ret = true;
	cubes_destroyed = 0;
	App->camera->Move(vec3(40.0f, 200.0f, 40.0f));
	App->camera->LookAt(vec3(40.0f, 0.0f, 40.0f));
  
	plane_Ground=Cube(600.0f, 0.2f,600.f);
	plane_Ground.wire = false;
	plane_Ground.color = Grey;
	
	plane_Ground.SetPos(0, -2, 0);
	Ground=App->physics->AddBody(plane_Ground,10000);
	Ground->type = GROUND;
  
	CreateCity(20, { -100,0,-50 });

	CreateStreetLight(vec3(10,0,10), 0.3, 7, 2,1.5, 1.5);

	return ret;
}


/*	* Creates a random city based on 3x3 blocks
	* Random size for each block
	* max_width		-> maximum width of the city (squared)
	* pos			-> position where first block will be placed
*/
void ModuleSceneIntro::CreateCity(float max_width, vec3 pos, float buildings_offset) {

	int n_buildings = 0;
	int placed = 0;

	float block_width  = 10;
	float block_height = 10;
	float block_depth  = 10;

	float road_offset = 10.0f;

	vec3 aux_pos = pos;

	// First bucle stands for x axis block creation
	while (pos.x <= (max_width + aux_pos.x))
	{
		//Defining next block
		n_buildings = (rand() + SDL_GetTicks()) % 8 + 2;
		block_width = (rand() % 20) + 5.0f;
		block_depth = ((rand() + SDL_GetTicks()) % 20) + 5.0f;

		CreateBlock3x3(pos, n_buildings, block_width, block_height, block_depth, buildings_offset);
		placed++;

		// For each block created on x axis, the following blocks in z axis must respect its width
		while (pos.z <= (max_width + aux_pos.z)) {

			pos.z += block_depth * 2 + buildings_offset * 4 + road_offset;
			block_depth = ((rand() + SDL_GetTicks()) % 20) + 5.0f;
			CreateBlock3x3(pos, n_buildings, block_width, block_height, block_depth, buildings_offset);

		}

		pos.z = aux_pos.z;
		pos.x += block_width * 2 + buildings_offset * 4 + road_offset;
	}

}

/* * Creates a block of buildings in a 3x3 grid
   * Maximum 9 buildings
   * Building width and height depends on block size
   * Places it randomly */
void ModuleSceneIntro::CreateBlock3x3(const vec3 &pos, int num_buildings, const float &block_width, const float &block_height, const float &block_depth, const float &offset)
{
	if (num_buildings > 9) num_buildings = 9;
	
	vec3 position = { pos.x + offset, pos.y, pos.z + offset };
	vec3 aux_position = position;

	int margin = 9 - num_buildings;
	int placed = 0;


	float width  = block_width  / 3;
	float height = block_height / 3;
	float depth  = block_depth  / 3;

	Cube *sidewalk = new Cube();

	// Creating sidewalk
	sidewalk->size = vec3(block_width*2 + offset*4, 0.2f, block_depth*2 + offset*4);
	sidewalk->SetPos(block_width + offset + position.x, 0.2f / 2, block_depth + offset + position.z);
	
	BuildingPhys_List.add(App->physics->AddBody(*sidewalk, 9999));
	Building_List.add(sidewalk);


	// Randomly fills a imaginary 3x3 boolean matrix to place the buildings
	for (int i = 0; i < 3; i++) 
	{
		position.z = aux_position.z + i * (depth*2 + offset);

		for (int j = 0; j < 3; j++) 
		{
			if (placed >= num_buildings) return;

			position.x = aux_position.x + j * (width*2 + offset);

			if (margin > 1) 
			{
				if ((rand() + SDL_GetTicks()) % 2) margin--;
				else {
					CreateBuilding(position, width, height, depth);
					placed++;
				}
			}
			else 
			{
				CreateBuilding(position, width, height, depth);
				placed++;
			}
		}
	}

}

void ModuleSceneIntro::CreateStreetLight(const vec3& Position, const float&light_w, const float&light_h, const float &cube_w, const float &cube_h, const float &cube_d) {

	float x = light_w / 2;
	float y = light_h / 2;
	float z = light_w / 2;
	
	Cube* streetLight= new Cube(light_w,light_h,light_w);
	Cube* Toplight = new Cube(cube_w, cube_h, cube_d);

	streetLight->SetPos(x + Position.x, y + Position.y, z + Position.z);
	streetLight->wire = false;
	streetLight->color = White;
	Toplight->SetPos(x + Position.x, light_h,z+Position.z);
	Toplight->wire = false;
	Toplight->color = Yellow;
	BuildingPhys_List.add(App->physics->AddBody(*streetLight, 5000));
	BuildingPhys_List.add(App->physics->AddBody(*Toplight, 5000));
	BuildingPhys_List.getLast()->data->type = STREET_LIGHT;
	BuildingPhys_List.getLast()->data->collision_listeners.add(App->scene_intro);

	Building_List.add(streetLight);
	Building_List.add(Toplight);

}

void ModuleSceneIntro::CreateBuilding(const vec3 &Position, const float &w, const float &h, const float &d)
{

	float x = w / 2;
	float y = h / 2;
	float z = d / 2;
	int HeightBuilding = rand() % 4 + 2;

	int colRand = rand() % 4;
	Color colorBuild;
	switch (colRand) {
	case 0:
		colorBuild = Grey;
		break;
	case 1:
		colorBuild = Grey2;
		break;
	case 2:
		colorBuild = Grey3;
		break;
	case 3:
		colorBuild = Grey4;
		break;
	}

	/*Cube *a = new Cube();
	a->size = vec3(w, h, d);*/

	for (z; z <= d * 2; z = z + d) {
		for (x; x <= w * 2; x = x + w) {
			for (y; y <= h * HeightBuilding; y = y + h) {
				Cube *a = new Cube();
				a->size = vec3(w, h, d);
				a->SetPos(x + Position.x, y + Position.y, z + Position.z);
				a->wire = false;
				a->color =colorBuild;
				BuildingPhys_List.add(App->physics->AddBody(*a, 50));
				BuildingPhys_List.getLast()->data->type = BUILDING;
				BuildingPhys_List.getLast()->data->collision_listeners.add(App->scene_intro);
				Building_List.add(a);
				total_city_cubes++;
			}
			y = h / 2;
		}
		x = w / 2;
	}
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	Building_List.clear();
	BuildingPhys_List.clear();
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	//Counter

	//Time.SetPos(App->player->vehicle->GetPosition().x, App->player->vehicle->GetPosition().y, App->player->vehicle->GetPosition().z);

	//Time.Render();
	plane_Ground.Render();



	p2List_item<PhysBody3D*>*BuildingsPhys = BuildingPhys_List.getFirst();
	p2List_item<Cube*>*Buildings=Building_List.getFirst();

	for (; Buildings != nullptr; Buildings = Buildings->next) {
		if (BuildingsPhys->data->toDelete == true) {
			BuildingsPhys->data->count++;
			if (BuildingsPhys->data->count >= 70) {
				BuildingsPhys->data->SetPos(5000, -200, 0);
				BuildingsPhys->data->toDelete = false;
			
			}
		}
		BuildingsPhys->data->GetTransform(&Buildings->data->transform);
		Buildings->data->Render();
		
		BuildingsPhys = BuildingsPhys->next;
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT)
	{
		if (!game_started) {
			game_started = true;
			main_timer.Start();

		}
	}
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		reStart();
	}

	

	return UPDATE_CONTINUE;
}
bool ModuleSceneIntro::reStart() {

	App->physics->CleanUp();
	App->physics->Start();
	App->player->Start();
	App->player->CleanUp();
	App->scene_intro->CleanUp();
	App->scene_intro->Start();


	return true;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{ 

	if (LastCollided == body1 || LastCollided == body2) {
		return;
	}
	if (body1->type == STREET_LIGHT&&body2->type!=NONE) {
		p2List_item<PhysBody3D*>*BuildingsPhys = BuildingPhys_List.getFirst();
		p2List_item<Cube*>*Buildings = Building_List.getFirst();
		for (; BuildingsPhys != nullptr; BuildingsPhys = BuildingsPhys->next) {
			if (BuildingsPhys->data == body1) {
				Buildings->data->color = Black;
				break;
			}
			Buildings = Buildings->next;
		}
	}
	else if (body2->type == STREET_LIGHT&&body1->type!=NONE) {
		p2List_item<PhysBody3D*>*BuildingsPhys = BuildingPhys_List.getFirst();
		p2List_item<Cube*>*Buildings = Building_List.getFirst();
		for (; BuildingsPhys != nullptr; BuildingsPhys = BuildingsPhys->next) {
			if (BuildingsPhys->data == body2) {
				Buildings->data->color = Black;
				break;
			}
			Buildings = Buildings->next;
		}
	}
	if ((body1->type== MISSILE || body1->type == GROUND)&&body2->type == BUILDING|| (body2->type==MISSILE || body2->type == GROUND)&&body1->type == BUILDING) {
		
		if (body1->type == BUILDING&&body2->type!=BUILDING&&body2->type!=NONE) {
			LastCollided = body1;
			p2List_item<PhysBody3D*>*BuildingsPhys = BuildingPhys_List.getFirst();
			p2List_item<Cube*>*Buildings = Building_List.getFirst();

			for (; BuildingsPhys != nullptr; BuildingsPhys = BuildingsPhys->next) {
				if (BuildingsPhys->data == body1&&body1->toDelete == false) {
					BuildingsPhys->data->toDelete = true;
					Buildings->data->color = Red;
					App->player->ScaleTime = 0;
					App->player->vehicle->TimerB = 1.0f;
					App->player->vehicle->TimerG = 1.0f;
					cubes_destroyed++;
					if (body2->type == MISSILE&&App->camera->DoCameraShake == false && AbletoCameraShake) {
						float distance;
						distance = sqrt(pow((body2->GetPosition().x - App->player->vehicle->Position().x), 2) + pow(body2->GetPosition().z - App->player->vehicle->Position().z, 2));
						distance /= 13;
						App->camera->CameraShake_Time.Start();
						App->camera->power = 1.0f/distance; //this makes the camera shake more Drastic if wee are near to the building
						App->camera->Time_Doing_Shake = 0.2f;
						App->camera->DoCameraShake = true;
						AbletoCameraShake = false;
					}
					//delete BuildingsPhys->data;
					break;
				}
				Buildings = Buildings->next;
			}
		}
		else if (body2->type == BUILDING&&body1->type != BUILDING&&body1->type != NONE) {
			LastCollided = body2;
			p2List_item<PhysBody3D*>*BuildingsPhys = BuildingPhys_List.getFirst();
			p2List_item<Cube*>*Buildings = Building_List.getFirst();

			for (; BuildingsPhys != nullptr; BuildingsPhys = BuildingsPhys->next) {
				if (BuildingsPhys->data == body2&&body2->toDelete == false) {
					BuildingsPhys->data->toDelete = true;
					Buildings->data->color = Red;
					App->player->ScaleTime = 0;
					App->player->vehicle->TimerB = 1.0f;
					App->player->vehicle->TimerG = 1.0f;
					cubes_destroyed++;

					if (body1->type==MISSILE&&App->camera->DoCameraShake == false&&AbletoCameraShake) {
						float distance;
						distance = sqrt(pow((body2->GetPosition().x - App->player->vehicle->Position().x), 2) + pow(body2->GetPosition().z - App->player->vehicle->Position().z, 2));
						distance /= 13;
						App->camera->CameraShake_Time.Start();
						App->camera->power = 1.0f/distance;
						App->camera->Time_Doing_Shake = 0.2f;
						App->camera->DoCameraShake = true;
						AbletoCameraShake = false;
					}
					break;
				}
				Buildings = Buildings->next;
			}
		}
	}
}

