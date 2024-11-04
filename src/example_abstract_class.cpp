#include "defs.h"
#include "example_abstract_class.h"

using namespace godot;

void ExampleAbstractClass::_bind_methods() {}

ExampleAbstractClass::ExampleAbstractClass() : MeshInstance3D() {
	some_shared_attribute = 2.0f;
}