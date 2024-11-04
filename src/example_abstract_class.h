#ifndef EXAMPLE_ABSTRACT_CLASS
#define EXAMPLE_ABSTRACT_CLASS

#include <godot_cpp/classes/mesh_instance3d.hpp> // for the debug statements
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements

// everything in gdextension is defined in this namespace
namespace godot {
class ExampleAbstractClass : public MeshInstance3D {
	GDCLASS(ExampleAbstractClass, MeshInstance3D);
	
protected:
	static void _bind_methods();
	float some_shared_attribute;

public:
	ExampleAbstractClass();
	
	virtual void _enter_tree ( ) override = 0;
	virtual void _ready ( ) override = 0;

	inline float get_some_shared_attribute() { return some_shared_attribute; }
	inline float set_some_shared_attribute(float in) { some_shared_attribute = in; }
};

}

#endif // EXAMPLE_ABSTRACT_CLASS