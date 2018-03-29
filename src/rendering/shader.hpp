#pragma once

#include "renderDevice.hpp"
#include "uniformBuffer.hpp"
#include "texture.hpp"
#include "sampler.hpp"

class Shader
{
public:
	FORCEINLINE Shader(RenderDevice& deviceIn, const String& text) :
		device(&deviceIn),
		deviceId(device->createShaderProgram(text)) {}
	FORCEINLINE ~Shader()
	{
		deviceId = device->releaseShaderProgram(deviceId);
	}

	FORCEINLINE void setUniformBuffer(const String& name, UniformBuffer& buffer);
	FORCEINLINE void setSampler(const String& name, Texture& texture, Sampler& sampler,
			uint32 unit);
	FORCEINLINE uint32 getId();
private:
	RenderDevice* device;
	uint32 deviceId;

	NULL_COPY_AND_ASSIGN(Shader);
};

FORCEINLINE uint32 Shader::getId()
{
	return deviceId;
}

FORCEINLINE void Shader::setUniformBuffer(const String& name, UniformBuffer& buffer)
{
	device->setShaderUniformBuffer(deviceId, name, buffer.getId());
}

FORCEINLINE void Shader::setSampler(const String& name, Texture& texture, Sampler& sampler,
		uint32 unit)
{
	device->setShaderSampler(deviceId, name, texture.getId(), sampler.getId(), unit);
}

