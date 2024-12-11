#ifndef ENVIRON_OBJECT_H
#define ENVIRON_OBJECT_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/csg_mesh3d.hpp>

#include "game_object.h"

namespace godot {
	class EnvironObject : public CSGMesh3D {
		GDCLASS(EnvironObject, CSGMesh3D);

	private:
		double time_passed; 


	protected:
		static void _bind_methods();


	public:
		EnvironObject();

		void _enter_tree() override;
		void _ready() override;

		
	};

}

#endif