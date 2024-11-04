#include "defs.h"
#include "example_derived_class.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>



using namespace godot;

void ExampleDerivedClass::_bind_methods() {}

ExampleDerivedClass::ExampleDerivedClass() : ExampleAbstractClass() {
	time_passed = 0.0;
}

void ExampleDerivedClass::_enter_tree ( ){
	if(DEBUG) UtilityFunctions::print("Enter Tree - ExampleDerivedClass."); 
}

void ExampleDerivedClass::_ready ( ){
	if(DEBUG) UtilityFunctions::print("Ready - ExampleDerivedClass."); 
}