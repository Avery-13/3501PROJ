shader_type spatial;

// this shader is like a blank canvas, if you want to start from something fresh. it doesn't change the screen texture at all

render_mode unshaded; // without this line, the world will attempt to apply lighting to the quad, which will make *everything* look darker (or lighter, depending)
render_mode skip_vertex_transform;
render_mode fog_disabled; // we aren't going to use fog, but putting this here anyways so that you know it would be necessary

uniform sampler2D screen_texture : source_color, hint_screen_texture, filter_nearest_mipmap;
// Included helper file to get constants  DB
//#include "helpers.gdshaderinc"

void vertex() {
	// Called for every vertex the material is visible on.
	POSITION = vec4(VERTEX, 1.0f); // which space is this? (clip space)
	//POSITION = vec4(VERTEX * 0.75, 1.0f); // what does this do?
}


void fragment() {
    vec4 pixel = textureLod(screen_texture, SCREEN_UV, 0.0);  // Create a pixel texture from the OG screen texture  DB
	// Made an if-else statement based on the current TIME to switch back and forth between brightening and darkening effect  DB
	if (mod(TIME,2.0) < 1.0){ 
		// Calculate a value which increases overtime based on the if statements TIME calculation  DB
		float lunge = 0.5+mod(TIME,2.0); // Added a constant value 0.5 to the lunge value so that the screen won't become completely black when mod(TIME,2.0) = 0.0  DB
		float g = (pixel.r+pixel.g+pixel.b); 
		pixel = vec4(g*0.8,g*0.1,g*0.1,1)*lunge; // Make the screen red and have it grow brighter overtime using lunge value  DB
	} else {
		float lunge = mod(TIME,2.0); // Calculate a value which increases overtime based on the if statements TIME calculation  DB
		float g = (pixel.r+pixel.g+pixel.b);
		pixel = vec4(g*0.8,g*0.1,g*0.1,1)/lunge; // Make the screen red and have it grow darker overtime using lunge value  DB
	
	}
	ALBEDO = pixel.rgb; // Set ALBEDO based on calculated pixel vec4  DB
}
