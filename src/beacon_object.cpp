#include "defs.h"
#include "game_object.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>
#include "beacon_object.h"

// This class remains mostly the same as example_derived, however is defined to fit the beacon/checkpoint object needed  DB

using namespace godot;

void BeaconObject::_bind_methods() {}

BeaconObject::BeaconObject() : GameObject() {
	time_passed = 0.0;
}

void BeaconObject::_enter_tree() {
	if (DEBUG) UtilityFunctions::print("Enter Tree - BeaconObject.");
}

void BeaconObject::_ready() {
	if (DEBUG) UtilityFunctions::print("Ready - BeaconObject.");
}

