#include <iostream>
#include <cstdio>
#include <cassert>

using namespace std;
const int MAXN = 1e5 + 5;

int n, m;
int a[MAXN];

struct Node{
	int val, sum, rev;
	Node *fa, *ch[2];

	Node(int val) : val(val), sum(val) {
		rev = 0;
		fa = NULL;
		ch[0] = ch[1] = NULL;
	}
};

Node *rt[MAXN];

int IsRoot(Node *now) {
	return (!now->fa) || (now != now->fa->ch[0] && now != now->fa->ch[1]);
}

void Update(Node *now) {
	assert(now);
	now->sum = now->val ^ (now->ch[0] ? now->ch[0]->sum : 0) ^ (now->ch[1] ? now->ch[1]->sum : 0);
}

void PushDown(Node *now) {
	if (!now->rev) return;
	swap(now->ch[0], now->ch[1]);
	if (now->ch[0]) now->ch[0]->rev ^= 1;
	if (now->ch[1]) now->ch[1]->rev ^= 1;
	now->rev = 0;
}

void PushAll(Node *now) {
	if (!IsRoot(now)) {
		PushAll(now->fa);
	}
	PushDown(now);
}

void Rotate(Node *now) {
	Node *fa = now->fa, *anc = fa->fa;
	PushDown(fa); PushDown(now);
	int d = (now == fa->ch[1]);
	if (!IsRoot(fa)) anc->ch[fa == anc->ch[1]] = now;
	now->fa = anc;
	fa->fa = now;
	fa->ch[d] = now->ch[!d];
	now->ch[!d] = fa;
	if (fa->ch[d]) fa->ch[d]->fa = fa;
	Update(fa); Update(now);
}

void Splay(Node *now) {
	if (!now) return;
	PushAll(now);
	while (!IsRoot(now)) {
		Node *fa = now->fa, *anc = fa->fa;
		if (!IsRoot(fa)) {
			if ((now == fa->ch[1]) ^ (fa == anc->ch[1])) Rotate(now);
			else Rotate(fa);
		}
		Rotate(now);
	}
}

void Access(Node *now) {
	Node *las = NULL;
	while (now) {
		Splay(now);
		now->ch[1] = las;
		Update(now);
		las = now;
		now = now->fa;
	}
}

void MakeRoot(Node *now) {
	Access(now);
	Splay(now);
	now->rev ^= 1;
}

Node *FindRoot(Node *now) {
	Access(now);
	Splay(now);
	PushDown(now);
	while (now->ch[0]) {
		now = now->ch[0];
		PushDown(now);
	}
	Splay(now);
	return now;
}

void Split(Node *x, Node *y) {
	MakeRoot(x);
	Access(y);
	Splay(y);
}

void Link(Node *x, Node *y) {
	MakeRoot(x);
	if (FindRoot(y) == x) return;
	x->fa = y;
}

void Cut(Node *x, Node *y) {
	MakeRoot(x);
	if (FindRoot(y) != x || y->fa != x || y->ch[0]) return;
	y->fa = x->ch[1] = NULL;
	Update(x);
}

void Modify(Node *now, int k){
	Splay(now);
	now->val = k;
	Update(now);
}

int Query(Node *x, Node *y) {
	Split(x, y);
	return y->sum;
}

int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) rt[i] = new Node(a[i]);
	while (m--) {
		int op, x, y;
		cin >> op >> x >> y;
		if (op == 0) {
			int ans = Query(rt[x], rt[y]);
			cout << ans << "\n";
		} else if (op == 1) {
			Link(rt[x], rt[y]);
		} else if (op == 2) {
			Cut(rt[x], rt[y]);
		} else {
			Modify(rt[x], y);
		}
	}
	return 0;
}
/*
3 3 
1
2
3
1 1 2
0 1 2 
0 1 1
*/

