#pragma once

class Node {
	private:
		int idx;
	public:
		static int GOAL;
		static inline void changeGoal(int goal) { GOAL = goal; }

		Node() {idx = -1;}
		Node(int idx): idx(idx) {}
		inline int get() const {return idx;}
		inline bool isSolved() {return idx == GOAL;}

		inline bool operator != (Node node) const { return idx != node.get(); }
		inline bool operator == (Node node) const { return idx == node.get(); }
		inline bool operator < (Node node) const { return idx < node.get(); }
		inline bool operator > (Node node) const { return idx > node.get(); }
};

int Node::GOAL = -1;

template<>
struct std::hash<Node> {
	std::size_t operator()(const Node &p) const
    {
        return std::hash<int>()(p.get());
    }
};