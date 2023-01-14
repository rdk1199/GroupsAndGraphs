#include "RandomNumberEngine.h"

#include <ctime>

RandomNumberEngine::RandomNumberEngine() :
	rand(std::minstd_rand(time(NULL)))
{}

RandomNumberEngine::RandomNumberEngine(int seed) :
	rand(std::minstd_rand(seed))
{}

int RandomNumberEngine::random_int()
{
	return rand() % 2;
}

int RandomNumberEngine::random_int(int l, int u)
{
	if (l > u)
	{
		int tmp = l;
		l = u;
		u = tmp;
	}

	return l + rand() % (u - l + 1);
}

float RandomNumberEngine::random_float()
{
	return static_cast<float>(rand()) / static_cast<float>(rand.max());
}

float RandomNumberEngine::random_float(float l, float u)
{
	if (l > u)
	{
		float tmp = l;
		l = u;
		u = tmp;
	}

	return l + (u - l) * random_float();

}
