#include "terrain_instance.h"
#include <godot_cpp/core/memory.hpp> // for memnew
#include <godot_cpp/core/object.hpp> // for memnew

using namespace godot;

void TerrainInstance::_bind_methods() {
}

TerrainInstance::TerrainInstance() : CSGMesh3D() {
	time_passed = 0.0;

}

TerrainInstance::~TerrainInstance() {
	// Add your cleanup here (if any is needed)
}

void TerrainInstance::_enter_tree() {
	set_name("Sand Dunes");
	if (DEBUG) UtilityFunctions::print("Enter Tree - ", get_name());

	TerrainMesh* mesh = memnew(TerrainMesh());
	StandardMaterial3D* mat = memnew(StandardMaterial3D());
	Ref<Texture2D> sandy = ResourceLoader::get_singleton()->load("res://assets/textures/Sand_texture.jpg");
	mat->set_texture(BaseMaterial3D::TEXTURE_ALBEDO, sandy);

	mesh->surface_set_material(0, mat);
	set_mesh(mesh);
	set_use_collision(true);
	set_collision_priority(1);
	terrain_mesh = mesh;

}

// Note: This section runs every time we open the game again. For a small game like what we will make for this assignment, it will be fine. If you want to refactor to make it load from saved data instead, you can, but it likely won't be a good use of your time. 
void TerrainInstance::_ready() {
	if (DEBUG) UtilityFunctions::print("Ready - ", get_name());

}
// 
// called every frame (as often as possible)
void TerrainInstance::_process(double delta) {
	// Set the light position in the shader constantly  DB

	time_passed += delta;

}


/*
*
* A MeshInstance3D class which intended to make using ShaderMaterials from a container class a bit easier.
*
* Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
*
*/