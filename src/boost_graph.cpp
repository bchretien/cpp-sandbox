// This example is based on: http://stackoverflow.com/a/8812758/1043187

#include <iostream>

#include <boost/shared_ptr.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>

using namespace std;
using namespace boost;


class dfs_chain_visitor : public default_dfs_visitor
{
    typedef vector<uint>    chain_t;
    typedef vector<chain_t> chains_t;

    public:
    dfs_chain_visitor(ostream& o = cout)
        : default_dfs_visitor (),
        o_ (o),
        current_chain_ (),
        chains_ ()
    {
        // Allocate memory for shared_ptr
        chains_ = shared_ptr<chains_t> (new chains_t ());
    }

    ~dfs_chain_visitor()
    {
        // Free shared_ptr memory
        chains_.reset ();
    }

    template <typename Vertex, typename Graph>
        void start_vertex(Vertex u, const Graph & /*g*/)
        {
            // Clear any previously computed chains
            chains_->clear ();
            predecessors_.clear ();
            current_chain_.clear ();

            o_ << "Root: " << u << endl;

            predecessors_.insert (std::pair<int,int> (static_cast<int> (u), -1));
        }

    template <typename Vertex, typename Graph>
        void discover_vertex(Vertex u, const Graph & /*g*/)
        {
            int parent_node = predecessors_.at (static_cast<int> (u));

            // If we are not on the same chain
            if (current_chain_.size () == 0 && parent_node != -1)
            {
                // Add the parent_node
                current_chain_.push_back (static_cast<uint> (parent_node));
            }

            current_chain_.push_back (static_cast<uint> (u));
        }

    template <typename Vertex, typename Graph>
        void finish_vertex(Vertex /*u*/, const Graph & /*g*/)
        {
            if (current_chain_.size () > 0)
            {
                chains_->push_back (current_chain_);
                current_chain_.clear ();
            }
        }

    template <typename Edge, typename Graph>
        void tree_edge(Edge e, const Graph & /*g*/)
        {
            predecessors_.insert (std::pair<int,int>
                                  (static_cast<int> (e.m_target),
                                   static_cast<int> (e.m_source)));
        }

    const chains_t& chains () const
    {
        return *chains_;
    }

    void print () const
    {
        for (chains_t::const_iterator
             i  = chains_->begin ();
             i != chains_->end (); ++i)
        {
            o_ << "Chain " << i - chains_->begin () << ": ";
            for (chain_t::const_iterator
                 j  = i->begin ();
                 j != i->end (); ++j)
            {
                o_ << *j << " ";
            }
            o_ << endl;
        }
    }

    private:
    ostream& o_;

    chain_t  current_chain_;
    std::map<int,int> predecessors_;

    // WARNING: use shared_ptr here (else state is erased)
    // See: http://www.boost.org/doc/libs/1_55_0/libs/graph/doc/undirected_dfs.html (note [1])
    shared_ptr<chains_t> chains_;
};


int main()
{
    /* Define the graph type:
     *   - listS: selects the STL list container to store
     *            the OutEdge list
     *   - vecS: selects the STL vector container to store
     *           the vertices
     *   - undirectedS: selects undirected edges
     */
    typedef adjacency_list<listS, vecS, undirectedS> graph_t;

    // Instantiate a graph object with 11 vertices
    graph_t g (11);

    /* Add some edges
     *
     *   +---+     +---+     +---+
     *   | 1 | --- | 0 | --- | 3 |
     *   +---+     +---+     +---+
     *     |         |         |
     *     |         |         |
     *     |         |         |
     *   +---+     +---+     +---+
     *   | 2 |  +- | 5 | -+  | 4 |
     *   +---+  |  +---+  |  +---+
     *          |    |    |
     *     +----+    |    +----+
     *     |	     |         |
     *   +---+	   +---+     +---+
     *   + 6 +     | 7 |     | 9 |
     *   +---+     +---+     +---+
     *               |         |
     *               |         |
     *               |         |
     *             +---+     +----+
     *             | 8 |     + 10 +
     *             +---+     +----+
     */
    add_edge (0,  1, g);
    add_edge (1,  2, g);

    add_edge (0,  3, g);
    add_edge (3,  4, g);

    add_edge (0,  5, g);
    add_edge (5,  6, g);

    add_edge (5,  7, g);
    add_edge (7,  8, g);

    add_edge (5,  9, g);
    add_edge (9, 10, g);

    // Represent graph in DOT format and send to graph.dot
    std::ofstream graph_file ("graph.dot");
    if (graph_file.is_open ())
    {
        write_graphviz (graph_file, g);
        graph_file.close ();
    }

    // Depth-first visit to further process the tree chain by chain
    dfs_chain_visitor vis = dfs_chain_visitor (cout);

    // Root = 0
    depth_first_search (g, visitor (vis).root_vertex (0));
    vis.print ();
    cout << endl;

    // Root = 10
    depth_first_search (g, visitor (vis).root_vertex (10));
    vis.print ();
    cout << endl;

    return 0;
}
