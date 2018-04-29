#pragma once
#include "core/common.hpp"

class DDSTexture
{
public:
	DDSTexture() : buffer(nullptr) {}
	virtual ~DDSTexture();

	bool load(const char* fileName);
	inline uint32 getMipMapCount() const {
		return mipMapCount;
	}

	inline uint32 getFourCC() const {
		return fourCC;
	}

	inline uint32 getWidth() const {
		return width;
	}

	inline uint32 getHeight() const {
		return height;
	}

	inline const unsigned char* getBuffer()  const {
		return buffer;
	}
private:
	unsigned char* buffer;
	uint32 height;
	uint32 width;
	uint32 mipMapCount;
	uint32 fourCC;
	
	void cleanup();
	NULL_COPY_AND_ASSIGN(DDSTexture);
};

