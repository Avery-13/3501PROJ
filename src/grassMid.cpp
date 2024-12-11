#include "grassMid.h"

using namespace godot;

void GrassMid::_bind_methods() {
}

// You may not unset this class as top level (from parent)
GrassMid::GrassMid() : HierarchyPart() {
	// This is just to line them up for the starting scene -- change this for sure. 
	local_position = Vector3(-0.01, 0.1, 0.0);
}

GrassMid::~GrassMid(){
	// Add cleanup here, if you have any. I don't, typically. 
}

void GrassMid::_enter_tree ( ) {
	//if(DEBUG) UtilityFunctions::print("Enter Tree - GrassMid.");
	
	// Create the springs to hold Jack up

        //Initialise mesh
    BoxMesh* grassBase = memnew(BoxMesh);

        //Set material
    StandardMaterial3D* material = memnew(StandardMaterial3D);
    material->set_albedo(Color(0.5, 0.7, 0.7, 1));
    grassBase->surface_set_material(0, material);

    grassBase->set_size(Vector3(0.02, 0.12, 0.02));

    set_mesh(grassBase);
    
    set_orbit_rotation(Vector3(0,0,0.2));
    
	

}

void GrassMid::_process(double delta){
	if (Engine::get_singleton()->is_editor_hint()) return; // Early return if we are in editor

	set_global_transform(get_transformation_matrix());  

	//         //Access current time for swaying
    // Time* timeTest = new Time;
    // int currTime = (timeTest->get_ticks_msec());

    //     //Sway the whole model
    
    // set_orbit_rotation(Vector3(0,0,0.2*sin(0.0025*currTime)));

}

