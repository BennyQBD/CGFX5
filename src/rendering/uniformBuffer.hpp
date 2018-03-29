#pragma once

#include "renderDevice.hpp"

class UniformBuffer
{
public:
	FORCEINLINE UniformBuffer(RenderDevice& deviceIn, uintptr dataSize,
		enum RenderDevice::BufferUsage usage, const void* data = nullptr) :
		device(&deviceIn), 
		deviceId(device->createUniformBuffer(data, dataSize, usage)),
		size(dataSize) {}
	FORCEINLINE ~UniformBuffer()
	{
		deviceId = device->releaseUniformBuffer(deviceId);
	}

	FORCEINLINE void update(const void* data, uintptr dataSize);
	FORCEINLINE void update(const void* data) { update(data, size); }
	
	FORCEINLINE uint32 getId();
private:
	RenderDevice* device;
	uint32 deviceId;
	uintptr size;

	NULL_COPY_AND_ASSIGN(UniformBuffer);
};

FORCEINLINE uint32 UniformBuffer::getId()
{
	return deviceId;
}

FORCEINLINE void UniformBuffer::update(const void* data, uintptr dataSize)
{
	device->updateUniformBuffer(deviceId, data, dataSize);
}

