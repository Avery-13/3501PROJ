#include "custom_scene_3501.h"
#include <godot_cpp/core/memory.hpp> // for memnew
#include <godot_cpp/classes/random_number_generator.hpp>

using namespace godot;

void CustomScene3501::_bind_methods() {}

CustomScene3501::CustomScene3501() : Node3D(), main_camera(nullptr)
{
	time_passed = 0.0;
}

CustomScene3501::~CustomScene3501()
{
	// Add your cleanup here.
}

void CustomScene3501::_enter_tree()
{

	if (DEBUG)
		UtilityFunctions::print("Enter Tree - CustomScene3501.");

    // Traverse up the tree to find the parent (World)
    Node* world_node = get_parent();
	while (world_node && world_node->get_name() != godot::String("World")) {
		world_node = world_node->get_parent();
	}

    if (!world_node) {
        UtilityFunctions::print("World node not found.");
        return;
    }

    // Use find_child to locate the Head node
    Node* head_node = world_node->find_child("Head", true, false); // Recursively search for "Head"
    if (!head_node) {
        UtilityFunctions::print("Head node not found under World.");
        return;
    }

    // Add QuatCamera as a child of Head
    Node3D* head_node_3d = Object::cast_to<Node3D>(head_node);
    if (!head_node_3d) {
        UtilityFunctions::print("Head node is not of type Node3D.");
        return;
    }

	Node* existing_camera = head_node_3d->find_child("QuatCamera", false, false);
	if (existing_camera)
	{
		main_camera = Object::cast_to<QuatCamera>(existing_camera);
		UtilityFunctions::print("QuatCamera already exists and is assigned to main_camera.");
		Node* existing_screen_effect = main_camera->find_child("Vision", false, false);
		if (existing_screen_effect)
		{
			screen_quad_instance = Object::cast_to<MeshInstance3D>(existing_screen_effect);
			UtilityFunctions::print("Screen effect already exists and is assigned to main_camera.");
			
		}
		else {
			screen_quad_instance = memnew(MeshInstance3D);
			screen_quad_instance->set_name("Vision");
			main_camera->add_child(screen_quad_instance);
			screen_quad_instance->set_owner(get_tree()->get_edited_scene_root());
			// Setup the screen-space shader
			QuadMesh* quad_mesh = memnew(QuadMesh);
			quad_mesh->set_size(Vector2(2, 2)); // this will cover the whole screen
			quad_mesh->set_flip_faces(true);

			screen_space_shader_material = memnew(ShaderMaterial);
			// make sure to tell your TA in your README how they should test different shaders; maybe it's to change the string below, maybe it's some other way of your own design
			Ref<Shader> shader = ResourceLoader::get_singleton()->load("res://assets/shaders/custom.gdshader", "Shader"); // I've set it to corrugated for the start of this assignment  DB
			screen_space_shader_material->set_shader(shader);
			quad_mesh->surface_set_material(0, screen_space_shader_material);
			screen_quad_instance->set_mesh(quad_mesh);
			screen_quad_instance->set_extra_cull_margin(50.0f); // as suggested in the Godot docs to prevent culling

		}
		
		return;
	}
	

	main_camera = memnew(QuatCamera);
	main_camera -> set_name("QuatCamera");
    head_node_3d->add_child(main_camera);
    main_camera->set_owner(get_tree()->get_edited_scene_root());

    UtilityFunctions::print("QuatCamera successfully added to Head node.");

	screen_quad_instance = memnew(MeshInstance3D);
	screen_quad_instance->set_name("Vision");
	main_camera->add_child(screen_quad_instance);
	screen_quad_instance->set_owner(get_tree()->get_edited_scene_root());
	// Setup the screen-space shader
	QuadMesh* quad_mesh = memnew(QuadMesh);
	quad_mesh->set_size(Vector2(2, 2)); // this will cover the whole screen
	quad_mesh->set_flip_faces(true);
	
	screen_space_shader_material = memnew(ShaderMaterial);
	// make sure to tell your TA in your README how they should test different shaders; maybe it's to change the string below, maybe it's some other way of your own design
	Ref<Shader> shader = ResourceLoader::get_singleton()->load("res://assets/shaders/custom.gdshader", "Shader"); // I've set it to corrugated for the start of this assignment  DB
	screen_space_shader_material->set_shader(shader);
	quad_mesh->surface_set_material(0, screen_space_shader_material);
	screen_quad_instance->set_mesh(quad_mesh);
	screen_quad_instance->set_extra_cull_margin(50.0f); // as suggested in the Godot docs to prevent culling

	//create_and_add_as_child<TerrainInstance>(sands, "Test terrain", false);

	// The vectors are brand new every time you run the simulation or reload the project.
}

void CustomScene3501::_ready()
{
	if (DEBUG)
		UtilityFunctions::print("Ready - CustomScene3501.");

	// set the player's position (the camera)
	main_camera->set_global_position(Vector3(5.0, 5.0, 25.0f));
	main_camera->look_at(Vector3(0, 0, 0)); // there are some bugs with this function if the up vector is parallel to the look-at position; check the manual for a link to more info
	//sands->set_global_position(Vector3(50.0, 0.0, 0.0));
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


void CustomScene3501::print_tree(Node* node, int depth) {
    if (!node) return;

    // Create indentation
    godot::String indentation = "";
    for (int i = 0; i < depth * 4; i++) {
        indentation += " ";
    }

    UtilityFunctions::print(indentation + node->get_name());

    Array children = node->get_children();
    for (int i = 0; i < children.size(); i++) {
        Node* child = Object::cast_to<Node>(children[i]);
        print_tree(child, depth + 1); // Recursive call
    }
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
