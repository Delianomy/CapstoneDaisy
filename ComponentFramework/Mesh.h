#pragma once
#include "Component.h"
#include <glew.h>
#include <vector>
#include "Vector.h"
using namespace MATH;

class Mesh : public Component {
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) = delete;
	Mesh& operator = (const Mesh&) = delete;
	Mesh& operator = (Mesh&&) = delete;

private:
	const char* filename;
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvCoords;
	size_t dateLength;
	GLenum drawmode;

	/// Private helper methods
	void LoadModel(const char* filename);
	void StoreMeshData(GLenum drawmode_);
	GLuint vao, vbo;
public:
	
	Mesh(Component *parent_,const char* filename_);
	~Mesh();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime);
	void Render() const;
	void Render(GLenum drawmode) const;
	
};

