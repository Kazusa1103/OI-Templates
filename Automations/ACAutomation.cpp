// luogu-judger-enable-o2
#include <iostream>
#include <queue>
#include <cstring>
#include <string>

using namespace std;

class ACAutomation{
private :
	struct Node{
		int cnt;
		Node *ch[26], *fail;
		
		Node() {
			cnt = 0;
			fail = NULL;
			memset(ch, 0, sizeof(ch));
		}
	};
	
	Node *rt;
	
public :
	ACAutomation() {
		rt = new Node();
	}
	
	void Insert(string str) {
		Node *now = rt;
		for (auto s : str) {
			int x = s - 'a';
			if (!now->ch[x]) now->ch[x] = new Node();
			now = now->ch[x];
		}
		now->cnt++;
	}
	
	void GetFail() {
		queue<Node*> q;
		for (int i = 0; i < 26; i++) {
			if (!rt->ch[i]) continue;
			rt->ch[i]->fail = rt;
			q.push(rt->ch[i]);
		}
		while (!q.empty()) {
			Node *now = q.front(); q.pop();
			for (int i = 0; i < 26; i++) {
				if (!now->ch[i]) continue;
				q.push(now->ch[i]);
				Node *p = now->fail;
				while (p) {
					if (p->ch[i]) {
						now->ch[i]->fail = p->ch[i];
						break;
					}
					p = p->fail;
				}
				if (!p) now->ch[i]->fail = rt;
			}
		}
	}
	
	int Find(string str) {
		int ret = 0;
		Node *now = rt;
		for (auto s : str) {
			int x = s - 'a';
			while (now != rt && !now->ch[x]) now = now->fail;
			now = now->ch[x];
			if (!now) now = rt;
			for (Node *p = now; p != rt; p = p->fail) {
				if (p->cnt) {
					ret += p->cnt;
					p->cnt = 0;
				} else {
					break;
				}
			}
		}
		return ret;
	}
};

int main() {
	int n;
	cin >> n;
	string str;
	ACAutomation ac;
	for (int i = 1; i <= n; i++) {
		cin >> str;
		ac.Insert(str);
	}
	ac.GetFail();
	cin >> str;
	cout << ac.Find(str) << endl;
	return 0;
}
