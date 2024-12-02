shader_type spatial;

// this shader is like a blank canvas, if you want to start from something fresh. it doesn't change the screen texture at all

render_mode unshaded; // without this line, the world will attempt to apply lighting to the quad, which will make *everything* look darker (or lighter, depending)
render_mode skip_vertex_transform;
render_mode fog_disabled; // we aren't going to use fog, but putting this here anyways so that you know it would be necessary

uniform sampler2D screen_texture : source_color, hint_screen_texture, filter_nearest_mipmap;

//#include "helpers.gdshaderinc"

void vertex() {
	// Called for every vertex the material is visible on.
	POSITION = vec4(VERTEX, 1.0f); // which space is this? (clip space)
	//POSITION = vec4(VERTEX * 0.75, 1.0f); // what does this do?
}

// this shader just shows the screen; it's see-through completely aka there is no effect
void fragment() {
	//float range = sin(TIME);
    vec3 pixel = textureLod(screen_texture, SCREEN_UV, 0.0).rgb; // the third argument has to do with the mip-map level
	ALBEDO = pixel;
}
