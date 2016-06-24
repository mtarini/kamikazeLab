
#include <string>
#include <fstream>
#include <streambuf>
#include <GL/glew.h>

#include "shader.h"

#define NOT_READY 666

Shader::Shader()
{
	isReady = false;
	programId = vertId = fragId = NOT_READY;
}

bool Shader::loadSourcesFromFile(const std::string fileVert, const std::string fileFrag){
	bool res = true;
	res &= loadFile(fileVert, sourceVert);
	res &= loadFile(fileFrag, sourceFrag);
	isReady = false;
	return res;
}

bool Shader::compile(int shadId, std::string &source){
	glShaderSource(shadId, 1, source.c_str(), source.length() );

	int compiled = 0;
	glGetObjectParameteriv(shadId, GL_COMPILE_STATUS, &compiled);

	return (compiled == 1);
}

bool Shader::loadFile(const std::string &fileName, std::string &fileContent){
	std::ifstream t(fileName);
	fileContent = std::string((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>() );
	return !fileContent.empty();
}

bool Shader::build(){
	if (vertId == NOT_READY) vertId = glCreateShader(GL_VERTEX_SHADER);
	if (fragId == NOT_READY) fragId = glCreateShader(GL_FRAGMENT_SHADER);

	compile( vertId, sourceVert );

	if (fragId == NOT_READY) fragId = glCreateShader(GL_FRAGMENT_SHADER);

}
