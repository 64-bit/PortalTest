#include "stdafx.h"
#include <immintrin.h>
#include <stdlib.h>
#include <random>
#include "Timer.h"

const int TEST_SIZE = 1024 * 1024 * 1;
const int Repeats = 1024;
const int ops = 10;

struct alignas(32) AlignedFloats
{
	float Data[16];
};

void ZeroOut(float* array, int count) 
{
	for (int i = 0; i < count; i += 1)
	{
		array[i] = 0.0f;
	}
}

void Test256(float* aIn, float* bIn, float* cIn, float* dIn) 
{
	__m256 b = _mm256_load_ps(&bIn[0]);

	for (int r = 0; r < Repeats; r++)
	{


		for (int i = 0; i < TEST_SIZE; i += 8)
		{
			__m256 a = _mm256_load_ps(&aIn[i]);
			__m256 c = _mm256_load_ps(&cIn[i]);
			__m256 d = _mm256_fmadd_ps(a, b, c);
			d = _mm256_fmadd_ps(a, b, d);
			d = _mm256_fmadd_ps(a, b, d);
			d = _mm256_fmadd_ps(a, b, d);

			d = _mm256_fmadd_ps(a, b, d);
			d = _mm256_fmadd_ps(a, b, d);

			_mm256_store_ps(&cIn[i], d);
		}
	}
}

void Test512(float* aIn, float* bIn, float* cIn, float* dIn)
{
	__m512 b = _mm512_load_ps(&bIn[0]);

	printf("f");

	for (int r = 0; r < Repeats; r++)
	{
		for (int i = 0; i < TEST_SIZE; i += 16)
		{
			float x = 2.0f;
			x *= (float)i;

			__m512 s = _mm512_set1_ps(x);


			__m512 a = _mm512_load_ps(&aIn[i]);
			__m512 c = _mm512_load_ps(&cIn[i]);
			__m512 d = _mm512_fmadd_ps(a, b, c);
			d = _mm512_fmadd_ps(a, b, d);
			d = _mm512_fmadd_ps(a, b, d);
			d = _mm512_fmadd_ps(a, b, d);
			d = _mm512_add_ps(d, s);

			_mm512_store_ps(&cIn[i], d);
		}
	}
}

void AVXTest() 
{


	float* a = (float*)_aligned_malloc(TEST_SIZE * sizeof(float), 64);
	float* b = (float*)_aligned_malloc(TEST_SIZE * sizeof(float), 64);
	float* c = (float*)_aligned_malloc(TEST_SIZE * sizeof(float), 64);
	float* d = (float*)_aligned_malloc(TEST_SIZE * sizeof(float), 64);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-1.0, 1.0);

	for (int i = 0; i < TEST_SIZE; i++)
	{
		a[i] = (float)dis(gen);
		b[i] = (float)dis(gen);
		c[i] = (float)dis(gen);
	}




	//Bench Regular
	EnigneTime regular = EnigneTime(false, 0);
	ZeroOut(c, TEST_SIZE);
	for (int r = 0; r < Repeats; r++)
	{
		float bb = b[0];
		for (int i = 0; i < TEST_SIZE; i++)
		{
			c[i] += a[i] * bb;
			c[i] += a[i] * bb;
		}
	}

	double regularDuration = regular.TimeSinceStartupDouble();

	float sumResult = 0.0f;
	for (int i = 0; i < TEST_SIZE; i++)
	{
		sumResult += c[i];
	}
	printf("Regular Sum: %f\n", sumResult);
	
	//Bench AVX 256
	ZeroOut(c, TEST_SIZE);
	EnigneTime t256 = EnigneTime(false, 0);
	Test256(a, b, c, d);
	double t256Time = t256.TimeSinceStartupDouble();

	sumResult = 0.0f;
	for (int i = 0; i < TEST_SIZE; i++)
	{
		sumResult += c[i];
	}
	printf("256 Sum: %f\n", sumResult);

	//Bench AVX 512
	ZeroOut(c, TEST_SIZE);
	EnigneTime t512 = EnigneTime(false, 0);
	Test512(a, b, c, d);
	double t512Time = t512.TimeSinceStartupDouble();

	sumResult = 0.0f;
	for (int i = 0; i < TEST_SIZE; i++)
	{
		sumResult += c[i];
	}
	printf("512 Sum: %f\n", sumResult);

	printf("Scalar Duration mS: %f\n", (float)(regularDuration * 1000.0));
	printf("Scalar GFlops mS: %f\n",(float)((Repeats * TEST_SIZE * 2.0 / regularDuration) / 1000000000.0));

	printf("\n\n");

	printf("256 Duration mS: %f\n", (float)(t256Time * 1000.0));
	printf("256 GFlops mS: %f\n", (float)((ops * TEST_SIZE / t256Time) / 1000000.0));

	printf("\n\n");

	printf("512 Duration mS: %f\n", (float)(t512Time * 1000.0));
	printf("512 GFlops mS: %f\n", (float)(((ops * TEST_SIZE) / t512Time) / 1000000.0));
}