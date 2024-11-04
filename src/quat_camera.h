#ifndef QUATCAMERA_H
#define QUATCAMERA_H

#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/variant/vector3.hpp>

namespace godot
{
	class QuatCamera : public Camera3D
	{
		GDCLASS(QuatCamera, Camera3D);

	private:
		Vector3 forward_ = Vector3(0, 0, -1);
		Vector3 side_ = Vector3(1, 0, 0);

		Vector3 GetForward() const;
		Vector3 GetSide() const;

		void Yaw(float angle);
		void Pitch(float angle);

		const float MAX_SPEED = 3.0f;

	protected:
		static void _bind_methods();

	public:
		QuatCamera();
		~QuatCamera();

		void _ready() override;
		void _process(double delta) override;
		void _input(const Ref<InputEvent> &event) override;
	};
}

#endif