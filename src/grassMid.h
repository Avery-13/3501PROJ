#ifndef GRASSMID_H
#define GRASSMID_H

#include "hierarchyPart.h"
#include <godot_cpp/classes/torus_mesh.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/time.hpp>
#include <math.h>

// everything in gdextension is defined in this namespace
namespace godot {
class GrassMid : public HierarchyPart {
    // this macro sets up a few internal things
	GDCLASS(GrassMid, HierarchyPart);

// private:

	
protected:
    // a static function that Godot will call to find out which methods can be called and which properties it exposes
	static void _bind_methods();

public:
	GrassMid();
	~GrassMid();

	void _enter_tree ( ) override;
	void _process(double delta) override;
	


};

}

#endif