#include "PermGroup.h"

#include <iostream>
#include <map>

using std::set;
using std::vector;
using std::map;

PermGroup::PermGroup(const std::vector<Permutation>& gens) :
	generators(gens),
	strong_gens({}),
	num_strong_gens(0)
{
	if (gens.empty())
	{
		throw InvalidGenerators();
	}

	n = gens.begin()->size();

	for (auto i = gens.begin(); i != gens.end(); i++)
	{
		if (i->size() != n)
		{
			throw InvalidGenerators();
		}
	}
	 //fast_schreier_sims();
}

PermGroup::PermGroup(const std::vector<std::vector<Permutation>>& strong_generators):
	strong_gens(strong_generators),
	n(strong_generators[0][0].size())
{
	for (int i = 0; i < strong_generators.size(); i++)
	{
		for (int j = 0; j < strong_generators[i].size(); j++)
		{
			generators.push_back(strong_generators[i][j]);
		}
	}

	num_strong_gens = generators.size();
}

PermGroup::PermGroup(int size):
	n(size),
	generators({Permutation(size)}),
	strong_gens({}),
	num_strong_gens(0)
{
	//schreier_sims();
}

std::pair<int, Permutation> PermGroup::filter(const Permutation& g)
{
	Permutation gamma = g;

	for (int i = 0; i < n-1; i++)
	{
		bool found_rep = false; 

		for (int j = 0; j < strong_gens[i].size(); j++)
		{
			Permutation h = strong_gens[i][j];

			if (h[i] == gamma[i])
			{
				found_rep = true;
				gamma = h.inverse() * gamma;

				break;
			}
		}

		if (!found_rep)
		{
			return std::pair<int, Permutation>(i, gamma);
		}
	}
	return std::pair<int, Permutation>(n - 1, Permutation(n));
}

void PermGroup::schreier_sims()
{
	strong_gens.resize(n - 1);

	for (int i = 0; i < strong_gens.size(); i++)
	{
		strong_gens[i] = { Permutation(n) };
	}

	std::vector<Permutation> gens = generators;

	while (gens.size() > 0)
	{
		Permutation g = gens.back();

		gens.pop_back();

		std::pair<int, Permutation> new_gen = filter(g);

		if (new_gen.first != n - 1)
		{
			int i = new_gen.first;
			Permutation gamma = new_gen.second;

			strong_gens[i].push_back(gamma);
			num_strong_gens++;

			for (int j = 0; j <strong_gens.size(); j++)
			{
				std::vector<Permutation> product_set = strong_gens[j] * gamma;

				for (int k = 0; k < product_set.size(); k++)
				{
					gens.push_back(product_set[k]);
				}
			}

			for (int j = 0; j < strong_gens.size(); j++)
			{
				std::vector<Permutation> product_set = gamma * strong_gens[j];

				for (int k = 0; k < product_set.size(); k++)
				{
					gens.push_back(product_set[k]);
				}
			}
		}
	}
}

void PermGroup::fast_schreier_sims()
{
	strong_gens.resize(n - 1);

	for (int i = 0; i < strong_gens.size(); i++)
	{
		strong_gens[i] = { Permutation(n) };
	}

	std::set<Permutation> gens = {};

	gens.insert(generators.begin(), generators.end());

	while (gens.size() > 0)
	{
		Permutation g = *gens.begin();

		gens.erase(gens.begin());

		std::pair<int, Permutation> new_gen = filter(g);

		if (new_gen.first != n - 1)
		{
			int i = new_gen.first;
			Permutation gamma = new_gen.second;

			strong_gens[i].push_back(gamma);
			num_strong_gens++;

			for (int j = 0; j < strong_gens.size(); j++)
			{
				std::vector<Permutation> product_set = strong_gens[j] * gamma;

				for (int k = 0; k < product_set.size(); k++)
				{
					gens.insert(product_set[k]);
				}
			}

			for (int j = 0; j < strong_gens.size(); j++)
			{
				std::vector<Permutation> product_set = gamma * strong_gens[j];

				for (int k = 0; k < product_set.size(); k++)
				{
					gens.insert(product_set[k]);
				}
			}
		}
	}
}

void PermGroup::print_generators()
{
	for (int i = 0; i < generators.size(); i++)
	{
		generators[i].print();
		std::cout << ", \n";
	}
}

void PermGroup::print_strong_gens()
{

	if (strong_gens.size() == 0)
	{
		fast_schreier_sims();
	}

	for (int i = 0; i < strong_gens.size(); i++)
	{
		std::cout << "C_" << i << ":" << std::endl;

		for (int j = 0; j < strong_gens[i].size(); j++)
		{
			strong_gens[i][j].print();
			std::cout << ", \n";
		}
		std::cout << '\n';
	}
}

bool PermGroup::contains(const Permutation& g)
{
	if (strong_gens.size() == 0)
	{
		fast_schreier_sims();
	}

	return (filter(g).first == n - 1);
}

unsigned long long int PermGroup::order()
{
	if (strong_gens.size() == 0)
	{
		fast_schreier_sims();
	}

	unsigned long long int order = 1;

	
	for (int i = 0; i < strong_gens.size(); i++)
	{
		order *= strong_gens[i].size();
	}
	

	return order; 
}

std::vector<unsigned long long int> PermGroup::order_factors()
{
	if (strong_gens.size() == 0)
	{
		fast_schreier_sims();
	}

	std::vector<unsigned long long int> out;

	for (int i = 0; i < strong_gens.size(); i++)
	{
		out.push_back(strong_gens[i].size());
	}

	return out; 
}

std::vector<std::set<int>> PermGroup::compute_orbits(std::set<int> s) //Based on solution to Ex. B.2 (Helfgott)
{
	std::map<int, std::set<int>> gen_orbits;

	for (auto i = s.begin(); i != s.end(); i++)
	{

		std::set<int> gen_set = {*i};

		for (int j = 0; j < generators.size(); j++)
		{
			gen_set.insert(generators[j][*i]);
		}

		/*
		for (int j = 0; j < strong_gens.size(); j++)
		{
			for (int k = 0; k < strong_gens[j].size(); k++)
			{
				gen_set.insert(strong_gens[j][k][*i]);
			}
		}
		*/

		gen_orbits[*i] = gen_set;
	}

	std::vector<std::set<int>> out;

	while (!s.empty())
	{
		int root = *s.begin();

		s.erase(root);

		std::set<int> orbit = gen_orbits[root];

		std::set<int> unexamined = gen_orbits[root];

		unexamined.erase(root);

		while (!unexamined.empty())
		{
			int next = *unexamined.begin();

			unexamined.erase(next);
			s.erase(next);

			for(auto i = gen_orbits[next].begin(); i != gen_orbits[next].end(); i++)
			{
				if (!(orbit.count(*i)))
				{
					orbit.insert(*i);
					unexamined.insert(*i);
				}
			}
		}
		out.push_back(orbit);
	}

	return out; 
}
/*
std::set<int> PermGroup::get_orbit(int a)
{
	std::set<int> orbit = { a };

	std::set<int> new_elements = {a};

	while (!new_elements.empty())
	{
		std::set<int> added_elts;

		for (auto i : new_elements)
		{
			for (int j = 0; j < strong_gens.size(); j++)
			{
				for (int k = 0; k < strong_gens[j].size(); k++)
				{
					int new_elt = strong_gens[j][k][i];

					if (!orbit.count(new_elt))
					{
						orbit.insert(new_elt);
						added_elts.insert(new_elt);
					}

				}
			}

		}

		new_elements.clear();
		new_elements.insert(added_elts.begin(), added_elts.end());

	}

	return orbit; 

}

std::set<std::set<int>> PermGroup::get_orbit(const std::set<int>& s)
{
	std::set<std::set<int>> orbit = { s };

	std::set<std::set<int>> new_elements = { s };

	while (!new_elements.empty())
	{
		std::set<std::set<int>> added_elts;

		for (auto i : new_elements)
		{
			for (int j = 0; j < strong_gens.size(); j++)
			{
				for (int k = 0; k < strong_gens[j].size(); k++)
				{
					std::set<int> new_elt = strong_gens[j][k][i];

					if (!orbit.count(new_elt))
					{
						orbit.insert(new_elt);
						added_elts.insert(new_elt);
					}

				}
			}
		}

		new_elements.clear();
		new_elements.insert(added_elts.begin(), added_elts.end());
	}

	return orbit;
}
*/

std::vector<std::set<int>> PermGroup::minimal_block_system(const std::set<int>& s)
{
	std::vector<std::set<int>> initial_blocks;

	for (auto i : s)
	{
		initial_blocks.push_back({ i });
	}

	return minimal_block_system(initial_blocks);
}

std::vector<std::set<int>> PermGroup::minimal_block_system(const std::vector<std::set<int>>& B) 
{
	if (B.empty())
	{
		return {};
	}

	std::set<int> anchor = B[0]; //vertex i of graph will correspond to B[i]

	map<set<int>, int> vertex_map; //maps each block in B to its corresponding vertex in the graph G

	for (int i = 0; i < B.size(); i++)
	{
		vertex_map[B[i]] = i;
	}

	for (int i = 1; i < B.size(); i++)
	{
		Graph G(B.size());

		set<set<int>> start_edge = {B[0] , B[i]};
		set<set<set<int>>> edge_orbit = get_orbit(start_edge);

		for (auto j : edge_orbit) //insert each edge into the graph
		{
			auto k = j.begin();

			set<int> first = *k;
			k++;
			set<int> second = *k;
			
			G.insert_edge(vertex_map.at(first), vertex_map.at(second));
		}

		vector<set<int>> conn_comps = G.conn_components();

		if (conn_comps.size() > 1)
		{
			vector<set<int>> new_block_sys;

			for (int i = 0; i < conn_comps.size(); i++)
			{
				set<int> new_block;

				for (int j : conn_comps[i])
				{
					new_block.insert(B[j].begin(), B[j].end());
				}

				new_block_sys.push_back(new_block);
			}

			return minimal_block_system(new_block_sys);
		}
	}
	
	return B; //block system was already minimal
}

std::pair<int, Permutation> PermGroup::block_filter(const Permutation& g, const std::vector<std::vector<Permutation>>& current_strong_gens ,const std::vector<std::set<int>>& B)
{
	Permutation gamma = g;

	for (int i = 0; i < B.size(); i++)
	{
		bool found_H_rep = false;

		for (int j = 0; j < current_strong_gens[i].size(); j++)
		{
			Permutation h = current_strong_gens[i][j];

			if (h[B[i]] == gamma[B[i]])
			{
				found_H_rep = true;
				gamma = h.inverse() * gamma;

				break;
			}
		}

		if (!found_H_rep)
		{
			return std::pair<int, Permutation>(i, gamma);
		}
	}

	for (int i = B.size(); i < current_strong_gens.size(); i++)
	{
		bool found_rep = false;

		for (int j = 0; j < current_strong_gens[i].size(); j++)
		{
			Permutation h = current_strong_gens[i][j];

			if (h[i - B.size()] == gamma[i - B.size()])
			{
				found_rep = true;
				gamma = h.inverse() * gamma;
				break;
			}
		}

		if (!found_rep)
		{
			return std::pair<int, Permutation>(i, gamma);
		}
	}

	return std::pair<int, Permutation>(current_strong_gens.size(), Permutation(n)); //g was already a product of strong gens
}

PermGroup PermGroup::block_stabilizer(const std::vector<std::set<int>>& B) 
{
	//H will be the subgroup of G which fixes all blocks of B

	
	std::vector<std::vector<Permutation>> H_strong_gens(n + B.size() - 1);
	
	/*
	* m = B.size()
	* first m-1 entries contains coset reps of G_(i)/G_(i+1), where G_(i) stabilizes first i blocks
	* middle entry contains coset reps of something 
	* remaining n-1 entries contain strong generators of H
	*
	*
	*/ 

	for (int i = 0; i < H_strong_gens.size(); i++)
	{
		H_strong_gens[i] = { Permutation(n) };
	}

	std::set<Permutation> gens(generators.begin(), generators.end());

	while (gens.size() > 0)
	{
		Permutation g = *gens.begin();

		gens.erase(gens.begin());

		std::pair<int, Permutation> new_gen = block_filter(g, H_strong_gens, B);

		if (new_gen.first != H_strong_gens.size())
		{
			int i = new_gen.first;
			Permutation gamma = new_gen.second;

			H_strong_gens[i].push_back(gamma);

			for (int j = 0; j < H_strong_gens.size(); j++)
			{
				std::vector<Permutation> right_product_set = H_strong_gens[j] * gamma;

				std::vector<Permutation> left_product_set = gamma * H_strong_gens[j];

				for (int k = 0; k < right_product_set.size(); k++)
				{
					gens.insert(right_product_set[k]);
				}

				for (int k = 0; k < left_product_set.size(); k++)
				{
					gens.insert(left_product_set[k]);
				}
			}
		}
	}

	std::vector<std::vector<Permutation>> final_strong_gens;

	for (int i = B.size(); i < H_strong_gens.size(); i++)
	{
		final_strong_gens.push_back(H_strong_gens[i]);
	}

	return PermGroup(final_strong_gens);
}
