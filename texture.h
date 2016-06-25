#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>

struct GpuTexture{
	unsigned int textureId = 666;
	void bind();
};

typedef unsigned char byte;

struct Texel{
	byte r,g,b,a;
};

struct CpuTexture{
	bool import(std::string filename);

	int sizeX, sizeY;
	std::vector<Texel> data;
	GpuTexture loadOnGPU();

	// TODO: un po' di image processing, maybe?
	void sharpen();
	void blur();
	void tuneHueSaturationBrightness();
	void turnIntoANormalMap();  // from a displacement map
};


#endif // TEXTURE_H
