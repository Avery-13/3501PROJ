shader_type spatial;

// START -- DO NOT CHANGE -- YOU WILL GET A ZERO
render_mode skip_vertex_transform; // self-explanatory
render_mode unshaded; // turns off environment lighting
// END -- DO NOT CHANGE -- YOU WILL GET A ZERO


// you can give a hint to uniforms, but do make sure that you are passing them correctly to the shader.
uniform vec3 color_in; // = vec3(1, 0, 0);

// TODO add uniform for light position! It must move around the scene, not be stationary. 
//uniform vec3 pos; // Added uniform for light source position  DB

uniform int num_lights;

uniform vec3 l_positions[50];
//uniform PA

// TODO add uniform for specular power!
uniform float specular_power; // Added uniform for specular power  DB

varying smooth vec3 position_interp; // what does "smooth" do? can you think of a situation where you would want a 'varying' to be "flat?"

// Added new varying to capture light sources position in Viewspace  DB
varying vec3 light_source_position;

varying vec3 light_source_positions[20];

varying vec3 lv_array[20];

varying float diffuse_arr[20];
varying float spec_arr[20];


void vertex() {
	// Called for every vertex the material is visible on.
	POSITION = (PROJECTION_MATRIX * VIEW_MATRIX * (MODEL_MATRIX * vec4(VERTEX, 1.0)));
    
	// VIEW SPACE FOR LIGHTING CALCULATIONS !
    position_interp = (VIEW_MATRIX * (MODEL_MATRIX * vec4(VERTEX, 1.0))).xyz;

	vec3 test_pos = vec3(0.0,5.0,0.0);
	vec3 test_pos2 = vec3(0.0,-5.0,0.0);
	// transform light position uniform from world space to view space  DB
	for (int i = 0; i < num_lights; i++){
		light_source_position = (VIEW_MATRIX * (vec4(l_positions[i], 1.0))).xyz;
		light_source_positions[i] = light_source_position;
	}
	

   	NORMAL = vec3(MODELVIEW_NORMAL_MATRIX * NORMAL);
}

void fragment() {
	// Called for every pixel the material is visible on.
	vec4 pixel; 
	// can start with a fixed color...
	pixel = vec4(1,1,1,1.0);
	// ...or color from vertex...
	pixel = COLOR;
	// ... or color from uniform
	pixel = vec4(color_in, 1.0f);
	float finalDiffuse = 0.0;
	float finalSpec = 0.0;
	vec4 light_color = vec4(1, 1, 1, 1);
	///*
	for (int q = 0; q<num_lights; q++){
		vec3 view_position = vec3(0, 0, 0); // origin in view space
		
		vec3 vv = normalize(view_position - position_interp); // view direction
		vec3 lv = normalize(light_source_positions[q] - position_interp);
		
		vec3 n = normalize(NORMAL); // must normalize interpolated normal

		float diffuse = dot(n,lv); // N.L -- anything wrong with this line?
		diffuse = max(0.0, diffuse);
		vec3 ref = 2.0*(dot(lv,n)*n); 
		vec3 reflection = -(lv) + ref; // Computed reflection direction for specular component  DB
		float spec = max(0.0,dot(vv,reflection)); // Computed specular component to use PHONG now  DB
		spec = pow(spec,specular_power); 
		
		finalDiffuse += diffuse;
		finalSpec += spec;
	}
	//*/
	/*
	vec3 view_position = vec3(0, 0, 0); // origin in view space
	
	vec3 vv = normalize(view_position - position_interp); // view direction
	//vec3 lv = normalize(light_source_position - position_interp); // Replaced static position with the viewspace light position uniform  DB
	for (int i = 0; i < num_lights; i++){
		vec3 lv = normalize(light_source_positions[i] - position_interp);
		lv_array[i] = lv;
	}
	
	vec3 n = normalize(NORMAL); // must normalize interpolated normal

	for (int d = 0; d < num_lights; d++){
		float diffuse = dot(n,lv_array[d]); 
		vec3 ref = 2.0*(dot(lv_array[d],n)*n); 
		vec3 reflection = -(lv_array[d]) + ref; // Computed reflection direction for specular component  DB
		float spec = dot(vv,reflection); // Computed specular component to use PHONG now  DB

		spec = pow(spec,specular_power); // specular power -- instead of hardcoded number, should have uniform here too
	
		diffuse_arr[d] = diffuse;
		spec_arr[d] = spec;
	}
	//float diffuse = dot(n,lv); // N.L -- anything wrong with this line?
	//float spec = max(0.0,dot(n,h)); // cannot be negative - BLINN-PHONG
	//vec3 ref = 2.0*(dot(lv,n)*n); 
	//vec3 reflection = -(lv) + ref; // Computed reflection direction for specular component  DB
	//float spec = dot(vv,reflection); // Computed specular component to use PHONG now  DB

	// spec power now uses the specular_power uniform  DB
	//spec = pow(spec,specular_power); // specular power -- instead of hardcoded number, should have uniform here too
	for (int a = 0; a < num_lights; a++){
		finalDiffuse += diffuse_arr[a];
		finalSpec += spec_arr[a];
		//light_color = vec4(0.5, 0.9, 0.3, 1);
	}
	*/
	vec4 ambcol = vec4(1,0,0,1); // ambient light -- in this case red, could be anything; should make uniform

	//spec = 0.0f;  // turn off specular
	//diffuse = 0.0f; // turn off diffuse
    float amb = 0.0; // ambient coefficient

    // Use variable "pixel", surface color, to help determine fragment color
    //ALBEDO = (light_color*pixel*diffuse +
	//   light_color*vec4(1,1,1,1)*spec + // specular might not be colored
	//   light_color*pixel*amb).xyz; // ambcol not used, could be included here
	
	
	ALBEDO = (light_color*pixel*finalDiffuse +
	   light_color*vec4(1,1,1,1)*finalSpec + // specular might not be colored
	   light_color*pixel*amb).xyz; // ambcol not used, could be included here

}
