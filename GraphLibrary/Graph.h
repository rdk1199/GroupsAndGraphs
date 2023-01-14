#pragma once

#include <vector>
#include <set>

#include "RandomNumberEngine/RandomNumberEngine.h"

using Edge = std::set<int>;

class DecomposedGraph;

class Graph
{

private:

	int n; //graph is presumed to be an undirected graph on n vertices, which are labelled 0,...,n-1
	int num_edges;

	std::vector<std::set<int>> adj; //adjacency lists; adj[i] is set of vertices adjacent to i

	std::vector<int> colors; // for internal use in algorithms (BFS, DFS, etc.); not the same as a "colored graph"

	void clear_colors(); //sets all entries of colors vector to 0	

public:

	Graph(int size); //creates a graph on size vertices with no edges
	Graph(const std::vector<std::set<int>>& adj_list); //creates a graph based on given adjacency list 

	int size() const { return n; }; //number of vertices

	bool insert_edge(int i, int j); //inserts edge between i and j, returning false if vertices do not exist or are the same and true otherwise
	void delete_edge(int i, int j); //deletes the edge between i and j, or does nothing if this is not possible for whatever reason

	bool const has_edge(int i, int j) const;
	bool const has_edge(Edge e) const;

	int const edges() const { return num_edges; }

	bool const is_connected();

	int degree(int i) const; 
	int max_degree() const;

	std::set<int> const con_comp(int i); //returns set of vertices which are connected to i; uses BFS
	std::vector<std::set<int>> const conn_components();

	static Graph gnp_random(int n, float p, RandomNumberEngine& rand_eng); //returns a graph drawn randomly according to the G(n,p) model; p is clamped to the range [0,1]

	void const print();

	std::vector<std::set<int>> const get_adj() const;
	
	const std::vector<std::set<int>>& adj_list() const {return adj;};

	bool operator==(const Graph& h);
};

struct ColoredSet
{
	std::set<int> set; //subset of 0...n-1, where n = size of colors vector
	std::vector<int> colors; //colors[i] = color of i
};

