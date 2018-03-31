#pragma once

#include "renderDevice.hpp"

class UniformBuffer
{
public:
	inline UniformBuffer(RenderDevice& deviceIn, uintptr dataSize,
		enum RenderDevice::BufferUsage usage, const void* data = nullptr) :
		device(&deviceIn), 
		deviceId(device->createUniformBuffer(data, dataSize, usage)),
		size(dataSize) {}
	inline ~UniformBuffer()
	{
		deviceId = device->releaseUniformBuffer(deviceId);
	}

	inline void update(const void* data, uintptr dataSize);
	inline void update(const void* data) { update(data, size); }
	
	inline uint32 getId();
private:
	RenderDevice* device;
	uint32 deviceId;
	uintptr size;

	NULL_COPY_AND_ASSIGN(UniformBuffer);
};

inline uint32 UniformBuffer::getId()
{
	return deviceId;
}

inline void UniformBuffer::update(const void* data, uintptr dataSize)
{
	device->updateUniformBuffer(deviceId, data, dataSize);
}

