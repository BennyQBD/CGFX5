#include <iostream>
#include "core/application.hpp"
#include "core/window.hpp"
#include "core/memory.hpp"
#include "math/transform.hpp"
#include "rendering/renderContext.hpp"

#include "timing.hpp"
#include "tests.hpp"

static String shaderText = String("#if defined(VS_BUILD)\n\
attribute vec3 position;\n\
attribute vec2 texCoord;\n\
attribute mat4 transformMat;\n\
out vec2 out_texCoord;\n\
void main()\n\
{\n\
	out_texCoord = texCoord;\n\
    gl_Position = vec4(position, 1.0) * transformMat;\n\
}\n\
#elif defined(FS_BUILD)\n\
in vec2 out_texCoord; \n\
out vec4 fragColor; \n\
\n\
uniform sampler2D texSamplerData;\n\
\n\
void main()\n\
{\n\
	fragColor = texture(texSamplerData, out_texCoord);\n\
//	fragColor = vec4(1.0, 0.0, 0.0, 0.0);\n\
}\n\
#endif\n");

#include "math/sphere.hpp"
#include "math/aabb.hpp"
#include "math/plane.hpp"
#include "math/intersects.hpp"

// NOTE: Profiling reveals that in the current instanced rendering system:
// - Updating the buffer takes more time than
// - Calculating the transforms which takes more time than
// - Performing the instanced draw
static int runApp(Application* app)
{
	Tests::runTests();
	Window window(*app, 800, 600, "My Window!");
	RenderDevice device(window);
	RenderTarget target(device);
	RenderContext context(device, target);

	IndexedModel model;
	model.allocateElement(3); // Positions
	model.allocateElement(2); // TexCoords
	model.setInstancedElementStartIndex(2); // Begin instanced data
	model.allocateElement(16); // Transform matrix
	
	model.addElement3f(0, -0.5f, -0.5f, 0.0f);
	model.addElement3f(0, 0.0f, 0.5f, 0.0f);
	model.addElement3f(0, 0.5f, -0.5f, 0.0f);
	model.addElement2f(1, 0.0f, 0.0f);
	model.addElement2f(1, 0.5f, 1.0f);
	model.addElement2f(1, 1.0f, 0.0f);

	model.addIndices3i(0, 1, 2);
	VertexArray vertexArray(device, model, RenderDevice::USAGE_STATIC_DRAW);
	Sampler sampler(device, RenderDevice::FILTER_NEAREST_MIPMAP_LINEAR);
	ArrayBitmap bitmap;
	if(!bitmap.load("./res/textures/bricks.jpg")) {
		DEBUG_LOG("Main", LOG_ERROR, "Could not load texture!");
		return 1;
	}
	Texture texture(device, bitmap, RenderDevice::FORMAT_RGB, true, false);
//	RenderTarget textureTarget(device, texture);
//	RenderContext textureContext(device, textureTarget);
//	textureContext.clear(Color::WHITE);

	Shader shader(device, shaderText);
	shader.setSampler("texSamplerData", texture, sampler, 0);

	
	
//	UniformBuffer uniformBuffer(device, sizeof(transformMatrix),
//			RenderDevice::USAGE_DYNAMIC_DRAW);

	
	Matrix perspective(Matrix::perspective(Math::toRadians(70.0f/2.0f),
				4.0f/3.0f, 0.1f, 1000.0f));
	float amt = 0.0f;
//	float amtX = 0.0f;
//	float amtY = 0.0f;
	Color color(0.0f, 0.15f, 0.3f);
	float randZ = 10.0f;
	float randScaleX = randZ * window.getWidth()/(float)window.getHeight();
	float randScaleY = randZ;
	
	uint32 numInstances = 1000;
	Matrix transformMatrix(Matrix::identity());
	Transform transform;
	Array<Matrix> transformMatrixArray(numInstances);
	for(uint32 i = 0; i < numInstances; i++) {
		transformMatrixArray.push_back(Matrix::identity());
	}

	uint32 fps = 0;
	double lastTime = Time::getTime();
	double fpsTimeCounter = 0.0;
	double updateTimer = 1.0;
	while(app->isRunning()) {
		double currentTime = Time::getTime();
		double passedTime = currentTime - lastTime;
		lastTime = currentTime;
		fpsTimeCounter += passedTime;
		updateTimer += passedTime;
		if(fpsTimeCounter >= 1.0) {
			double msPerFrame = 1000.0/(double)fps;
			DEBUG_LOG("FPS", "NONE", "%f ms (%d fps)", msPerFrame, fps);
			fpsTimeCounter = 0;
			fps = 0;
		}
		fps++;

		app->processMessages(passedTime);
		//transform.setScale(Vector3f((Math::abs(Math::sin(amt)/4.0f)+0.75f)));
		//transform.setTranslation(Vector3f(0.0f, Math::cos(amt*6.0f/7.0f)/2.0f, 1.0f));
		//transform.setTranslation(Vector3f(0.0f, 0.0f, Math::cos(amt*6.0f/7.0f)/2.0f) + 1.0f);
//		transform.setTranslation(Vector3f(0.0f, 0.0f, -Math::cos(amt/4)*4 + 4.0f));
		transform.setRotation(Quaternion(Vector3f(1.0f, 1.0f, 1.0f).normalized(), amt*10.0f/11.0f));
		if(updateTimer >= 1.0/60.0) {
			for(uint32 i = 0; i < transformMatrixArray.size(); i++) {
				transform.setTranslation(Vector3f(
//							amtX, amtY,
							(Math::randf() * randScaleX)-randScaleX/2.0f,
							(Math::randf() * randScaleY)-randScaleY/2.0f,
							randZ));
				transformMatrixArray[i] = (perspective * transform.toMatrix());
//				amtX += 12.3357f;
//				amtX = Math::fmod(amtX + randScaleX, randScaleX*2) - randScaleX;
//				amtY += 0.12313f;
//				amtY = Math::fmod(amtY + randScaleY, randScaleY*2) - randScaleY;
			}
			vertexArray.updateBuffer(2, &transformMatrixArray[0],
					transformMatrixArray.size() * sizeof(Matrix));

			updateTimer = 0.0;
		}
		
//		uniformBuffer.update(&transformMatrix);
//		shader.setUniformBuffer("transformData", uniformBuffer);

		context.clear(color);
		context.draw(shader, vertexArray, RenderDevice::PRIMITIVE_TRIANGLES, numInstances);

		window.present();

		//
		amt += (float)passedTime/2.0f;
	}
	return 0;
}

int main(int argc, char** argv)
{
	Application* app = Application::create();
	int result = runApp(app);
	delete app;
	return result;
}

