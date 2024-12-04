#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <godot_cpp/classes/mesh_instance3d.hpp> 
#include <godot_cpp/variant/utility_functions.hpp> 

// This class is a copy of the "example abstract class" repurposed to act as the parent class to the new gaming objects being added  DB
namespace godot {
	class GameObject : public MeshInstance3D {
		GDCLASS(GameObject, MeshInstance3D);

	protected:
		static void _bind_methods();
		// Added this attribute so that certain gaming objects have a way to check when to stop their behaviour when the game ends  DB
		bool stop;	

	public:
		GameObject();

		virtual void _enter_tree() override = 0;
		virtual void _ready() override = 0;

		
		void end_state() { stop = true; }	// Added this function to be called in the scene when the game ends to stop object processes  DB
		bool check_collisions(Vector3);		// Added this function so that all derived classes can check if they have been collided with  DB
		
	};

}

#endif 