// NOTE: There is an error because num_particles isn't defined here. **That is okay. Just ignore it.** The helper snippet doesn't have to be a valid shader on its own.

// Define some useful constants ( you can use these, or the builtin ones, this is just another example of something you can do in here ) 
const float pi = 3.1415926536; // PI
const float pi_over_two = 1.5707963268;
const float two_pi = 2.0*pi; // TAU

// returns a pseudo-random number in the range 0-1
// has been tuned to work well with float(INDEX) and similar values; you can make another version if you want to use a vastly different seed, just remove / num_particles. 
// factor 1 acts as an offset
// source: Book of Shaders -> https://thebookofshaders.com/10/#:~:text=a%20different%20value.-,2D%20Random,-Now%20that%20we
float pseudo_random_from_inputs(float seed, float factor1){
	//return (mod((seed*factor1+factor2),factor3)/factor3);
	return fract(sin(dot(vec2((seed)*68.379 / num_particles + 0.236472, factor1), // IGNORE THE ERROR ON THIS LINE
	 vec2(12.9898,78.233)))*48.5453123);
}

float round_to(float input, float round_to){
	return round(input/round_to) * round_to;
}

float ping_pong(float max_, float min_){
	float range = max_ - min_;
	float out_ = min_ + abs((mod((TIME * 0.25),(range*2.0f)))-range);
	return out_;
} // note: variable names with underscores are reserved without them
