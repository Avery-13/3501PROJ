#include "terrain.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;



void TerrainMesh::_bind_methods() {
}

TerrainMesh::TerrainMesh() {
	// Initialize any variables here.
	time_passed = 0.0;
	array_width = -1;
	array_height = -1;
	setup_terrain();
	setup_arrays();
}

TerrainMesh::~TerrainMesh() {
	// Add your cleanup here.
}

void TerrainMesh::clear_arrays() {
	vertices.clear();
	normals.clear();
	uv.clear();
	colors.clear();
	indices.clear();
	clear_surfaces();
}

void TerrainMesh::setup_arrays() {
	// actually use the arrays that we setup in the other functions to make a mesh.
	Array arrays = Array();
	arrays.resize((int)Mesh::ARRAY_MAX);
	arrays[(int)Mesh::ARRAY_VERTEX] = vertices;
	arrays[(int)Mesh::ARRAY_NORMAL] = normals;
	arrays[(int)Mesh::ARRAY_TEX_UV] = uv;
	arrays[(int)Mesh::ARRAY_COLOR] = colors;
	arrays[(int)Mesh::ARRAY_INDEX] = indices;

	// could also use a triangle strip, but it is a bit more confusing. I don't recommend it.
	add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
}

void TerrainMesh::setup_terrain(float total_height, float circle_radius, int grid_cell, int octave) {
	clear_arrays();
	

	// Create vertices 
	float phi, theta, new_rad, center, scale, amplitude, height;
	Vector3 loop_center;
	Vector3 vertex_position;
	Vector3 vertex_normal;
	Color vertex_color;
	Vector2 vertex_coord;

	int xDimension = 200;
	int zDimension = 200;
	int octaveCount = 7;
	float x_offset, z_offset, currTest;
	FastNoiseLite* noisyBoy = new FastNoiseLite();
	noisyBoy->TYPE_SIMPLEX_SMOOTH;
	UtilityFunctions::print(noisyBoy->get_noise_2dv(Vector2(1.0, 1.0)));

	currTest = 0.0;
	for (int i = 0; i < xDimension; i++) { // large loop   
		theta = 2.0 * Math_PI * i / xDimension; // loop sample (angle theta)

		for (int j = 0; j < zDimension; j++) { // small circle
			height = 0.0;
			scale = 1.0;
			amplitude = 1.0;
			for (int h = 0; h < octaveCount; h++) {
				currTest = (noisyBoy->get_noise_2d(i, j) * scale) * amplitude;  //crashing
				scale = scale * 2.0;
				amplitude = amplitude * 0.5;
				height += currTest;
			}

			phi = 2.0 * Math_PI * j / zDimension; // circle sample (angle phi)

			// Define position, normal and color of vertex
			vertex_position = Vector3(float(i), height, float(j)); 
			vertex_normal = vertex_position;
			vertex_color = Color(float(i), 0.0, float(j));
			vertex_coord = Vector2(theta / 2.0 * Math_PI, phi / 2.0 * Math_PI); 

			vertices.push_back(vertex_position);
			normals.push_back(vertex_normal);
			uv.push_back(vertex_coord);
			colors.push_back(vertex_color);
		}
	}

	// Create triangles
	for (int i = 0; i < xDimension - 1; i++) { //Modified to rid of looping back issue
		for (int j = 0; j < zDimension - 1; j++) {
			// Two triangles per quad
			Vector3 t1 = Vector3(((i + 1) % xDimension) * zDimension + j,
				i * zDimension + ((j + 1) % xDimension),
				i * zDimension + j);

			Vector3 t2 = Vector3(((i + 1) % xDimension) * zDimension + j,
				((i + 1) % xDimension) * zDimension + ((j + 1) % zDimension),
				i * zDimension + ((j + 1) % zDimension));


			// Add first triangle to the data buffer
			indices.push_back(t1[0]);
			indices.push_back(t1[1]);
			indices.push_back(t1[2]);

			// Add second triangle to the data buffer
			indices.push_back(t2[0]);
			indices.push_back(t2[1]);
			indices.push_back(t2[2]);
		}
	}

	array_width = xDimension;
	array_height = zDimension;
	for (int x = 0; x < array_width; x++) {
		Vector<float> height_map_row;
		for (int y = 0; y < array_width; y++) {
			height_map_row.append(vertices[x * array_width + y].y);
		}
		height_map.append(height_map_row);
	}

}

Vector<Vector<float>> TerrainMesh::get_height_map() {
	return height_map;
}

/*
*
* A mesh class which prompts students to create a procedural cone. Has code for a procedural torus to get them started.
*
* Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
*
*/