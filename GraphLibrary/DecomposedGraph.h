#pragma once

#include <vector>
#include <map>

#include "Graph.h"

struct ColoredSetCollection;

class DecomposedGraph
{

private:

	std::vector<std::set<int>> vertices; // V(X_0) = empty, ... , V(X_{n-1})
	std::vector<std::set<Edge>> edges; // E(X_0) = empty, ... , E(X_{n-1})

	void decompose(const Graph& G, Edge root); // sets vertices and edge sets to decomposition of G around root edge 

public:

	const Graph& base_graph;

	DecomposedGraph(const Graph& G, Edge root_edge);

	void print_decomposition();

	ColoredSetCollection construct_colored_sets(int r); // returns colored subsets of X_r;

};

struct ColoredSetCollection
{
	std::set<int> base_set; //subset of 0,...,n-1
	int max_size; //set of all subsets of base_set of size max_size or less
	int max_color;

	std::map<std::set<int>, int> colors; //default color is 0

	void print();
};

struct ReductionMap
{
	ColoredSetCollection original_collection;

};

class InvalidDecomposition : public std::exception
{
public:

	virtual char const* what() const throw()
	{
		return "Invalid root edge given";
	}
};

