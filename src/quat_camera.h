#ifndef QUATCAMERA_H
#define QUATCAMERA_H

#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/variant/vector3.hpp>

namespace godot
{
	class QuatCamera : public Camera3D
	{
		GDCLASS(QuatCamera, Camera3D);

	private:
		// Movement-related
		Vector3 velocity;
		static constexpr float WALK_SPEED = 5.0f;
		static constexpr float SPRINT_SPEED = 8.0f;
		static constexpr float JUMP_VELOCITY = 8.0f;

		// Gravity
		float gravity = 9.8f;

		// Mouse look sensitivity
		float sensitivity = 0.003f;

		// Vertical rotation clamping
		float min_pitch = -40.0f; // degrees
		float max_pitch = 60.0f;  // degrees
		float pitch_angle = 0.0f;

		CharacterBody3D* player_body;
		Node3D* head_node;
		Node* player_node;
		Node* world_node;

		// Helper methods
		Vector3 get_input_direction();
		void apply_gravity(float delta);
		void handle_movement(float delta);
		void handle_jump();

	protected:
		static void _bind_methods();

	public:
		QuatCamera();
		~QuatCamera();

		void _ready() override;
		void _process(double delta) override;
		void _physics_process(double delta) override;
		void _input(const Ref<InputEvent> &event) override;

		CharacterBody3D* get_player() { return player_body; }
	};
}

#endif
