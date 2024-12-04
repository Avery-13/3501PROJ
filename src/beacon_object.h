#ifndef BEACON_H
#define BEACON_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>

#include <godot_cpp/variant/vector3.hpp>

// included proper header for use in this derived class  DB
#include "game_object.h"

// This is a repurposed copy of the "example_derived_class" that will act as the checkpoints in this game  DB
namespace godot {
	class BeaconObject : public GameObject {
		GDCLASS(BeaconObject, GameObject);

	private:
		double time_passed; 


	protected:
		static void _bind_methods();


	public:
		BeaconObject();

		void _enter_tree() override;
		void _ready() override;

		
	};

}

#endif