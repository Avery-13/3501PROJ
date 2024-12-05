shader_type particles;

// Uniforms
uniform float num_particles;
uniform vec3 min_position_range;
uniform vec3 max_position_range;

// Include helper functions
#include "res://assets/shaders/helpers.gdshaderinc"

void start() {
    // Generate per-particle random values
    float u = pseudo_random_from_inputs(float(INDEX), 0.5);
    float v = pseudo_random_from_inputs(float(INDEX), 0.6);
    float w = pseudo_random_from_inputs(float(INDEX), 0.7);

    // Set initial position using the range uniforms
    float x = mix(min_position_range.x, max_position_range.x, u);
    float y = mix(min_position_range.y, max_position_range.y, v);
    float z = mix(min_position_range.z, max_position_range.z, w);
    TRANSFORM[3].xyz = vec3(x, y, z);

    // Store random coefficients in CUSTOM for wandering motion
    CUSTOM.x = pseudo_random_from_inputs(float(INDEX), 0.8) * 2.0 * PI; // Initial angle
    CUSTOM.y = mix(0.5, 1.5, v); // Speed
    CUSTOM.z = pseudo_random_from_inputs(float(INDEX), 0.9) * 2.0 * PI; // Vertical oscillation phase
    CUSTOM.w = LIFETIME;

    // Store normalized particle ID in COLOR.a
    float normalized_id = float(INDEX) / max(num_particles - 1.0, 1.0);
    COLOR = vec4(1.0, 1.0, 1.0, normalized_id);
}
