#ifndef TERRAIN_MESH_H_
#define TERRAIN_MESH_H_

// parent class
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/object.hpp>

#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/shader.hpp>

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/array.hpp>

#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/variant/packed_color_array.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>


#include <godot_cpp/classes/fast_noise_lite.hpp>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/os.hpp>

#include <godot_cpp/classes/resource.hpp> 
#include <godot_cpp/classes/resource_loader.hpp>

#include <godot_cpp/templates/vector.hpp>

#include "defs.h"

// everything in gdextension is defined in this namespace
namespace godot {

	class TerrainMesh : public ArrayMesh {
		// this macro sets up a few internal things
		GDCLASS(TerrainMesh, ArrayMesh);

	private:
		double time_passed;

		PackedVector3Array vertices; 	// vertex positions in model space
		PackedVector3Array normals; 	// vertex normals
		PackedVector2Array uv;	 		// texture coordinates
		PackedColorArray colors;		// color associated with that vertex
		PackedInt32Array indices;		// creates the triangles; interpreted as triples.

		// dimensions of mesh in number of vertices
		int array_width;
		int array_height;

		// since Vector<Vector<float>> is not an option for saving, we need to have a PackedFloat32Array that has the same information. 
		Vector<Vector<float>> height_map;

	protected:
		// a static function that Godot will call to find out which methods can be called and which properties it exposes
		static void _bind_methods();

	public:
		TerrainMesh();
		~TerrainMesh();

		void setup_terrain(float total_height = 5.0f, float circle_radius = 2.0f, int grid_cell = 90, int octave = 30);
		void setup_arrays();
		void clear_arrays();
		Vector<Vector<float>> get_height_map();
	};

}

#endif