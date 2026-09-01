// Copyright (C) 2004-2009 The Trustees of Indiana University.
// Copyright (C) 2026 Arnaud Becheler.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Nick Edmonds
//           Andrew Lumsdaine

#ifndef BOOST_GRAPH_USE_MPI_HPP
#define BOOST_GRAPH_USE_MPI_HPP

#define BOOST_GRAPH_USE_MPI

#include <boost/mpl/bool.hpp>

namespace boost
{
template < typename G > struct graph_traits;
template < typename T, typename Tag, typename Base > struct bgl_named_params;

namespace detail
{
    // Declared before breadth_first_search.hpp so bfs_dispatch finds it by
    // ordinary lookup. Defined in distributed/breadth_first_search.hpp.
    template < class DistributedGraph, class ColorMap, class BFSVisitor,
        class P, class T, class R >
    void bfs_helper(DistributedGraph& g,
        typename graph_traits< DistributedGraph >::vertex_descriptor s,
        ColorMap color, BFSVisitor vis,
        const bgl_named_params< P, T, R >& params, boost::mpl::true_);
}
}

// property<> serialization traits, needed wherever MPI serialization happens.
#include <boost/graph/distributed/detail/property_serialize.hpp>

// Distributed algorithm overloads.
#include <boost/graph/distributed/concepts.hpp>
#include <boost/graph/distributed/breadth_first_search.hpp>
#include <boost/graph/distributed/connected_components.hpp>
#include <boost/graph/distributed/depth_first_search.hpp>
#include <boost/graph/distributed/dijkstra_shortest_paths.hpp>
#include <boost/graph/distributed/strong_components.hpp>
#include <boost/graph/distributed/page_rank.hpp>
#include <boost/graph/distributed/fruchterman_reingold.hpp>
#include <boost/graph/distributed/rmat_graph_generator.hpp>
#include <boost/graph/distributed/one_bit_color_map.hpp>
#include <boost/graph/distributed/two_bit_color_map.hpp>
#include <boost/graph/distributed/graphviz.hpp>

#endif // BOOST_GRAPH_USE_MPI_HPP
