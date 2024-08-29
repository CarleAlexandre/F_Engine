#ifndef PATHFINDING_HPP
# define PATHFINDING_HPP

#include "include.hpp"

class Pathfinding {
	public:
		typedef struct node {
			int x,y;
			int heuristic, cost;
			node* parent;
			node(int x_, int y_, int cost_, int heuristic_, node* parent_ = nullptr) : x(x_), y(y_), cost(cost_), heuristic(heuristic_), parent(parent_){}
		};
	private:
		int heuristic(Vector2 a, Vector2 b) {
			return (abs(a.x - b.x) + abs(a.y - b.y));
		}

		struct CompareNodes {
			bool operator()(node* n1, node* n2) {
				return (n1->heuristic + n1->cost) > (n2->heuristic + n2->cost);
			}
		};

    std::vector<node*> reconstructPath(node* current) {
        std::vector<node *> path;
        while (current != nullptr) {
            path.push_back(current);
            current = current->parent;
        }
        std::reverse(path.begin(), path.end());
		return path;
    }

	public:
	int nodeCmp(node n1, node n2) {
		if (n1.heuristic < n2.heuristic) {
			return (1);
		} else if (n1.heuristic == n2.heuristic) {
			return (0);
		} else {
			return (-1);
		}
	}

	std::vector<node*> findShortest(char graph[16][16], Vector2 pos, Vector2 topos) {
		int startX = static_cast<int>(pos.x / 32);
        int startY = static_cast<int>(pos.y / 32);
        int goalX = static_cast<int>(topos.x / 32);
        int goalY = static_cast<int>(topos.y / 32);
        node* start = new node(startX, startY, 0, heuristic(pos, topos));
        node* goal = new node(goalX, goalY, 0, 0);

        // Open and closed lists
        std::priority_queue<node*, std::vector<node*>, CompareNodes> openList;
        std::vector<node*> closedList;

        // Add the start node to the open list
        openList.push(start);

        while (!openList.empty()) {
            // Get the node with the lowest f (cost + heuristic)
            node* current = openList.top();
            openList.pop();

            // If we've reached the goal, reconstruct the path
            if (current->x == goal->x && current->y == goal->y) {
                std::vector<node*> path = reconstructPath(current);
                delete goal;
                return (path);
            }

            // Add the current node to the closed list
            closedList.push_back(current);

            // Explore neighbors (up, down, left, right)
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue; // Skip the current node
                    if (abs(i) + abs(j) == 2) continue; // Skip diagonal neighbors

                    int newX = current->x + i;
                    int newY = current->y + j;

                    if (newX < 0 || newX >= 16 || newY < 0 || newY >= 16) continue; // Skip out of bounds
                    if (graph[newX][newY] == 1) continue; // Skip obstacles

                    int newCost = current->cost + 1; // Assuming uniform cost (1) for each move
                    int newHeuristic = heuristic((Vector2){newX, newY}, topos);
                    node* neighbor = new node(newX, newY, newCost, newHeuristic, current);

                    // Check if this neighbor is in the closed list
                    if (std::find_if(closedList.begin(), closedList.end(), [neighbor](node* n) {
                        return n->x == neighbor->x && n->y == neighbor->y;
                    }) != closedList.end()) {
                        delete neighbor;
                        continue; // Skip if already explored
                    }

                    // Add to the open list
                    openList.push(neighbor);
                }
            }
        }
        delete goal;
		return (std::vector<node *>());
	}

	void deleteNodes(std::vector<node *> nodes) {
		for (auto node : nodes) {
			delete node;
		}
	}
	/*
	
	Here’s how you might use deleteNodes in practice:

	cpp

	Pathfinding pf;
	std::vector<Pathfinding::node*> path = pf.findShortest(graph, start, end);

	if (!path.empty()) {
		// Use the path
		for (auto n : path) {
			std::cout << "(" << n->x << "," << n->y << ") -> ";
		}
		std::cout << "end" << std::endl;

		// After using the path, delete the nodes to free memory
		pf.deleteNodes(path);
	} else {
		std::cout << "No path found" << std::endl;
	}
	*/

	Pathfinding() {

	}
	~Pathfinding() {

	}
};

#endif