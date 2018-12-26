#include "PhysVehicle3D.h"
#include "Primitive.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}
	

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	info.cubeCounter = Cube(info.cubeCounter.size.x, info.cubeCounter.size.y, info.cubeCounter.size.z);
	info.cubeCounter.color.Set(1.0f,TimerG,TimerB );
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&info.cubeCounter.transform);
	btVector3 offsettimer(info.OfsetCounter.x, info.OfsetCounter.y, info.OfsetCounter.z);
	offsettimer = offsettimer.rotate(q.getAxis(), q.getAngle());

	info.cubeCounter.transform.M[12] += offsettimer.getX();
	info.cubeCounter.transform.M[13] += offsettimer.getY();
	info.cubeCounter.transform.M[14] += offsettimer.getZ();

	Cube Deco1(info.Additional_Piece1.x, info.Additional_Piece1.y, info.Additional_Piece1.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&Deco1.transform);

	btVector3 offset2(info.Additional_Piece1_offset.x, info.Additional_Piece1_offset.y, info.Additional_Piece1_offset.z);
	offset2 = offset2.rotate(q.getAxis(), q.getAngle());

	Deco1.transform.M[12] += offset2.getX();
	Deco1.transform.M[13] += offset2.getY();
	Deco1.transform.M[14] += offset2.getZ();

	Cube Missiles_Left(info.Misiles_Left.x, info.Misiles_Left.y,info.Misiles_Left.z);
	info.Missiles_Left_Point = &Missiles_Left;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&Missiles_Left.transform);
	btVector3 offset3(info.Misiles_Left_Offset.x, info.Misiles_Left_Offset.y, info.Misiles_Left_Offset.z);
	offset3 = offset3.rotate(q.getAxis(), q.getAngle());


	Missiles_Left.transform.M[12] += offset3.getX();
	Missiles_Left.transform.M[13] += offset3.getY();
	Missiles_Left.transform.M[14] += offset3.getZ();


	Cube Missiles_Right(info.Misiles_Right.x, info.Misiles_Right.y, info.Misiles_Right.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&Missiles_Right.transform);

	btVector3 offset4(info.Misiles_Right_Offset.x, info.Misiles_Right_Offset.y, info.Misiles_Right_Offset.z);
	offset4 = offset4.rotate(q.getAxis(), q.getAngle());

	Missiles_Right.transform.M[12] += offset4.getX();
	Missiles_Right.transform.M[13] += offset4.getY();
	Missiles_Right.transform.M[14] += offset4.getZ();

	info.cubeCounter.Render();
	chassis.Render();
	Deco1.Render();
	Missiles_Right.Render();
	Missiles_Left.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

vec3 PhysVehicle3D::Position() {

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	return vec3(chassis.transform.M[12], chassis.transform.M[13], chassis.transform.M[14]);
}
vec3 PhysVehicle3D::Misiles_leftPos() {

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	Cube Missiles_Left(info.Misiles_Left.x, info.Misiles_Left.y, info.Misiles_Left.z);
	info.Missiles_Left_Point = &Missiles_Left;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&Missiles_Left.transform);

	btVector3 offset3(info.Misiles_Left_Offset.x, info.Misiles_Left_Offset.y, info.Misiles_Left_Offset.z);
	offset3 = offset3.rotate(q.getAxis(), q.getAngle());

	Missiles_Left.transform.M[12] += offset3.getX();
	Missiles_Left.transform.M[13] += offset3.getY();
	Missiles_Left.transform.M[14] += offset3.getZ();
	
	return vec3(Missiles_Left.transform.M[12], Missiles_Left.transform.M[13], Missiles_Left.transform.M[14]);
}



vec3 PhysVehicle3D::Misiles_RightPos() {

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	Cube Missiles_Right(info.Misiles_Right.x, info.Misiles_Right.y, info.Misiles_Right.z);
	//info.Missiles_Right_Point = &Missiles_Right;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&Missiles_Right.transform);

	btVector3 offset3(info.Misiles_Right_Offset.x, info.Misiles_Right_Offset.y, info.Misiles_Right_Offset.z);
	offset3 = offset3.rotate(q.getAxis(), q.getAngle());

	Missiles_Right.transform.M[12] += offset3.getX();
	Missiles_Right.transform.M[13] += offset3.getY();
	Missiles_Right.transform.M[14] += offset3.getZ();

	return vec3(Missiles_Right.transform.M[12], Missiles_Right.transform.M[13], Missiles_Right.transform.M[14]);
}


// ---------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

vec3 PhysVehicle3D::GoingForward() {

	btVector3 ret = vehicle->getForwardVector();

	return vec3(ret.getX(), ret.getY(), ret.getZ());
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}