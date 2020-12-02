#include <iostream>
#include <queue>
#include <vector>
#include <random>

using namespace std;

#define MS 5
#define Q_SIZE 1

int maze[MS][MS];

int mv_x[] = { 0,0,1,-1 };
int mv_y[] = { 1,-1,0,0 };

struct node {
	int x;
	int y;
	double f;
	double g;
	double h;
	node* prev;

	node(double h) : x(0), y(0), f(999), g(999), h(h) {
	}
};

double getHeuristic(node q, node t) {
	return (t.y - q.y) * (t.y - q.y) + (t.x - q.x) * (t.x - q.x);
}


struct sorting {
	bool operator()(node n1, node n2) {
		return n1.f > n2.f;
	}
};

priority_queue<node, vector <node>, sorting> ol;
vector<priority_queue<node, vector <node>, sorting>> open_list;

vector<node> a_star(node s, node t, int k);
int main(void){
	
	node s(0), t(0);

	for (int h = 0; h < 5; h++)
		for (int w = 0; w < 5; w++)
			maze[h][w] = 1;
	maze[2][2] = 10;

	s.x = s.y = 1;
	t.x = t.y = 4;

	vector<node> min_path;
	min_path = a_star(s, t, Q_SIZE);

	//if (min_path.empty())
	//	printf("empty");

	while (min_path.empty()) {
		node n(0);
		n = min_path.back();
		printf("(%d, %d)", n.x, n.y);
		min_path.pop_back();
	}
	printf("\n");
}

bool isEmpty(vector<priority_queue<node, vector <node>, sorting>> openL) {
	for (int i = 0; i < Q_SIZE; i++) {
		if (openL[i].empty() != true) {
			return false;
		}
	}
	return false;
}

int selectQ(vector<priority_queue<node, vector <node>, sorting>> openL) {
	int maxSize = 0;
	int maxIdx = 0;
	for (int i = 0; i < Q_SIZE; ++i) {
		if (maxSize < openL[i].size()) {
			maxSize = openL[i].size();
			maxIdx = i;
		}
	}
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, Q_SIZE - 1);
	int randIdx = dis(gen);

	int flag = 1;
	for (int i = 0; i < Q_SIZE; ++i) { // 모든 큐의 길이가 maxSize인지 확인
		if (openL[i].size() < maxSize) {
			flag = 0;
			break;
		}
	}
	if (flag)
		return randIdx; // 모든 큐의 길이가 maxSize이면 그냥 난수 리턴

	while (randIdx == maxIdx || openL[randIdx].size() == maxSize) {
		randIdx = dis(gen);
	}
	return randIdx;
}

vector<node> a_star(node s, node t, int k)
{	
	vector<node> H;
	vector<node> S;
	vector<node> T;

	s.g = 0;
	s.h = getHeuristic(s, t);
	s.f = s.g + s.h;
	s.prev = &s;

	int m_flag = 0;
	int same_flag1 = 0;
	int same_flag2 = 0;
	double f_min = 999;

	node q(0), tmp(0), m(0);

	for (int i = 0; i < Q_SIZE; i++) {
		open_list.push_back(ol);
	}
	open_list[0].push(s);

	while ((isEmpty(open_list)) != true) {
		for (int i = 0; i < Q_SIZE; i++) {
			if (open_list[i].empty()) {
				continue;
			}

			q = open_list[i].top();
			//q.prev = open_list[i].top().prev;
			open_list[i].pop();

			printf("q: (%d %d)\n", q.x, q.y);
			printf("q.prev: (%d %d)\n", (*q.prev).x, (*q.prev).y);
			printf("q.f: %f\n", q.f);

			if (q.x == t.x && q.y == t.y) {
				m = q;
				m_flag = 1;
				continue;
			}
			for (int i = 0; i < 4; ++i) {
				tmp.x = q.x + mv_x[i];
				tmp.y = q.y + mv_y[i];

				if (tmp.x < 0 || tmp.x >= MS)
					continue;
				if (tmp.y < 0 || tmp.y >= MS)
					continue;
				
				tmp.g = q.g + maze[tmp.y][tmp.x];
				tmp.h = getHeuristic(tmp, t);
				tmp.f = tmp.g + tmp.h;
				tmp.prev = &q;

				printf("tmp: (%d %d)\n", tmp.x, tmp.y);          
				printf("tmp.prev: (%d %d)\n", (*tmp.prev).x, (*tmp.prev).y);
				
				S.push_back(tmp);
			}
		}
		
		printf("f_min: %f", f_min);

		if (m_flag && m.f <= f_min) { // 
			vector<node> path;
			//node prev_n(0);
			//path.push_back(m);
			while (&m != &s) {
				
				//node* pre_n;
				//printf("m: (%d %d)", m.x, m.y);
				//printf("m.prev: (%d %d)", (*m.prev).x, (*m.prev).y);
				//pre_n = m.prev;
				path.push_back(m);
				m = *m.prev;
				//path.push_back(prev_n);
				//path.push_back(m);
				
				//m = *m.prev;
			}
			path.push_back(m);
			return path;
		}
		printf("S.size: %d\n", S.size());
		printf("H.size: %d\n", H.size());

		for (int i = 0; i < S.size(); ++i) {
			same_flag1 = 0;
			same_flag2 = 0;
			
			for (int j = 0; j < H.size(); ++j)
				if ((S[i].x == H[j].x) && (S[i].y == H[j].y)) {
					same_flag1 = 1;
					break;
				}
			for (int j = i + 1; j < S.size(); ++j)
				if ((S[i].x == S[j].x) && (S[i].y == S[j].y)) {
					if (S[i].f > S[j].f) {
						same_flag2 = 1;
						break;
					}
				}

			printf("S: (%d %d)\n", S[i].x, S[i].y);
			printf("S.prev: (%d %d)\n", (*S[i].prev).x, (*S[i].prev).y);
			
			if (same_flag1 == 0 && same_flag2 == 0)
				T.push_back(S[i]);
		}
		S.clear();

		printf("T.size: %d\n", T.size());
		
		for (int j = 0; j < T.size(); ++j) {
			// T[j].f = T[j].g + getHeuristic(T[j], t);
			if (T[j].f < f_min)
				f_min = T[j].f;

			printf("T: (%d %d)\n", T[j].x, T[j].y);
			printf("T.prev: (%d %d)\n", (*T[j].prev).x, (*T[j].prev).y);

			int Idx = selectQ(open_list);
			open_list[Idx].push(T[j]);
			H.push_back(T[j]);
			
			//node a(0);
			//a = ol[Idx].top();
			//printf("a: (%d %d)\n", a.x, a.y);
			//printf("a.prev: (%d %d)\n", (*a.prev).x, (*a.prev).y);
		}
		
		T.clear();	
	}
}