#include "grassBase.h"

using namespace godot;

void GrassBase::_bind_methods() {
}

// You may not unset this class as top level (from parent)
GrassBase::GrassBase() {
	// This is just to line them up for the starting scene -- change this for sure. 
	local_position = Vector3(0.235,0.109,0.183);
	//timeTest = Time::get_singleton()->get_ticks_msec;
	currTime = Time::get_singleton()->get_ticks_msec();
}

GrassBase::~GrassBase(){
	// Add cleanup here, if you have any. I don't, typically. 
}

void GrassBase::_enter_tree ( ) {
	if(DEBUG) UtilityFunctions::print("Enter Tree - GrassBase.");
	
	// Create a Box (base of the grass)
	BoxMesh* boxBase = memnew(BoxMesh);

    boxBase->set_size(Vector3(0.02, 0.12, 0.02));

	StandardMaterial3D* material = memnew(StandardMaterial3D);
	material->set_albedo(Color(0.2, 0.8, 0, 1));
	boxBase->surface_set_material(0, material);
	set_joint_position(Vector3(0,-0.06,0));

	set_mesh(boxBase);

}

void GrassBase::_process(double delta){
	if (Engine::get_singleton()->is_editor_hint()) return; // Early return if we are in editor

	set_global_transform(get_transformation_matrix()); 

	        //Access current time for swaying
    currTime = Time::get_singleton()->get_ticks_msec();
	

        //Sway the whole model
    set_orbit_rotation(Vector3(0,0,0.2*sin(0.0025*currTime)));

}

