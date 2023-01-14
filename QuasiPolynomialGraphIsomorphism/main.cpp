
#include <iostream>
#include "GroupTheoryLibrary/PermGroup.h"
#include <cstdlib>
#include <ctime>
#include <random>

#include "RandomNumberEngine/RandomNumberEngine.h"
#include "GraphLibrary/Graph.h"
#include "GraphLibrary/DecomposedGraph.h"

using std::cout;
using std::endl;

using std::vector;
using std::set;


int main()
{

	RandomNumberEngine engine;

	
	Graph g = Graph::gnp_random(1000, 0.01f, engine);

	while (!g.is_connected())
	{
		g = Graph::gnp_random(1000, 0.01f, engine);
	}

	g.print();

	Edge root_edge({ 0, *(g.adj_list()[0].begin()) });

	DecomposedGraph decomposition(g, root_edge);

	decomposition.print_decomposition();

	ColoredSetCollection test_collection = decomposition.construct_colored_sets(2);

	test_collection.print();
	

	/*
	std::vector<Permutation> generators;
	
	int test_size = 30;


	int num_gens = engine.random_int(2, 4);

	
	for (int i = 0; i < num_gens; i++)
	{
		Permutation new_gen = Permutation::rand_perm(test_size, engine);
		generators.push_back(new_gen);
		new_gen.print_cycles();
		cout << endl; 
	}

	generators.push_back(Permutation({ 29, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,19,20,21,22,23,24,25,26,27,28, }));

	cout << "finished generating random permutations" << endl;
	
	PermGroup G(generators);
	
	std::set<int> test = {};

	for (int i = 0; i < test_size; i++)
	{
		test.insert(i);
	}
	
	std::vector<std::set<int>> orbits = G.compute_orbits(test);
	

	cout << "orbits: " << endl;

	for (int i = 0; i < orbits.size(); i++)
	{
		cout << "{ ";
		for (auto j : orbits[i])
		{
			cout << j << ", ";
		}

		cout << "} " << endl;
	}

	

	std::set<int> test_set = { engine.random_int(0, test_size - 1), engine.random_int(0, test_size - 1) };

	std::set<std::set<int>> orbit = G.get_orbit(test_set);

	cout << "orbit of { ";
	
	for (auto i : test_set)
	{
		cout << i << ", ";
	}
	
	cout << "} :" << endl;
	
	cout << "{ ";

	for (auto i : orbit)
	{
		cout << "{ ";

		for (auto j : i)
		{
			cout << j << ", ";
		}

		cout << " }";
	}

	cout << "} " << endl;

	vector<set<int>> min_block_test = G.minimal_block_system(test);

	cout << "min blocks: " << endl;

	for (int i = 0; i < min_block_test.size(); i++)
	{
		
		cout << "{ ";

		for (int j : min_block_test[i])
		{
			cout << j << ", ";
		}

		cout << " }" << endl;
	}
	
	PermGroup stab_group = G.block_stabilizer(min_block_test);

	stab_group.print_strong_gens();
	*/


}
