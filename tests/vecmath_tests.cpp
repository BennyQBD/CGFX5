#include "minunit.h"
#include "../src/math/vecmath.hpp"
#include "../src/math/math.hpp"
#include "../src/core/memory.hpp"

static const float errorMargin=1e-4f;

static void printVec(const Vector& vec)
{
	printf("%f\n", vec[0]);
	printf("%f\n", vec[1]);
	printf("%f\n", vec[2]);
	printf("%f\n", vec[3]);
}

const char* init_tests()
{
	Vector test(Vector::make(1.0f, 2.0f, 3.0f, 4.0f));
	mu_assert(test[0] == 1.0f, "Vector setup/getting failed");
	mu_assert(test[1] == 2.0f, "Vector setup/getting failed");
	mu_assert(test[2] == 3.0f, "Vector setup/getting failed");
	mu_assert(test[3] == 4.0f, "Vector setup/getting failed");

//	Vector equal = test == Vector::make(1.0f, 0.0f, 1.0f, 0.0f);
	mu_assert((test != test).isZero4f(), "Vector equals failed");
//	printf("%f\n", equal[0]);
//	printf("%f\n", equal[1]);
//	printf("%f\n", equal[2]);
//	printf("%f\n", equal[3]);
//	//mu_assert(equal[0] == , "Vector equals failed");
	return NULL;
}

const char* load_tests()
{
	Vector test(Vector::load1f(133.7f));
	mu_assert(test[0] == test[1], "Vector load1f failed");
	mu_assert(test[1] == test[2], "Vector load1f failed");
	mu_assert(test[2] == test[3], "Vector load1f failed");
	mu_assert(test[3] == 133.7f, "Vector load1f failed");

	float testVals[4] = { 3.0f, 4.0f, 5.0f, 8.6f };
	Vector test2(Vector::load3f(testVals, 0.9f));
	mu_assert(test2[0] == 3.0f, "Vector load3f failed");
	mu_assert(test2[1] == 4.0f, "Vector load3f failed");
	mu_assert(test2[2] == 5.0f, "Vector load3f failed");
	mu_assert(test2[3] == 0.9f, "Vector load3f failed");

	Vector test3(Vector::load4f(testVals));
	mu_assert(test3[0] == 3.0f, "Vector load4f failed");
	mu_assert(test3[1] == 4.0f, "Vector load4f failed");
	mu_assert(test3[2] == 5.0f, "Vector load4f failed");
	mu_assert(test3[3] == 8.6f, "Vector load4f failed");

//	float* alignedFloats = (float*)Memory::malloc(sizeof(float)*4, 16);
//	Memory::memcpy(alignedFloats, testVals, sizeof(float)*4);
//
//	Vector test4(Vector::loadAligned(alignedFloats));
//	mu_assert(test4[0] == 3.0f, "Vector loadAligned failed");
//	mu_assert(test4[1] == 4.0f, "Vector loadAligned failed");
//	mu_assert(test4[2] == 5.0f, "Vector loadAligned failed");
//	mu_assert(test4[3] == 8.6f, "Vector loadAligned failed");
//
//	Memory::free(alignedFloats);
	return NULL;
}

const char* store_tests()
{
	Vector test(Vector::make(8.7f, 9.2f, 1.6f, 4.9f));
	float out1[4];
	float out2[4];
	float out3[4];

	test.store1f(out1);
	mu_assert(out1[0] == 8.7f, "Vector store1f failed");

	test.store3f(out2);
	mu_assert(out2[0] == 8.7f, "Vector store3f failed");
	mu_assert(out2[1] == 9.2f, "Vector store3f failed");
	mu_assert(out2[2] == 1.6f, "Vector store3f failed");

	test.store4f(out3);
	mu_assert(out3[0] == 8.7f, "Vector store4f failed");
	mu_assert(out3[1] == 9.2f, "Vector store4f failed");
	mu_assert(out3[2] == 1.6f, "Vector store4f failed");
	mu_assert(out3[3] == 4.9f, "Vector store4f failed");

	return NULL;
}

const char* arrange_tests()
{
	Vector test(Vector::make(1.0f, 2.0f, 3.0f, 4.0f));
	Vector test2;
	test2 = test.replicate(0);
	mu_assert(test2[0] == 1.0f, "Vector replicate failed");
	mu_assert(test2[1] == 1.0f, "Vector replicate failed");
	mu_assert(test2[2] == 1.0f, "Vector replicate failed");
	mu_assert(test2[3] == 1.0f, "Vector replicate failed");
	test2 = test.replicate(1);
	mu_assert(test2[0] == 2.0f, "Vector replicate failed");
	mu_assert(test2[1] == 2.0f, "Vector replicate failed");
	mu_assert(test2[2] == 2.0f, "Vector replicate failed");
	mu_assert(test2[3] == 2.0f, "Vector replicate failed");
	test2 = test.replicate(2);
	mu_assert(test2[0] == 3.0f, "Vector replicate failed");
	mu_assert(test2[1] == 3.0f, "Vector replicate failed");
	mu_assert(test2[2] == 3.0f, "Vector replicate failed");
	mu_assert(test2[3] == 3.0f, "Vector replicate failed");
	test2 = test.replicate(3);
	mu_assert(test2[0] == 4.0f, "Vector replicate failed");
	mu_assert(test2[1] == 4.0f, "Vector replicate failed");
	mu_assert(test2[2] == 4.0f, "Vector replicate failed");
	mu_assert(test2[3] == 4.0f, "Vector replicate failed");

//	test2 = test.swizzle(2, 3, 0, 1);
//	mu_assert(test2[0] == 3.0f, "Vector swizzle failed");
//	mu_assert(test2[1] == 4.0f, "Vector swizzle failed");
//	mu_assert(test2[2] == 1.0f, "Vector swizzle failed");
//	mu_assert(test2[3] == 2.0f, "Vector swizzle failed");
//	test2 = test.swizzle(2, 3, 2, 3);
//	mu_assert(test2[0] == 3.0f, "Vector swizzle failed");
//	mu_assert(test2[1] == 4.0f, "Vector swizzle failed");
//	mu_assert(test2[2] == 3.0f, "Vector swizzle failed");
//	mu_assert(test2[3] == 4.0f, "Vector swizzle failed");
	return NULL;
}

const char* math_funcs_tests()
{
	Vector test(Vector::make(1.0f, 2.0f, 3.0f, 4.0f));
	Vector test2(Vector::make(-1.0f, 2.0f, -3.0f, 4.0f));
	Vector test3(Vector::make(1.0f, -2.0f, 3.0f, -4.0f));
	Vector test4(Vector::make(-1.0f, -2.0f, -3.0f, -4.0f));

	mu_assert((test2.abs() != test).isZero4f(), "Vector abs failed");
	mu_assert((test3.abs() != test).isZero4f(), "Vector abs failed");

	mu_assert((test2.min(test3) != test4).isZero4f(), "Vector min failed");
	mu_assert((test2.max(test3) != test).isZero4f(), "Vector max failed");
	mu_assert((test4.neg() != test).isZero4f(), "Vector neg failed");
	mu_assert((test4 != test.neg()).isZero4f(), "Vector neg failed");
	return NULL;
}

const char* dotcross_tests()
{
	Vector test(Vector::make(1.0f, 2.0f, 3.0f, 4.0f));
	Vector test2(Vector::make(3.0f, 4.0f, 5.0f, 6.0f));
	Vector cross(Vector::make(-2.0f, 4.0f, -2.0f, 0.0f));
	mu_assert(Math::equals(test.dot4(test2)[0], 50.0f, errorMargin), "Vector dot4 failed");
	mu_assert(Math::equals(test.dot3(test2)[0], 26.0f, errorMargin), "Vector dot3 failed");

	mu_assert(test.cross3(test2).notEquals(cross, errorMargin).isZero3f(), "Vector cross failed");
	return NULL;
}

const char* highermath_tests()
{
	Vector test(Vector::make(1.0f, 2.0f, 3.0f, 4.0f));
	Vector test2(Vector::make(2.7f, 2.7f, 9.0f, 2.7f));
	Vector powResult(Vector::make(1.0f, 6.49801917084988f,
				19683.0f, 42.2242531447326f));
	Vector rsqrtResult(Vector::make(1.0f, 0.707106781186547f,
				0.577350269189626f, 0.5f));
	Vector reciprocalResult(Vector::make(1.0f, 0.5f,
				0.333333333333333f, 0.25f));
	Vector sinResult(Vector::make(0.841470984807897f, 0.909297426825682f,
				0.141120008059867f, -0.756802495307928f));
	Vector cosResult(Vector::make(0.54030230586814f, -0.416146836547142f,
				-0.989992496600445f, -0.653643620863612f));
	Vector outSin, outCos;

	mu_assert(test.pow(test2).notEquals(powResult, errorMargin).isZero4f(),
			"Vector pow failed");

	mu_assert(test.rsqrt().notEquals(rsqrtResult, errorMargin).isZero4f(),
			"Vector rsqrt failed");
	
	mu_assert(test.reciprocal().notEquals(reciprocalResult, errorMargin).isZero4f(),
			"Vector reciprocal failed");
	test.sincos(&outSin, &outCos);
	mu_assert(outSin.notEquals(sinResult, errorMargin).isZero4f(),
			"Vector sin failed");
	mu_assert(outCos.notEquals(cosResult, errorMargin).isZero4f(),
			"Vector sin failed");

	return NULL;
}

const char* length_normal_tests()
{
	Vector test(Vector::make(1.0f, 2.0f, 3.0f, 4.0f));
	Vector one(Vector::load1f(1.0f));
	Vector rlen3Result(Vector::load1f(0.267261241912424f));
	Vector rlen4Result(Vector::load1f(0.182574185835055f));

	mu_assert(test.rlen3().notEquals(rlen3Result, errorMargin).isZero4f(),
			"Vector rlen3 failed");
	mu_assert(test.rlen4().notEquals(rlen4Result, errorMargin).isZero4f(),
			"Vector rlen4 failed");
	mu_assert(test.normalize4().rlen4().notEquals(one, errorMargin).isZero4f(),
			"Vector normalize4 failed");
	mu_assert(test.normalize3().rlen3().notEquals(one, errorMargin).isZero4f(),
			"Vector normalize3 failed");
	return NULL;
}

const char* all_tests()
{
	mu_suite_start();

	if(false) {
		Vector temp;
		printVec(temp);
	}

    mu_run_test(init_tests);
    mu_run_test(load_tests);
    mu_run_test(store_tests);
    mu_run_test(arrange_tests);
    mu_run_test(math_funcs_tests);
    mu_run_test(dotcross_tests);
    mu_run_test(highermath_tests);
    mu_run_test(length_normal_tests);

    return NULL;
}

RUN_TESTS(all_tests);
