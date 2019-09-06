#include <vector>
#include <algorithm>


// efficient union-find based H_0 computation from a filtered graph


// union-find data structure. Implements the efficient "disjoint-set forest" 
// implementation. See Cormen et al. 21.3
class UnionFind{
    public:
        UnionFind(const int N) : rank(N, 0), parent(N){
            for(int i = 0; i < N; i++) 
                parent[i] = i;
        }

        int find(const int id){
            //path compression heuristic
            if(id != UnionFind::parent[id])
                UnionFind::parent[id] = find(UnionFind::parent[id]);

            return UnionFind::parent[id];
        }

        void make_set(){
            UnionFind::parent.push_back(UnionFind::parent.size());
            UnionFind::rank.push_back(0);
        }


        void make_union(const int id1, const int id2){
            UnionFind::make_link(UnionFind::find(id1), UnionFind::find(id2));
        }

    private:
        std::vector<int> parent;
        std::vector<int> rank;


        void make_link(const int id1, const int id2){
            //union by rank heuristic
            if(UnionFind::rank[id1] > UnionFind::rank[id2])
                UnionFind::parent[id2] = id1;

            else{
                UnionFind::parent[id1] = id2;

                if(UnionFind::rank[id1] == UnionFind::rank[id2])
                    UnionFind::rank[id1] += 1;

            }
        }
};


struct edge_sorting_info{
    double last_birth;
    double first_birth;

    int index;
};

// Calculates H0 PD directly from filtered graph. Filtered graph represented by 
// vertex birth times and edges at final filtration time.

std::vector<std::pair<double, double>>
H0_diagram(std::vector<std::pair<int,int>> edges, std::vector<double> vertex_births){
   int V = vertex_births.size();
   int E = edges.size();

   // sort edges by time of last birth (break ties by first birth).
   std::vector<edge_sorting_info> edges_births;
   
   for(std::pair<int,int> vertices : edges){
        double a = std::max(vertex_births[vertices.second], vertex_births[vertices.first]);
        double b = std::min(vertex_births[vertices.second], vertex_births[vertices.first]);
        
        edge_sorting_info edge = {a,b,(int)edges_births.size()};
        edges_births.push_back(edge);
   }
   
   //sort using last_birth with first_birth as a tie-breaker.
   std::sort(edges_births.begin(), edges_births.end(),
            [](const edge_sorting_info& a, const edge_sorting_info &b) -> bool 
            {return (a.last_birth > b.last_birth) || 
                    (a.last_birth == b.last_birth && a.first_birth > b.first_birth); });
   
   // start a union find class
   UnionFind sets(V);
    
   // persistence diagram
   std::vector<std::pair<double,double>> pd;

   for(edge_sorting_info edge : edges_births){
        //find connected component representatives for each vertex
        int first_vertex = sets.find(edges[edge.index].first);
        int second_vertex = sets.find(edges[edge.index].second);
        
        //find death, birth of connected component
        int death_value = std::max(vertex_births[edges[edge.index].first],
                                   vertex_births[edges[edge.index].second]);

        int birth_value = std::max(vertex_births[first_vertex], 
                                   vertex_births[second_vertex]);

        int birth_vertex = vertex_births[first_vertex] < vertex_births[second_vertex] ? 
                            first_vertex : second_vertex;
        
        // add to pd if valid interval
        if( birth_value != death_value )
            pd.push_back(std::make_pair<double, double>(birth_value, death_value));
        
        // connect the two vertices.
        sets.make_union(edges[edge.index].first, edges[edge.index].second);
   }

   return pd;
}






