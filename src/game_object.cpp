#include "defs.h"
#include "game_object.h"

using namespace godot;

void GameObject::_bind_methods() {}

GameObject::GameObject() : MeshInstance3D() {
	//some_shared_attribute = 2.0f;
	stop = false;  // stop is set to false by default so that objects will behave as intended until game over conditions are met  DB
}

/*
	This function takes in a position vector,
	calculates how far this position is from us,
	then if the vector is close enough returns true   DB
*/
bool GameObject::check_collisions(Vector3 in)
{
	Vector3 me = this->get_position();
	float distance = me.distance_to(in);
	//UtilityFunctions::print(distance);
	if (distance < 2.0) {
		return true;
	}
	return false;   // returns false if given position isn't close enough to this object to be considered colliding  DB
}