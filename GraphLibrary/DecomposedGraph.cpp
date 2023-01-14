
#include "DecomposedGraph.h"

#include <iostream>
#include <cmath>
#include <map>
#include <algorithm>
#include <iterator>

using std::set;
using std::vector;
using std::map;

using std::cout;
using std::endl;

void DecomposedGraph::decompose(const Graph& G, Edge e)
{
	if (!G.has_edge(e) || e.size() != 2)
	{
		throw InvalidDecomposition();
	}

	int n = G.size();

	set<int> v_visited;
	set<Edge> e_visited;

	vertices.resize(n);
	edges.resize(n);

	vertices[0] = {};
	edges[0] = {};

	vertices[1] = e;
	edges[1] = { e };

	e_visited.insert(e);

	v_visited = e;
	
	for (int i = 2; i < n; i++)
	{
		for (int j : vertices[i - 1]) //for each vertex j in X_{i-1}
		{
			for (int k : G.adj_list()[j]) //for each vertex adjacent to j
			{
				Edge new_edge({ j, k });

				if (!e_visited.count(new_edge))
				{
					edges[i].insert(new_edge);
					e_visited.insert(new_edge);
				}
				
				if (!v_visited.count(k))
				{
					vertices[i].insert(k);
					v_visited.insert(k);
				}
			}
		}

		if (e_visited.size() == G.edges())
		{
			break;
		}
	}
}

DecomposedGraph::DecomposedGraph(const Graph& G, Edge root_edge):
	base_graph(G)
{
	decompose(G, root_edge);
}

void DecomposedGraph::print_decomposition()
{
	for (int i = 1; i < base_graph.size(); i++)
	{
		cout << "V(X_" << i << "): {";

		for (int j : vertices[i])
		{
			cout << j << ", ";
		}
		cout << " }" << endl;

		cout << "E(X_" << i << "): { ";

		for (Edge j : edges[i])
		{
			auto k = j.begin();

			
			cout << "{ " << *k << " , ";
			k++;
			cout << *k << " }, ";
		}

		cout << " } " << endl;
	}
}

ColoredSetCollection DecomposedGraph::construct_colored_sets(int r)
{
	set<int> old_verts;
	//set<int> new_verts = vertices[r + 1];

	for (int i = 0; i <= r; i++)
	{
		old_verts.insert(vertices[i].begin(), vertices[i].end());
	}

	set<Edge> new_edges; //A'

	for (Edge e : edges[r + 1])
	{
		bool new_edge = true;

		for (int v : e)
		{
			if (!old_verts.count(v))
			{
				new_edge = false;
			}
		}

		if (new_edge)
		{
			new_edges.insert(e);
		}
	}

	/*
	map<std::set<int>, int> pre_image_size_map;
	
	for (int i : vertices[r + 1])
	{
		std::set<int> image;

		for (int j : base_graph.adj_list()[i])
		{
			if (vertices[r].count(j))
			{
				image.insert(j);
			}
		}

		pre_image_size_map[image] += 1;
	}*/

	map<std::set<int>, std::set<int>> pre_image_map;

	for (int i : vertices[r + 1])
	{
		std::set<int> image;

		for (int j : base_graph.adj_list()[i])
		{
			if (vertices[r].count(j))
			{
				image.insert(j);
			}
		}

		pre_image_map[image].insert(i);
	}

	int t = base_graph.max_degree();

	vector<set<set<int>>> A(t); //A[i] = sets with preimage of size i

	for (auto pair : pre_image_map)  //sets with empty preimage are implicitly placed in A[0]
	{
		A[pair.second.size()].insert(pair.first);
	}

	ColoredSetCollection set_collection;

	set_collection.base_set = old_verts;
	set_collection.max_size = t;

	int current_color = 1;

	for (int i = 1; i < t; i++)
	{
		set<set<int>> difference;    //A_i - A'
		std::set_difference(A[i].begin(), A[i].end(), new_edges.begin(), new_edges.end(), std::inserter(difference, difference.end()));

		for (set<int> s : difference)
		{
			set_collection.colors[s] = current_color;
		}

		current_color++;
	}

	for (int i = 1; i < t; i++)
	{
		set<set<int>> intersection; //A_i ^ A'
		std::set_intersection(A[i].begin(), A[i].end(), new_edges.begin(), new_edges.end(), std::inserter(intersection, intersection.end()));

		new_edges.erase(intersection.begin(), intersection.end());

		for (set<int> s : intersection)
		{
			set_collection.colors[s] = current_color;
		}

		current_color++;
	}

	for (set<int> s : new_edges) //A_0 ^ A'
	{
		set_collection.colors[s] = current_color;
	}

	set_collection.max_color = current_color;

	return set_collection; 
}

void ColoredSetCollection::print()
{
	for (auto pair : colors)
	{
		cout << "set : { ";

		for (auto j : pair.first)
		{
			cout << j << ", ";
		}

		cout << " }, color: " << pair.second << endl;
	}
}