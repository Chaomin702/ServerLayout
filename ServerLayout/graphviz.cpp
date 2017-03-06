//#include "graphviz.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/config.hpp>  
#include <boost/graph/graph_utility.hpp>  
#include <boost/graph/adjacency_list.hpp> 
#include "optialLayout.h"
using namespace boost;


void drawGraph(const Solution& so,std::vector<std::vector<int>> path)
{
	std::ofstream gvf("gvTest.dot",std::ofstream::out);
	gvf << "digraph G {\n";
	//gvf << "size=\"10,10\"\n";
	gvf << "node [shape=circle,fixedsize=true,width=0.3];\n";
	gvf << "edge [decorate = true,fontsize=8];\n";
	Graph::vertex_iterator vertexIt, vertexEnd;
	Graph:: adjacency_iterator neighbourIt, neighbourEnd;
	boost::tie(vertexIt, vertexEnd) = vertices(so.graph_);

	
	for (; vertexIt != vertexEnd; ++vertexIt)
	{
		boost::tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt, so.graph_);
		for (; neighbourIt != neighbourEnd; ++neighbourIt)
		{
			auto c=boost::get(boost::edge_capacity, so.graph_, boost::edge(*vertexIt, *neighbourIt, so.graph_).first);
			auto w = boost::get(boost::edge_weight, so.graph_, boost::edge(*vertexIt, *neighbourIt, so.graph_).first);
			if (c != 0 )
			{
				gvf << *vertexIt << " -> "
					<< *neighbourIt << "[dir=none label=\"(" << c << "," << w << ")\"];" << "\n";
			}
		}
//		gvf << "\n";
	}

	for (int i = 0; i < so.BwRequirements.size(); ++i)
	{
		int t = so.graph_.m_vertices.size() - so.BwRequirements.size();
		gvf << i + t << "[label=\"" << i << "\"" << " , shape=doublecircle];\n";
	}


	std::vector<std::string> color={"red", "blue", "green", "yellow", "salmon2", "deepskyblue", "darkseagreen", "chocolate", "lightskyblue1", "darkorange1", 
		"firebrick", "lightsteelblue1", "crimson", "greenyellow", "aquamarine2" }; //15
#define COLOR_LENGTH 15;

	int length = path[0][0];
	//int length = 0;
	for (int i = 1; i < length + 1; ++i)
	{
		int m = path[i].size();
		int cr = i%COLOR_LENGTH;
		gvf << "edge [color = \"" << color[cr] << "\" fontcolor = \"" << color[cr] << "\"];\n";
		int j;
		for (j = 0; j < m-3; ++j)
		{
			gvf << path[i][j] << " -> " << path[i][j + 1] << "[label=" << path[i][m - 1] << "];\n";
		}
		gvf << path[i][j] << "->" << path[i][j + 1] + so.graph_.m_vertices.size() - so.BwRequirements.size() << "[label=" << path[i][m - 1] << "];\n";
	}
	gvf << "}";
	gvf.close();
}
