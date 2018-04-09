#include <iostream>
#include "core/application.hpp"
#include "core/window.hpp"
#include "core/memory.hpp"
#include "math/transform.hpp"
#include "rendering/renderContext.hpp"

#include "timing.hpp"
#include "tests.hpp"

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
	Texture texture(device, bitmap, RenderDevice::FORMAT_RGB, true, true);
//	RenderTarget textureTarget(device, texture);
//	RenderContext textureContext(device, textureTarget);
//	textureContext.clear(Color::WHITE);

	String shaderText;
	StringFuncs::loadTextFileWithIncludes(shaderText, "./res/shaders/basicShader.glsl", "#include");
	Shader shader(device, shaderText);
	shader.setSampler("diffuse", texture, sampler, 0);
	
	Matrix perspective(Matrix::perspective(Math::toRadians(70.0f/2.0f),
				4.0f/3.0f, 0.1f, 1000.0f));
	float amt = 0.0f;
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
		transform.setRotation(Quaternion(Vector3f(1.0f, 1.0f, 1.0f).normalized(), amt*10.0f/11.0f));
		if(updateTimer >= 1.0/60.0) {
			for(uint32 i = 0; i < transformMatrixArray.size(); i++) {
				transform.setTranslation(Vector3f(
							(Math::randf() * randScaleX)-randScaleX/2.0f,
							(Math::randf() * randScaleY)-randScaleY/2.0f,
							randZ));
				transformMatrixArray[i] = (perspective * transform.toMatrix());
			}
			vertexArray.updateBuffer(2, &transformMatrixArray[0],
					transformMatrixArray.size() * sizeof(Matrix));

			updateTimer = 0.0;
		}
		
		context.clear(color);
		context.draw(shader, vertexArray, RenderDevice::PRIMITIVE_TRIANGLES, numInstances);

		window.present();

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

