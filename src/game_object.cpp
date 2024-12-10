#include "defs.h"
#include "game_object.h"

using namespace godot;

void GameObject::_bind_methods() {}

GameObject::GameObject() : MeshInstance3D() {
	stop = false; 
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
	if (distance < 2.0) {
		return true;
	}
	return false;  
}