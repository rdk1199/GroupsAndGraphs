#include "Permutation.h"

#include <set>
#include <iostream>

using std::cout;
using std::endl;
using std::set;
using std::vector;

Permutation::Permutation(std::vector<int> vals) :
	values(vals),
	sz(vals.size())
{
	std::set<int> repeats = {};

	for (int i = 0; i < vals.size(); i++)
	{
		if (vals[i] < 0 || vals[i] >= sz || repeats.count(vals[i]))
		{
			throw InvalidPermutation();
		}

		else
		{
			repeats.insert(vals[i]);
		}
	}
}

Permutation::Permutation(int n) :
	values({}),
	sz(n)
{
	for (int i = 0; i < n; i++)
	{
		values.push_back(i);
	}
}

bool operator==(Permutation const& s, Permutation const& t)
{
	if (s.size() != t.size())
	{
		return false;
	}

	for (int i = 0; i < s.size()-1; i++)
	{
		if (s[i] != t[i])
		{
			return false; 
		}
	}
	return true;
}

bool operator<(Permutation const& s, Permutation const& t)
{
	if (s.size() < t.size())
	{
		return true;
	}

	else if (s.size() > t.size())
	{
		return false; 
	}

	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] < t[i])
		{
			return true;
		}

		else if (s[i] > t[i])
		{
			return false;
		}
	}

	return false; 
}


Permutation operator*(Permutation const& s, Permutation const& t)
{
	if (s.size() != t.size())
	{
		throw InvalidPermOperation();
	}

	std::vector<int> c_vals(s.size());

	for (int i = 0; i < c_vals.size(); i++)
	{
		c_vals[i] = s[t[i]];
	}

	return Permutation(c_vals);
}

std::vector<Permutation> operator*(Permutation const& s, std::vector<Permutation> const& set)
{
	std::vector<Permutation> out;

	for (int i = 0; i < set.size(); i++)
	{
		out.push_back(s * set[i]);
	}

	return out;
}



std::vector<Permutation> operator*(const std::vector<Permutation>& set, Permutation const& s)
{
	std::vector<Permutation> out;

	for (int i = 0; i < set.size(); i++)
	{
		out.push_back(set[i] * s);
	}

	return out;
}

/*
std::set<int> Permutation::operator[](const std::set<int>& s) const
{
	std::set<int> out;

	for (auto i = s.begin(); i != s.end(); i++)
	{
		out.insert(values[*i]);
	}

	return out;
}


std::vector<int> Permutation::operator[](const std::vector<int>& v) const
{
	std::vector<int> out;

	for (auto i = v.begin(); i != v.end(); i++)
	{
		out.push_back(values[*i]);
	}

	return out;
}
*/

Graph Permutation::operator[](const Graph& G) const
{
	if (G.size() != sz)
	{
		throw InvalidPermOperation();
	}

	std::vector<std::set<int>> G_adj = G.get_adj();

	Graph out(G.size());

	for (int i = 0; i < G.size(); i++)
	{
		for (auto j = G_adj[i].begin(); j != G_adj[i].end(); j++)
		{
			out.insert_edge(values[i], values[*j]);
		}
	}
	return out; 
}

Permutation Permutation::inverse() const
{
	std::vector<int> i_vals(sz);

	for (int i = 0; i < sz; i++)
	{
		i_vals[values[i]] = i;
	}

	return Permutation(i_vals);
}

void Permutation::print() const
{
	cout << "(";

	for (int i = 0; i < sz; i++)
	{
		cout << " " << values[i];
	}

	cout << " )";

}

void Permutation::print_cycles() const
{
	set<int> not_printed = {};

	not_printed.insert(values.begin(), values.end());

	while (not_printed.size() > 0)
	{
		cout << "(";

		int i = *not_printed.begin();

		while (not_printed.count(i))
		{
			not_printed.erase(i);

			cout << " " << i;

			i = values[i];

		}

		cout << " )";
	}
}

Permutation Permutation::rand_perm(int n,RandomNumberEngine& rand_eng)
{
	vector<int> vals = {};

	for (int i = 0; i < n; i++)
	{
		vals.push_back(i);
	}

	for (int i = 0; i < n-1; i++)
	{
		int offset = rand_eng.random_int(0, n-i-1);

		int tmp = vals[i];

		vals[i] = vals[i + offset];
		vals[i + offset] = tmp;

	}
	return Permutation(vals);
}

bool Permutation::fixes_blocks(const std::vector<std::set<int>>& B)
{
	for (int i = 0; i < B.size(); i++)
	{
		if ((*this)[B[i]] != B[i])
		{
			return false;
		}
	}

	return true;
}