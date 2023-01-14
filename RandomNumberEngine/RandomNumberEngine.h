#pragma once


#include <random>


class RandomNumberEngine
{

private:

	std::minstd_rand rand;
	

public:

	RandomNumberEngine(); //seeds rng with time
	RandomNumberEngine(int seed);

	int random_int(); //returns 0 or 1 with probability 50%
	int random_int(int l, int u); //uniform random integer in range [l, u]
	
	float random_float(); //returns uniform float in [0,1]
	float random_float(float l, float u); //uniform random float in range [l,u]


};

