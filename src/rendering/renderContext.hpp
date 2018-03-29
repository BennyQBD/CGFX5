#pragma once

#include "renderDevice.hpp"
#include "shader.hpp"
#include "vertexArray.hpp"
#include "renderTarget.hpp"

class RenderContext
{
public:
	FORCEINLINE RenderContext(RenderDevice& deviceIn, RenderTarget& targetIn) :
		device(&deviceIn),
		target(&targetIn) {}

	FORCEINLINE void clear(bool shouldClearColor, bool shouldClearDepth,
			bool shouldClearStencil, const Color& color, uint32 stencil);
	FORCEINLINE void clear(const Color& color, bool shouldClearDepth=false);
	FORCEINLINE void draw(Shader& shader, VertexArray& vertexArray, 
			enum RenderDevice::PrimitiveType primitive, uint32 numInstances=1);
	FORCEINLINE void draw(Shader& shader, VertexArray& vertexArray, 
			enum RenderDevice::PrimitiveType primitive, uint32 numInstances,
			uint32 numIndices);

private:
	RenderDevice* device;
	RenderTarget* target;
};

FORCEINLINE void RenderContext::draw(Shader& shader, VertexArray& vertexArray, 
			enum RenderDevice::PrimitiveType primitive, uint32 numInstances)
{
	device->draw(target->getId(), shader.getId(), vertexArray.getId(),
			primitive, numInstances, vertexArray.getNumIndices());
}

FORCEINLINE void RenderContext::draw(Shader& shader, VertexArray& vertexArray, 
			enum RenderDevice::PrimitiveType primitive, uint32 numInstances,
			uint32 numIndices)
{
	device->draw(target->getId(), shader.getId(), vertexArray.getId(),
			primitive, numInstances, numIndices);
}

FORCEINLINE void RenderContext::clear(bool shouldClearColor, bool shouldClearDepth,
		bool shouldClearStencil, const Color& color, uint32 stencil)
{
	device->clear(target->getId(), shouldClearColor, shouldClearDepth, shouldClearStencil,
			color, stencil);
}

FORCEINLINE void RenderContext::clear(const Color& color, bool shouldClearDepth)
{
	device->clear(target->getId(), true, shouldClearDepth, false, color, 0);
}

