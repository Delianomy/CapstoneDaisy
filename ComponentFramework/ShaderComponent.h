#pragma once
#include "Component.h"
#include "glew.h"
#include <string>
#include <unordered_map>

class ShaderComponent: public Component {
	ShaderComponent(const ShaderComponent&) = delete;
	ShaderComponent(ShaderComponent&&) = delete;
	ShaderComponent& operator = (const ShaderComponent&) = delete;
	ShaderComponent& operator = (ShaderComponent&&) = delete;

private:
	const char* vertexShaderFilename;
	const char* fragmentShaderFilename;
	const char* tessellationControlFilename;
	const char* tesselationEvaluationFilename;
	const char* geometryShaderFilename;
	unsigned int shaderID;
	unsigned int vertShaderID; 
	unsigned int fragShaderID;
	unsigned int tcShaderID;
	unsigned int teShaderID;
	unsigned int geomShaderID;
	std::unordered_map<std::string, GLuint > uniformMap;

	/// Private helper methods
	char* ReadTextFile(const char *fileName);	
	bool CompileAttach();
	bool Link();
	void SetUniformLocations();
	

public:

	ShaderComponent(Component* parent_, const char* vsFilename_, const char* fsFilename_);
	ShaderComponent(Component* parent_, const char* vsFilename_, const char* fsFilename_, const char* gsFilename_);
	ShaderComponent(Component* parent_, const char* vsFilename_, const char* fsFilename_, const char* tcsFilename_, const char* tesFilename_, const char* gsFilename_ );
	
	~ShaderComponent();
	inline GLuint GetProgram() const { return shaderID;}
	GLuint GetUniformID(std::string name);
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float deltaTime ) override;
	void Render() const override;
};


