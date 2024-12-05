#include "particle_system_3501.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/shader_material.hpp>

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/quad_mesh.hpp>

using namespace godot;

void ParticleSystem3501::_bind_methods() {
	// IMPORTANT NOTE: in order to use a non-zero argument constructor, you need to bind the methods. 
	// Think about why that is and ask about it if you can't figure it out. It will maybe help you understand what's going on with the SceneTree a bit better. 
	ClassDB::bind_method(D_METHOD("set_shader_name", "shader_name"), &ParticleSystem3501::set_shader_name);
	ClassDB::bind_method(D_METHOD("get_shader_name"), &ParticleSystem3501::get_shader_name);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "shader_name"), "set_shader_name", "get_shader_name");
}

ParticleSystem3501::ParticleSystem3501() : GPUParticles3D() {
	// Initialize any variables here.
	shader_name = "default_value"; // there won't be a shader with this name
}

ParticleSystem3501::ParticleSystem3501(String shader_prefix) : GPUParticles3D() {
	// Initialize any variables here.
	shader_name = shader_prefix;
}

void ParticleSystem3501::_enter_tree(){

}

void ParticleSystem3501::_ready(){
	// Particle system properties, can be changed from the custom scene code as well
	set_amount(25000);
	set_lifetime(2.0);
	
	// set up the material to attach to the quad
	ShaderMaterial* spatial_material = memnew(ShaderMaterial);
	Ref<Shader> shader = ResourceLoader::get_singleton()->load(vformat("assets/shaders/%s_ss.gdshader", shader_name), "Shader");
	if (!shader.is_valid()) {
    	UtilityFunctions::print("Failed to load shader!");
	}
	spatial_material->set_shader(shader); 
	// this will be the default texture of the spatial material unless you change it
	spatial_material->set_shader_parameter("texture_image", ResourceLoader::get_singleton()->load("res://assets/textures/droplet_texture.png"));

	// Set up the quad to use for each particle
	QuadMesh* mesh = memnew(QuadMesh);
	mesh->set_size(Vector2(0.5, 1));

	// set up process material
	ShaderMaterial* process_material = memnew(ShaderMaterial);
	shader = ResourceLoader::get_singleton()->load(vformat("assets/shaders/%s_ps.gdshader", shader_name), "Shader");
	process_material->set_shader(shader);

	// attach the materials and mesh
	mesh->set_material(spatial_material);
	set_draw_pass_mesh(0, mesh);
	set_process_material(process_material);

	// can be useful
	spatial_material->set_shader_parameter("num_particles", get_amount());
	process_material->set_shader_parameter("num_particles", get_amount());

}

ParticleSystem3501::~ParticleSystem3501() {
	// Add your cleanup here.
}

void ParticleSystem3501::_process(double delta){
	if (Engine::get_singleton()->is_editor_hint()) return; // Early return if we are in editor
}

/*
*
* A class which assists in addition of particle systems to the scene. 
*
* Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
*
*/
