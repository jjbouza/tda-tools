#include <vector>

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

        int make_set(){
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



