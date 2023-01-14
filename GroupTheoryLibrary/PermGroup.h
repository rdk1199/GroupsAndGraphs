#pragma once

#include <set>
#include <vector>
#include "Permutation.h"

class PermGroup
{
private:

	int n; //subgroup of S_n
	int num_strong_gens;

	std::vector<Permutation> generators;

	std::vector<std::vector<Permutation>> strong_gens; //to be created by call to schreier_sims; 

	PermGroup(const std::vector<std::vector<Permutation>>& strong_generators); //constructs a group given its strong generators

	void schreier_sims();
	void fast_schreier_sims();

	std::pair<int, Permutation> filter(const Permutation& g);
	std::pair<int, Permutation> block_filter(const Permutation& g, const std::vector<std::vector<Permutation>>& H_strong_gens, const std::vector<std::set<int>>& B);

	//bool fixes_blocks(const Permutation& g, const std::vector<std::set<int>>& B); //B is a G-block system; returns whether g fixes all blocks in B or not
 
public:

	PermGroup(const std::vector<Permutation>& gens); 
	PermGroup(int size); //returns trivial group of identity permutation on n elements  

	bool contains(const Permutation& g); //returns true if group contains g, false otherwise; uses strong generating set

	std::vector<Permutation> factor(const Permutation& g); // returns {} if group does not contain g, returns factorization in canonical form otherwise (factors given in order) 

	unsigned long long int order();
	std::vector<unsigned long long int> order_factors();

	void print_strong_gens(); 
	void print_generators();

	std::vector<std::set<int>> compute_orbits(std::set<int> s); // assumes s is a subset of {0,1, ... n-1}; copies s for use in algorithm

	//std::set<int> get_orbit(int a); //returns orbit of a 
	//std::set<std::set<int>> get_orbit(const std::set<int>& s); // returns orbit of s (as a collection of (image) sets)

	template<class T>
	std::set<T> get_orbit(const T& a) //returns orbit of just a; a could be a collection of sets, sets of sets, etc.
	{
		std::set<T> orbit = { a };

		std::set<T> new_elements = { a };

		while (!new_elements.empty())
		{
			std::set<T> added_elts;

			for (T i : new_elements)
			{
				for (int j = 0; j < generators.size(); j++)
				{
					T new_elt = generators[j][i];

					if (!orbit.count(new_elt))
					{
						orbit.insert(new_elt);
						added_elts.insert(new_elt);
					}
				}

				/*
				for (int j = 0; j < strong_gens.size(); j++)
				{
					for (int k = 0; k < strong_gens[j].size(); k++)
					{
						
						T new_elt = strong_gens[j][k][i];

						if (!orbit.count(new_elt))
						{
							orbit.insert(new_elt);
							added_elts.insert(new_elt);
						}
						
					}
				}*/
			}

			new_elements.clear();
			new_elements.insert(added_elts.begin(), added_elts.end());
		}

		return orbit;
	}


	std::vector<std::set<int>> minimal_block_system(const std::set<int>& s); //s is a subset of {0,1, ... n-1}
	std::vector<std::set<int>> minimal_block_system(const std::vector<std::set<int>>& B); // B is a G-block system

	PermGroup block_stabilizer(const std::vector<std::set<int>>& B); // B is a G-block system; returns the subgroup of G which fixes all blocks; finds strong generators 
};

std::vector<Permutation> operator*(Permutation const& s, std::vector<Permutation> const& set); //returns set*s
std::vector<Permutation> operator*(const std::vector<Permutation>& set, Permutation const& s); // returns s*set


class InvalidGenerators : public std::exception
{
public:

	virtual char const* what() const throw()
	{
		return "Invalid generator set";
	}
};


struct Coset
{
	Permutation representative;
	PermGroup subgroup;
};