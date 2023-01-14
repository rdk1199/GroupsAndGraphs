#include "Graph.h"

#include <algorithm>
#include <iostream>

#include "DecomposedGraph.h"

using std::vector;
using std::set;

using std::cout;
using std::endl;

#include <queue>

Graph::Graph(int size):
	n(size),
	adj(vector<set<int>>(n)),
	colors(vector<int>(n)),
	num_edges(0)
{}

Graph::Graph(const vector<set<int>>& adj_list) :
	n(adj_list.size()),
	adj(vector<set<int>>(n)),
	colors(vector<int>(n)),
	num_edges(0)
{
	for (int i = 0; i < adj_list.size(); i++)
	{
		for (auto j = adj_list[i].begin(); j != adj_list[i].end(); j++)
		{
			insert_edge(i, *j);
		}
	}
}

std::vector<std::set<int>> const Graph::get_adj() const
{
	return adj;
}

bool Graph::operator==(const Graph& h)
{
	return adj == h.get_adj();
}

bool const Graph::has_edge(int i, int j) const 
{
	if (i < 0 || i >= n)
	{
		return false;
	}

	return (adj[i].count(j));
}

bool const Graph::has_edge(Edge e) const
{
	auto i = e.begin();

	int first = *i;

	i++;

	int second = *i;

	return has_edge(first, second);
}

bool Graph::insert_edge(int i, int j)
{
	if (i < 0 || i >= n || j < 0 || j >= n || i == j)
	{
		return false;
	}

	if (!adj[i].count(j))
	{
		adj[i].insert(j);
		adj[j].insert(i);

		num_edges++;
	}
	return true;
}

void Graph::delete_edge(int i, int j)
{
	if (adj[i].count(j))
	{
		adj[i].erase(j);
		adj[j].erase(i);

		num_edges--;
	}
}

int Graph::degree(int i) const
{
	return adj[i].size();
}

int Graph::max_degree() const
{
	int max = degree(0);

	for (int i = 1; i < n; i++)
	{
		int deg = degree(i);
		if (deg > max)
		{
			max = deg;
		}
	}

	return max;
}

void Graph::clear_colors()
{
	for (int i = 0; i < colors.size(); i++)
	{
		colors[i] = 0;
	}
}

Graph Graph::gnp_random(int n, float p, RandomNumberEngine& rand_eng)
{
	p = std::clamp(p, 0.0f, 1.0f);

	Graph out = Graph(n);

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			float draw = rand_eng.random_float();

			if (draw <= p)
			{
				out.insert_edge(i, j);
			}
		}
	}
	return out;
}

void const Graph::print()
{
	for (int i = 0; i < n; i++)
	{
		cout << i << ": ";

		for (auto j = adj[i].begin(); j != adj[i].end(); j++)
		{
			cout << *j << ", ";
		}

		cout << endl;
	}
}

std::set<int> const Graph::con_comp(int i) 
{
	if (i < 0 || i >= n)
	{
		return {};
	}

	clear_colors();

	std::queue<int> Q;

	Q.push(i);

	colors[i] = 1;

	std::set<int> out = { i };

	while (!Q.empty())
	{
		int current = Q.front();
		Q.pop();

		for (auto j = adj[current].begin(); j != adj[current].end(); j++)
		{
			if (colors[*j] == 0)
			{
				out.insert(*j);
				Q.push(*j);
				colors[*j] = 1;
			}
		}
	}

	clear_colors();

	return out;
}

std::vector<std::set<int>> const Graph::conn_components()
{
	std::set<int> unfound_vertices = {};

	for (int i = 0; i < n; i++)
	{
		unfound_vertices.insert(i);
	}

	std::vector<std::set<int>> out;

	while (!unfound_vertices.empty())
	{
		int next_vert = *unfound_vertices.begin();

		std::set<int> conn_comp = con_comp(next_vert);

		for (int i : conn_comp)
		{
			unfound_vertices.erase(i);
		}
		//unfound_vertices.erase(conn_comp.begin(), conn_comp.end());

		out.push_back(conn_comp);
	}

	return out;
}


bool const Graph::is_connected() 
{
	return (con_comp(0).size() == n);
}

