#ifndef CUSTOM_SCENE_H
#define CUSTOM_SCENE_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/window.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include <godot_cpp/templates/vector.hpp> // GODOT EQUIVALENT TO std::vector !!!! 

#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

#include <godot_cpp/variant/rect2.hpp> // for viewport size
#include <godot_cpp/classes/canvas_item.hpp> // for viewport size
#include <godot_cpp/classes/control.hpp> // for the anchors preset
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/quad_mesh.hpp>
#include <godot_cpp/classes/array_mesh.hpp>

#include <godot_cpp/classes/grid_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>

#include "defs.h"
#include "quat_camera.h"
#include "example_derived_class.h"
#include "beacon_object.h"
#include "particle_system_3501.h"
// everything in gdextension is defined in this namespace
namespace godot {
class CustomScene3501 : public Node3D {
    // this macro sets up a few internal things
	GDCLASS(CustomScene3501, Node3D);

private:
	double time_passed;

	QuatCamera* main_camera;
	GridContainer* main_ui;
	Vector<ExampleDerivedClass*> reference_instances;
	Vector<BeaconObject*> collectibles;
	Vector<ParticleSystem3501*> particle_systems;

	//TerrainInstance* sands;
	MeshInstance3D* screen_quad_instance;
	ShaderMaterial* screen_space_shader_material;
	// create and setup the boxes; for this one they don't need to have separate create and setup functions. 
	// This shouldn't be called in the assignment that you hand in. You can choose to delete the code if you want to. 
	void setup_reference_boxes();
	void set_object_positions();


	int numObjs = 5;
	int collectCount;

protected:
    // a static function that Godot will call to find out which methods can be called and which properties it exposes
	static void _bind_methods();

public:
	CustomScene3501();
	~CustomScene3501();

	void _process(double delta) override;
	void _enter_tree ( ) override;
	void _ready ( ) override;

	RandomNumberGenerator rng;

	void print_tree(Node* node, int depth = 0);

	// shader prefix refers to name_ss.gdshader and name_ps.gdshader; should become more clear when you look at the particle system class provided's code
	void create_particle_system(String node_name, String shader_name);

	// the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
	// search defines whether the scenetree should be checked for an instance
	template <class T>
	bool create_and_add_as_child(T* &pointer, String name, bool search = false);

	template <class T>
	bool add_as_child(T* &pointer, String name, bool search = false);

};

}

#endif