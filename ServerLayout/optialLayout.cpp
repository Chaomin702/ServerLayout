#include <iostream>  
#include <fstream>
#include <boost/config.hpp>  
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>  
#include <boost/graph/push_relabel_max_flow.hpp>  
#include <boost/graph/edmonds_karp_max_flow.hpp>  
#include <boost/graph/read_dimacs.hpp>  
#include <boost/graph/graph_utility.hpp>  
#include "optialLayout.h"
using std::cout;
using std::endl;
using std::string;
using namespace boost;

void AddEdge(Traits::vertex_descriptor v1,
	Traits::vertex_descriptor v2,
	boost::property_map < Graph, boost::edge_reverse_t >::type &rev,
	const int capacity,const int cost,
	Graph &g)
{
	Traits::edge_descriptor e1 = boost::add_edge(v1, v2, g).first;
	Traits::edge_descriptor e2 = boost::add_edge(v2, v1, g).first;
	boost::put(boost::edge_capacity, g, e1, capacity);
	boost::put(boost::edge_capacity, g, e2, 0);
	boost::put(boost::edge_weight, g, e1, cost);
	rev[e1] = e2;
	rev[e2] = e1;
}
int importGraphFromFile(const string& filename, Graph&g,
	std::vector<std::pair<Traits::vertex_descriptor,int>>& consumers) {
	std::ifstream infile(filename);
	if (!infile.is_open()) {
		cout << "Fail to open file " << filename << endl;
		return -1;
	}
	cout << "Open file" << filename << " OK.\n";
	property_map<Graph, edge_capacity_t>::type
		capacity = get(edge_capacity, g);
	property_map<Graph, edge_reverse_t>::type
		rev = get(edge_reverse, g);
	property_map<Graph, edge_residual_capacity_t>::type
		residual_capacity = get(edge_residual_capacity, g);

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
		AddEdge(u, v, rev, cap, cost, g);
	}
	std::getline(infile, buf);
	cout << "There are " << num_vertices(g) << " vertexes, " << num_edges(g) << " edges.\n";
	for (int i = 0; i < numConsumer; ++i) {
		std::getline(infile, buf);
		std::stringstream ss(buf);
		ss >> u >> v >> cap;
		consumers.push_back({ u,cap });
	}
	return 0;
}

//int main(int argc, char* argv[])
//{
//	Graph g;
//	property_map<Graph, edge_capacity_t>::type
//		capacity = get(edge_capacity, g);
//	property_map<Graph, edge_reverse_t>::type
//		rev = get(edge_reverse, g);
//	property_map<Graph, edge_residual_capacity_t>::type
//		residual_capacity = get(edge_residual_capacity, g);
//
//	//boost::add_edge(u, v, g);  
//	AddEdge(0, 1, rev, 4, g);
//	AddEdge(0, 2, rev, 9, g);
//	AddEdge(1, 3, rev, 8, g);
//	AddEdge(2, 3, rev, 6, g);
//
//
//	//double flow = boost::boykov_kolmogorov_max_flow(g, u, x);  
//	//double flow = boost::push_relabel_max_flow(g, u, x);  
//
//	long flow = push_relabel_max_flow(g, 0, 3);
//	std::cout << "c  The total flow:" << std::endl;
//	std::cout << "s " << flow << std::endl << std::endl;
//	boost::graph_traits < Graph >::vertex_iterator u_iter, u_end;
//	boost::graph_traits<Graph>::out_edge_iterator ei, e_end;
//	for (boost::tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter)
//		for (boost::tie(ei, e_end) = out_edges(*u_iter, g); ei != e_end; ++ei)
//			if (capacity[*ei] > 0)
//				std::cout << "f " << *u_iter << " " << target(*ei, g) << " "
//				<< (capacity[*ei] - residual_capacity[*ei]) << std::endl;
//
//	return 0;
//}