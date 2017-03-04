//#include "graphviz.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/config.hpp>  
#include <boost/graph/graph_utility.hpp>  
#include <boost/graph/adjacency_list.hpp> 
#include "optialLayout.h"
using namespace boost;


void drawGraph(const Graph& g,std::vector<std::vector<int>> path)
{
	std::ofstream gvf("gvTest.dot",std::ofstream::out);
	gvf << "digraph G {\n";
	//gvf << "size=\"10,10\"\n";
	gvf << "node [shape=circle,fixedsize=true,width=0.4];\n";
	Graph::vertex_iterator vertexIt, vertexEnd;
	Graph:: adjacency_iterator neighbourIt, neighbourEnd;
	boost::tie(vertexIt, vertexEnd) = vertices(g);

	
	for (; vertexIt != vertexEnd; ++vertexIt)
	{
		boost::tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt, g);
		for (; neighbourIt != neighbourEnd; ++neighbourIt)
		{
			auto c=boost::get(boost::edge_capacity, g, boost::edge(*vertexIt, *neighbourIt, g).first);
			auto w = boost::get(boost::edge_weight, g, boost::edge(*vertexIt, *neighbourIt, g).first);
			if (c != 0 && w != 0)
			{
				gvf << *vertexIt << " -> "
					<< *neighbourIt << "[dir=none label=\"(" << c << "," << w << ")\"];" << "\n";
			}
		}
		gvf << "\n";
	}

	std::vector<std::string> color={"red", "blue", "green", "yellow", "salmon2", "deepskyblue", "darkseagreen", "chocolate", "lightskyblue1", "darkorange1", 
		"darkorange1", "lightsteelblue1", "lemonchiffon", "greenyellow", "aquamarine2" }; //15
#define COLOR_LENGTH 15;

//	int length = path[0][0];
	int length = 0;
	for (int i = 1; i < length; ++i)
	{
		int m = path[i].size();
		int cr = i%COLOR_LENGTH;
		gvf << "edge [color = \"" << color[cr] << "\" fontcolor = \"" << color[cr] << "\"];\n";
		for (int j = 0; j < m-2; ++j)
		{
			gvf << path[i][j] << " -> " << path[i][j + 1] << "[label=" << path[i][m - 1] << "];\n";
		}
	}
	gvf << "}";
	gvf.close();
}
