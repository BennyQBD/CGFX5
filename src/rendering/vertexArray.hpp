#pragma once

#include "renderDevice.hpp"
#include "indexedModel.hpp"

class VertexArray
{
public:
	FORCEINLINE VertexArray(RenderDevice& deviceIn, const IndexedModel& model,
			enum RenderDevice::BufferUsage usage) :
		device(&deviceIn),
		deviceId(model.createVertexArray(deviceIn, usage)),
		numIndices(model.getNumIndices()) {}
	FORCEINLINE ~VertexArray()
	{
		deviceId = device->releaseVertexArray(deviceId);
	}

	FORCEINLINE void updateBuffer(uint32 bufferIndex, const void* data, uintptr dataSize);

	FORCEINLINE uint32 getId();
	FORCEINLINE uint32 getNumIndices();
private:
	RenderDevice* device;
	uint32 deviceId;
	uint32 numIndices;

	NULL_COPY_AND_ASSIGN(VertexArray);
};

FORCEINLINE uint32 VertexArray::getId()
{
	return deviceId;
}

FORCEINLINE uint32 VertexArray::getNumIndices()
{
	return numIndices;
}

FORCEINLINE void VertexArray::updateBuffer(uint32 bufferIndex,
		const void* data, uintptr dataSize)
{
	return device->updateVertexArrayBuffer(deviceId, bufferIndex, data, dataSize);
}

