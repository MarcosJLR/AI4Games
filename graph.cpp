#include "graph.hpp"

namespace aifg
{
    bool Node::hasPoint(Vector3 p)
    {
        bool inAngle102 = (v0.orient(v1, p) <= 0 && v0.orient(p, v2) <= 0) ||
                          (v0.orient(v1, p) >= 0 && v0.orient(p, v2) >= 0);

        bool inAngle012 = (v1.orient(v0, p) <= 0 && v1.orient(p, v2) <= 0) ||
                          (v1.orient(v0, p) >= 0 && v1.orient(p, v2) <= 0);

        return inAngle012 && inAngle102; 
    }

    void Node::draw(SDL_Renderer* renderer)
    {
        int x0 = floor(v0.x), y0 = floor(v0.z);
        int x1 = floor(v1.x), y1 = floor(v1.z);
        int x2 = floor(v2.x), y2 = floor(v2.z);
        
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
        SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        SDL_RenderDrawLine(renderer, x2, y2, x0, y0);
    }

    int Graph::whereAmI(Vector3 pos, int lastNode = -1)
    {
        if(lastNode != -1){
            std::set<int> visited;
            std::queue<int> q;

            q.push(lastNode);
            visited.insert(lastNode);

            while(!q.empty()){
                int v = q.front();
                q.pop();

                if(nodes[v].hasPoint(pos))
                    return v;

                for(int u : adjacencyList[v])
                    if(!visited.count(u)){
                        q.push(u);
                        visited.insert(u);
                    }
            }
        }
        for(int i = 0; i < N; i++){
            if(nodes[i].hasPoint(pos))
                return i;
        }
        return -1;
    }

    std::vector<Vector3> Graph::Astar(int s, int t)
    {
        priority_queue <std::pair<double, int>> pq;
        std::vector <double> dist(N, INF);
        std::vector <int> pat(N, -1);
        dist[s] = 0.0;
        pq.push({0.0, s});

        while(!pq.empty())
        {
            auto [v, c] = pq.top();
            pq.pop();

            if(c > dist[v]) continue;
            
            for(int u : adjacencyList[v])
            {
                c = cost(u, v);
                if(dist[v] + c < dist[u])
                {
                    dist[u] = dist[v] + c;
                    par[u] = v;
                    pq.push({-dist[u] - cost(u, t), u});
                }
            }
        }
        
        std::vector<Vector3> result;
        for(int v = t; par[v] != -1 && par[v] != v; v = par[v])
            result.push_back(nodes[v].getCenter());
        
        if(result.empty() && s != t) 
            return result;
        
        result.push_back(nodes[s].getCenter());
        reverse(result.begin(), result.end());
        return result;
    }

};