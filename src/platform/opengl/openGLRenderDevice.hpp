#pragma once

#include "../../core/window.hpp"
#include "../../math/color.hpp"
#include "../../dataStructures/map.hpp"
#include <SDL2/SDL.h>
#include <GL/glew.h>

typedef SDL_GLContext DeviceContext;

class OpenGLRenderDevice
{
public:
	enum BufferUsage
	{
		USAGE_STATIC_DRAW = GL_STATIC_DRAW,
		USAGE_STREAM_DRAW = GL_STREAM_DRAW,
		USAGE_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,

		USAGE_STATIC_COPY = GL_STATIC_COPY,
		USAGE_STREAM_COPY = GL_STREAM_COPY,
		USAGE_DYNAMIC_COPY = GL_DYNAMIC_COPY,

		USAGE_STATIC_READ = GL_STATIC_READ,
		USAGE_STREAM_READ = GL_STREAM_READ,
		USAGE_DYNAMIC_READ = GL_DYNAMIC_READ,
	};

	enum SamplerFilter
	{
		FILTER_NEAREST = GL_NEAREST,
		FILTER_LINEAR = GL_LINEAR,
		FILTER_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		FILTER_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
		FILTER_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		FILTER_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
	};

	enum SamplerWrapMode
	{
		WRAP_CLAMP = GL_CLAMP_TO_EDGE,
		WRAP_REPEAT = GL_REPEAT,
		WRAP_CLAMP_MIRROR = GL_MIRROR_CLAMP_TO_EDGE,
		WRAP_REPEAT_MIRROR = GL_MIRRORED_REPEAT,
	};

	enum PixelFormat
	{
		FORMAT_R,
		FORMAT_RG,
		FORMAT_RGB,
		FORMAT_RGBA,
		FORMAT_DEPTH,
		FORMAT_DEPTH_AND_STENCIL,
	};

	enum PrimitiveType
	{
		PRIMITIVE_TRIANGLES = GL_TRIANGLES,
		PRIMITIVE_POINTS = GL_POINTS,
		PRIMITIVE_LINE_STRIP = GL_LINE_STRIP,
		PRIMITIVE_LINE_LOOP = GL_LINE_LOOP,
		PRIMITIVE_LINES = GL_LINES,
		PRIMITIVE_LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
		PRIMITIVE_LINES_ADJACENCY = GL_LINES_ADJACENCY,
		PRIMITIVE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		PRIMITIVE_TRIANGLE_FAN = GL_TRIANGLE_FAN,
		PRIMITIVE_TRAINGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
		PRIMITIVE_TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
		PRIMITIVE_PATCHES = GL_PATCHES,
	};

	enum FramebufferAttachment
	{
		ATTACHMENT_COLOR = GL_COLOR_ATTACHMENT0,
		ATTACHMENT_DEPTH = GL_DEPTH_ATTACHMENT,
		ATTACHMENT_STENCIL = GL_STENCIL_ATTACHMENT,
	};
	
	static void globalInit();
	OpenGLRenderDevice(Window& window);
	virtual ~OpenGLRenderDevice();

	// TODO: Finish this!
	// NOTE: More detailed comments about what to do and how to do it in the cpp file
	// - Set scissor rect?
	// - Depth stencil!
	// - Blend states?

	uint32 createRenderTarget(uint32 texture, int32 width, int32 height,
			enum FramebufferAttachment attachment, uint32
			attachmentNumber, uint32 mipLevel);
	uint32 releaseRenderTarget(uint32 fbo);

	uint32 createVertexArray(const float** vertexData, const uint32* vertexElementSizes,
			uint32 numVertexComponents, uint32 numInstanceComponents,
			uint32 numVertices, const uint32* indices,
			uint32 numIndices, enum BufferUsage usage);
	void updateVertexArrayBuffer(uint32 vao, uint32 bufferIndex,
			const void* data, uintptr dataSize);
	uint32 releaseVertexArray(uint32 vao);

	uint32 createSampler(enum SamplerFilter minFilter, enum SamplerFilter magFilter,
			enum SamplerWrapMode wrapU, enum SamplerWrapMode wrapV, float anisotropy);
	uint32 releaseSampler(uint32 sampler);

	uint32 createTexture2D(int32 width, int32 height, const void* data,
			enum PixelFormat dataFormat, enum PixelFormat internalFormat,
			bool generateMipmaps, bool compress);
	uint32 releaseTexture2D(uint32 texture2D);

	uint32 createUniformBuffer(const void* data, uintptr dataSize, enum BufferUsage usage);
	void updateUniformBuffer(uint32 buffer, const void* data, uintptr dataSize);
	uint32 releaseUniformBuffer(uint32 buffer);

	uint32 createShaderProgram(const String& shaderText);
	void setShaderUniformBuffer(uint32 shader, const String& uniformBufferName,
			uint32 buffer);
	void setShaderSampler(uint32 shader, const String& samplerName,
		uint32 texture, uint32 sampler, uint32 unit);
	uint32 releaseShaderProgram(uint32 shader);

	void clear(uint32 fbo,
			bool shouldClearColor, bool shouldClearDepth, bool shouldClearStencil,
			const Color& color, uint32 stencil);
	void draw(uint32 fbo, uint32 shader, uint32 vao, enum PrimitiveType primitiveType,
			uint32 numInstances, uint32 numElements);
private:
	struct VertexArray
	{
		uint32* buffers;
		uintptr* bufferSizes;
		uint32  numBuffers;
		uint32  numElements;
		uint32  instanceComponentsStartIndex;
		enum BufferUsage usage;
	};

	struct ShaderProgram
	{
		Array<uint32>      shaders;
		Map<String, int32> uniformMap;
		Map<String, int32> samplerMap;
	};

	struct FBOData
	{
		int32 width;
		int32 height;
	};

	static bool isInitialized;
	DeviceContext context;
	String shaderVersion;
	uint32 version;
	Map<uint32, VertexArray> vaoMap;
	Map<uint32, FBOData> fboMap;
	Map<uint32, ShaderProgram> shaderProgramMap;

	uint32 boundFBO;
	uint32 viewportFBO;
	uint32 boundVAO;
	uint32 boundShader;

	void setFBO(uint32 fbo);
	void setViewport(uint32 fbo);
	void setVAO(uint32 vao);
	void setShader(uint32 shader);

	uint32 getVersion();
	String getShaderVersion();
};
