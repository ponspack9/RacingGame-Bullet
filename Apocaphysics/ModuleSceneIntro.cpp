#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	total_city_cubes = 0;
	game_started = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	App->camera->Move(vec3(40.0f, 200.0f, 40.0f));
	App->camera->LookAt(vec3(40.0f, 0.0f, 40.0f));
  
	plane_Ground=Cube(600.0f, 0.2f,600.f);
	plane_Ground.wire = false;
	plane_Ground.color = Grey;
	
	plane_Ground.SetPos(0, -2, 0);
	Ground=App->physics->AddBody(plane_Ground,10000);
	Ground->type = GROUND;
  
	CreateCity(200, { -100,0,-50 });
 
	return ret;
}

/*
	fer que seliminin els blocs_top al tocar lacera
	Percentatge 
*/


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
	float z = 0;

	vec3 aux_pos = pos;

	// First bucle stands for x axis block creation
	while (pos.x < (max_width + aux_pos.x))
	{
		//Defining next block
		n_buildings = (rand() + SDL_GetTicks()) % 8 + 2;
		block_width = (rand() % 20) + 5.0f;
		block_depth = ((rand() + SDL_GetTicks()) % 20) + 5.0f;

		//Create last block
		if (pos.x + block_width * 2 + buildings_offset * 4 >= (max_width + aux_pos.x)) 
		{
			block_width = ((aux_pos.x + max_width) - pos.x) / 2;
			CreateBlock3x3(pos, n_buildings, block_width, block_height, block_depth, buildings_offset);

			if (block_width < 5) n_buildings = 0;
		}
		else CreateBlock3x3(pos, n_buildings, block_width, block_height, block_depth, buildings_offset);

		placed++;

		// For each block created on x axis, the following blocks in z axis must respect its width
		while (pos.z < (max_width + aux_pos.z)) 
		{
			n_buildings = (rand() + SDL_GetTicks()) % 8 + 2;
			pos.z += block_depth * 2 + buildings_offset * 4 + road_offset;
			block_depth = ((rand() + SDL_GetTicks()) % 20) + 5.0f;

			//Create last block
			if (pos.z + block_depth * 2 + buildings_offset * 4 >= (max_width + aux_pos.z)) {
				block_depth = ((aux_pos.z + max_width) - pos.z)/2;
				CreateBlock3x3(pos, n_buildings, block_width, block_height, block_depth, buildings_offset);
				if (block_width < 5) n_buildings = 0;

			}
			else CreateBlock3x3(pos, n_buildings, block_width, block_height, block_depth, buildings_offset);
		}

		z = pos.z;
		pos.z = aux_pos.z;
		pos.x += block_width * 2 + buildings_offset * 4 + road_offset;
	}

	// Create surrounding wall
	
	Cube* wall1 = new Cube(pos.x-aux_pos.x + 10, 10, 10);   wall1->SetPos(aux_pos.x + (pos.x - aux_pos.x) / 2 - 5, 5, aux_pos.z - 15);
	Cube* wall2 = new Cube(10, 10, z - aux_pos.z +10); wall2->SetPos(aux_pos.x - 15, 5, aux_pos.z + (z - aux_pos.z) / 2 - 5);
	Cube* wall3 = new Cube(*wall1); wall3->SetPos(aux_pos.x + (pos.x - aux_pos.x) / 2 - 5, 5, z+5);
	Cube* wall4 = new Cube(*wall2); wall4->SetPos(pos.x + 5, 5, aux_pos.z + (z - aux_pos.z) / 2 -5);

	BuildingPhys_List.add(App->physics->AddBody(*wall1, 9999));
	Building_List.add(wall1);
	BuildingPhys_List.add(App->physics->AddBody(*wall2, 9999));
	Building_List.add(wall2);
	BuildingPhys_List.add(App->physics->AddBody(*wall3, 9999));
	Building_List.add(wall3);
	BuildingPhys_List.add(App->physics->AddBody(*wall4, 9999));
	Building_List.add(wall4);

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


void ModuleSceneIntro::CreateBuilding(const vec3 &Position, const float &w, const float &h, const float &d)
{

	float x = w / 2;
	float y = h / 2;
	float z = d / 2;
	int HeightBuilding = rand() % 4 + 2;

	Cube *a = new Cube();
	a->size = vec3(w, h, d);

	for (z; z <= d * 2; z = z + d) {
		for (x; x <= w * 2; x = x + w) {
			for (y; y <= h * HeightBuilding; y = y + h) {
        
				a->SetPos(x + Position.x, y + Position.y, z + Position.z);
				a->wire = false;
				a->color = Red;
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
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{



	//p.wire = false;
	//p.color = Grey;
	//p.Scale(200, 0, 200);
	////p.axis = true;
	//p.Render();

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

	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->type == GROUND&&body2->type == BUILDING || body2->type == GROUND&&body1->type == BUILDING) {
		if (body1->type == BUILDING) {
			p2List_item<PhysBody3D*>*BuildingsPhys = BuildingPhys_List.getFirst();
			p2List_item<Cube*>*Buildings = Building_List.getFirst();

			for (; BuildingsPhys != nullptr; BuildingsPhys = BuildingsPhys->next) {
				if (BuildingsPhys->data == body1) {
					BuildingsPhys->data->toDelete = true;
					//delete BuildingsPhys->data;
					break;
				}
				Buildings = Buildings->next;
			}
		}
		else if (body2->type == BUILDING) {
			p2List_item<PhysBody3D*>*BuildingsPhys = BuildingPhys_List.getFirst();
			p2List_item<Cube*>*Buildings = Building_List.getFirst();

			for (; BuildingsPhys != nullptr; BuildingsPhys = BuildingsPhys->next) {
				if (BuildingsPhys->data == body2) {
					BuildingsPhys->data->toDelete = true;
					break;
				}
				Buildings = Buildings->next;
			}
		}
	}
}

