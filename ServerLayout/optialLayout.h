#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/graph/adjacency_list.hpp>  
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>  
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
	boost::no_property,
	boost::property<boost::edge_capacity_t, long,
	boost::property<boost::edge_weight_t, int,
	boost::property<boost::edge_residual_capacity_t, long,
	boost::property<boost::edge_reverse_t, Traits::edge_descriptor > > > >
> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
const int INF_CAPACITY = 1000000;
class Solution {
public:
	Graph graph_;
	std::map<Vertex, int> BwRequirements;
	void constructGraph(const std::string& filename) {
		importGraphFromFile(filename);
	}
	void informationCollected();

private:
	int importGraphFromFile(const std::string& filename);
	void AddEdge(Vertex v1,
		Vertex v2,
		boost::property_map < Graph, boost::edge_reverse_t >::type &rev,
		const int capacity, const int cost,
		Graph &g)
	{
		Edge e1 = boost::add_edge(v1, v2, g).first;
		Edge e2 = boost::add_edge(v2, v1, g).first;
		boost::put(boost::edge_capacity, g, e1, capacity);
		boost::put(boost::edge_capacity, g, e2, 0);
		//boost::put(boost::edge_weight, g, e1, cost);
		rev[e1] = e2;
		rev[e2] = e1;
	}
};