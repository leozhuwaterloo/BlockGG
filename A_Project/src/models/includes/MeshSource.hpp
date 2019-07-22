#pragma once


// Includes the source data for a mesh
struct MeshSource {
public:
	MeshSource()
    : vao(0),
		  vbo_position(0),
      vbo_normal(0),
			vbo_uv_coord(0) {}

  GLuint vao;
  GLuint vbo_position;
  GLuint vbo_normal;
  GLuint vbo_uv_coord;
	GLuint vbo_tangent;
};

struct MeshSource2D {
	MeshSource2D()
		: vao(0),
			vbo_position(0) {}

	GLuint vao;
	GLuint vbo_position;
};
