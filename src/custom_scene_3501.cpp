#include "custom_scene_3501.h"
#include <godot_cpp/core/memory.hpp> // for memnew
#include <godot_cpp/classes/random_number_generator.hpp>

using namespace godot;

void CustomScene3501::_bind_methods() {}

CustomScene3501::CustomScene3501() : Node3D(), main_camera(nullptr), rng()
{
	time_passed = 0.0;
	collectCount = 0;
	rng.randomize();
}

CustomScene3501::~CustomScene3501()
{
	// Add your cleanup here.
}

void CustomScene3501::_enter_tree()
{

	if (DEBUG)
		UtilityFunctions::print("Enter Tree - CustomScene3501.");
	setup_reference_boxes();

	// Add the Firefly Swarm particle system
    create_particle_system("Firefly Swarm", "firefly");


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
	
	// Use find_child to locate the UI node
	Node* ui_node = world_node->find_child("UI", true, false); // Recursively search for "Head"
	if (ui_node) {
		UtilityFunctions::print("UI found under World.");
		main_ui = Object::cast_to<GridContainer>(ui_node);
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
			Ref<Shader> shader = ResourceLoader::get_singleton()->load("res://assets/shaders/test.gdshader", "Shader"); // I've set it to corrugated for the start of this assignment  DB
			screen_space_shader_material->set_shader(shader);
			quad_mesh->surface_set_material(0, screen_space_shader_material);
			screen_quad_instance->set_mesh(quad_mesh);
			screen_quad_instance->set_extra_cull_margin(50.0f); // as suggested in the Godot docs to prevent culling

		}
		
	}else{

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
		Ref<Shader> shader = ResourceLoader::get_singleton()->load("res://assets/shaders/test.gdshader", "Shader"); // I've set it to corrugated for the start of this assignment  DB
		screen_space_shader_material->set_shader(shader);
		quad_mesh->surface_set_material(0, screen_space_shader_material);
		screen_quad_instance->set_mesh(quad_mesh);
		screen_quad_instance->set_extra_cull_margin(50.0f); // as suggested in the Godot docs to prevent culling
	}




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
	set_object_positions();
	// now that we have set the camera's starting state, let's reinitialize its variables
	main_camera->_ready();


	UtilityFunctions::print(particle_systems.size());
	//particle set up
	for(int index = 0; index < particle_systems.size(); index++){
		// the current particle system we are setting up
		GPUParticles3D* particle_system = particle_systems[index];
		int num_particles;

		// this should never be needed, but can't hurt to have. 
		if(particle_system == nullptr) continue; 
		
		// Grabs the pointer to the spatial material so that you can change stuff. 
		// Note: The * operator with a Ref<Class> instance will give you the pointer to it
		ShaderMaterial* shader_material = dynamic_cast<ShaderMaterial*>(*particle_system->get_draw_pass_mesh(0)->surface_get_material(0));
		ShaderMaterial* process_material = dynamic_cast<ShaderMaterial*>(*particle_system->get_process_material());
		switch(index){
			// if you need anything to be different, do it here!
			case 0: 
				// Setup for Firefly Swarm
				num_particles = 50;
				particle_system->set_amount(num_particles);
				particle_system->set_lifetime(10.0);
				//particle_system->set_pre_process_time(10.0);
				

				// Set the texture image
				shader_material->set_shader_parameter("texture_image", ResourceLoader::get_singleton()->load("res://assets/textures/flame4x4orig.png"));

				// Set shader parameters
				shader_material->set_shader_parameter("num_particles", num_particles);
				process_material->set_shader_parameter("num_particles", num_particles);

				process_material->set_shader_parameter("min_position_range", Vector3(0.0, 0.0, 0.0f));
    			process_material->set_shader_parameter("max_position_range", Vector3(20.0, 20.0, 10.0f));

				particle_system->set_global_position(Vector3(20.0f, 0, 0));
				particle_system->set_emitting(true);
				break;

		}

	}

}

// called every frame (as often as possible)
void CustomScene3501::_process(double delta)
{
	if (Engine::get_singleton()->is_editor_hint())
		return; // Early return if we are in editor
	if (!collectibles.is_empty()) {
		for (int i = 0; i < numObjs; i++) {
			BeaconObject* check = collectibles[i];
			if (check->check_collisions(main_camera->get_player()->get_position())) {
				if (i == 0) {
					Node* label = main_ui->find_child("Label", false, false);
					Label* l1 = Object::cast_to<Label>(label);
					l1->set_text("Item 1: FOUND");
				}
				UtilityFunctions::print("HIT");
				numObjs -= 1;
				collectibles.remove_at(i);
				check->queue_free();
				collectCount += 1;
			}
		}
	}
	else {
		UtilityFunctions::print("DONE");
	}
	
	if (collectCount >= 5) {
		UtilityFunctions::print("All have been collected, ending now");
		get_tree()->quit();
	}
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

// it felt a bit cleaner in my eyes to bundle this together
// not full file name for the shader; see the particle system code for more detail
void CustomScene3501::create_particle_system(String node_name, String shader_name){
	// if you want to use non-zero argument constructors, here is an example of how to do that
	ParticleSystem3501* system = memnew(ParticleSystem3501(shader_name));
	add_as_child(system, node_name, true); 
	particle_systems.push_back(system);
}

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

// This is a variant of the usual one. It allows you to more easily use a non-zero argument constructor, which I noticed some of you have struggled with. Hope this helps!
// returns true if pointer is brand-new; false if retrieved from SceneTree
// deletes the memory if the node exists in the scenetree and isn't null when passed in
// IMPORTANT: IF SEARCH IS FALSE, IT ASSUMES THAT THE POINTER IS TO A VALID INSTANCE ALREADY AKA MEMNEW HAS ALREADY BEEN CALLED
template <class T>
bool CustomScene3501::add_as_child(T* &pointer, String name, bool search){
	// this is the default behaviour
	// added the search parameter so that we can skip the slow "find_child" call during runtime
	if(search == false){
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}

	// always only have to search once if we save it here
	Node* child = find_child(name);
	
	// if the node hasn't been added to the SceneTree yet
	if(child == nullptr){
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}
	// if we are grabbing the existent one, clean up the memory to the new one that was just made and passed as an argument
	else{
		if(pointer == nullptr){
			UtilityFunctions::print("There is a nullptr being passed to add_as_child...");
		}
		else{
			memdelete(pointer);
		}
		pointer = dynamic_cast<T*>(child);
		return false;
	}
}



// This function is repurposed from the setup_references() function, now it creates our checkpoints  DB 
void CustomScene3501::setup_reference_boxes() {
	
	for (int index = 0; index < numObjs; index++) {
		BeaconObject* obj_instance;

		create_and_add_as_child(obj_instance, (vformat("Check_point_%d", index)), true); // Create the beacon as a node and add it to the scene tree  DB

		if (index == 0) { // Set the mesh of the first checkpoint as special to signify it as the first beacon that must be collected  DB
			Ref<ArrayMesh> collectMesh = ResourceLoader::get_singleton()->load("res://assets/models/ship/Sketchfab_Scene_defaultMaterial20.res", "ArrayMesh");
			
			//StandardMaterial3D* box_material1 = memnew(StandardMaterial3D);
			//box_material1->set_albedo(Color(1.0f, 1.0f, 0.0f, 1.0f));
			//box1->surface_set_material(0, box_material1);
			obj_instance->set_mesh(collectMesh);
		}
		else { // Set all other beacon meshes to be normal  DB
			BoxMesh* box = memnew(BoxMesh);
			box->set_size(Vector3(1.0f, 2.0f, 1.0f));
			StandardMaterial3D* box_material = memnew(StandardMaterial3D);
			box_material->set_albedo(Color(1.0f, 1.0f, 1.0f, 1.0f));
			box->surface_set_material(0, box_material);
			obj_instance->set_mesh(box);
		}
		collectibles.push_back(obj_instance);  // Add each beacon to the collectibles collection  DB
	}
}


// Set all the object positions
void CustomScene3501::set_object_positions()
{
	RandomNumberGenerator* rng = memnew(RandomNumberGenerator); //added to allow for randomized positions

	for (int i = 0; i < numObjs; i++) // Set all the checkpoint positions
	{
		// x and y values are randomized, but to make this similar to a real collectibles the z value is consistently moving further and further back  DB
		// ergo, the last checkpoint will be at the opposite end of the track as the first   DB
		float x = rng->randf_range(0.0f, 200.0f);
		float y = rng->randf_range(0.0f, 3.0f);
		float z = rng->randf_range(0.0f, 200.0f);
		collectibles.get(i)->set_global_position(Vector3(x, y, z));
		//powers.get(i)->set_global_position(Vector3(x-5.0, y, z-5.0)); //a power up is set next to every checkpoint
	}

}

/*
 *
 * The usual container class added to the demos. This one should be used to create a collectibles.
 *
 * Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
 *
 */
