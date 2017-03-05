#include <iostream>  
#include <fstream>
#include <boost/config.hpp>  
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>  
#include <boost/graph/push_relabel_max_flow.hpp>  
#include <boost/graph/edmonds_karp_max_flow.hpp>  
#include <boost/graph/read_dimacs.hpp>  
#include <boost/graph/graph_utility.hpp>  
#include <assert.h>
#include "optialLayout.h"
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::map;
using namespace boost;


int Solution::importGraphFromFile(const string& filename) {
	std::ifstream infile(filename);
	if (!infile.is_open()) {
		cout << "Fail to open file " << filename << endl;
		return -1;
	}
	cout << "Open file" << filename << " OK.\n";
	property_map<Graph, edge_capacity_t>::type
		capacity = get(edge_capacity, graph_);
	property_map<Graph, edge_reverse_t>::type
		rev = get(edge_reverse, graph_);
	property_map<Graph, edge_residual_capacity_t>::type
		residual_capacity = get(edge_residual_capacity, graph_);

	int numVertex = 0, numEdge = 0, numConsumer = 0;
	double serverCost = 0;
	string buf;
	std::getline(infile, buf);
	{
		std::stringstream ss(buf);
		ss >> numVertex >> numEdge >> numConsumer;
	}
	std::getline(infile, buf);
	std::getline(infile, buf);
	{
		std::stringstream ss(buf);
		ss >> serverCost;
	}
	std::getline(infile, buf);
	int v, u, cap, cost;
	for (int i = 0; i < numEdge; ++i) {
		std::getline(infile, buf);
		std::stringstream ss(buf);
		ss >> u >> v >> cap >> cost;
		AddEdge(u, v, rev, cap, cost, graph_);
	}
	std::getline(infile, buf);
	cout << "There are " << num_vertices(graph_) << " vertexes, " << num_edges(graph_) << " edges.\n";
	//add consumer nodes
	for (int i = 0; i < numConsumer; ++i) {
		Vertex t = num_vertices(graph_);
		std::getline(infile, buf);
		std::stringstream ss(buf);
		ss >> u >> v >> cap;
		AddEdge(v, t, rev, cap, 0, graph_);
		BwRequirements[t] = cap;
	}
	return 0;
}

void Solution::informationCollected() {
	int bwSum = 0;
	for (auto i : BwRequirements)
		bwSum += i.second;
	cout << "bandwidth requirement sum: " << bwSum << endl;
	//sort nodes by bandwidth
	typedef pair<Vertex, int> flows;
	struct comp {
		bool operator()(const flows& lhs, const flows& rhs) { return lhs.second > rhs.second; };
	};
	std::set<flows, comp> S;
	for (auto vi = vertices(graph_).first; vi != vertices(graph_).second; ++vi) {
		if (BwRequirements.find(*vi) != BwRequirements.end()) continue;
		int flowSum = 0;
		for (auto ei = out_edges(*vi, graph_).first; ei != out_edges(*vi, graph_).second; ++ei) {
			flowSum += get(edge_capacity, graph_, *ei);
		}
		S.insert({ *vi,flowSum });
	}
	//select nodes with large bandwidth
	cout << "\nselect nodes with large bandwidth:\nnode	bandwidth" << endl;
	vector<Vertex> center;
	int curFlow = 0;
	for (auto i : S) {
		cout << i.first << "	" << i.second << endl;
		curFlow += i.second;
		center.push_back(i.first);
		if (curFlow > bwSum * 4) break;
	}
	//add super start point and super end point
	Vertex s = num_vertices(graph_), t = s + 1;
	property_map<Graph, edge_capacity_t>::type
		capacity = get(edge_capacity, graph_);
	property_map<Graph, edge_reverse_t>::type
		rev = get(edge_reverse, graph_);
	property_map<Graph, edge_residual_capacity_t>::type
		residual_capacity = get(edge_residual_capacity, graph_);
	for (auto u : center)
		AddEdge(s, u, rev, INF_CAPACITY, 0, graph_);
	for (auto u : BwRequirements)
		AddEdge(u.first, t, rev, INF_CAPACITY, 0, graph_);
	//maxflow 
	long flow = push_relabel_max_flow(graph_, s, t);
	//cout flow of comsumer
	cout << "source	target	flow	requirement" << endl;
	for (auto i : BwRequirements) {
		Vertex v = i.first;
		auto ei = out_edges(v, graph_).first;
		Vertex u = target(*ei, graph_);
		Edge e = edge(u, v, graph_).first;
		//edge must exist
		assert(edge(u, v, graph_).second);
		std::cout << "f " << u << "->" << v << "	"
			<< (capacity[e] - residual_capacity[e]) << "	" << BwRequirements[v] << std::endl;
	}
	cout << "c  The total flow:" << endl;
	cout << "s " << flow << endl;
}
