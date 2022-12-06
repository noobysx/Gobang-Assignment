#include "Chess.h"
#include "framework.h"
Trie::Trie() {
	exist = -1;
	for (int i = 0; i < 3; ++i) next[i] = nullptr;
	fail = nullptr;
}

void Trie::insert(const char* str, int pos, int id) {
	if (pos == strlen(str)) {
		exist = id; return;
	}
	if (next[str[pos] - '0'] == nullptr)
		next[str[pos] - '0'] = new Trie;
	next[str[pos] - '0']->insert(str, pos + 1, id);
}

AI::AI() {
	//初始化AC自动机
	match = new Trie;
	match->fail = nullptr;
	std::queue<Trie*> Q;
	Q.emplace(match);
	while (!Q.empty()) {
		Trie* tmp = Q.front(); Q.pop();
		for (int i = 0; i < 3; ++i) {
			if (tmp->next[i]) {
				Q.emplace(tmp->next[i]);
				Trie* fafail = tmp->fail;
				while (fafail && fafail->next[i] == nullptr) fafail = fafail->fail;
				if (fafail == nullptr) tmp->next[i]->fail = match;
				else tmp->next[i]->fail = fafail->next[i];
			}
		}
	}
}

void AI::Query(const char* str, Trie* head, int* nums) {
	int len = strlen(str);
	for (int i = 0; i < len; ++i) {
		int c = str[i] - '0';
		while (head->next[c] == NULL && head->fail) head = head->fail;
		if (head->next[c]) head = head->next[c];
		else continue;
		if (head->exist != -1) nums[head->exist]++;
	}
}