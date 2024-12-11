#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <godot_cpp/classes/mesh_instance3d.hpp> 
#include <godot_cpp/variant/utility_functions.hpp> 

namespace godot {
	class GameObject : public MeshInstance3D {
		GDCLASS(GameObject, MeshInstance3D);

	protected:
		static void _bind_methods();
		bool stop;	

	public:
		GameObject();

		virtual void _enter_tree() override = 0;
		virtual void _ready() override = 0;

		
		void end_state() { stop = true; }
		bool check_collisions(Vector3);		
		
	};

}

#endif 