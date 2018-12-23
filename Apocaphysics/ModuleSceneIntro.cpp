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
	
	App->camera->Move(vec3(40.0f, 200.0f, 40.0f));
	App->camera->LookAt(vec3(40.0f, 0.0f, 40.0f));
	
	CreateCity(9, 150, { -100,0,0 });

	return ret;
}


/*	* Creates a random city based on 3x3 blocks
	* Random size for each block
	* num_blocks	-> number of blocks of the city
	* max_width		-> maximum width of the city
	* pos			-> position where first block will be placed
*/
void ModuleSceneIntro::CreateCity(int num_blocks,float max_width, vec3 pos, float buildings_offset) {

	int n_buildings = 5;
	int placed = 0;

	float block_width = 10;
	float block_height = 10;
	float block_depth = 10;

	float road_offset = 10.0f;

	vec3 aux_pos = pos;

	float positions[15][15] = { 0.0f };

	int j = 0;
	int i = 0;

	// First bucle stands for x block creation
	while (pos.x <= max_width)
	{
		//Defining next block
		n_buildings = (rand() + SDL_GetTicks()) % 9 + 1;
		//block_width = (rand() % 15) + 10.0f;
		//block_depth = (rand() % 15) + 10.0f;

		CreateBlock3x3(pos, n_buildings, block_width, block_height, block_depth, buildings_offset);
		placed++;

		//if (j<=0) pos.z += block_depth * 2 + buildings_offset * 4 + road_offset;

		// For each block created on x axis, the following blocks in z axis must respect its width
		while (pos.z <= max_width) {

			pos.z += block_depth * 2 + buildings_offset * 4 + road_offset;
			CreateBlock3x3(pos, n_buildings, block_width, block_height, block_depth, buildings_offset);
			//block_depth = (rand() % 15) + 10.0f;
			j++;

		}
		pos.z = aux_pos.z;
		pos.x += block_width * 2 + buildings_offset * 4 + road_offset;
		i++;
	}




	//for (int i = 0; i < num_blocks; i++)
	//{
	//	if (placed >= num_blocks) return;

	//	//Defining next block
	//	n_buildings = (rand() + SDL_GetTicks()) % 9 + 1;
	//	block_width = (rand() % 15) + 10.0f;
	//	block_depth = block_width;

	//	CreateBlock3x3(pos, n_buildings, block_width, block_height, block_depth, buildings_offset);

	//	placed++;
	//	pos.x += block_width * 2 + buildings_offset * 4 + road_offset;
	//	positions[i][j] = pos.x;

	//	if (pos.x >= max_width) {
	//		pos.x = aux_pos.x;
	//		pos.z += block_width * 2 + buildings_offset * 4 + road_offset;
	//		j++;
	//		if (pos.z >= max_width) {
	//			LOG("City limits exceeded, blocks remaining to place: %d", num_blocks - placed);
	//			return;
	//		}
	//	}

	//}

	


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
	sidewalk->size = vec3(block_width*2 + offset*4, 0.2f, block_width*2 + offset*4);
	sidewalk->SetPos(block_width + offset + position.x, 0.2f / 2, block_width + offset + position.z);

	BuildingPhys_List.add(App->physics->AddBody(*sidewalk, 500));
	Building_List.add(sidewalk);


	// Randomly fills a imaginary 3x3 boolean matrix to place the buildings
	for (int i = 0; i < 3; i++) 
	{
		position.z = aux_position.z + i * (width*2 + offset);

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
				CreateBuilding(position, width, height,depth);
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
	a->size = vec3(w, h, w);

	for (z; z <= d * 2; z = z + d) {
		for (x; x <= w * 2; x = x + w) {
			for (y; y <= h * HeightBuilding; y = y + h) {
				a->SetPos(x + Position.x, y + Position.y, z + Position.z);
				a->wire = false;
				a->color = Red;
				BuildingPhys_List.add(App->physics->AddBody(*a, 50));
				Building_List.add(a);
			}
			y = h / 2;
		}
		x = w / 2;
	}
}

//void ModuleSceneIntro::CreateBuilding(vec3 Position) {
//
//	//sidewalk.SetPos();
//
//	float w = 5;
//	float h = 3;
//
//	float x = w / 2;
//	float y = h / 2;
//	float z = w / 2;
//	int HeightBuilding = rand() % 4 + 6;
//
//	sidewalk.size = vec3(w * 2+2, 0.2f, w * 2+2);
//	sidewalk.SetPos(w + Position.x, 0.2 / 2, w + Position.z);
//
//	
//	BuildingPhys_List.add(App->physics->AddBody(sidewalk, 500));
//	Building_List.add(&sidewalk);
//	//a.transform.scale(w,h,w);
//	a.size = vec3(w, h , w);
//
//	for (z; z <= w * 2; z = z + w) {
//		for (x; x <= w * 2; x = x + w) {
//			for (y; y <= h * HeightBuilding; y = y + h) {
//				a.SetPos(x+Position.x, y+Position.y, z+Position.z);
//				a.wire = false;
//				a.color = Red;
//				BuildingPhys_List.add(App->physics->AddBody(a, 50));
//				Building_List.add(&a);
//			}
//			y = h / 2;
//		}
//		x = w / 2;
//	}
//
//}

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

