#include "ddstexture.hpp"
#include "core/memory.hpp"

DDSTexture::~DDSTexture() {
	cleanup();
}

bool DDSTexture::load(const char* fileName) {
	unsigned char header[124];
	FILE* fp = fopen(fileName, "rb");
	if (fp == NULL) {
		return false;
	}

	// Verify valid DDS file
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return false;
	}

	// Read header
	fread(&header, ARRAY_SIZE_IN_ELEMENTS(header), 1, fp); 
	height      = *(uint32*)&(header[8 ]);
	width       = *(uint32*)&(header[12]);
	uint32 linearSize  = *(uint32*)&(header[16]);
	mipMapCount = *(uint32*)&(header[24]);
	fourCC      = *(uint32*)&(header[80]);

	// Allocate memory for DDS file
	uint32 bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)Memory::malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	fclose(fp);
	
	return true;
}

void DDSTexture::cleanup() {
	if(buffer != nullptr) {
		Memory::free(buffer);
	}
	buffer = nullptr;

}
