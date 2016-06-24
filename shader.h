#ifndef SHADER_H
#define SHADER_H

#include <string>

//typedef unsigned int uint;

class Shader
{
	bool loadSourcesFromFile(const std::string fileVert, const std::string fileFrag );
	void build(); // compile and link
	void use();
	static void useFixed();
public:
	Shader();
	std::string sourceVert, sourceFrag;
	int programId;
	int fragId, vertId;
	bool loadFile(const std::string& fileName, std::string& fileContent);
	bool compile(int shadId, std::string& source);
	bool isReady;

};

#endif // SHADER_H
