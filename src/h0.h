#include <vector>
#include <algorithm>
#include <iostream> //debugging
#include <Rcpp.h>

#include "rip.h"


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
    
   std::vector<edge_sorting_info> edge_births = edge_sort(edges, vertex_births);
   
   // start a union find class
   UnionFind sets(V, vertex_births);
    
   // persistence diagram
   std::vector<std::pair<double,double>> pd;

   for(edge_sorting_info edge : edge_births){
        //find connected component representatives for each vertex
        int first_vertex = sets.find(edges[edge.index].first-1);
        int second_vertex = sets.find(edges[edge.index].second-1);

        //find death, birth of connected component
        double death_value = std::max(vertex_births[edges[edge.index].first-1],
                                   vertex_births[edges[edge.index].second-1]);

        double birth_value = std::max(vertex_births[first_vertex], 
                                   vertex_births[second_vertex]);

        // add to pd if valid interval
        if( birth_value != death_value )
            pd.push_back(std::make_pair(birth_value, death_value));
        
        // connect the two vertices.
        sets.make_union(edges[edge.index].first-1, edges[edge.index].second-1);
   }
    
   return pd;
}




//converts H0 results to common format as usual pd call
Rcpp::List
H0_diagram_interface(std::vector<int> edges, std::vector<double> vertex_births){
    //reconstruct edge matrix
    std::vector<std::pair<int,int>> edges_mat;

    for(int i = 0; i < edges.size(); i+=2)
        edges_mat.push_back(std::make_pair(edges[i], edges[i+1]));

    std::vector<std::pair<double,double>> output = H0_diagram(edges_mat, vertex_births);
    
    ripserResults results;
    results.births_and_deaths_by_dim = std::vector<std::vector<float>>();
    results.num_edges = edges.size();

    //unroll output
    std::vector<float> dim0_unrolled;
    results.births_and_deaths_by_dim.push_back(dim0_unrolled);

    for(std::pair<double,double> interval : output){
        results.births_and_deaths_by_dim[0].push_back(interval.first);
        results.births_and_deaths_by_dim[0].push_back(interval.second);
    }

    return ripserResultToR(results, 2, 0, -1, 0);
}




