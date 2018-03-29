#include <iostream>
#include "core/application.hpp"
#include "core/window.hpp"
#include "core/memory.hpp"
#include "math/transform.hpp"
#include "rendering/renderContext.hpp"

#include "timing.hpp"

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

static void testSphere()
{
	Sphere sphere1(Vector3f(0.0f, 0.0f, 0.0f), 1.0f);
	Sphere sphere2(Vector3f(0.0f, 3.0f, 0.0f), 1.0f);
	Sphere sphere3(Vector3f(0.0f, 0.0f, 2.0f), 1.0f);
	Sphere sphere4(Vector3f(1.0f, 0.0f, 0.0f), 1.0f);
	Sphere sphere5(Vector3f(1.0f, 0.0f, 0.0f), 2.0f);

	assert(!sphere1.intersects(sphere2));
	assert(!sphere1.intersects(sphere3,0.0f));
	assert(sphere1.intersects(sphere3));
	assert(sphere1.intersects(sphere4));
	assert(sphere1.intersects(sphere5));
	assert(sphere5.contains(sphere1));
	assert(!sphere1.contains(sphere5));
	assert(sphere1.contains(Vector3f(0.0f,1.0f,0.0f)));
	assert(!sphere1.contains(Vector3f(-1.1f,0.0f,0.0f)));
	assert(sphere1.moveTo(Vector3f(-1.0f,0.0f,0.0f)).contains(Vector3f(-1.1f,0.0f,0.0f)));

	Sphere superSphere = sphere1.addSphere(sphere2).addSphere(sphere3).addSphere(sphere4).addSphere(sphere5);
	assert(superSphere.contains(sphere1));
	assert(superSphere.contains(sphere2));
	assert(superSphere.contains(sphere3));
	assert(superSphere.contains(sphere4));
	assert(superSphere.contains(sphere5));
}

static void testAABB()
{
	AABB aabb1(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f));
	AABB aabb2(Vector3f(1.0f, 1.0f, 1.0f), Vector3f(2.0f, 2.0f, 2.0f));
	AABB aabb3(Vector3f(1.0f, 0.0f, 0.0f), Vector3f(2.0f, 1.0f, 1.0f));
	AABB aabb4(Vector3f(0.0f, 0.0f, -2.0f), Vector3f(1.0f, 1.0f, -1.0f));
	AABB aabb5(Vector3f(0.0f, 0.5f, 0.0f), Vector3f(1.0f, 1.5f, 1.0f));
	AABB aabb6(Vector3f(0.3f, 0.5f, 0.7f), Vector3f(1.3f, 1.5f, 1.7f));
	AABB aabb7(Vector3f(0.3f, 0.5f, 0.7f), Vector3f(0.5f, 0.7f, 0.9f));

	assert(aabb1.intersects(aabb2) == false);
	assert(aabb1.intersects(aabb2.translate(Vector3f(-0.5f))) == true);
	assert(aabb1.intersects(aabb2.translate(Vector3f(0.5f))) == false);
	assert(aabb1.intersects(aabb3) == false);
	assert(aabb1.intersects(aabb4) == false);
	assert(aabb1.intersects(aabb5) == true);
	assert(aabb1.intersects(aabb6) == true);

	assert(aabb1.intersects(aabb6.translate(Vector3f(1.0f))) == false);
	assert(aabb1.intersects(aabb6.translate(Vector3f(0.2f))) == true);
	assert(aabb1.contains(aabb1) == false);
	assert(aabb1.contains(aabb2) == false);
	assert(aabb1.contains(aabb3) == false);
	assert(aabb1.contains(aabb4) == false);
	assert(aabb1.contains(aabb5) == false);
	assert(aabb1.contains(aabb6) == false);
	assert(aabb1.contains(aabb7) == true);
	AABB aabb8(aabb1.overlap(aabb6));
	assert(aabb1.intersects(aabb8) == true);
	assert(aabb2.intersects(aabb8) == false);
	assert(aabb3.intersects(aabb8) == false);
	assert(aabb4.intersects(aabb8) == false);
	assert(aabb5.intersects(aabb8) == true);
	assert(aabb6.intersects(aabb8) == true);
	assert(aabb7.intersects(aabb8) == true);

	Transform transform(
			Vector3f(2.0f,1.0f,-1.0f),
			Quaternion(0.0f,0.0f,0.0f,1.0f),
			Vector3f(0.5f,2.0f,3.0f));
	AABB aabb1Transformed = aabb1.transform(transform.toMatrix());
	assert(Math::abs(aabb1Transformed.getCenter()[0]-2.25f) < 1.e-4f);
	assert(Math::abs(aabb1Transformed.getCenter()[1]-2.0f) < 1.e-4f);
	assert(Math::abs(aabb1Transformed.getCenter()[2]-0.5f) < 1.e-4f);
	assert(Math::abs(aabb1Transformed.getExtents()[0]-0.25f) < 1.e-4f);
	assert(Math::abs(aabb1Transformed.getExtents()[1]-1.0f) < 1.e-4f);
	assert(Math::abs(aabb1Transformed.getExtents()[2]-1.5f) < 1.e-4f);
}

static void testMath()
{
	Vector3f translation(5.64635f, 1.325345f, 2.02523f);
	Quaternion rotation(Vector3f(1.242f, 2.2432f, 3.75354f).normalized(), 2.54343f);
	Vector3f scale(1.4215f,0.123141f,3.7423f);
	Transform transform(
			translation,
			rotation,
			scale);
	Matrix transformMat(transform.toMatrix());

	Quaternion rot2(transformMat.getRotation());
	assert(scale.equals(Vector3f(transformMat.getScale())));
	assert(translation.equals(Vector3f(transformMat.getTranslation())));
	assert(rot2.equals(rotation));
	
	assert(Math::abs(transformMat.determinant4x4() - 0.655071f) < 1.e-4f);
	Matrix inverseMat = transformMat.inverse();
	Matrix shouldBeIdentity = inverseMat * transformMat;
	assert(shouldBeIdentity.equals(Matrix::identity()));

	Vector3f point(1.337f,3.778f,-2.419f);
	Vector3f point2(1.337f,3.778f,-2.419f);
	point = transformMat.transform(point.toVector(1.0f));
	point2 = transform.transform(point2.toVector(1.0f));
	assert(point.equals(point2));

	Vector3f point3(1.337f,3.778f,-2.419f);
	Vector3f point4(1.337f,3.778f,-2.419f);
	point3 = transformMat.inverse().transform(point3.toVector(1.0f));
	point4 = transform.inverse().transform(point4.toVector(1.0f));
	assert(point3.equals(point4));
	assert(Vector3f(transform.transform(Vector3f(transform.inverseTransform(point3, 1.0f)), 1.0f)).equals(point3));
	
	Matrix inverseMat2(transform.inverse());
	Matrix testResult(inverseMat2*transformMat);
	assert(inverseMat.equals(inverseMat2));

	Quaternion rotation2(Vector3f(13.242f, 22.2432f, 3.745354f).normalized(), -22.54343f);
	Quaternion mul = rotation*rotation2;
	mul = mul*rotation2.inverse();
	assert(mul.equals(rotation));
}

static void testPlane()
{
	Plane plane1(Vector3f(1.0f,0.0f,0.0f),-1.0f);
	Plane plane2(Vector3f(0.0f,1.0f,0.0f),-1.0f);
	Plane plane3(Vector3f(0.0f,0.0f,1.0f),-1.0f);
	assert(Math::abs(plane1.dot(Vector3f(2.0f,0.0f,0.0f))-1.0f) < 1.e-4f);
	assert(Math::abs(plane2.dot(Vector3f(2.0f,0.0f,0.0f))+1.0f) < 1.e-4f);
	assert(Math::abs(plane3.dot(Vector3f(2.0f,0.0f,0.0f))+1.0f) < 1.e-4f);
	Vector3f intersectionPoint;
	plane1.intersectPlanes(intersectionPoint,plane2,plane3);
	assert(Math::abs(plane1.dot(intersectionPoint)) < 1.e-4f);
	assert(Math::abs(plane2.dot(intersectionPoint)) < 1.e-4f);
	assert(Math::abs(plane3.dot(intersectionPoint)) < 1.e-4f);

	Plane plane4(Vector3f(0.13f,0.46f,0.89f).normalized(),-0.1f);
	Plane plane5(Vector3f(-0.74f,2.3f,-0.1f).normalized(),2.3f);
	Plane plane6(Vector3f(1.0f,-2.0f,10.0f).normalized(),-23.7f);
	plane4.intersectPlanes(intersectionPoint,plane5,plane6);
	assert(Math::abs(plane4.dot(intersectionPoint)) < 1.e-4f);
	assert(Math::abs(plane5.dot(intersectionPoint)) < 1.e-4f);
	assert(Math::abs(plane6.dot(intersectionPoint)) < 1.e-4f);

	Vector3f lineStart(0.0f,0.0f,0.0f);
	Vector3f lineEnd(2.0f,0.0f,0.0f);
	assert(Math::abs(plane1.intersectLine(lineStart,lineEnd)-0.5f) < 1.e-4);
	float amt = plane5.intersectLine(lineStart,lineEnd);
	intersectionPoint = lineStart+(lineEnd-lineStart)*amt;
	assert(Math::abs(plane5.dot(intersectionPoint)) < 1.e-4f);
	amt = plane6.intersectLine(lineStart,lineEnd);
	intersectionPoint = lineStart+(lineEnd-lineStart)*amt;
	assert(Math::abs(plane6.dot(intersectionPoint)) < 1.e-4f);

	Vector3f scale = Vector3f(0.4f,2.3f,1.7f);
	Transform transform(Vector3f(0.0f,0.0f,2.0f),
			Quaternion(Vector3f(0.0f,1.0f,0.0f),Math::toRadians(90.0f)),
		scale);
	Matrix transformMat = transform.toMatrix();

	Plane plane1Transformed = plane1.transform(transformMat);
	assert(Math::abs(plane1Transformed.dot(Vector3f(2.0f,0.0f,0.0f))-1.6f) < 1.e-4f);
}

static void testIntersects()
{
	Plane plane1(Vector3f(1.0f,0.0f,0.0f),0.0f);
	AABB aabb1(Vector3f(1.0f), Vector3f(2.0f));
	AABB aabb2(Vector3f(-2.0f), Vector3f(-1.0f));
	AABB aabb3(Vector3f(-1.0f), Vector3f(1.0f));
	bool isFullyInside;
	bool isPartiallyInside;
	
	Intersects::intersectPlaneAABB(aabb1, plane1, isFullyInside, isPartiallyInside);
	assert(isFullyInside && isPartiallyInside);
	Intersects::intersectPlaneAABB(aabb2, plane1, isFullyInside, isPartiallyInside);
	assert(!isFullyInside && !isPartiallyInside);
	Intersects::intersectPlaneAABB(aabb3, plane1, isFullyInside, isPartiallyInside);
	assert(!isFullyInside && isPartiallyInside);

	Sphere sphere1(Vector3f(1.0f), 0.5f);
	Sphere sphere2(Vector3f(-1.0f), 0.5f);
	Sphere sphere3(Vector3f(0.0f), 0.5f);

	Intersects::intersectPlaneSphere(sphere1,plane1,isFullyInside,isPartiallyInside);
	assert(isFullyInside && isPartiallyInside);
	Intersects::intersectPlaneSphere(sphere2,plane1,isFullyInside,isPartiallyInside);
	assert(!isFullyInside && !isPartiallyInside);
	Intersects::intersectPlaneSphere(sphere3,plane1,isFullyInside,isPartiallyInside);
	assert(!isFullyInside && isPartiallyInside);

	assert(Intersects::intersectSphereAABB(sphere1,aabb1));
	assert(!Intersects::intersectSphereAABB(sphere1,aabb2));
	assert(Intersects::intersectSphereAABB(sphere1,aabb3));
	assert(!Intersects::intersectSphereAABB(sphere2,aabb1));
	assert(Intersects::intersectSphereAABB(sphere2,aabb2));
	assert(Intersects::intersectSphereAABB(sphere2,aabb3));
	assert(!Intersects::intersectSphereAABB(sphere3,aabb1));
	assert(!Intersects::intersectSphereAABB(sphere3,aabb2));
	assert(Intersects::intersectSphereAABB(sphere3,aabb3));

	Sphere sphere4(Vector3f(0.0f,1.2f,0.0f), 0.5f);
	AABB aabb4(Vector3f(-100.0f,0.3f,0.2f),Vector3f(0.5f,0.6f,0.5f));
	assert(!Intersects::intersectSphereAABB(sphere4,aabb4));

	float p1,p2;
	assert(sphere3.intersectRay(Vector3f(0.0f,0.0f,-3.0f),Vector3f(0.0f,0.0f,1.0f),p1,p2));
	assert(Math::abs(p1-2.5f) < 1.e-4f);
	assert(Math::abs(p2-3.5f) < 1.e-4f);

	assert(sphere3.intersectRay(Vector3f(-0.5f,0.0f,-3.0f),Vector3f(0.0f,0.0f,1.0f),p1,p2));
	assert(Math::abs(p1-3.0f) < 1.e-4f);
	assert(Math::abs(p2-3.0f) < 1.e-4f);
	assert(!sphere3.intersectRay(Vector3f(0.6f,0.0f,-3.0f),Vector3f(0.0f,0.0f,1.0f),p1,p2));

	assert(sphere3.intersectLine(Vector3f(0.0f,0.0f,-3.0f),Vector3f(0.0f,0.0f,3.0f)));
	assert(sphere3.intersectLine(Vector3f(-0.4f,0.0f,-3.0f),Vector3f(-0.4f,0.0f,3.0f)));
	assert(sphere3.intersectLine(Vector3f(0.4f,0.0f,-3.0f),Vector3f(0.4f,0.0f,3.0f)));
	assert(!sphere3.intersectLine(Vector3f(0.6f,0.0f,-3.0f),Vector3f(0.6f,0.0f,3.0f)));
	assert(sphere3.intersectLine(Vector3f(0.5f,0.0f,-3.0f),Vector3f(0.5f,0.0f,3.0f)));

	assert(aabb3.intersectRay(Vector3f(0.0f,0.0f,-3.0f),Vector3f(0.0f,0.0f,1.0f),p1,p2));
	assert(Math::abs(p1-2.0f) < 1.e-4f);
	assert(Math::abs(p2-4.0f) < 1.e-4f);
	assert(aabb3.intersectRay(Vector3f(0.99f,0.0f,-3.0f),Vector3f(0.0f,0.0f,1.0f),p1,p2));
	assert(Math::abs(p1-2.0f) < 1.e-4f);
	assert(Math::abs(p2-4.0f) < 1.e-4f);
	assert(aabb3.intersectRay(Vector3f(1.0f,0.0f,-3.0f),Vector3f(0.0f,0.0f,1.0f),p1,p2));
	assert(!aabb3.intersectRay(Vector3f(1.01f,0.0f,-3.0f),Vector3f(0.0f,0.0f,1.0f),p1,p2));
	assert(!aabb3.intersectRay(Vector3f(2.0f,0.0f,-3.0f),Vector3f(0.0f,0.0f,1.0f),p1,p2));
	assert(aabb3.intersectRay(Vector3f(-0.5f,0.0f,-3.0f),Vector3f(0.0f,0.0f,1.0f),p1,p2));
	assert(Math::abs(p1-2.0f) < 1.e-4f);
	assert(Math::abs(p2-4.0f) < 1.e-4f);

	Vector3f points[] = {
		Vector3f(1.0f,1.0f,1.0f),
		Vector3f(1.0f,0.0f,1.0f),
		Vector3f(0.0f,1.0f,1.0f),
		Vector3f(0.0f,-1.0f,-1.0f),
		Vector3f(0.0f,0.0f,0.2f),
		Vector3f(0.7f,0.4f,0.3f),
		Vector3f(0.3f,-0.8f,0.4f),
		Vector3f(0.3f,-0.8f,-0.4f),
	};
	AABB boundingAABB(points, ARRAY_SIZE_IN_ELEMENTS(points));
	assert(boundingAABB.getMinExtents().equals(Vector3f(0.0f,-1.0f,-1.0f)));
	assert(boundingAABB.getMaxExtents().equals(Vector3f(1.0f,1.0f,1.0f)));

	Sphere boundingSphere(points, ARRAY_SIZE_IN_ELEMENTS(points));
	assert(boundingSphere.getCenter().equals(Vector3f(0.5f,0.0f,0.0f)));
	assert(Math::equals(boundingSphere.getRadius(), 1.5f, 1.e-4f));

	float points2[] = {
		1.0f,1.0f,1.0f,
		1.0f,0.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,-1.0f,-1.0f,
		0.0f,0.0f,0.2f,
		0.7f,0.4f,0.3f,
		0.3f,-0.8f,0.4f,
		0.3f,-0.8f,-0.4f,
	};

	boundingAABB = AABB(points2, ARRAY_SIZE_IN_ELEMENTS(points2)/3);
	assert(boundingAABB.getMinExtents().equals(Vector3f(0.0f,-1.0f,-1.0f)));
	assert(boundingAABB.getMaxExtents().equals(Vector3f(1.0f,1.0f,1.0f)));

	boundingSphere = Sphere(points2, ARRAY_SIZE_IN_ELEMENTS(points2)/3);
	assert(boundingSphere.getCenter().equals(Vector3f(0.5f,0.0f,0.0f)));
	assert(Math::equals(boundingSphere.getRadius(), 1.5f, 1.e-4f));
}

static void tests()
{
	testSphere();
	testAABB();
	testMath();
	testPlane();
	testIntersects();
}


// NOTE: Profiling reveals that in the current instanced rendering system:
// - Updating the buffer takes more time than
// - Calculating the transforms which takes more time than
// - Performing the instanced draw
static int runApp(Application* app)
{
	tests();
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

