#pragma once

#include <vector>
#include <set>
#include <exception>

#include "RandomNumberEngine/RandomNumberEngine.h"

#include "GraphLibrary/Graph.h"

class Permutation
{

private:

	int sz; //holds size of perm 
	std::vector<int> values; 

public:

	Permutation(std::vector<int> vals); 
	Permutation(int n); //constructs identity permutation on n elements. 

	int operator[](int i) const { return values[i]; }
	
	//std::set<int> operator[](const std::set<int>& s) const; //given a set s of integers, evaluates perm(s) (image of s)
	//std::vector<int> operator[](const std::vector<int>& v) const; //returns image of vector of ints


	template<class T>
	std::set<T> operator[](const std::set<T>& s) const //returns image of a set
	{
		std::set<T> out;

		for (auto i : s)
		{
			out.insert((*this)[i]);
		}
		return out; 
	}

	template<class T>
	std::vector<T> operator[](const std::vector<T>& v) const //returns image of a vector
	{
		std::vector<T> out;
		
		for (auto i : v)
		{
			out.push_back((*this)[i]);
		}

		return out; 
	}

	Graph operator[](const Graph& G) const; //returns image of graph

	 int size() const { return sz; }
	
	Permutation inverse() const;

	void print() const;
	void print_cycles() const; //prints cycle decomposition of permutation

	static Permutation rand_perm(int n, RandomNumberEngine& rand_eng) ; //returns a u.a.r selected permutation of size n, assuming rand returns a uniform integer

	bool fixes_blocks(const std::vector<std::set<int>>& B); //returns true if perm fixes each set in B
};

Permutation operator*(Permutation const& s, Permutation const& t); //returns composition st

bool operator<(Permutation const& s, Permutation const& t); //lexicographical order.
bool operator==(Permutation const& s, Permutation const& t);

class InvalidPermutation : public std::exception
{
public:

	virtual char const* what() const throw()
	{
		return "Attempted to create invalid permutation";
	}
};

class InvalidPermOperation : public std::exception
{
public:
	virtual char const* what() const throw()
	{
		return "Attempted invalid permutation operation";
	}
};