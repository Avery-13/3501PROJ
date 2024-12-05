#include "quat_camera.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void QuatCamera::_bind_methods() {}

QuatCamera::QuatCamera() : Camera3D() {}

QuatCamera::~QuatCamera() {}

void QuatCamera::_ready()
{
    UtilityFunctions::print("QuatCamera ready.");

    // Hide the mouse cursor and capture its movement
    if (!Engine::get_singleton()->is_editor_hint())
    {
        Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
    }

    // Locate the World node
    Node *current = get_parent();
    while (current != nullptr)
    {
        if (current->get_name() == StringName("World"))
        {
            world_node = current;
            break;
        }
        current = current->get_parent();
    }

    if (!world_node)
    {
        UtilityFunctions::print("World node not found.");
        return;
    }

    // Find the Player node
    player_node = world_node->find_child("Player", true, false);
    if (!player_node)
    {
        UtilityFunctions::print("Player node not found.");
        return;
    }

    // Ensure the Player node is a CharacterBody3D
    player_body = Object::cast_to<CharacterBody3D>(player_node);
    if (!player_body)
    {
        UtilityFunctions::print("Player node is not a CharacterBody3D.");
        return;
    }

    // Find the Head node under Player
    Node *head_temp = player_node->find_child("Head", true, false);
    head_node = Object::cast_to<Node3D>(head_temp);
    if (!head_node)
    {
        UtilityFunctions::print("Head node not found or is not a Node3D.");
        return;
    }
}
void QuatCamera::_process(double delta){}


void QuatCamera::_physics_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint() || !head_node)
        return;

    // Handle jump logic
    handle_jump();
    // Handle movement and physics for the player
    handle_movement(delta);

    // Sync camera position to the Head node
    Transform3D head_transform = head_node->get_global_transform();
    Transform3D camera_transform = get_global_transform();
    camera_transform.origin = head_transform.origin; // Update position only
    set_global_transform(camera_transform);
}

void QuatCamera::_input(const Ref<InputEvent> &event)
{
    if (Engine::get_singleton()->is_editor_hint())
        return;  

    if (!head_node)
        return;

    if (const InputEventMouseMotion *mouse_event = Object::cast_to<InputEventMouseMotion>(event.ptr()))
    {
        Vector2 mouse_motion = mouse_event->get_relative();
        float yaw = -mouse_motion.x * sensitivity;
        float pitch = -mouse_motion.y * sensitivity;

        // Rotate the parent (head) on the Y-axis
        head_node->rotate_y(yaw);

        // Clamp pitch rotation and apply to the camera
        pitch_angle += pitch;
        pitch_angle = Math::clamp(pitch_angle, Math::deg_to_rad(min_pitch), Math::deg_to_rad(max_pitch));
        set_rotation(Vector3(pitch_angle, get_rotation().y, get_rotation().z));
    }

    // Toggle mouse mode with Escape key
    if (Input::get_singleton()->is_action_just_pressed("ui_cancel"))
    {
        Input *input = Input::get_singleton();
        if (input->get_mouse_mode() == Input::MOUSE_MODE_CAPTURED)
        {
            input->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);
            UtilityFunctions::print("Mouse released.");
        }
        else
        {
            input->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
            UtilityFunctions::print("Mouse captured.");
        }
    }
}

void QuatCamera::apply_gravity(float delta)
{
    if (!player_body)
        return;

    // Apply gravity to the player body
    if (!player_body->is_on_floor())
    {
        velocity.y -= gravity * delta;
    }
    else
    {
        velocity.y = 0.0f;
    }
}

void QuatCamera::handle_jump()
{
    if (!player_body)
        return;

    // Handle jump
    if (Input::get_singleton()->is_action_just_pressed("jump") && player_body->is_on_floor())
    {
        velocity.y = JUMP_VELOCITY;
		player_body->set_velocity(velocity);
		player_body->move_and_slide();
    }
}

void QuatCamera::handle_movement(float delta)
{
    if (!player_body)
    {
        UtilityFunctions::print("Player body reference is invalid.");
        return;
    }

    // Get input direction
    Vector3 input_dir = get_input_direction();
    if (input_dir != Vector3())
    {
        input_dir = input_dir.normalized();

        // Check if sprinting
        float current_speed = Input::get_singleton()->is_action_pressed("sprint") ? SPRINT_SPEED : WALK_SPEED;

        velocity.x = input_dir.x * current_speed;
        velocity.z = input_dir.z * current_speed;
    }
    else
    {
        // Smooth deceleration when no input
        velocity.x = Math::lerp(velocity.x, 0.0f, delta * 7.0f);
        velocity.z = Math::lerp(velocity.z, 0.0f, delta * 7.0f);
    }

    apply_gravity(delta);

    // Debug velocity
	//UtilityFunctions::print("Velocity after gravity and jump: ", velocity);

    // Set and apply velocity
    player_body->set_velocity(velocity);
    player_body->move_and_slide();
}

Vector3 QuatCamera::get_input_direction()
{
    if (!head_node)
        return Vector3();

    Vector2 input_vector = Input::get_singleton()->get_vector("ui_left", "ui_right", "ui_up", "ui_down");
    //UtilityFunctions::print("Input vector: ", input_vector);

    Transform3D head_transform = head_node->get_global_transform();
    Vector3 forward = head_transform.basis.get_column(Vector3::AXIS_Z); // Forward direction
    Vector3 right = head_transform.basis.get_column(Vector3::AXIS_X);    // Right direction

    return (forward * input_vector.y + right * input_vector.x);
}

