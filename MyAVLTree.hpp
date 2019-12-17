#ifndef __PROJ_THREE_AVL_HPP
#define __PROJ_THREE_AVL_HPP

#include "runtimeexcept.hpp"
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <algorithm>

class ElementNotFoundException : public RuntimeException 
{
public:
	ElementNotFoundException(const std::string & err) : RuntimeException(err) {}
};


template<typename Key, typename Value>
class MyAVLTree
{
private:
	// fill in private member data here
	// If you need to declare private functions, do so here too.

	struct Node {

		Node(Key k, Value v, Node * l, Node * r) : key(k), val(v), left(l), right(r), height(0){};
		Key key;
		Value val;
		Node * left;
		Node * right;
		int height;

	};

	Node * root = nullptr;
	int numElements = 0;


	void deleteNode(Node * n);

	void in(Node * n, std::vector<Key> & list) const;
	void post(Node * n, std::vector<Key> & list) const;
	void pre(Node * n, std::vector<Key> & list) const;

	Node * llRotate(Node * n); 
	Node * rrRotate(Node * n); 
	Node * lrRotate(Node * n);
	Node * rlRotate(Node * n);
	int height(Node * n);
	Node * inserter(Node * n, const Key & k, const Value & v);

public:
	MyAVLTree();

	// In general, a copy constructor and assignment operator
	// are good things to have.
	// For ICS 46, Fall 2019, I am not requiring these. 
	//	MyAVLTree(const MyAVLTree & st);
	//	MyAVLTree & operator=(const MyAVLTree & st);


	// The destructor is, however, required. 
	~MyAVLTree()
	{
		deleteNode(root);
	}

	// size() returns the number of distinct keys in the tree.
	size_t size() const noexcept;

	// isEmpty() returns true if and only if the tree has no values in it. 
	bool isEmpty() const noexcept;

	// contains() returns true if and only if there
	//  is a (key, value) pair in the tree
	//	that has the given key as its key.
	bool contains(const Key & k) const; 

	// find returns the value associated with the given key
	// If !contains(k), this will throw an ElementNotFoundException
	// There needs to be a version for const and non-const MyAVLTrees.
	Value & find(const Key & k);
	const Value & find(const Key & k) const;

	// Inserts the given key-value pair into 
	// the tree and performs the AVL re-balance
	// operation, as described in lecture. 
	// If the key already exists in the tree, 
	// you may do as you please (no test cases in
	// the grading script will deal with this situation)
	void insert(const Key & k, const Value & v);

	// in general, a "remove" function would be here
	// It's a little trickier with an AVL tree
	// and I am not requiring it for Fall 2019's ICS 46.
	// You should still read about the remove operation
	// in your textbook. 

	// The following three functions all return
	// the set of keys in the tree as a standard vector.
	// Each returns them in a different order.
	std::vector<Key> inOrder() const;
	std::vector<Key> preOrder() const;
	std::vector<Key> postOrder() const;

};

template<typename Key, typename Value>
void MyAVLTree<Key,Value>::deleteNode(Node * n)
{
	if (n -> left != nullptr) deleteNode(n -> left);
	if (n -> right != nullptr) deleteNode(n -> right);
	delete n;
}

template<typename Key, typename Value>
void MyAVLTree<Key,Value>::in(Node * n, std::vector<Key> & list) const
{
	if (n == nullptr) return;
	else {
		in(n -> left, list);
		list.push_back(n -> key);
		in (n -> right, list);
	}
}


template<typename Key, typename Value>
void MyAVLTree<Key,Value>::post(Node * n, std::vector<Key> & list) const
{
	if (n == nullptr) return;
	else {
		post(n -> left, list);
		post (n -> right, list);
		list.push_back(n -> key);
	}
}

template<typename Key, typename Value>
void MyAVLTree<Key,Value>::pre(Node * n, std::vector<Key> & list) const
{
	if (n == nullptr) return;
	else {
		list.push_back(n -> key);
		pre(n -> left, list);
		pre(n -> right, list);
	}
}

template<typename Key, typename Value>
typename MyAVLTree<Key, Value>::Node * MyAVLTree<Key,Value>::llRotate(Node * n)
{
	Node * c = n -> left;
	Node * T2 = c -> right;

	c -> right = n;
	n -> left = T2;

	n -> height = 1 + std::max(height(n -> left), height(n -> right));
	c -> height = 1 + std::max(height(c -> left), height(c -> right));

	return c;
}

template<typename Key, typename Value>
typename MyAVLTree<Key, Value>::Node * MyAVLTree<Key,Value>::rrRotate(Node * n)
{
	Node * c = n -> right;
	Node * T1 = c -> left;

	n -> right = T1;
	c -> left = n;

	n -> height = 1 + std::max(height(n -> left), height(n -> right));
	c -> height = 1 + std::max(height(c -> left), height(c -> right));

	return c;
}

template<typename Key, typename Value>
typename MyAVLTree<Key, Value>::Node * MyAVLTree<Key,Value>::rlRotate(Node * n)
{
	Node * c = n -> right;
	Node * z = c -> left;
	Node * T3 = z -> left;
	Node * T4 = z -> right;

	z -> left = n;
	z -> right = c;
	c -> left = T4;
	n -> right = T3;

	n -> height = 1 + std::max(height(n -> left ), height(n -> right));
	c -> height = 1 + std::max(height(c -> left), height(c -> right));
	z -> height = 1 + std::max(height(z -> left), height(z -> right));

	return z;
}

template<typename Key, typename Value>
typename MyAVLTree<Key, Value>::Node * MyAVLTree<Key,Value>::lrRotate(Node * n)
{
	Node * c = n -> left;
	Node * z = c -> right;
	Node * T3 = z -> left;
	Node * T4 = z -> right;

	z -> left = c;
	z -> right = n;
	c -> right = T3;
	n -> left = T4;

	n -> height = 1 + std::max(height(n -> left), height(n -> right));
	c -> height = 1 + std::max(height(c -> left), height(c -> right));
	z -> height = 1 + std::max(height(z -> left), height(z -> right));

	return z;
}

template<typename Key, typename Value>
int MyAVLTree<Key,Value>::height(Node * n)
{

	if (n == nullptr) return -1;
	else return n -> height;
}

template<typename Key, typename Value>
typename MyAVLTree<Key, Value>::Node * MyAVLTree<Key,Value>::inserter(Node * n, const Key & k, const Value & v)
{

	if (n == nullptr) {
		return new Node(k, v, nullptr, nullptr); 
	}
	if (k < n -> key) {
		n -> left = inserter(n -> left, k, v);
	}
	else if (k > n -> key) {
		n -> right = inserter(n -> right, k, v);
	}
	else {
		return n;
	}

	//n is ancestor of newly added node
	n -> height = 1 + std::max(height(n -> left), height(n -> right));
	int diff = height(n -> left) - height(n -> right);

	//left subtree is taller than right
	//newNode is added to left of left
	//left-left case
	if (diff > 1 && k < (n -> left -> key)) {
		return llRotate(n);
	}

	//newNode is added to right of left
	//     0
	//	0
	//	  0
	//left right case
	if (diff > 1 && k > (n -> left -> key)) {
		return lrRotate(n);
	}

	//right subtee is taller than left
	//newNode is added to right of right
	//right right case
	if (diff < -1 && k > (n -> right -> key)) {
		return rrRotate(n);
	}

	//newNode is to left of right
	//right left case
	if (diff < -1 && k < (n -> right -> key)) {
		return rlRotate(n);
	}

	return n;
}


template<typename Key, typename Value>
MyAVLTree<Key,Value>::MyAVLTree()
{
	root = nullptr;
	numElements = 0;
}

template<typename Key, typename Value>
size_t MyAVLTree<Key, Value>::size() const noexcept
{
	return numElements;
}

template<typename Key, typename Value>
bool MyAVLTree<Key, Value>::isEmpty() const noexcept
{
	return numElements == 0;
}


template<typename Key, typename Value>
bool MyAVLTree<Key, Value>::contains(const Key &k) const
{

	Node * tmp = root;

	while (tmp != nullptr) {
		if (k == tmp -> key) return true;
		else if (k > tmp -> key) tmp = tmp -> right;
		else tmp = tmp -> left;
	}
	return false;
}



template<typename Key, typename Value>
Value & MyAVLTree<Key, Value>::find(const Key & k)
{

	Node * tmp = root;

	while (tmp != nullptr) {
		if (k == tmp -> key) return tmp -> val;
		else if (k > tmp -> key) tmp = tmp -> right;
		else tmp = tmp -> left;
	}

	throw ElementNotFoundException("Key not in tree");
}

template<typename Key, typename Value>
const Value & MyAVLTree<Key, Value>::find(const Key & k) const
{

	Node * tmp = root;

	while (tmp != nullptr) {
		if (k == tmp -> key) return tmp -> val;
		else if (k > tmp -> key) tmp = tmp -> right;
		else tmp = tmp -> left;
	}

	throw ElementNotFoundException("Key not in tree");
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::insert(const Key & k, const Value & v)
{
	root = inserter(root, k, v);
	numElements++;
}


template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::inOrder() const 
{
	std::vector<Key> list;
	in(root, list);
	return list; 
}


template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::preOrder() const
{
	std::vector<Key> list;
	pre(root, list);
	return list; 
}




template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::postOrder() const
{
	std::vector<Key> list;
	post(root, list);
	return list; 
}





#endif 
