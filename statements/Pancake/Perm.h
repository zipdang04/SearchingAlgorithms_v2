#pragma once

#include <bits/stdc++.h>
#include "../Problem.h"

class InvalidPermException: public Exception {
	inline static const std::string PREFIX = "InvalidMoveException";
	public:
		InvalidPermException(std::string message): Exception(PREFIX, message) {}
};

class Perm{
	private:
		std::vector<int> values;
		inline void _selfMove(int k) {
			std::reverse(values.begin(), values.begin() + k);
		}
	protected:
		
	public:
		static int SIZE;
		inline static void changeSize(int size) { 
			if (SIZE != 0)
				throw InvalidPermException("Perm SIZE already defined");
			SIZE = size; 
		}

		Perm(std::string tmp) {if (tmp != "NOT_INIT") throw NotImplementedException("Not initialized yet");}
		Perm();
		Perm(int size);
		Perm(std::vector<int> values);

		inline int operator[] (int idx) const;
		inline bool operator != (Perm another) const;
		inline bool operator == (Perm another) const;
		inline bool operator < (Perm another) const;
		inline bool operator > (Perm another) const;
		inline Perm move(int k) {
			Perm newPerm = *this;
			newPerm._selfMove(k);
			return newPerm;
		}

		inline bool isSolved() {
			for (int i = 0; i < SIZE; i++)
				if (values[i] != i) return false;
			return true;
		}

		inline std::string toString() const {
			std::string answer = "[";
			for (int i = 0; i < (int)values.size(); i++) {
				answer += std::to_string(values[i]);
				if (i + 1 < (int)values.size()) answer += ", ";
				else answer += "]";
			}
			return answer;
		}
};

int Perm::SIZE = 0;

template<>
inline double Problem<Perm>::heuristic(Perm);

template<>
std::vector<Transition<Perm>> Problem<Perm>::getAdjacent(Perm);


inline int Perm::operator[] (int idx) const {
	return values[idx];
}
inline bool Perm::operator != (Perm another) const {
	for (int i = 0; i < SIZE; i++)
		if (values[i] != another.values[i])
			return true;
	return false;
}
inline bool Perm::operator == (Perm another) const {
	for (int i = 0; i < SIZE; i++)
		if (values[i] != another.values[i])
			return false;
	return true;
}
inline bool Perm::operator < (Perm another) const {
	for (int i = 0; i < SIZE; i++)
		if (values[i] != another.values[i])
			return values[i] < another.values[i];
	return false;
}
inline bool Perm::operator > (Perm another) const {
	for (int i = 0; i < SIZE; i++)
		if (values[i] != another.values[i])
			return values[i] > another.values[i];
	return false;
}

Perm::Perm() {
	if (SIZE == 0)
		throw InvalidPermException("Perm SIZE not yet defined");
	values = std::vector<int>(SIZE);
	std::iota(values.begin(), values.end(), 0);
}
Perm::Perm(int size) {
	if (size == 0)
		throw InvalidPermException("Init size must be different than 0");
	changeSize(size);

	values = std::vector<int>(SIZE);
	std::iota(values.begin(), values.end(), 0);
}
Perm::Perm(std::vector<int> values) {
	if (SIZE == 0)
		throw InvalidPermException("Perm SIZE not yet defined");
	if (SIZE != (int)values.size())
		throw InvalidPermException("The inputed perm did not match the required size");
	this->values = values;
}

template<>
struct std::hash<Perm> {
	std::size_t operator()(const Perm &p) const
    {
        unsigned long long num = 0;
		for (int i = 0; i < p.SIZE; i++)
			num = (num << 20) | (num << 10) | (num << 5) | (num << 3) | p[i];
		return num;
    }
};