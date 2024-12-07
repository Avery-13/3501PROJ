shader_type particles;

// Uniforms
uniform float num_particles;
uniform vec3 min_position_range;
uniform vec3 max_position_range;

uniform vec3 curr_position;

const float loop_radius = 0.5;
const float circle_radius = 5.0;

// Include helper functions
#include "res://assets/shaders/helpers.gdshaderinc"

void start() {
    // Generate per-particle random values
	float u, v, w;
	u = pseudo_random_from_inputs(float(INDEX), 0.23678);
	v = pseudo_random_from_inputs(float(INDEX), 0.6321);
	w = pseudo_random_from_inputs(float(INDEX), 0.7654);

    // calculate starting position
	u = pseudo_random_from_inputs(float(INDEX), 0.4532);
	v = pseudo_random_from_inputs(float(INDEX), 1.2942);
    float theta, phi;
	theta = u * 2.0*PI; // random angle in 360 degree range
	phi = v * 2.0*PI; // random angle in 360 degree range




    // Set initial position
    vec3 position = vec3(loop_radius*sin(theta), 0.0, 0.0) + curr_position; //added vector adjusts displacement
    TRANSFORM[3].xyz = position;

    //Velocity
    VELOCITY = vec3(0.0,5000, 0.0);


    // Store random coefficients in CUSTOM for wandering motion
    float spray = 0.8*pow(w, (1.0/3.0));
    vec3 wander = vec3(0.0,spray,0.0);

    CUSTOM.xyz = wander;
    CUSTOM.w = LIFETIME;

    float normalized_id = float(INDEX) / max(num_particles - 1.0, 1.0);
    COLOR = vec4(1.0, 1.0, 1.0, normalized_id);
    }
