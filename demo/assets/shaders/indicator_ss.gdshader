shader_type spatial;

render_mode blend_add;
//render_mode alpha_to_coverage;
// DO NOT CHANGE -- YOU WILL GET A 0 -- YOU MUST INCLUDE THIS IN YOUR OTHER SHADERS FOR THIS ASSIGNMENT
render_mode unshaded; // without this line, the world will attempt to apply lighting to the quad, which will make *everything* look darker (or lighter, depending)
render_mode skip_vertex_transform;
// END (DO NOT CHANGE -- YOU WILL GET A 0)

uniform sampler2D texture_image;
uniform float num_particles;

varying float timestep;
varying float circtime;
varying float particle_id;
varying float lifespan;

// Simulation parameters (constants)
const vec3 up_vec = vec3(0.0, 1.0, 0.0); // Up direction
const float accel = 2.0; // An acceleration applied to the particles coming from some attraction force
const float speed = 3.0; // Control the speed of the motion

void vertex() {	
    // Define particle id
    particle_id = COLOR.r; // We put that in the instance custom values. We use the id to keep track of particles

    // Define time in a cyclic manner
    float phase = TAU*particle_id; // Start the sin wave later depending on the particle_id
	// you could pass in a uniform here instead of TIME for a local timer
    timestep = (TIME + phase) / INSTANCE_CUSTOM.w; // The constant that divides "timer" also helps to adjust the "speed" of the fire
    float rem = mod(timestep, PI/2.0); // Use the remainder of dividing by pi/2 so that we are always in the range [0..pi/2] where sin() gives values in [0..1]
    circtime = sin(rem); // Get time value in [0..1], according to a sinusoidal wave
	COLOR.a = COLOR.a * (1.0-circtime*0.7);
                                    
    // Set up parameters of the particle motion
    float t = abs(circtime)*(0.3 + abs(INSTANCE_CUSTOM.z)); // Our time parameter

    // First, work in local model coordinates (do not apply any transformation)
    vec3 position = VERTEX;
    vec3 offset = speed*up_vec*accel*t*t + 1.5*INSTANCE_CUSTOM.xyz * speed*accel*t*t ; // Particle moves up
    
    // Define amount of blending depending on the cyclic time
    //float alpha = 1.0 - circtime*circtime;
    //COLOR = vec4(1.0, 1.0, 1.0, alpha);
    // BILLBOARDING -- if you do not put the offset HERE then the particles will travel relative to the viewer, not the world. 
	MODELVIEW_MATRIX = VIEW_MATRIX * mat4(INV_VIEW_MATRIX[0], INV_VIEW_MATRIX[1], INV_VIEW_MATRIX[2], vec4(offset, 0.0)+MODEL_MATRIX[3]);
	MODELVIEW_NORMAL_MATRIX = mat3(MODELVIEW_MATRIX);
	//// Called for every vertex the material is visible on.
	POSITION = (PROJECTION_MATRIX * MODELVIEW_MATRIX * vec4(position, 1.0));
	//index = INSTANCE_CUSTOM.x;
	lifespan = INSTANCE_CUSTOM.w;
	timestep = circtime;
}

// Simulation parameters (constants)
const vec3 object_color = vec3(0.9, 0.3, 0.3);
const vec3 color_change = vec3(-0.1, 0.2, -0.1);

void fragment() {
	vec2 uv = UV * 0.5;
	uv.x = uv.x + mod(floor(particle_id * 4.0), 2.0) * 0.5;
	uv.y = uv.y + mod(floor(particle_id * 4.0) + 1.0, 2.0) * 0.5;
    // Get pixel from texture
    vec4 outval = texture(texture_image, uv);
	vec3 color = (timestep * color_change) + object_color;
    // Adjust specified object color according to the grayscale texture value
    outval = vec4(outval.r*color.r, outval.g*color.g, outval.b*color.b, sqrt(sqrt(outval.r))*COLOR.a);
    // Set output fragment color
	float alpha = outval.a * min(1.5*(1.0-timestep), 1.0) + 0.01;
    ALBEDO = outval.rgb * alpha;
}

//void light() {
	//// Called for every pixel for every light affecting the material.
	//// Uncomment to replace the default light processing function with this one.
//}
