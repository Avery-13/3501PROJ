#ifndef GRASS_H
#define GRASS_H

// parent class
#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/classes/input.hpp>

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include "hierarchyPart.h"
// #include "jackBox.h"
// #include "jackSpring.h"
// #include "jackBody.h"
// #include "jackHead.h"
// #include "jackEye.h"
// #include "jackArm.h"

#include "grassBase.h"
#include "grassMid.h"

#define NUM_PARTS 9 // this is just for looping


// everything in gdextension is defined in this namespace
namespace godot {
class Grass : public Node3D {
    // this macro sets up a few internal things
	GDCLASS(Grass, Node3D);

private:
    // JackBox* jBox;
	// JackSpring* jSpring0;
	// JackSpring* jSpring1;
	// JackSpring* jSpring2;
	// JackSpring* jSpring3;
	// JackSpring* jSpring4;
	// JackSpring* jSpring5;
	// JackBody* jBody;
	// JackHead* jHead;
	// JackEye* jEye0;
	// JackEye* jEye1;
	// JackArm* jArmUpRight;
	// JackArm* jArmUpLeft;
	// JackArm* jArmLowRight;
	// JackArm* jArmLowLeft;

	GrassBase* gBase;
	GrassMid* gMid;
	GrassMid* gMid2;

	GrassBase* gBaseB;
	GrassMid* gMidB;
	GrassMid* gMidB2;

	GrassBase* gBaseC;
	GrassMid* gMidC;
	GrassMid* gMidC2;



protected:
	static void _bind_methods();


public:
	Grass();
	~Grass();

	void _enter_tree ( ) override;
	void _ready ( ) override;

	template <class T>
	bool create_and_add_as_child(T* &pointer, String name, bool search = false);

	template <class T2, class T3>
	bool create_and_add_as_child_custom(T2* &pointer, String name, T3* &pointerParent, bool search = false);

};

}

#endif // GRASS_H