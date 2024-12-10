#include "defs.h"
#include "game_object.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>
#include "environ_object.h"

// This class remains mostly the same as example_derived, however is defined to fit the beacon/checkpoint object needed  DB

using namespace godot;

void EnvironObject::_bind_methods() {}

EnvironObject::EnvironObject() : CSGMesh3D() {
	time_passed = 0.0;
}

void EnvironObject::_enter_tree() {
	if (DEBUG) UtilityFunctions::print("Enter Tree - EnvironObject.");

	set_use_collision(true);
	set_collision_priority(1);
}

void EnvironObject::_ready() {
	if (DEBUG) UtilityFunctions::print("Ready - EnvironObject.");
}

