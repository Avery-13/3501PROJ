shader_type spatial;

// Render Modes
render_mode unshaded;
render_mode skip_vertex_transform;
render_mode cull_disabled;
render_mode blend_add;

// Uniforms
uniform sampler2D texture_image;
uniform float num_particles;

// Constants
const vec3 object_color = vec3(0.6, 1.0, 0.2); // Neon yellowish-green
const vec3 color_change = vec3(0.0, 0.0, 0.0); // No color change

// Flicker parameters
const float lit_duration = 3.0;       // Firefly stays lit for 3 seconds
const float flicker_duration = 2.0;   // Flicker duration of 2 seconds
const float flicker_speed = 20.0;     // Speed of flickering during flicker phase

// Varyings
varying float particle_id;
varying float lifespan;

void vertex() {
    // Retrieve particle ID from COLOR.a
    particle_id = COLOR.a;

    // Get the particle's lifespan
    lifespan = INSTANCE_CUSTOM.w;

    // Retrieve motion parameters from CUSTOM
    float initial_angle = INSTANCE_CUSTOM.x;
    float speed = INSTANCE_CUSTOM.y;
    float phase = INSTANCE_CUSTOM.z;

    // Calculate the particle's age
    float spawn_time = particle_id * lifespan;
    float age = mod(TIME - spawn_time, lifespan);

    // Wandering motion
    float angle = initial_angle + speed * age;
    float x_offset = cos(angle) * 0.5;
    float z_offset = sin(angle) * 0.5;
    float y_offset = sin(phase + age) * 0.5;

    vec3 offset = vec3(x_offset, y_offset, z_offset);

    // Apply the offset to the particle position
    vec3 position = VERTEX + offset;

    // Apply billboarding
    MODELVIEW_MATRIX = VIEW_MATRIX * mat4(
        INV_VIEW_MATRIX[0], 
        INV_VIEW_MATRIX[1], 
        INV_VIEW_MATRIX[2], 
        vec4(offset, 0.0)+MODEL_MATRIX[3]
    );
    MODELVIEW_NORMAL_MATRIX = mat3(MODELVIEW_MATRIX);
    POSITION = PROJECTION_MATRIX * MODELVIEW_MATRIX * vec4(position, 1.0);
}

void fragment() {
    // Adjust UV coordinates to select different parts of the texture
    vec2 uv = UV * 0.5;
    uv.x += mod(floor(particle_id * 4.0), 2.0) * 0.5;
    uv.y += mod(floor(particle_id * 4.0) + 1.0, 2.0) * 0.5;

    // Sample the texture
    vec4 outval = texture(texture_image, uv);

    // Calculate the particle's age
    float spawn_time = particle_id * lifespan;
    float age = mod(TIME - spawn_time, lifespan);

    // Flicker logic
    float cycle_duration = lit_duration + flicker_duration;
    float flicker_offset = particle_id * cycle_duration; // Introduce per-particle variation
    float cycle_time = mod(age + flicker_offset, cycle_duration);
    float flicker;

    if (cycle_time < lit_duration) {
        // Lit phase
        flicker = 1.0;
    } else {
        // Flicker phase
        float flicker_time = cycle_time - lit_duration;
        flicker = abs(sin(flicker_time * flicker_speed));
    }

    // Compute dynamic color
    vec3 color = object_color * flicker;

    // Adjust the sampled color
    outval = vec4(outval.rgb * color, sqrt(sqrt(outval.r)) * flicker);

    // Compute the alpha value
    float alpha = outval.a * flicker;

    // Set the final color and alpha
    ALBEDO = outval.rgb;
    ALPHA = alpha;
}
