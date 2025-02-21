#include "ShaderComponent.h"
#include "Debug.h"
#include <fstream>
#include <string.h>

ShaderComponent::ShaderComponent(Component* parent_, const char* vsFilename_, const char* fsFilename_, const char* tcsFilename_, const char* tesFilename_, const char* gsFilename_): 
	Component(parent_),shaderID(0),vertShaderID(0),fragShaderID(0),tcShaderID(0),teShaderID(0),geomShaderID(0) {
	vertexShaderFilename = vsFilename_;
	fragmentShaderFilename = fsFilename_;
	tessellationControlFilename = tcsFilename_;
	tesselationEvaluationFilename = tesFilename_;
	geometryShaderFilename = gsFilename_;
}

ShaderComponent::ShaderComponent(Component* parent_, const char* vsFilename_, const char* fsFilename_):
	ShaderComponent(parent_, vsFilename_, fsFilename_, nullptr, nullptr, nullptr) {
}

ShaderComponent::ShaderComponent(Component* parent_, const char* vsFilename_, const char* fsFilename_, const char* gsFilename_):
	ShaderComponent(parent_, vsFilename_, fsFilename_, nullptr, nullptr, gsFilename_) {

}

bool ShaderComponent::OnCreate() {
	if (isCreated) return isCreated;
	isCreated = CompileAttach();
	if (isCreated == false) return isCreated;
	isCreated = Link();
	if (isCreated == false) return isCreated;

	SetUniformLocations();
	return true;
}





bool ShaderComponent::CompileAttach(){
    GLint status;

	
	try { 
		const char* vsText = nullptr;
		const char* fsText = nullptr;
		const char* gsText = nullptr;

		vsText = ReadTextFile(vertexShaderFilename);
		if (vsText == nullptr) {
			std::string errorMsg("Can't open file:");
			throw errorMsg + vertexShaderFilename;
		}
		fsText = ReadTextFile(fragmentShaderFilename);
		if (vsText == nullptr) {
			std::string errorMsg("Can't open file:");
			throw errorMsg + fragmentShaderFilename;
		}
		/// GL_VERTEX_SHADER and GL_FRAGMENT_SHADER are defined in glew.h
		vertShaderID = glCreateShader(GL_VERTEX_SHADER);
		fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		/// Check for errors
		if (vertShaderID == 0 || fragShaderID == 0) {
			std::string errorMsg("Can't create a new shader ID");
			throw errorMsg;
		}
    
		glShaderSource(vertShaderID, 1, &vsText, 0);   
		glShaderSource(fragShaderID, 1, &fsText, 0);
    
		glCompileShader(vertShaderID);
		/// Check for errors
		glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &status);
			if(status == 0) {
				GLsizei errorLogSize = 0;
				GLsizei titleLength;
				std::string errorLog = vertexShaderFilename;
				errorLog +="\nVERT:\n";
				titleLength = errorLog.length();
				glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
				errorLog.resize( titleLength + errorLogSize );
				glGetShaderInfoLog(vertShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
				throw errorLog;
			}

		glCompileShader(fragShaderID);
		/// Check for errors
		glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &status);
			if(status == 0) {
				GLsizei errorLogSize = 0;
				GLsizei titleLength;
				std::string errorLog = fragmentShaderFilename;
				errorLog += "\nFrag:\n";
				titleLength = errorLog.length();
				glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
				errorLog.resize( titleLength + errorLogSize );
				glGetShaderInfoLog(fragShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
				throw errorLog;
			}
    


		if(geometryShaderFilename != nullptr){
			gsText = ReadTextFile(geometryShaderFilename);
			if (vsText == nullptr) {
				std::string errorMsg("Can't open file:");
				throw errorMsg + geometryShaderFilename;
			}
			geomShaderID = glCreateShader(GL_GEOMETRY_SHADER);
			if (geomShaderID == 0) {
				std::string errorMsg("Can't create a new geom shader ID");
				throw errorMsg;
			}
			glShaderSource(geomShaderID, 1, &gsText, 0);
			glCompileShader(geomShaderID);
			/// Check for errors
			glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &status);
			if(status == 0) {
				GLsizei errorLogSize = 0;
				GLsizei titleLength;
				std::string errorLog = geometryShaderFilename;
				errorLog +="\nGEOM:\n";
				titleLength = errorLog.length();
				glGetShaderiv(geomShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
				errorLog.resize( titleLength + errorLogSize );
				glGetShaderInfoLog(geomShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
				throw errorLog;
			}

		}
			

		shaderID = glCreateProgram();
		if(fragShaderID != 0) glAttachShader(shaderID, fragShaderID);
		if(vertShaderID != 0)glAttachShader(shaderID, vertShaderID);
		if(geomShaderID != 0)glAttachShader(shaderID, geomShaderID);

		if(vsText) delete[] vsText;
		if(fsText) delete[] fsText;
		if(gsText) delete[] gsText;

	}catch(std::string error){
		printf("%s\n",&error[0]);
		Debug::Error(error, __FILE__, __LINE__);
		return false;
	}
	return true;
}


bool ShaderComponent::Link(){
	GLint status;
	try{
			glLinkProgram(shaderID);
			/// Check for errors
			glGetProgramiv(shaderID, GL_LINK_STATUS, &status);
			if(status == 0) {
				GLsizei errorLogSize = 0;
				std::string errorLog;
				glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
				errorLog.resize(errorLogSize);
				glGetProgramInfoLog(shaderID, errorLogSize, &errorLogSize, &errorLog[0]);
				throw errorLog;
			}
		
	}catch(std::string error){
		Debug::Error(error, __FILE__, __LINE__);
		return false;
	}
	return true;
}

GLuint ShaderComponent::GetUniformID(std::string name) { 	
	auto id = uniformMap.find(name);
#ifdef _DEBUG
	if (id == uniformMap.end()) {
		///Debug::Error(name + " :No such uniform name", __FILE__, __LINE__);
		return -1;
	}
#endif 
	return id->second;
}

void ShaderComponent::SetUniformLocations(){
	int count;
	GLsizei actualLen;
	GLint size;
	GLenum type;
	char *name;
	int maxUniformListLength;
	unsigned int loc;
	char buffer[512];

	std::string activeUniformList = "Uniforms:\n";
	glGetProgramiv(shaderID, GL_ACTIVE_UNIFORMS, &count);
	sprintf_s(buffer,512,"There are %d active Uniforms\n",count);

	/// get the length of the longest named uniform 
	glGetProgramiv(shaderID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformListLength);

	/// Create a little buffer to hold the uniform's name - old C memory call just for fun 
	name = (char *)malloc(sizeof(char) * maxUniformListLength);

	
	for (int i = 0; i < count; ++i) {
		
		/// Get the name of the ith uniform
		
		glGetActiveUniform(shaderID, i, maxUniformListLength, &actualLen, &size, &type, name);
		/// Get the (unsigned int) loc for this uniform
		loc = glGetUniformLocation(shaderID, name);
		std::string uniformName = name;
		uniformMap[uniformName] = loc;

		sprintf_s(buffer,512,"\"%s\" loc:%d\n", uniformName.c_str() , uniformMap[uniformName]);
		activeUniformList += buffer;
	}
	Debug::Info(activeUniformList, __FILE__, __LINE__);
	free(name);
}

/// Read from a specified file and return a char array from the heap 
/// The memory must be deleted within this class. It may not be the best way 
/// to do this but it is all private and I did delete it! SSF
char* ShaderComponent::ReadTextFile(const char *filename){
	char* buffer = nullptr;
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {								/// Open the file
        file.seekg(0, std::ios::end);					/// goto the end of the file
        int bufferSize = (int) file.tellg();			/// Get the length of the file
		if(bufferSize == 0) {							/// If zero, bad file
			std::string errorMsg("Can't read shader file: ");
			std::string str2(filename);
			errorMsg += str2;
			throw errorMsg;								/// Bail out
		}
		buffer = new char[(int)(bufferSize + 1)];		/// Need to NULL terminate the array
        file.seekg(0, std::ios::beg);
        file.read(buffer, bufferSize);
		buffer[bufferSize] = NULL;						/// Add the NULL
		file.close();
    } else { 
		std::string errorMsg  ("Can't open shader file: ");
		errorMsg += filename;
		Debug::Error(errorMsg, __FILE__, __LINE__);
	}
	return buffer;
}

ShaderComponent::~ShaderComponent() {
	OnDestroy();
}

void ShaderComponent::OnDestroy() {
	glDetachShader(shaderID, fragShaderID);
	glDetachShader(shaderID, vertShaderID);  
	glDeleteShader(fragShaderID);
	glDeleteShader(vertShaderID);
	glDeleteProgram(shaderID);
	isCreated = false;
}

/// Not used, but must be made concrete
void ShaderComponent::Update(const float deltaTime) {}
void ShaderComponent::Render() const {}