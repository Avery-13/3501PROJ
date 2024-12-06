#ifndef TERRAIN_INSTANCE_H
#define TERRAIN_INSTANCE_H

// parent class
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/shader.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/resource.hpp> 
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/torus_mesh.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/compressed_texture2d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/templates/vector.hpp> // GODOT EQUIVALENT TO std::vector !!!! 

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/transform3d.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/string.hpp>

#include "defs.h"
#include "terrain.h"

#include <godot_cpp/classes/csg_mesh3d.hpp>

#define DEBUG true

// everything in gdextension is defined in this namespace
namespace godot {
	class TerrainInstance : public CSGMesh3D {
		// this macro sets up a few internal things
		GDCLASS(TerrainInstance, CSGMesh3D);

	private:
		double time_passed;

		TerrainMesh* terrain_mesh;

	protected:
		// a static function that Godot will call to find out which methods can be called and which properties it exposes
		static void _bind_methods();

	public:
		TerrainInstance();
		~TerrainInstance();

		void _enter_tree() override;
		void _ready() override;
		void _process(double delta) override;

		inline TerrainMesh* get_terrain_mesh() { return terrain_mesh; }

	};

}

#endif