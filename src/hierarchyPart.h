#ifndef HIERARCHYPART_H
#define HIERARCHYPART_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/cylinder_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#define DEBUG true
#define LINEUP_SPACE 3.0f

// everything in gdextension is defined in this namespace
namespace godot {
class HierarchyPart : public MeshInstance3D {
    // this macro sets up a few internal things
	GDCLASS(HierarchyPart, MeshInstance3D);

private:
	double time_passed;
	
protected:
    // a static function that Godot will call to find out which methods can be called and which properties it exposes
	static void _bind_methods();

	// the position relative to the transformation hierarchy parent (*T*orso)
	Vector3 local_position;

	// these two are to set up the orbit matrix (t*O*rso)
	Vector3 joint_position;
	Quaternion orbit_rotation;

	// the rotation relative to the transformation hierarchy parent (to*R*so)
	Quaternion local_rotation;

	Vector3 scale; // (x-scale, y-scale, z-scale) (tor*S*o)

	// this is the parent for our transformation hierarchy
	HierarchyPart* parent;

public:
	HierarchyPart();
	~HierarchyPart();
	
	void _enter_tree ( ) override;
	void _ready ( ) override;

	// for our transformation hierarchy, since we will not be using the built-in one. 
	// To be extra clear: The parent in the SceneTree will be Crane (as set up by add_child); the parent in our internal transformation hierarchy will be another CranePart according to the assignment and set up by you! 
	void set_parent(HierarchyPart* parent); 
	HierarchyPart* get_parent(); 

	// These functions are specifically for hierarchical transformations as we are not using the hierarchical transformations provided by Godot
	Transform3D get_transformation_matrix();
	Transform3D get_transformation_matrix_without_scaling();

	// Getters and Setters for the primary variables, if you want / need them
	Vector3 get_local_position();
	void set_local_position(Vector3);
	Quaternion get_local_rotation();
	void set_local_rotation(Quaternion);
	Vector3 get_scale();
	void set_scale(Vector3);
	Quaternion get_orbit_rotation();
	void set_orbit_rotation(Quaternion);
	Vector3 get_joint_position();
	void set_joint_position(Vector3);

	void set_color(Vector3);
};

}

#endif