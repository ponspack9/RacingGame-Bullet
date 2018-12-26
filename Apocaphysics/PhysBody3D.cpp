#include "PhysBody3D.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{
	body->setUserPointer(this);
}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	delete body;
}

vec3 PhysBody3D::GetPosition() {

	btVector3 ret;
	ret = body->getWorldTransform().getOrigin();

	return vec3(ret.getX(),ret.getY(),ret.getZ());
}
// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

void PhysBody3D::Rotate(vec3 axis, float angle,bool conversion) {

	if(conversion)
		angle = angle * (M_PI / 180);

	btVector3 ax = btVector3(axis.x, axis.y, axis.z);

	btTransform t = body->getWorldTransform();
	t.setRotation(btQuaternion(ax, angle));
	body->setWorldTransform(t);
}




// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}
