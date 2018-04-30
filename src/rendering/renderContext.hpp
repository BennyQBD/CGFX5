#pragma once

#include "renderDevice.hpp"
#include "shader.hpp"
#include "vertexArray.hpp"
#include "renderTarget.hpp"

class RenderContext
{
public:
	inline RenderContext(RenderDevice& deviceIn, RenderTarget& targetIn) :
		device(&deviceIn),
		target(&targetIn) {}

	inline void clear(bool shouldClearColor, bool shouldClearDepth,
			bool shouldClearStencil, const Color& color, uint32 stencil);
	inline void clear(const Color& color, bool shouldClearDepth=false);
	inline void draw(Shader& shader, VertexArray& vertexArray, 
			const RenderDevice::DrawParams& drawParams, uint32 numInstances=1);
	inline void draw(Shader& shader, VertexArray& vertexArray, 
			const RenderDevice::DrawParams& drawParams, uint32 numInstances,
			uint32 numIndices);

private:
	RenderDevice* device;
	RenderTarget* target;
};

inline void RenderContext::draw(Shader& shader, VertexArray& vertexArray, 
			const RenderDevice::DrawParams& drawParams, uint32 numInstances)
{
	device->draw(target->getId(), shader.getId(), vertexArray.getId(),
			drawParams, numInstances, vertexArray.getNumIndices());
}

inline void RenderContext::draw(Shader& shader, VertexArray& vertexArray, 
			const RenderDevice::DrawParams& drawParams, uint32 numInstances,
			uint32 numIndices)
{
	device->draw(target->getId(), shader.getId(), vertexArray.getId(),
			drawParams, numInstances, numIndices);
}

inline void RenderContext::clear(bool shouldClearColor, bool shouldClearDepth,
		bool shouldClearStencil, const Color& color, uint32 stencil)
{
	device->clear(target->getId(), shouldClearColor, shouldClearDepth, shouldClearStencil,
			color, stencil);
}

inline void RenderContext::clear(const Color& color, bool shouldClearDepth)
{
	device->clear(target->getId(), true, shouldClearDepth, false, color, 0);
}

