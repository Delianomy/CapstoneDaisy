#pragma once
#include "Component.h"
#include <glew.h>
#include <vector>
#include "VMath.h"
#include "MEW.h"
using namespace MATH;

class MeshComponent : public Component {
	MeshComponent(const MeshComponent&) = delete;
	MeshComponent(MeshComponent&&) = delete;
	MeshComponent& operator = (const MeshComponent&) = delete;
	MeshComponent& operator = (MeshComponent&&) = delete;

private:
	const char* filename;
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvCoords;
	size_t dataLength;
	GLenum drawmode;

	/// Private helper methods
	void LoadModel(const char* filename);
	void StoreMeshData(GLenum drawmode_);
	GLuint vao, vbo;
public:
	int minX, minY, minZ, maxX, maxY, maxZ;
	
	MeshComponent(Component *parent_,const char* filename_);
	~MeshComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime);
	void Render() const;
	void Render(GLenum drawmode) const;
	/*float GetMaxX() { 
		std::cout << "Vertices" << std::endl;
		MEW::PrintVecList(vertices);
		std::cout << std::endl;

		float m = MEW::GetMaxX(vertices);
		std::cout << m << std::endl;
		return MEW::GetMaxX(vertices); }*/
	
	/*void GetMaxX() {
		std::cout << "===================" << std::endl;
		MEW::PrintVecList(vertices);
		std::cout << "===================" << std::endl;
		std::cout << "Max value: " << MEW::GetMaxX(vertices) << std::endl;
	}*/
};

