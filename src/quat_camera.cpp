#include "quat_camera.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

using namespace godot;

void QuatCamera::_bind_methods() {}

QuatCamera::QuatCamera() : Camera3D() {}

QuatCamera::~QuatCamera() {}

void QuatCamera::_ready()
{
	set_position(Vector3(0, 1.8, 0)); // Example eye level height
}

void QuatCamera::_process(double delta)
{
	if (Engine::get_singleton()->is_editor_hint())
		return;

	Vector3 direction;
	Input *input = Input::get_singleton();

	if (input->is_action_pressed("ui_up")) // W key
	{
		direction += GetForward();
	}
	if (input->is_action_pressed("ui_down")) // S key
	{
		direction -= GetForward();
	}
	if (input->is_action_pressed("ui_right")) // D key
	{
		direction += GetSide();
	}
	if (input->is_action_pressed("ui_left")) // A key
	{
		direction -= GetSide();
	}

	if (direction != Vector3())
	{
		direction = direction.normalized();
		set_position(get_position() + direction * MAX_SPEED * delta);
	}
}

void QuatCamera::_input(const Ref<InputEvent> &event)
{
	if (const InputEventMouseMotion *mouse_event = Object::cast_to<InputEventMouseMotion>(event.ptr()))
	{
		Vector2 mouse_motion = mouse_event->get_relative();
		float rotation_factor = 0.002f; // Adjust sensitivity as needed

		Yaw(-mouse_motion.x * rotation_factor);
		Pitch(-mouse_motion.y * rotation_factor);
	}
}

Vector3 QuatCamera::GetForward() const
{
	return -get_global_transform().basis.z;
}
Vector3 QuatCamera::GetSide() const
{
	return get_global_transform().basis.x;
}

void QuatCamera::Pitch(float angle)
{
	Transform transform = get_global_transform();
	transform.basis = transform.basis.rotated(transform.basis.x, angle);
	set_global_transform(transform);
}

void QuatCamera::Yaw(float angle)
{
	Transform transform = get_global_transform();
	transform.basis = transform.basis.rotated(Vector3(0, 1, 0), angle);
	set_global_transform(transform);
}