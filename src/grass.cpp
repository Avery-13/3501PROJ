#include "grass.h"
#include <godot_cpp/core/memory.hpp> // for memnew
#include <godot_cpp/classes/scene_tree.hpp> // for root

using namespace godot;

void Grass::_bind_methods() {
	
}

Grass::Grass() : Node3D() {}

Grass::~Grass() {
	// Add cleanup here, if you have any. I don't, typically. 
}


void Grass::_enter_tree ( ){
	//if(DEBUG) UtilityFunctions::print("Entering Tree - Grass.");

		//Create the base of the grass blade
	create_and_add_as_child<GrassBase>(gBase, "grassBase", true);
	create_and_add_as_child<GrassBase>(gBaseB, "grassBaseB", true);
	create_and_add_as_child<GrassBase>(gBaseC, "grassBaseC", true);




		//Create the middle and end of the grass blade

		//Blade A
	create_and_add_as_child<GrassMid>(gMid, "grassMid", true);
	gMid->set_parent(gBase);

		create_and_add_as_child<GrassMid>(gMid2, "grassMid2", true);
	gMid2->set_parent(gMid);


		//Blade B
	create_and_add_as_child<GrassMid>(gMidB, "grassMidB", true);
	gMidB->set_parent(gBaseB);

		create_and_add_as_child<GrassMid>(gMidB2, "grassMidB2", true);
	gMidB2->set_parent(gMidB);
	
		//Blade C
	create_and_add_as_child<GrassMid>(gMidC, "grassMidC", true);
	gMidC->set_parent(gBaseC);

	create_and_add_as_child<GrassMid>(gMidC2, "grassMidC2", true);
	gMidC2->set_parent(gMidC);
	

	
}

void Grass::_ready(){
	//if(DEBUG) UtilityFunctions::print("Ready - Grass.");

	gBase->set_local_position(get_global_position());

	gBaseB->set_local_position(gBase->get_local_position() + Vector3(-0.09,-0.1,0.07));
	gBaseC->set_local_position(gBase->get_local_position() + Vector3(0.12,-0.074,0.18));

		//Set different orbits
	gBaseB->set_orbit_rotation(Vector3(0, 0.0024, gBase->get_orbit_rotation().z * -0.8873));
	gBaseB->set_orbit_rotation(Vector3(0, -0.0011, gBase->get_orbit_rotation().z * 0.7224));

}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool Grass::create_and_add_as_child(T* &pointer, String name, bool search){
	// this is the default behaviour
	// added the search parameter so that we can skip the slow "find_child" call during runtime (not applicable to this demo, you should always use search = true until next assignment)
	if(search == false){
		pointer = memnew(T);
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}

	// always only have to search once if we save it here
	Node* child = find_child(name);
	
	if(child == nullptr){
		pointer = memnew(T);
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}
	else{
		pointer = dynamic_cast<T*>(child);
		return false;
	}
}


template <class T2, class T3>
bool Grass::create_and_add_as_child_custom(T2* &pointer, String name, T3* &pointerParent, bool search){

// this is the default behaviour
	// added the search parameter so that we can skip the slow "find_child" call during runtime (not applicable to this demo, you should always use search = true until next assignment)
	if(search == false){
		pointer = memnew(T2);
		pointer->set_name(name);
		pointerParent->add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}

	// always only have to search once if we save it here
	Node* child = find_child(name);
	
	if(child == nullptr){
		pointer = memnew(T2);
		pointer->set_name(name);
		pointerParent->add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}
	else{
		pointer = dynamic_cast<T2*>(child);
		return false;
	}
}

void Grass:: set_Local_pos(Vector3 input){
	gBase->set_local_position(input);
	gBaseB->set_local_position(gBase->get_local_position() + Vector3(-0.09,-0.1,0.07));
	gBaseC->set_local_position(gBase->get_local_position() + Vector3(0.12,-0.074,0.18));
}


