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

	// Add the Thruster particle system
    create_particle_system("Thruster", "thruster");


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
			
			Ref<Shader> shader = ResourceLoader::get_singleton()->load("res://assets/shaders/effect2.gdshader", "Shader"); 
			screen_space_shader_material->set_shader(shader);
			quad_mesh->surface_set_material(0, screen_space_shader_material);
			screen_quad_instance->set_mesh(quad_mesh);
			screen_quad_instance->set_extra_cull_margin(50.0f); 

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
		Ref<Shader> shader = ResourceLoader::get_singleton()->load("res://assets/shaders/effect2.gdshader", "Shader"); 
		screen_space_shader_material->set_shader(shader);
		quad_mesh->surface_set_material(0, screen_space_shader_material);
		screen_quad_instance->set_mesh(quad_mesh);
		screen_quad_instance->set_extra_cull_margin(50.0f);

	}

	Node* existing_terrain = find_child("Sand Dunes", false, false);
	if (existing_terrain)
	{
		sands = Object::cast_to<TerrainInstance>(existing_terrain);
		UtilityFunctions::print("Sand Dunes already exists and is assigned to sands.");
	}
	else {
		create_and_add_as_child(sands, "Sand Dunes", false);
	}
}

void CustomScene3501::_ready()
{
	if (DEBUG)
		UtilityFunctions::print("Ready - CustomScene3501.");

	// set the player's position (the camera)
	//sands->set_global_position
	float mapTest = sands->get_terrain_mesh()->get_height_map().get(5.0).get(25.0);
	main_camera->set_global_position(Vector3(28.0f, mapTest + 1.0, 162.0f));
	main_camera->get_player()->set_global_position(Vector3(28.0f, mapTest + 1.0, 162.0f));
	main_camera->look_at(Vector3(0, 0, 0)); // there are some bugs with this function if the up vector is parallel to the look-at position; check the manual for a link to more info
	set_object_positions();
	set_grass_positions();

	
	// now that we have set the camera's starting state, let's reinitialize its variables
	main_camera->_ready();
	screen_quad_instance->hide();


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
			case 0: {
				// Setup for Firefly Swarm
				num_particles = 200;
				particle_system->set_amount(num_particles);
				particle_system->set_lifetime(10.0);
				particle_system->set_pre_process_time(10.0);

				// Set the texture image
				shader_material->set_shader_parameter("texture_image", ResourceLoader::get_singleton()->load("res://assets/textures/flame4x4orig.png"));

				// Set shader parameters
				shader_material->set_shader_parameter("num_particles", num_particles);
				process_material->set_shader_parameter("num_particles", num_particles);

				process_material->set_shader_parameter("min_position_range", Vector3(0.0, 0.0, 0.0f));
    			process_material->set_shader_parameter("max_position_range", Vector3(200.0, 20.0, 200.0f));

				particle_system->set_global_position(Vector3(80.0f, 0, 20.0f));
				particle_system->set_emitting(true);

				AABB box(Vector3(-80, 0, -20), Vector3(200, 30, 200));
				particle_system->set_visibility_aabb(box);
				break;				
			}

			case 1:
			
				num_particles = 20000;
				particle_system->set_amount(num_particles);
				particle_system->set_lifetime(10.0);

				// Set the texture image
				shader_material->set_shader_parameter("texture_image", ResourceLoader::get_singleton()->load("res://assets/textures/flame4x4orig.png"));

				// Set shader parameters
				shader_material->set_shader_parameter("num_particles", num_particles);
				
				particle_system->set_global_position(Vector3(20,2,160));
				particle_system->set_emitting(true);
				break;
			case 2:
			
				num_particles = 20000;
				particle_system->set_amount(num_particles);
				particle_system->set_lifetime(100.0);

				// Set the texture image
				shader_material->set_shader_parameter("texture_image", ResourceLoader::get_singleton()->load("res://assets/textures/flame4x4orig.png"));

				// Set shader parameters
				shader_material->set_shader_parameter("num_particles", num_particles);
				shader_material->set_shader_parameter("curr_position", Vector3(50.0,0.0,100.0));

				
				particle_system->set_global_position(Vector3(50,0,50));
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
				collectCount += 1;
				Node* label = main_ui->find_child("Label", false, false);
				Label* l1 = Object::cast_to<Label>(label);
				l1->set_text(vformat("Items collected: %d/5",collectCount));
				
				UtilityFunctions::print("HIT");
				numObjs -= 1;

				//Change arrow to check mark and rotate
				Ref<ArrayMesh> arrowMesh = ResourceLoader::get_singleton()->load("res://assets/Environment/Meshes/MeshCheck.res", "ArrayMesh");
				arrows.get(i)->set_mesh(arrowMesh);
				arrows.get(i)->set_rotation(Vector3(42.3,0,0));
				arrows.get(i)->set_global_position(arrows.get(i)->get_global_position() - Vector3(0.0, 9.0, 0.0));
				arrows.remove_at(i);

				collectibles.remove_at(i);
				check->queue_free();
			}

		}
	}
	else {
		UtilityFunctions::print("DONE");
	}
	
if (playerOOB()) {
        screen_quad_instance->show();
        stopPlayer();
    } else {
        screen_quad_instance->hide();
    }

	if (collectCount >= 5) {
		UtilityFunctions::print("All have been collected, ending now");
		Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);
		get_tree()->change_scene_to_file("res://ui/scenes/victory_screen.tscn");
		
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


void CustomScene3501::setup_reference_boxes() {
	
	for (int index = 0; index < numObjs; index++) {
		BeaconObject* obj_instance;
		BeaconObject* marker_instance;

		create_and_add_as_child(obj_instance, (vformat("Check_point_%d", index)), true); 
		create_and_add_as_child(marker_instance, (vformat("Arrow_%d", index)), true); 


		if (index == 0) { 
			Ref<ArrayMesh> collectMesh = ResourceLoader::get_singleton()->load("res://assets/models/ship/Sketchfab_Scene_defaultMaterial20.res", "ArrayMesh");
			
			obj_instance->set_mesh(collectMesh);
			obj_instance->set_scale(Vector3(3.0,3.0,3.0));
		}
		//Set the different collectibles to be different ship parts
		else if (index == 1) { 

			Ref<ArrayMesh> collectMesh2 = ResourceLoader::get_singleton()->load("res://assets/Environment/Meshes/Mesh01.res", "ArrayMesh");
			obj_instance->set_mesh(collectMesh2);
			obj_instance->set_scale(Vector3(3.0,3.0,3.0));
		}
		else if (index == 2){
			Ref<ArrayMesh> collectMesh2 = ResourceLoader::get_singleton()->load("res://assets/Environment/Meshes/Mesh02.res", "ArrayMesh");
			obj_instance->set_mesh(collectMesh2);
			obj_instance->set_scale(Vector3(3.0,3.0,3.0));

		}
		else if (index == 3){
			Ref<ArrayMesh> collectMesh2 = ResourceLoader::get_singleton()->load("res://assets/Environment/Meshes/Mesh03.res", "ArrayMesh");
			obj_instance->set_mesh(collectMesh2);
			obj_instance->set_scale(Vector3(3.0,3.0,3.0));

		}
		else if (index == 4){
			Ref<ArrayMesh> collectMesh2 = ResourceLoader::get_singleton()->load("res://assets/Environment/Meshes/Mesh04.res", "ArrayMesh");
			obj_instance->set_mesh(collectMesh2);
			obj_instance->set_scale(Vector3(3.0,3.0,3.0));
		}
		collectibles.push_back(obj_instance); 

		//Create marker for each collectible
		Ref<ArrayMesh> markerMesh = ResourceLoader::get_singleton()->load("res://assets/Environment/Meshes/MeshArrow.res", "ArrayMesh");
		marker_instance->set_mesh(markerMesh);
		marker_instance->set_rotation_degrees(Vector3(90,0,0));
		marker_instance->set_scale(Vector3(0.5,0.5,2.0));
		arrows.push_back(marker_instance);
	}

	for (int t = 0; t < 400; t++) {
		MeshInstance3D* tree_instance;

		create_and_add_as_child(tree_instance, (vformat("Tree_%d", t+1)), true); // Create Trees

		Ref<ArrayMesh> collectMesh2 = ResourceLoader::get_singleton()->load("res://assets/Environment/Meshes/MeshTree1.res", "ArrayMesh");
		tree_instance->set_mesh(collectMesh2);

		//Set texture
		StandardMaterial3D* mat = memnew(StandardMaterial3D());
		Ref<Texture2D> spaceshipTexture = ResourceLoader::get_singleton()->load("res://assets/textures/trees_texture.jpg");

		mat->set_texture(BaseMaterial3D::TEXTURE_ALBEDO, spaceshipTexture);
		tree_instance->set_surface_override_material(0, mat);

		tree_instance->set_scale(Vector3(4.0, 4.0, 4.0));
	
		treeObjs.push_back(tree_instance);
	}

	for(int s = 0; s < numEnvObjs; s++){
		CSGMesh3D* obj_instance;

		if (s == 0){
			create_and_add_as_child(obj_instance, "SpaceShip", true);

			Ref<ArrayMesh> collectMesh2 = ResourceLoader::get_singleton()->load("res://assets/Environment/Meshes/MeshShip.res", "ArrayMesh");
			obj_instance->set_mesh(collectMesh2);
			obj_instance->set_use_collision(true);
			obj_instance->set_collision_priority(1);

			//Set texture
			StandardMaterial3D* mat = memnew(StandardMaterial3D());
			Ref<Texture2D> spaceshipTexture = ResourceLoader::get_singleton()->load("res://assets/textures/spaceship_texture.jpg");

			mat->set_texture(BaseMaterial3D::TEXTURE_ALBEDO, spaceshipTexture);
			obj_instance->set_material(mat);
			
		}

		//Create crystals to place around the environment
		else if(s > 0 && s <= 50){ 
			create_and_add_as_child(obj_instance, (vformat("Crystal_%d", s-1)), true); // Create crystals

			Ref<ArrayMesh> collectMesh2 = ResourceLoader::get_singleton()->load("res://assets/Environment/Meshes/MeshCrystal.res", "ArrayMesh");
			obj_instance->set_mesh(collectMesh2);
			obj_instance->set_use_collision(true);
			obj_instance->set_collision_priority(1);

			//Set texture
			StandardMaterial3D* mat = memnew(StandardMaterial3D());
			Ref<Texture2D> spaceshipTexture = ResourceLoader::get_singleton()->load("res://assets/textures/trees_texture.jpg");

			mat->set_texture(BaseMaterial3D::TEXTURE_ALBEDO, spaceshipTexture);
			obj_instance->set_material(mat);

			//Set Size
			float x = rng.randf_range(0.6f, 1.0f);
			obj_instance->set_scale(Vector3(0.01 * x,0.01 * x,0.01 * x));
		}

		//Start creating parkour platforms
			//Platform 1
		else if (s > 50 && s <= 58){

			//Create platform
			create_and_add_as_child(obj_instance, (vformat("Platform_%d", s-50)), true);
			Ref<ArrayMesh> collectMesh2 = ResourceLoader::get_singleton()->load("res://assets/Environment/Meshes/MeshBlock.res", "ArrayMesh");
			obj_instance->set_mesh(collectMesh2);
			obj_instance->set_use_collision(true);
			obj_instance->set_collision_priority(1);

			//Set texture
			StandardMaterial3D* mat = memnew(StandardMaterial3D());
			Ref<Texture2D> spaceshipTexture = ResourceLoader::get_singleton()->load("res://assets/textures/block_texture.jpg");
			mat->set_texture(BaseMaterial3D::TEXTURE_ALBEDO, spaceshipTexture);
			obj_instance->set_material(mat);

			//Set Size
			obj_instance->set_scale(Vector3(0.3,0.3,0.3));

		}

		//Start Creating Walls
		else if (s > 58){

			//Create Wall
			create_and_add_as_child(obj_instance, (vformat("Wall_%d", s-58)), true);
			Ref<ArrayMesh> collectMesh2 = ResourceLoader::get_singleton()->load("res://assets/Environment/Meshes/MeshWall.res", "ArrayMesh");
			obj_instance->set_mesh(collectMesh2);
			obj_instance->set_use_collision(true);
			obj_instance->set_collision_priority(1);

			//Set texture
			StandardMaterial3D* mat = memnew(StandardMaterial3D());
			Ref<Texture2D> spaceshipTexture = ResourceLoader::get_singleton()->load("res://assets/textures/block_texture.jpg");
			mat->set_texture(BaseMaterial3D::TEXTURE_ALBEDO, spaceshipTexture);
			obj_instance->set_material(mat);

			//Set Size
			obj_instance->set_scale(Vector3(20.0,20.0,20.0));


		}

		envObjects.push_back(obj_instance);
	}


	// NEW GRASS CREATION LOOP ------------------------------------------------------------------------------ They're created but their meshes aren't there

	for (int num = 0; num < 200; num++) {
		Grass* grass_instance;

		create_and_add_as_child(grass_instance, (vformat("Grass_%d", num)), true); // Create grass
		grass_collection.push_back(grass_instance);  // Add grass to the collection
		
	}
}


// Set all the object positions
void CustomScene3501::set_object_positions()
{

	for (int i = 0; i < numObjs; i++) // Set all the checkpoint positions
	{
		float x = rng.randf_range(30.0f, 170.0f); //test make back to big terrain
		float y = rng.randf_range(0.0f, 3.0f);
		float z = rng.randf_range(30.0f, 170.0f);
		float newY = sands->get_terrain_mesh()->get_height_map().get(x).get(z);
		collectibles.get(i)->set_global_position(Vector3(x, newY+0.1, z));
		arrows.get(i)->set_global_position(Vector3(x, newY+13.0, z));
	}

	//Override positions of first 2 collectibles to be in fixed locations

	collectibles.get(0)->set_global_position(envObjects.get(58)->get_global_position() + Vector3(0.0, 4.5, 0.0));
	arrows.get(0)->set_global_position(envObjects.get(58)->get_global_position() + Vector3(0.0, 17.6, 0.0));

	collectibles.get(1)->set_global_position(Vector3(121,0.0,75));
	arrows.get(1)->set_global_position(collectibles.get(1)->get_global_position() + Vector3(0.0, 17.6, 0.0));

	//Set environment object positions

		//Spaceship positioning
	envObjects.get(0)->set_global_position(Vector3(20,2,160));
	envObjects.get(0)->set_global_rotation(Vector3(-89.5,0,0));
	

		//Trees positioning

	for (int i = 0; i < 100; i++){
		
		//Generate random positions for the trees
		float x = rng.randf_range(190.0f, 200.0f);
		float z = rng.randf_range(0.0f, 200.0f);

		float y = rng.randf_range(12.0f, 17.0f);
		float newY = sands->get_terrain_mesh()->get_height_map().get(x).get(z);

		
		treeObjs.get(i)->set_global_position(Vector3(x, newY + y, z));
	}

	for (int i = 100; i < 200; i++){
		
		//Generate random positions for the trees
		float x = rng.randf_range(0.0f, 200.0f);
		float z = rng.randf_range(190.0f, 200.0f);

		float y = rng.randf_range(12.0f, 17.0f);
		float newY = sands->get_terrain_mesh()->get_height_map().get(x).get(z);

		
		treeObjs.get(i)->set_global_position(Vector3(x, newY + y, z));
	}

	for (int i = 200; i < 300; i++){
		
		//Generate random positions for the trees
		float x = rng.randf_range(0.0f, 200.0f);
		float z = rng.randf_range(0.0f, 5.0f);

		float y = rng.randf_range(12.0f, 17.0f);
		float newY = sands->get_terrain_mesh()->get_height_map().get(x).get(z);

		
		treeObjs.get(i)->set_global_position(Vector3(x, newY + y, z));
	}

	for (int i = 300; i < 400; i++){
		
		//Generate random positions for the trees
		float x = rng.randf_range(0.0f, 5.0f);
		float z = rng.randf_range(0.0f, 200.0f);

		float y = rng.randf_range(12.0f, 17.0f);
		float newY = sands->get_terrain_mesh()->get_height_map().get(x).get(z);

		
		treeObjs.get(i)->set_global_position(Vector3(x, newY + y, z));
	}

	//Position the crystals
	for(int i = 1; i < 50; i++){
		//Generate random positions for the crystals
		float x = rng.randf_range(15.0f, 185.0f);
		float z = rng.randf_range(15.0f, 185.0f);
		float newY = sands->get_terrain_mesh()->get_height_map().get(x).get(z);

		envObjects.get(i)->set_global_position(Vector3(x, newY, z));


	}

	//Set Platforms Positions
		//Platform 1
	envObjects.get(51)->set_global_position(Vector3(18,-1.0,118));
	envObjects.get(51)->set_global_rotation(Vector3(-89.5,0,0));

		//Platform 2
	envObjects.get(52)->set_global_position(envObjects.get(51)->get_global_position() + Vector3(5, 2, -19));
	envObjects.get(52)->set_global_rotation(Vector3(-89.5,1,0));

		//Platform 3
	envObjects.get(53)->set_global_position(envObjects.get(52)->get_global_position() + Vector3(6.5, 3, -14));
	envObjects.get(53)->set_global_rotation(Vector3(-89.5,2,0));

		//Platform 4
	envObjects.get(54)->set_global_position(envObjects.get(53)->get_global_position() + Vector3(24, 2, -6));
	envObjects.get(54)->set_global_rotation(Vector3(-89.5,2,0));
	envObjects.get(54)->set_scale(Vector3(0.3,1.5,0.3));

		//Platform 5
	envObjects.get(55)->set_global_position(envObjects.get(54)->get_global_position() + Vector3(10, 3, -7));
	envObjects.get(55)->set_global_rotation(Vector3(-89.5,2,0));
	envObjects.get(55)->set_scale(Vector3(0.3,0.3,0.3));

		//Platform 6
	envObjects.get(56)->set_global_position(envObjects.get(55)->get_global_position() + Vector3(-6, 3, 0));
	envObjects.get(56)->set_global_rotation(Vector3(-89.5,2,0));
	envObjects.get(56)->set_scale(Vector3(0.3,0.3,0.3));

		//Platform 7
	envObjects.get(57)->set_global_position(envObjects.get(56)->get_global_position() + Vector3(-4, 2, 9));
	envObjects.get(57)->set_global_rotation(Vector3(-89.5,4,0));
	envObjects.get(57)->set_scale(Vector3(0.3,0.3,0.3));

		//Platform 8
	envObjects.get(58)->set_global_position(envObjects.get(57)->get_global_position() + Vector3(7.5, 0.7, 11));
	envObjects.get(58)->set_global_rotation(Vector3(-89.5,3,0));
	envObjects.get(58)->set_scale(Vector3(0.5,0.5,0.7));

	//Set Walls Positions

		//Wall 1
	envObjects.get(59)->set_global_position(Vector3(120,0.0,50));

		//Wall 2
	envObjects.get(60)->set_global_position(Vector3(120,0.0,60));

		//Wall 3
	envObjects.get(61)->set_global_position(Vector3(140,0.0,50));

		//Wall 4
	envObjects.get(62)->set_global_position(Vector3(140,0.0,60));
	envObjects.get(62)->set_global_rotation_degrees(Vector3(0,90,0));
	envObjects.get(62)->set_scale(Vector3(20,20,20));

		//Wall 5
	envObjects.get(63)->set_global_position(Vector3(160,0.0,60));
	envObjects.get(63)->set_global_rotation_degrees(Vector3(0,90,0));
	envObjects.get(63)->set_scale(Vector3(40,20,20));

		//Wall 6
	envObjects.get(64)->set_global_position(Vector3(130,0.0,90));
	envObjects.get(64)->set_global_rotation_degrees(Vector3(0,0,0));
	envObjects.get(64)->set_scale(Vector3(40,20,20));

		//Wall 7
	envObjects.get(65)->set_global_position(Vector3(120,0.0,80));
	envObjects.get(65)->set_scale(Vector3(20,20,20));

		//Wall 8
	envObjects.get(66)->set_global_position(Vector3(118,0.0,77));
	envObjects.get(66)->set_global_rotation_degrees(Vector3(0,90,0));
	envObjects.get(66)->set_scale(Vector3(10,20,20));

}


void CustomScene3501::set_grass_positions()
{

		// NEW GRASS POSITIONING USING HEIGHT MAP-------------------------------------------------------------------------
		for (int g = 0; g < 200; g++) // Set all the checkpoint positions
		{
			float x = rng.randf_range(0.0f, 200.0f);
			float z = rng.randf_range(0.0f, 200.0f);
			float newY = sands->get_terrain_mesh()->get_height_map().get(x).get(z);
			grass_collection.get(g)->set_global_position(Vector3(x, newY + 0.1, z));
		}

	

}

	//Range for out of bounds
bool CustomScene3501::playerOOB()
{
	if (main_camera->get_player()->get_position().x < 15.0
		|| main_camera->get_player()->get_position().x > 185.0 
		|| main_camera->get_player()->get_position().z < 15.0 
		|| main_camera->get_player()->get_position().z > 185.0) {
		return true;
	}
	else {
		return false;
	}
}

void CustomScene3501::stopPlayer() // Stops the player from moving further our of bounds in the level
{
    if (main_camera->get_player()->get_position().x < 13.0) {
        main_camera->get_player()->set_position(Vector3(13.0, main_camera->get_player()->get_position().y, main_camera->get_player()->get_position().z));
    }
    if (main_camera->get_player()->get_position().x > 187.0) {
        main_camera->get_player()->set_position(Vector3(187.0, main_camera->get_player()->get_position().y, main_camera->get_player()->get_position().z));
    }
    if (main_camera->get_player()->get_position().z < 13.0) {
        main_camera->get_player()->set_position(Vector3(main_camera->get_player()->get_position().x, main_camera->get_player()->get_position().y, 13.0));
    }
    if (main_camera->get_player()->get_position().z > 187.0) {
        main_camera->get_player()->set_position(Vector3(main_camera->get_player()->get_position().x, main_camera->get_player()->get_position().y, 187.0));
    }

}

/*
 *
 * The usual container class added to the demos. This one should be used to create a collectibles.
 *
 * Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
 *
 */
