
// Distributed under the Boost Software License, Version 1.0.
#include <boost/graph/use_mpi.hpp>
#include <boost/graph/distributed/delta_stepping_shortest_paths.hpp>
#include <boost/graph/distributed/mpi_process_group.hpp>
#include <boost/graph/distributed/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/mpi.hpp>
#include <boost/core/lightweight_test.hpp>
#include <cstdlib>
#include <utility>
#include <vector>

template<class W> struct WeightedEdge {
  W weight;
  WeightedEdge(W value=0) : weight(value) {}
  template<class Archive> void serialize(Archive& ar, unsigned) {ar & weight;}
};
template<class W> struct VertexProperties {
  W distance;
  VertexProperties(W value=0) : distance(value) {}
  template<class Archive> void serialize(Archive& ar, unsigned) {ar & distance;}
};
template<class W> void run_case(const char* const weights[4],const char* delta_text) {
  using namespace boost;
  typedef adjacency_list<listS,distributedS<graph::distributed::mpi_process_group,vecS>,
      directedS,VertexProperties<W>,WeightedEdge<W> > Graph;
  std::vector<std::pair<unsigned long,unsigned long> > edges;
  std::vector<WeightedEdge<W> > values;
  W expected[5]={0,0,0,0,0};
  for(unsigned i=0;i<4;++i) {
    edges.push_back(std::make_pair(i,i+1));
    W value=static_cast<W>(std::strtod(weights[i],0));
    values.push_back(WeightedEdge<W>(value));
    expected[i+1]=expected[i]+value;
  }
  Graph graph(edges.begin(),edges.end(),values.begin(),5);
  graph::distributed::delta_stepping_shortest_paths(graph,vertex(0,graph),
      dummy_property_map(),get(&VertexProperties<W>::distance,graph),
      get(&WeightedEdge<W>::weight,graph),static_cast<W>(std::strtod(delta_text,0)));
  typename property_map<Graph,vertex_global_t>::const_type global=get(vertex_global,graph);
  BGL_FORALL_VERTICES_T(v,graph,Graph) {
    unsigned index=graph.distribution().global(get(global,v).first,get(global,v).second);
    BOOST_TEST_EQ(get(&VertexProperties<W>::distance,graph,v),expected[index]);
  }
}
int main(int argc,char** argv) {
  boost::mpi::environment environment(argc,argv);
  { const char* weights[4]={"0x1.1a7d68e8aa4eep+13","0x1.350c7b8632a9bp+4","0x1.9c10a4b2ee374p+2","0x1.9c10a4b2ee374p+2"};
    run_case<double>(weights,"0x1.350c7b8632a97p+4"); }
  { const char* weights[4]={"0x1.0771920000000p+13","0x1.f407c20000000p+2","0x1.4d5a7c0000000p+1","0x1.4d5a7c0000000p+1"};
    run_case<float>(weights,"0x1.f407ba0000000p+2"); }
  return boost::report_errors();
}
