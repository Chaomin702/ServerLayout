#pragma once
#include <string>
#include <vector>
#include <boost/graph/adjacency_list.hpp>  
#include <boost/graph/graph_traits.hpp>
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
	boost::no_property,
	boost::property<boost::edge_capacity_t, long,
	boost::property<boost::edge_weight_t, int,
	boost::property<boost::edge_residual_capacity_t, long,
	boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >
> Graph;

int importGraphFromFile(const std::string& filename, Graph&g,
	std::vector<std::pair<Traits::vertex_descriptor, int>>& consumers);