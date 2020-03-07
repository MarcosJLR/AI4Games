#include "graph.hpp"

namespace aifg
{
    bool Node::hasPoint(Vector3 p)
    {
        double A = abs(v0.orient2D(v1,v2));
        double B = abs(p.orient2D(v0,v1)) + abs(p.orient2D(v0,v2)) + abs(p.orient2D(v1,v2));

        return abs(A - B) < 0.01;
    }

    void Node::draw(SDL_Renderer* renderer)
    {
        int x0 = round(v0.x), y0 = round(v0.z);
        int x1 = round(v1.x), y1 = round(v1.z);
        int x2 = round(v2.x), y2 = round(v2.z);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        SDL_RenderDrawLine(renderer, x2, y2, x0, y0);
    }

    int Graph::whereAmI(Vector3 pos, int lastNode)
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
        std::priority_queue <std::tuple<double, int, int>> pq;
        std::vector <double> dist(N, INF);
        std::vector <int> par(N, -1);
        dist[s] = 0.0;
        par[s] = s;
        pq.push({0.0, s, s});

        while(!pq.empty())
        {
            auto [c, v, p] = pq.top();
            pq.pop();

            if(p != par[v]) continue;
            
            for(int u : adjacencyList[v])
            {
                c = cost(u, v);
                if(dist[v] + c < dist[u])
                {
                    dist[u] = dist[v] + c;
                    par[u] = v;
                    pq.push({-dist[u] - cost(u, t), u, v});
                }
            }
        }
        
        std::vector<Vector3> result;
        for(int v = t; par[v] != -1 && par[v] != v; v = par[v])
            result.push_back(nodes[v].getCenter());
        
        if(result.empty() && s != t) 
            return result;
        
        result.push_back(nodes[s].getCenter());
        std::reverse(result.begin(), result.end());
        return result;
    }
    
    void Graph::draw(SDL_Renderer* renderer)
    {
        for(auto node : nodes)
            node.draw(renderer);
    }

    void Graph::init(const std::string& path)
    {
        std::ifstream nodeStream;
        std::ifstream eleStream;
        std::ifstream neighStream;

        nodeStream.open(path + ".node" );
        if(!nodeStream.is_open())
        {
            std::cerr << "Couldn't open " + path + ".node file!" << std::endl;
            return;
        }        

        eleStream.open(path + ".ele");
        if(!eleStream.is_open())
        {
            std::cerr << "Couldn't open " + path + ".ele file!" << std::endl;
            return;
        }

        neighStream.open(path + ".neigh");
        if(!neighStream.is_open())
        {
            std::cerr << "Couldn't open " + path + ".ele file!" << std::endl;
            return;
        }

        int n, dim, attrNo, boundMarks;
        std::vector<Vector3> points;
        nodeStream >> n >> dim >> attrNo >> boundMarks;
        for(int i = 0; i < n; i++)
        {
            int id;
            double x, z, attrIgnore, boundMarkIgnore;
            
            nodeStream >> id >> x >> z;

            for(int j = 0; j < attrNo; j++)
                nodeStream >> attrIgnore;

            for(int j = 0; j < boundMarks; j++)
                nodeStream >> boundMarkIgnore;

            points.push_back(Vector3(x, 0, z));
        }

        std::cout << "Node file loaded correctly" << std::endl;

        int ppt;
        eleStream >> N >> ppt >> attrNo;
        for(int i = 0; i < N; i++)
        {
            int id, v0, v1, v2;
            double attrIgnore;

            eleStream >> id >> v0 >> v1 >> v2;
            for(int j = 0; j < attrNo; j++)
                eleStream >> attrIgnore;

            nodes.push_back(Node(points[v0-1], points[v1-1], points[v2-1]));

        }

        std::cout << "Element file loaded correctly" << std::endl;
        
        adjacencyList = std::vector<std::vector<int>>(N);

        int neighN, npt;
        neighStream >> neighN >> npt;
        for(int i = 0; i < N; i++)
        {
            int id, t;
            neighStream >> id;
            id--;

            neighStream >> t;
            if(t != -1)
                adjacencyList[id].push_back(t-1);

            neighStream >> t;
            if(t != -1)
                adjacencyList[id].push_back(t-1);

            neighStream >> t;
            if(t != -1)
                adjacencyList[id].push_back(t-1);
        }

        std::cout << "Neighbours file loaded correctly" << std::endl;

        nodeStream.close();
        eleStream.close();
        neighStream.close();
    }

};