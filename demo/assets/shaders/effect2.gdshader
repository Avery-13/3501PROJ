shader_type spatial;

render_mode unshaded; // without this line, the world will attempt to apply lighting to the quad, which will make *everything* look darker (or lighter, depending)
render_mode skip_vertex_transform;
render_mode fog_disabled; // we aren't going to use fog, but putting this here anyways so that you know it would be necessary

uniform sampler2D screen_texture : source_color, hint_screen_texture, filter_nearest_mipmap;

void vertex() {
	// Called for every vertex the material is visible on.
	POSITION = vec4(VERTEX, 1.0f); // which space is this? (clip space)
	//POSITION = vec4(VERTEX * 0.75, 1.0f); // what does this do?
}

void fragment() {
	//float range = sin(TIME);
    vec4 pixel = textureLod(screen_texture, SCREEN_UV, 0.0); // the third argument has to do with the mip-map level
	
	
	if (SCREEN_UV.x > 0.0 && SCREEN_UV.y < 1.0)
	{

		vec2 pos = SCREEN_UV;

		// Generate pseudo-random number value
		float u = fract(sin(dot(vec2(SCREEN_UV.x*68.379,  SCREEN_UV.y), vec2(12.9898,78.233)))*48.5453123);
		float v = fract(sin(dot(vec2(SCREEN_UV.x*68.379,  SCREEN_UV.y), vec2(12.4598,78.233)))*43.2135563);

		// Create the noise/recording corruption effect
		float g = (pixel.r+pixel.g+pixel.b)/3.0;
		vec4 finalColour = vec4(sin(u)*g, sin(v)*g, sin(u)*g,1);
		
		//pixel;

		// Make objects look wavy to make the video seem distorted
		float offset  = 0.025*(sin(TIME*8.2+u*v)); //u and v values create the "blurring" effect

		pos.y = pos.y + offset;
		pos.x = pos.x + offset/6.0; //moving in x and y directions by different values to make it look more disordered


		// Apply diagonal distortion every few seconds
		if(int(TIME) % 3 == 0 || int(TIME) % 4 == 0){
			pos.x = pos.x + pos.y * cos(TIME)*0.1;
		}
		

		// Apply the movement distortion effect
		pixel = texture(screen_texture, pos);

		// Apply the noise distortion effect every few seconds
		if(int(TIME) % 6 == 0){
			pixel = finalColour;
		}

		// Or apply a screen glitch effect every few seconds
		else if(int(TIME) % 2 == 0){
			if((SCREEN_UV.x > 0.2 && SCREEN_UV.x < 0.3) || (SCREEN_UV.x > 0.9 && SCREEN_UV.x < 0.96) || (SCREEN_UV.x > 0.5 && SCREEN_UV.x < 0.7)){
				pixel.r +=  0.5;
				pixel.g += 0.8;
				pixel.b += 0.23;
			}
		}

	}

    ALBEDO = pixel.rgb;
}



//void light() {
	//// Called for every pixel for every light affecting the material.
	//// Uncomment to replace the default light processing function with this one.
//}

// Attribution: This effect has been adapted from the one provided in the OpenGL delivery of the course. 