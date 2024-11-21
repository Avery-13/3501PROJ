#include "custom_scene_3501.h"
#include <godot_cpp/core/memory.hpp> // for memnew
#include <godot_cpp/classes/random_number_generator.hpp>

using namespace godot;

void CustomScene3501::_bind_methods() {}

CustomScene3501::CustomScene3501() : Node3D()
{
	time_passed = 0.0;
}

CustomScene3501::~CustomScene3501()
{
	// Add your cleanup here.
}

void CustomScene3501::_enter_tree()
{
	UtilityFunctions::print("PLEASEEEEEEEE");
	if (DEBUG)
		UtilityFunctions::print("Enter Tree - CustomScene3501.");

	create_and_add_as_child<QuatCamera>(main_camera, "QuatCamera", true);

	// The vectors are brand new every time you run the simulation or reload the project.
}

void CustomScene3501::_ready()
{
	if (DEBUG)
		UtilityFunctions::print("Ready - CustomScene3501.");

	// set the player's position (the camera)
	main_camera->set_global_position(Vector3(5.0, 5.0, 25.0f));
	main_camera->look_at(Vector3(0, 0, 0)); // there are some bugs with this function if the up vector is parallel to the look-at position; check the manual for a link to more info

	// now that we have set the camera's starting state, let's reinitialize its variables
	main_camera->_ready();
}

// called every frame (as often as possible)
void CustomScene3501::_process(double delta)
{
	if (Engine::get_singleton()->is_editor_hint())
		return; // Early return if we are in editor

	time_passed += delta;
}

// this is just so that you have references when you are coding the camera movement.
// REMOVE THIS WHEN YOU GO TO MAKE THE RACETRACK

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool CustomScene3501::create_and_add_as_child(T *&pointer, String name, bool search)
{
	// this is the default behaviour
	// added the search parameter so that we can skip the slow "find_child" call during runtime (not applicable to this demo, you should always use search = true until next assignment)
	if (search == false)
	{
		pointer = memnew(T);
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}

	// always only have to search once if we save it here
	Node *child = find_child(name);

	if (child == nullptr)
	{
		pointer = memnew(T);
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}
	else
	{
		pointer = dynamic_cast<T *>(child);
		return false;
	}
}

/*
 *
 * The usual container class added to the demos. This one should be used to create a racetrack.
 *
 * Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
 *
 */