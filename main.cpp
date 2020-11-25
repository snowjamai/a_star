#include <iostream>
#include <queue>
#include <vector>

#define MS 5

using namespace std;

int maze[MS][MS];

int mv_x[] = { 0,0,1,-1 };
int mv_y[] = { 1,-1,0,0 };


struct node {
	int x;
	int y;
	double cost;
	node(double cost) : x(0), y(0), cost(cost) {
	}
};

struct sorting {
	bool operator()(node n1, node n2) {
		return n1.cost > n2.cost;
	}
};

priority_queue<node,vector <node>, sorting> ol;



vector<node> a_star(node, node);
int main(void)
{
	vector<node> h;
	node s(0), t(0);
	for (int h = 0; h < 5; h++)
		for (int w = 0; w < 5; w++)
			maze[h][w] = 0;
	maze[2][2] = 10;

	s.cost = 0;
	s.x = s.y = 1;
	t.cost = 0;
	t.x = t.y = 4;

	h = a_star(s, t);

	for (int i = 0; i < h.size(); ++i)
		printf("(%d, %d)", h[i].x, h[i].y);
	
}

vector<node> a_star(node s, node t)
{
	vector<node> H;
	vector<node> R;
	vector<node> S;
	int same_flag = 0;

	node q(0), tmp(0);

	ol.push(s);
	
	while ((ol.empty()) != true) {
		q = ol.top();
		ol.pop();
		
		if (q.x == t.x && q.y == t.y)
			return H;
		for (int i = 0; i < 4; ++i) {
			tmp.x = q.x + mv_x[i];
			tmp.y = q.y + mv_y[i];
			tmp.cost = maze[tmp.y][tmp.x];

			if (tmp.x < 0 || tmp.x >= MS)
				continue;
			if (tmp.y < 0 || tmp.y >= MS)
				continue;
			R.push_back(tmp);

			for (int j = 0; j < R.size(); ++j) {
				same_flag = 0;

				for (int k = 0; k < H.size(); ++k)
					if ((R[j].x == H[k].x) && (R[j].y == H[k].y)) {
						same_flag = 1;
						break;
					}
				if (same_flag == 0)
					S.push_back(R[j]);
			}
			R.clear();

			for (int j = 0; j < S.size(); ++j) {
				S[j].cost = S[j].cost + (t.y - S[j].y) * (t.y - S[j].y) + (t.x - S[j].x) * (t.x - S[j].x);
				ol.push(S[j]);
			}
			S.clear();
		}
		H.push_back(q);
	}
	return H;
}
