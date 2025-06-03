#pragma once

#include <bits/stdc++.h>

// TODO: improve 

struct DEdge { int u; double w; };
class Perm {
	private:
		std::vector<int> perm;
		std::vector<bool> used;
		inline void _selfAdd(const int node) {
			assert((not used[node]));
			perm.push_back(node); used[node] = true;
		}
		inline void _selfPop() {
			int u = *perm.rbegin(); used[u] = false;
			perm.pop_back();
		}
	public:
		inline static const int START = 0;
		static int SIZE;
		static inline void changeSize(int size) { SIZE = size; }

		Perm(std::vector<int> perm = {START}): perm(perm) {
			used = std::vector<bool>(SIZE, false);
			for (int i: perm) used[i] = true;
		}

		inline int operator[] (int idx) const { return perm[idx]; }
		inline int get(int idx) const { return perm[idx]; }
		inline int size() const { return perm.size(); }
		inline int getHead() const { return *(perm.rbegin()); }
		inline bool isUsed(int node) const { return used[node]; }

		inline Perm add(const int node) const {
			Perm newPerm = *this;
			newPerm._selfAdd(node);
			return newPerm;
		}
		inline Perm pop() const {
			Perm newPerm = *this;
			newPerm._selfPop();
			return newPerm;
		}
		inline bool isSolved() const { return size() == SIZE; }

		inline bool operator != (Perm node) const;
		inline bool operator == (Perm node) const;
		inline bool operator < (Perm node) const;
		inline bool operator > (Perm node) const;

		inline std::string toString() const {
			std::string answer = "[";
			for (int i = 0; i < (int)perm.size(); i++) {
				answer += std::to_string(perm[i]);
				if (i + 1 < (int)perm.size()) answer += ", ";
				else answer += "]";
			}
			return answer;
		}
};
int Perm::SIZE = -1;

inline bool Perm::operator == (Perm node) const{
	if (size() != node.size()) return false;
	for (int i = 0; i < node.size(); i++)
		if (get(i) != node[i]) return false;
	return true;
}
inline bool Perm::operator != (Perm node) const {
	if (size() != node.size()) return true;
	for (int i = 0; i < node.size(); i++)
		if (get(i) != node[i]) return true;
	return false;
}
inline bool Perm::operator < (Perm node) const{
	for (int i = 0; i < (int)perm.size() and i < (int)node.size(); i++) 
		if (get(i) != node[i]) 
			return get(i) < node[i];
	return size() < node.size();
}
inline bool Perm::operator > (Perm node) const{
	for (int i = 0; i < (int)perm.size() and i < (int)node.size(); i++) 
		if (get(i) != node[i]) 
			return get(i) > node[i];
	return size() > node.size();
}


template<>
struct std::hash<Perm> {
	std::size_t operator()(const Perm &p) const
    {
        unsigned long long num = 0;
		for (int i = 0; i < p.size(); i++)
			num = (num << 20) | (num << 10) | (num << 5) | (num << 3) | p[i];
		return num;
    }
};