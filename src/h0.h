#include <vector>
#include <algorithm>


// efficient union-find based H_0 computation from a filtered graph


// union-find data structure. 
// note: we do not use union-by-rank heuristic since we want the rep to always
// be the vertex with earliest birth.
class UnionFind{
    public:
        UnionFind(const int N, const std::vector<double>& vertex_values) : 
                    parent(N), vertex_values(vertex_values){
            for(int i = 0; i < N; i++) 
                parent[i] = i;
        }

        int find(const int id){
            //path compression heuristic
            if(id != UnionFind::parent[id])
                UnionFind::parent[id] = find(UnionFind::parent[id]);

            return UnionFind::parent[id];
        }


        void make_union(const int id1, const int id2){
            int rep1 = UnionFind::find(id1);
            int rep2 = UnionFind::find(id2);
            
            if(rep1 != rep2){
                if(vertex_values[rep1] > vertex_values[rep2])
                    parent[rep1] = rep2;

                else
                    parent[rep2] = rep1;
            }
        }

    private:
        std::vector<int> parent;
        const std::vector<double>& vertex_values;
};


struct edge_sorting_info{
    double last_birth;
    double first_birth;

    int index;
};


// sort edges by time of last birth (break ties by first birth).
std::vector<edge_sorting_info> 
edge_sort(std::vector<std::pair<int,int>> edges, std::vector<double> vertex_births){
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

   return edges_births;
}

// Calculates H0 PD directly from filtered graph. Filtered graph represented by 
// vertex birth times and edges at final filtration time.
std::vector<std::pair<double, double>>
H0_diagram(std::vector<std::pair<int,int>> edges, std::vector<double> vertex_births){
   int V = vertex_births.size();
   int E = edges.size();
    
   std::vector<edge_sorting_info> edge_births = edge_sort(edges, vertex_births);
   
   // start a union find class
   UnionFind sets(V, vertex_births);
    
   // persistence diagram
   std::vector<std::pair<double,double>> pd;

   for(edge_sorting_info edge : edge_births){
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






