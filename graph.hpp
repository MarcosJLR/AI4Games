#pragma once

#include <SDL.h>
#include <vector>
#include <set>
#include <queue>

#include "vector3D.hpp"

namespace aifg
{
    class Node
    {
    private:
        Vector3 v0, v1, v2;
        Vector3 center;

    public:
        Node(const Vector3& v0, const Vector3& v1, const Vector3& v2)
            : v0(v0), v1(v1), v2(v2), center((v0 + v1 + v2) / 3) {}

        inline Vector3 getCenter() { return center; }
        bool hasPoint(Vector3 p);
        void draw(SDL_Renderer* renderer);
    };

    class Graph
    {
    private:
        int N;
        std::vector<Node> nodes;
        std::vector<std::vector<int>> adjacencyList;
        const static double INF = 1.0 / 0.0;

    public:
        void init(const char* filepath);
        inline double cost(int i, int j)
        {
            return nodes[i].getCenter().distance(nodes[j].getCenter());
        }
        int whereAmI(Vector3 pos, int lastNode = -1);
        std::vector<Vector3> Astar(int s, int t);
        void draw(SDL_Renderer* renderer);
    };
};