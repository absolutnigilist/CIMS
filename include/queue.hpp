# pragma once
#include <vector>
#include <stdexcept>

template<typename T>
class Queue {

	struct Node {
		T data;
		Node* next;
		Node* prev;
		Node(T val) :data(val), next(nullptr), prev(nullptr) {};
	};
	Node* head;
	Node* tail;
	size_t sz;

public:
	Queue() :head(nullptr), tail(nullptr), sz(0) {};
	~Queue() { clear(); }
	void push(T val) {
		Node* node = new Node(val);
		if (!tail)
		{
			head = tail = node;
		}
		else
		{
			tail->next = node;
			node->prev = tail;
			tail = node;
		}
		++sz;
	};
	T pop() {
		if (!head)
		{
			throw std::runtime_error("Queue empty");
		}
		Node* tmp = head;
		T val = head->data;
		head = head->next;
		if (head)
		{
			head->prev = nullptr;
		}
		else
		{
			tail = nullptr;
		}
		delete tmp;
		--sz;
		return val;
	};
	size_t size() const { return sz; };
	bool empty() const { return sz == 0; };

	std::vector<T> to_vector() const {
		std::vector<T> res;
		res.reserve(sz);
		Node* curr = head;
		while (curr)
		{
			res.push_back(curr->data);
			curr = curr->next;
		}
		return res;
	};
	void clear() noexcept{
		Node* p = head;
		while (p)
		{
			Node* next = p->next;
			delete p;
			p = next;
		}
		head = nullptr;   
		tail = nullptr;   
		sz = 0;
	};
};