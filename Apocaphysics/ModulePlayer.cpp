#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "Timer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{

	ScaleTime = 0;
	LOG("Loading player");
	VehicleInfo car;	
	


	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1.5f, 4);
	car.chassis_offset.Set(0, 0.5f, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;
	//Decoration
	car.Additional_Piece1.Set(2, 1.0f, 1);
	car.Additional_Piece1_offset.Set(0, 0, -2);

	//MISILES

	car.Misiles_Left.Set(0.7,0.7,3);
	car.Misiles_Left_Offset.Set(1.35, 1.4, -0.9);

	car.cubeCounter = Cube(1.5,7,1.5);
	car.cubeCounter.SetRotation(40, vec3(1, 0, 0));
	car.OfsetCounter.Set(-5,0,0);
	car.cubeCounter.size.Set(0.7, ScaleTime, 0.7);

	car.Misiles_Right.Set(0.7, 0.7, 3);
	car.Misiles_Right_Offset.Set(-1.35, 1.4, -0.9);

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;//1.2
	float wheel_radius = 0.9f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.8f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(App->scene_intro->SpotPoint.x+10,10, App->scene_intro->SpotPoint.z+10);
	vehicle->type = VEHICLE;
	
	vehicle->IntitialRot = vehicle->vehicle->getForwardVector();
	vehicle->TimerG = 1.0f;
	vehicle->TimerB = 1.0f;
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{

	Shooot_List.clear();
	ShootPhys_List.clear();
	LOG("Unloading player");

	return true;
}



void ModulePlayer::Shoot() {

	float force = 8000.0f;

	if (shootLeft) {
		Sphere* Left = new Sphere(0.8f);
		Left->wire = false;
		Left->color = Blue;
		
		vec3 aux = vehicle->Misiles_leftPos();
		Left->SetPos(aux.x, aux.y - 5, aux.z);
		Shooot_List.add(Left);
		
		ShootPhys_List.add(App->physics->AddBody(*Left, 50,MISSILE));
		ShootPhys_List.getLast()->data->Push(-(App->camera->Z.x * force), -(App->camera->Z.y * force), -(App->camera->Z.z * force));
		shootLeft = false;
		ShootRight = true;
	}
	else if (ShootRight) {
		Sphere* Right = new Sphere(0.8f);
		Right->wire = false;
		Right->color = Blue;

		vec3 aux2 = vehicle->Misiles_RightPos();
		Right->SetPos(aux2.x, aux2.y - 5, aux2.z);
		Shooot_List.add(Right);
		ShootPhys_List.add(App->physics->AddBody(*Right, 50,MISSILE));
		ShootPhys_List.getLast()->data->Push(-(App->camera->Z.x * force), -(App->camera->Z.y * force), -(App->camera->Z.z * force));
		shootLeft = true;
		ShootRight = false;
	}

}


// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	char *c = "PRESS ENTER TO PLAY";


	if (App->camera->DoCameraShake == false) {
		App->camera->Position = vehicle->GetPosition()- vehicle->GoingForward() * 10 + vec3(0, 5, 0);
	}

	App->camera->LookAt(vehicle->Position());
	if (App->scene_intro->game_started) {

		c = "";
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			if (vehicle->GetKmh() < 0) {
				brake = BRAKE_POWER / 5;
			}
			else
				acceleration = MAX_ACCELERATION;

		}
		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
		{
			acceleration = 8000.0f;

		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			Shoot();
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
			float angle = vehicle->IntitialRot.angle(vehicle->vehicle->getForwardVector());
			vehicle->Rotate(vec3(0, 1, 0), -angle, false);
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			if (vehicle->GetKmh() >= 0)
				brake = BRAKE_POWER;
			else
				acceleration = -MAX_ACCELERATION / 2;
		}

		if (vehicle->GetKmh() <= 5.0f) {
			if (!no_velocity_timer.IsRunning()) no_velocity_timer.Start();
			c = "TIMER ON";
		}
		else {
			no_velocity_timer.Reset();
		}

		if (no_velocity_timer.ReadSeconds() >= 5.0f) {
			App->scene_intro->game_started = false;
			App->scene_intro->main_timer.Reset();
			no_velocity_timer.Reset();
			c = "YOU LOSE";
		}

		// END PLAYING CONDITION



		p2List_item<PhysBody3D*>*Missiles_Phys = ShootPhys_List.getFirst();
		p2List_item<Sphere*>*Missiles = Shooot_List.getFirst();

		for (; Missiles != nullptr; Missiles = Missiles->next) {
			if (Missiles->data != nullptr) {
				Cube* Gas = new Cube(0.9f, 0.9f, 0.9f);
				int a = (rand() % 3) - 1;
				Gas->SetPos(Missiles_Phys->data->GetPosition().x + a, Missiles_Phys->data->GetPosition().y + a, Missiles_Phys->data->GetPosition().z + a);
				Missiles->data->Gas.add(Gas);
				p2List_item<Cube*>*Gas_Particle = Missiles->data->Gas.getFirst();
				for (; Gas_Particle != nullptr; Gas_Particle = Gas_Particle->next) {
					Gas_Particle->data->Scale(Gas_Particle->data->size.x, Gas_Particle->data->size.y, Gas_Particle->data->size.z);
					Gas_Particle->data->size.x += 0.08;
					Gas_Particle->data->size.y += 0.08;
					Gas_Particle->data->size.z += 0.08;
					Gas_Particle->data->Render();
				}
				if (Missiles->data->Gas.count() >= 10) {
					Gas_Particle = Missiles->data->Gas.getFirst();
					Missiles->data->Gas.del(Missiles->data->Gas.getFirst());
				}
				Missiles_Phys->data->GetTransform(&Missiles->data->transform);
				Missiles->data->Render();
				Missiles->data->time++;
			}
			if (Missiles->data->time >= 50) {
				Missiles->data->Gas.clear();
				ShootPhys_List.del(Missiles_Phys);
				Shooot_List.del(Missiles);
				App->scene_intro->AbletoCameraShake = true;
				break;
			}

			Missiles_Phys = Missiles_Phys->next;
		}

		if (ScaleTime >= 6.2f) {
			App->scene_intro->reStart();
		}
		vehicle->info.cubeCounter.size.Set(0.7, ScaleTime, 0.7);
		vehicle->TimerG -= 0.002;
		vehicle->TimerB -= 0.002;


		ScaleTime += 0.01;
		vehicle->ApplyEngineForce(acceleration);
		vehicle->Turn(turn);
		vehicle->Brake(brake);
	}
	vehicle->Render();

	char title[100];
	sprintf_s(title, "%.1f Km/h  Total cubes: %d Total cubes killed: %d Time left: %f %s", vehicle->GetKmh(),App->scene_intro->total_city_cubes,App->scene_intro->cubes_destroyed, 60.0f - App->scene_intro->main_timer.ReadSeconds(),c);
	App->window->SetTitle(title);

	


	return UPDATE_CONTINUE;
}



