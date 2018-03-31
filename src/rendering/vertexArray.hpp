#pragma once

#include "renderDevice.hpp"
#include "indexedModel.hpp"

class VertexArray
{
public:
	inline VertexArray(RenderDevice& deviceIn, const IndexedModel& model,
			enum RenderDevice::BufferUsage usage) :
		device(&deviceIn),
		deviceId(model.createVertexArray(deviceIn, usage)),
		numIndices(model.getNumIndices()) {}
	inline ~VertexArray()
	{
		deviceId = device->releaseVertexArray(deviceId);
	}

	inline void updateBuffer(uint32 bufferIndex, const void* data, uintptr dataSize);

	inline uint32 getId();
	inline uint32 getNumIndices();
private:
	RenderDevice* device;
	uint32 deviceId;
	uint32 numIndices;

	NULL_COPY_AND_ASSIGN(VertexArray);
};

inline uint32 VertexArray::getId()
{
	return deviceId;
}

inline uint32 VertexArray::getNumIndices()
{
	return numIndices;
}

inline void VertexArray::updateBuffer(uint32 bufferIndex,
		const void* data, uintptr dataSize)
{
	return device->updateVertexArrayBuffer(deviceId, bufferIndex, data, dataSize);
}

