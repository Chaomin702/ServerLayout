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
	for (int i = 0; i < numConsumer; ++i) {
		std::getline(infile, buf);
		std::stringstream ss(buf);
		ss >> u >> v >> cap;
		BwRequirements[u] = cap;
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