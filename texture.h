#ifndef TEXTURE_H
#define TEXTURE_H

/* class Texture:
 *
 * an asset for a texure map.
 *
 */
#include <string>
#include <vector>

struct GpuTexture{
	unsigned int textureId = 666;
	void bind() const;
};

typedef unsigned char byte;

struct Texel{
	byte r,g,b,a;
};

struct CpuTexture{
	bool import(std::string filename);

	int sizeX, sizeY;
	std::vector<Texel> data;
	GpuTexture uploadToGPU() const;

	// procedura creation of textures!
	void createRandom(int size);

	// TODO: un po' di image processing, maybe?
	void sharpen();
	void blur();
	void tuneHueSaturationBrightness();
	void turnIntoANormalMap();  // from a displacement map
};


#endif // TEXTURE_H
