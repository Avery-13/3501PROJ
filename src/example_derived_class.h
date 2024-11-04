#ifndef POWERUP_H
#define POWERUP_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>

#include <godot_cpp/variant/vector3.hpp>


#include "example_abstract_class.h"

// everything in gdextension is defined in this namespace
namespace godot {
class ExampleDerivedClass : public ExampleAbstractClass {
	GDCLASS(ExampleDerivedClass, ExampleAbstractClass);

private:
	double time_passed; // maybe you don't need this, just an example
	
protected:
	static void _bind_methods();

public:
	ExampleDerivedClass();
	
	void _enter_tree ( ) override;
	void _ready ( ) override;
};

}

#endif