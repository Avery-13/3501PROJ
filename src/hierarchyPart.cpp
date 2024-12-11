#include "hierarchyPart.h"


using namespace godot;

void HierarchyPart::_bind_methods() {}

HierarchyPart::HierarchyPart() : MeshInstance3D(), parent(nullptr) {
	time_passed = 0.0;

	// primary attributes
	local_position = Vector3();
	local_rotation = Quaternion();

	joint_position = Vector3(0, 0, 0);
	orbit_rotation = Quaternion();

	scale = Vector3(1, 1, 1);

	// this makes it so that any hierarchy established in the node structure does not apply to the hierarchical transformations done by Godot for us, which would interfere
	set_as_top_level(true); // DO NOT CHANGE THIS LINE
}

HierarchyPart::~HierarchyPart(){
}

void HierarchyPart::_enter_tree ( ){
	//if(DEBUG) UtilityFunctions::print("Enter Tree - HierarchyPart."); 
}

void HierarchyPart::_ready ( ){
	//if(DEBUG) UtilityFunctions::print("Ready - ", get_name()); // since this will be called by many of the parts

	set_global_transform(get_transformation_matrix());  
}

void HierarchyPart::set_parent(HierarchyPart* par){
	this->parent = par;
}

HierarchyPart* HierarchyPart::get_parent(){
	return parent;
}

// This function is specifically for use as a parent for hierarchical transformations. 
Transform3D HierarchyPart::get_transformation_matrix(){	

	Transform3D translation = Transform3D().translated(local_position);
	Transform3D orbit = Transform3D(orbit_rotation).translated(joint_position); // TODO QUESTION 1: finish implementing orbits
	Transform3D rotation = Transform3D(local_rotation);
	Transform3D scaling = Transform3D().scaled(scale);
	// UtilityFunctions::print(get_name()); 

	Transform3D local_transform = translation * orbit * rotation * scaling;

	return (get_parent() == nullptr)? local_transform : parent->get_transformation_matrix_without_scaling() * local_transform; // for hierarchical transformations, we typically leave scaling off
}

// You usually don't want to encorporate the parent's scaling.
Transform3D HierarchyPart::get_transformation_matrix_without_scaling(){	
	Transform3D translation = Transform3D().translated(local_position);
	Transform3D orbit = Transform3D(orbit_rotation).translated(joint_position); // TODO QUESTION 1: finish implementing orbits	
	Transform3D rotation = Transform3D(local_rotation);
	// UtilityFunctions::print(get_name()); 

	Transform3D local_transform = translation * orbit * rotation;

	return (get_parent() == nullptr)? local_transform : parent->get_transformation_matrix_without_scaling() * local_transform; // for hierarchical transformations, we typically leave scaling off
}

Vector3 HierarchyPart::get_local_position(){
	return local_position;
}

void HierarchyPart::set_local_position(Vector3 input){
	local_position = input;
}

Quaternion HierarchyPart::get_local_rotation(){
	return local_rotation;
}

void HierarchyPart::set_local_rotation(Quaternion input){
	local_rotation = input;
}

Vector3 HierarchyPart::get_scale(){
	return scale;
}

void HierarchyPart::set_scale(Vector3 input){
	scale = input;
}

Quaternion HierarchyPart::get_orbit_rotation(){
	return orbit_rotation;
}

void HierarchyPart::set_orbit_rotation(Quaternion input){
	orbit_rotation = input;
}

Vector3 HierarchyPart::get_joint_position(){
	return joint_position;
}

void HierarchyPart::set_joint_position(Vector3 input){
	joint_position = input;
}
