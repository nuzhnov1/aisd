// BST.h
#pragma once
#ifndef _INC_BST_
#define _INC_BST_
// ������������ ������������ �����
#include <iostream>
#include <string>
#include <stack>


//************************************************************ ����� ���������� *******************************************************************//
class BST_exception
{

	using string = std::string;

	string error;

public:

	BST_exception(const string& str) : error(str) {}

	string what() const { return this->error; }

};
//*************************************************************************************************************************************************//


//**************************************************** ����� ��� "�������� ������ ������" *********************************************************//
template <class Key_T, class Data_T>
class BST
{
	// ����� - ���� BST-������
	struct Node
	{

		Key_T key;		// ���� ����
		Data_T data;	// ������ ����
		Node* left;		// ��������� �� ����� ���������
		Node* right;	// ��������� �� ������ ���������

		// ����������� ���� � �����������
		Node(const Key_T& key, const Data_T& data) : key(key), data(data), left(nullptr), right(nullptr) {}

	};

	size_t size;			// ���������� ��������� � ������
	Node* root;				// ��������� �� ������ ������
	mutable size_t last_op;	// ���������� ��������� �� ��������� ��������

	// ���� ������������ �����������
	void _copy(const BST& tree);

public:

	class Iterator;
	class RIterator;

	// ����������� �� ���������
	BST() : size(0), root(nullptr), last_op(0) {}
	// ����������� �����������
	BST(const BST& tree) { _copy(tree); }
	// ����������
	~BST() { clear(); }

	// �������� ������� � �������� �� �����
	Data_T& operator[](const Key_T& key);
	const Data_T& operator[](const Key_T& key) const { return const_cast<BST&>(*this)[key]; }
	// �������� ������������ ��������
	const BST& operator=(const BST& tree);

	// �������� ������ ������
	size_t getSize() const { return this->size; }
	// �������� ���������� ��������� �� ��������� ��������
	size_t getLastOp() const { return this->last_op; }
	// �������� ����� �������� ���� ������ �� ����� O(n)
	size_t getExternalPath() const;
	// �������� ������ �� �������
	bool isEmpty() const { return this->size == 0; }

	// ������ �������� ������ �� �����
	const Data_T& getItemByKey(const Key_T& key) const { return const_cast<BST&>(*this)[key]; }
	// ������ �������� �� ��������� �����
	void setItemByKey(const Key_T& key, const Data_T& data) { (*this)[key] = data; }

	// ������� ������
	void clear();
	// �������� ������� � �������� ������ � ������
	void append(const Key_T& key, const Data_T& data);
	// ������� ������� � �������� ������
	void remove(const Key_T& key);
	
	// ������� ������ � ����������� �����
	void print() const;
	// ������� ������ � ����� �������: ����-��������
	void printAsDict() const;
	// ������� ����� ������ � ������������ �������
	void printKeys() const;

	// ��������� ��������� �� ����������� ������� ������
	Iterator begin() const;
	// ��������� ��������� � "���������������" ���������
	Iterator end() const { return Iterator(*this); }
	// ��������� ��������� �������� �� ������������ ������� ������
	RIterator rbegin() const;
	// ��������� ��������� ��������� � "���������������" ���������
	RIterator rend() const { return RIterator(*this); }

	// ����� ������� ���������
	class Iterator
	{

		friend class BST;

		const BST* tree;	// ��������� �� ������� BST-������
		const Node* cur;	// ������� ����, �� ������� ���������� ��������

		// private-����������� ������� ��������� � �����������: tree - ��������� �� ������, node - ��������� �� ����
		Iterator(const BST* tree, const Node* node) : tree(tree), cur(node) {}

	public:

		// ����������� ������� ��������� �� ���������
		Iterator() : tree(nullptr), cur(nullptr) {}
		// ����������� ������� ��������� � ���������� tree - ������� �� BST-������
		Iterator(const BST& tree) : tree(&tree), cur(nullptr) {}
		// ����������� ������� ��������� � �����������: tree - ������ �� BST-������, key - ������
		Iterator(const BST& tree, const Key_T& key);
		// ����������� ����������� ������� ���������
		Iterator(const Iterator& iter) : tree(iter.tree), cur(iter.cur) {}

		// �������� "���������������" - ��������� �������� �������� ���� ������ �� ������
		const Key_T& operator*() const;

		// �������� ����������� ����������������� ������� ���������
		Iterator& operator++();
		// �������� ����������� ����������������� ������� ���������
		Iterator& operator--();
		// �������� ������������ ����������������� ������� ���������
		Iterator operator++(int);
		// �������� ������������ ����������������� ������� ���������
		Iterator operator--(int);

		// �������� ��������� ������ ����������
		bool operator==(const Iterator& iter) const;
		// �������� ����������� ������ ����������
		bool operator!=(const Iterator& iter) const;

		// �������� ������������ ������� ���������
		const Iterator& operator=(const Iterator& iter);

	};

	// ����� ��������� ���������
	class RIterator
	{

		friend class BST;

		const BST* tree;	// ��������� �� ������� BST-������
		const Node* cur;	// ������� ����, �� ������� ���������� ��������

		// private-����������� ��������� ��������� � �����������: tree - ��������� �� ������, node - ��������� �� ����
		RIterator(const BST* tree, const Node* node) : tree(tree), cur(node) {}

	public:

		// ����������� ��������� ��������� �� ���������
		RIterator() : tree(nullptr), cur(nullptr) {}
		// ����������� ��������� ��������� � ���������� tree - ������� �� BST-������
		RIterator(const BST& tree) : tree(&tree), cur(nullptr) {}
		// ����������� ��������� ��������� � �����������: tree - ������ �� BST-������, key - ����
		RIterator(const BST& tree, const Key_T& key);
		// ����������� ����������� ��������� ���������
		RIterator(const RIterator& riter) : tree(riter.tree), cur(riter.cur) {}

		// �������� "���������������" - ��������� �������� �������� ���� ������ �� ������
		const Key_T& operator*() const;
		// �������� ����������� ����������������� ��������� ���������
		RIterator& operator++();
		// �������� ����������� ����������������� ��������� ���������
		RIterator& operator--();
		// �������� ������������ ����������������� ��������� ���������
		RIterator operator++(int);
		// �������� ������������ ����������������� ��������� ���������
		RIterator operator--(int);

		// �������� ��������� �������� ����������
		bool operator==(const RIterator& riter) const;
		// �������� ����������� �������� ����������
		bool operator!=(const RIterator& riter) const;

		// �������� ������������ ��������� ���������
		const RIterator& operator=(const RIterator& riter);

	};

};
//*************************************************************************************************************************************************//


//************************************************************* ���������� ������� BST ************************************************************//
// private-������:
template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::_copy(const BST& tree)
{
	// ������� ������ tree � ������ ������ this ���������, �������� ����-����� ������ tree ��� ������ this

	using std::stack;

	Node* tree_cur = tree.root, * this_cur = nullptr;	// ��������� �� ������� ���� tree � this 
	stack<Node*> tree_stk, this_stk;					// ��������������� ����� ��� ������ ��������

	this->root = nullptr;
	this->size = 0;
	this->last_op = 0;

	// ���� tree ���������� �����, �� ������ ������ ������
	if (tree_cur == nullptr)
		return;
	// ����� �������� ������ � ���������� �����
	else
		this_cur = this->root = new Node(*tree_cur);

	// ������� ������ tree � ������ ������ this, �������� ��������������� ���� tree � ��������������� ���� this
	tree_stk.push(tree_cur);
	this_stk.push(this_cur);
	while (!tree_stk.empty())
	{
		tree_cur = tree_stk.top();
		tree_stk.pop();

		this_cur = this_stk.top();
		this_stk.pop();

		if (tree_cur->right != nullptr)
		{
			this_cur->right = new Node(*tree_cur->right);
			tree_stk.push(tree_cur->right);
			this_stk.push(this_cur->right);
		}
		if (tree_cur->left != nullptr)
		{
			this_cur->left = new Node(*tree_cur->left);
			tree_stk.push(tree_cur->left);
			this_stk.push(this_cur->left);
		}
	}

	this->size = tree.size;
}


// public-������:
template <class Key_T, class Data_T> typename Data_T& BST<Key_T, Data_T>::operator[](const Key_T& key)
{
	// ������� ������ � ������ �������� � ������, ������ ���������

	Node* cur = this->root;

	this->last_op = 1;

	while (cur != nullptr && key != cur->key)
	{
		if (key < cur->key)
			cur = cur->left;
		else
			cur = cur->right;

		this->last_op++;
	}

	if (cur == nullptr)
		throw BST_exception("�������� � ����� ������ ��� � BST-������");
	else
		return cur->data;
}

template <class Key_T, class Data_T> const typename BST<Key_T, Data_T>& BST<Key_T, Data_T>::operator=(const BST& tree)
{
	if (this == &tree)
		return *this;

	clear();
	_copy(tree);

	return *this;
}

template <class Key_T, class Data_T> typename size_t BST<Key_T, Data_T>::getExternalPath() const
{
	// ������� ������ ��������, ������ ������� ���� � ��������� �� ������

	using std::stack;

	Node* cur = this->root;					// ������� ���� ������
	size_t cur_level = 0;					// ������� ������� ��������
	size_t result = 0;						// ����� �������� ����
	stack<Node*> stk1, stk2;				// ��������������� ����� �����
	stack<size_t> level_stk1, level_stk2;	// ��������������� ����� �������

	this->last_op = 1;

	if (cur == nullptr)
		return result;

	stk1.push(cur);
	level_stk1.push(cur_level);
	while (!stk1.empty() || !stk2.empty())
	{
		if (!stk1.empty())
		{
			cur = stk1.top();
			stk1.pop();
			cur_level = level_stk1.top();
			level_stk1.pop();

			stk2.push(cur);
			level_stk2.push(cur_level);

			if (cur->right != nullptr)
			{
				stk1.push(cur->right);
				level_stk1.push(cur_level + 1);
			}
		}
		else
		{
			cur = stk2.top();
			stk2.pop();
			cur_level = level_stk2.top();
			level_stk2.pop();

			// ���� ���� �������, �� ��������� ��� ������� � ����������
			if (cur->left == nullptr || cur->right == nullptr)
				result += cur_level;

			if (cur->left != nullptr)
			{
				stk1.push(cur->left);
				level_stk1.push(cur_level + 1);
			}
		}

		this->last_op++;
	}

	return result;
}

template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::clear()
{
	// ������� ������ ����������, �������� ���������� ������� � ���� stk2,
	// � ����� ������� ���, ���� �������� ������ ������� � �����

	using std::stack;

	Node* cur = this->root;		// ������� ����
	stack<Node*> stk1, stk2;	// ��������������� ����� ��� ������ ������

	// ���� ������ �����, �� �������
	if (cur == nullptr)
		return;

	// ������� ������, �������� ���� ������
	stk1.push(cur);
	while (!stk1.empty())
	{
		cur = stk1.top();
		stk1.pop();

		stk2.push(cur);

		if (cur->left != nullptr)
			stk1.push(cur->left);
		if (cur->right != nullptr)
			stk1.push(cur->right);
	}

	// ������� �� �����, ����������� ������ �� ��� ����� ������ � ������� �� �� �����
	while (!stk2.empty())
	{
		cur = stk2.top();
		stk2.pop();

		delete cur;
	}

	this->root = nullptr;
	this->size = 0;
}

template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::append(const Key_T& key, const Data_T& data)
{
	// ���� ������� ������ � ������ key, ���� �� ����, �� ����������� ����������, ���� ��� ��� �� ��������� ����� ��� ������
	// � ����������� �� key � ����� ����������� ����, �� �������� �����. ���� key ������ ����� � ���� ����, �� ��������� �����,
	// ���� ������, �� ������

	Node* cur = this->root; // ������� �������

	this->last_op = 1;

	// ���� ������ ���������� �����, �� ��������� ������� � ������ ������
	if (cur == nullptr)
	{
		this->root = new Node(key, data);
		this->size++;

		return;
	}

	// ����� ��������� ������� ��� �������� � ��� �� ������
	while (true)
	{
		// ���� ������� � ����� ������ ��� ���� � ������, �� ����������� ����������
		if (key == cur->key)
			throw BST_exception("������� � ����� ������ ��� ���� � BST-������");
		if (key < cur->key)
			if (cur->left != nullptr)
				cur = cur->left;
			else
				break;
		else
			if (cur->right != nullptr)
				cur = cur->right;
			else
				break;

		this->last_op++;
	}

	// ���������� �������� � ������ � ����������� �� ��������� �����
	if (key < cur->key)
		cur->left = new Node(key, data);
	else
		cur->right = new Node(key, data);

	this->size++;
}

template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::remove(const Key_T& key)
{
	// ��� �������� �������� �� ������ ������� ���� ������� � �������� ������, ���� ������ �������� ���,
	// �� ����������� ����������, ����� ������� �������

	Node* cur = this->root;	// ������� ������� ������
	Node* par = nullptr;	// ������������ ���� ��� ��������

	this->last_op = 1;

	// ����� ��������
	while (cur != nullptr && key != cur->key)
	{
		par = cur;

		if (key < cur->key)
			cur = cur->left;
		else
			cur = cur->right;

		this->last_op++;
	}

	// ���� ������� �� ������, �� ����������� ����������
	if (cur == nullptr)
		throw BST_exception("�������� � ����� ������ ��� � BST-������");

	// ������ ��������� �������� ��� �������� ���������� ��������
	// ������ - ������� �� �������� ��������
	if (cur->left == nullptr && cur->right == nullptr)
	{
		if (cur == this->root) // ���� ��������� ������� - ������ ������
			this->root = nullptr;
		else
			// ������������ �����, ������� ��� � ������� �������� ����������� nullptr
			if (par->left == cur)
				par->left = nullptr;
			else
				par->right = nullptr;
	}
	// ������ - ������� �������� ������ ������� �������
	else if (cur->left == nullptr)
	{
		if (cur == this->root)
			this->root = this->root->right;
		else
		{
			// ������������ ����� ����������� ������ ����� ���������� ��������
			if (par->left == cur)
				par->left = cur->right;
			else
				par->right = cur->right;
		}
	}
	// ������ - ������� �������� ������ ������ �������
	else if (cur->right == nullptr)
	{
		if (cur == this->root)
			this->root = this->root->left;
		else
		{
			// ������������ ����� ����������� ����� ����� ���������� ��������
			if (par->left == cur)
				par->left = cur->left;
			else
				par->right = cur->left;
		}
	}
	// �������� - ������� �������� ��� �������
	else
	{
		Node* del = cur; // ��������� �� ��������� �������

		cur = cur->right; // ��������� � ����� ������ ��������. ������ cur - ��������� �� ���������� �������
		this->last_op++;

		// ��������� � �������� � ����������� ������ ����� �������� cur
		while (cur->left != nullptr)
		{
			par = cur;
			cur = cur->left;

			this->last_op++;
		}

		// ���� � ������ ����������� �������� ����������� �������������� ��������� ���������� ��������
		del->key = cur->key;
		del->data = cur->data;
		if (del->right == cur)  // ���� ���������� ������� - ��� ������ ������ ��������
			del->right = cur->right; // �� ��������� ������ ����� ���������� �������� � ������ ������ �����������
		else
			par->left = cur->right; // ����� ����� ����� �������� ����������� �������� ����������� ������ ����� ����������� ��������
	}

	delete cur;

	this->size--;
	this->last_op++;
}

template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::print() const
{
	// ������� ������ ��������, ������ � ������� ��������� ������

	using std::stack;
	using std::cout;
	using std::endl;

	Node* cur = this->root;		// ������� ���� ������
	size_t cur_level = 0;		// ������� ������� ��������
	stack<Node*> stk1, stk2, stk3;						// ��������������� ����� �����
	stack<size_t> level_stk1, level_stk2, level_stk3;	// ��������������� ����� �������

	if (cur == nullptr)
	{
		cout << endl;

		return;
	}

	stk1.push(cur);
	level_stk1.push(cur_level);
	while (!stk1.empty() || !stk2.empty())
	{
		if (!stk1.empty())
		{
			cur = stk1.top();
			stk1.pop();
			cur_level = level_stk1.top();
			level_stk1.pop();

			stk2.push(cur);
			level_stk2.push(cur_level);

			if (cur->left != nullptr)
			{
				stk1.push(cur->left);
				level_stk1.push(cur_level + 1);
			}
		}
		else
		{
			cur = stk2.top();
			stk2.pop();
			cur_level = level_stk2.top();
			level_stk2.pop();

			stk3.push(cur);
			level_stk3.push(cur_level);

			if (cur->right != nullptr)
			{
				stk1.push(cur->right);
				level_stk1.push(cur_level + 1);
			}
		}
	}

	while (!stk3.empty())
	{
		cur = stk3.top();
		stk3.pop();
		cur_level = level_stk3.top();
		level_stk3.pop();

		if (cur_level > 0)
		{
			for (size_t i = 0; i < cur_level - 1; i++)
				cout << "  ";
			cout << "--";
		}
		cout << cur->key << endl << endl;
	}
}

template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::printAsDict() const
{
	// ������� ������ ��������, ������ � ������� ��������� ������ � ���� �������

	using std::stack;
	using std::cout;
	using std::endl;

	Node* cur = this->root;			// ������� ���� ������
	stack<Node*> stk1, stk2, stk3;	// ��������������� ����� �����

	if (cur == nullptr)
	{
		cout << "{}" << endl;

		return;
	}

	stk1.push(cur);
	while (!stk1.empty() || !stk2.empty())
	{
		if (!stk1.empty())
		{
			cur = stk1.top();
			stk1.pop();

			stk2.push(cur);

			if (cur->right != nullptr)
				stk1.push(cur->right);
		}
		else
		{
			cur = stk2.top();
			stk2.pop();

			stk3.push(cur);

			if (cur->left != nullptr)
				stk1.push(cur->left);
		}
	}

	cout << "{";
	while (stk3.size() > 1)
	{
		cur = stk3.top();
		stk3.pop();

		cout << cur->key << ": " << cur->data << ", ";
	}
	cur = stk3.top();
	stk3.pop();

	cout << cur->key << ": " << cur->data << "}" << endl;
}

template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::printKeys() const
{
	// ������� ������ ��������, ������ � ������� ������ ������ ������

	using std::stack;
	using std::cout;
	using std::endl;

	Node* cur = this->root;			// ������� ���� ������
	stack<Node*> stk1, stk2, stk3;	// ��������������� ����� �����

	if (cur == nullptr)
	{
		cout << "[]" << endl;
		cout << endl;
		return;
	}

	stk1.push(cur);
	while (!stk1.empty() || !stk2.empty())
	{
		if (!stk1.empty())
		{
			cur = stk1.top();
			stk1.pop();

			stk2.push(cur);

			if (cur->right != nullptr)
				stk1.push(cur->right);
		}
		else
		{
			cur = stk2.top();
			stk2.pop();

			stk3.push(cur);

			if (cur->left != nullptr)
				stk1.push(cur->left);
		}
	}

	cout << "[";
	while (stk3.size() > 1)
	{
		cur = stk3.top();
		stk3.pop();

		cout << cur->key << ", ";
	}
	cur = stk3.top();
	stk3.pop();

	cout << cur->key << "]" << endl;
	cout << endl;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::Iterator BST<Key_T, Data_T>::begin() const
{
	Node* cur = this->root;

	if (cur == nullptr)
		return Iterator(*this);

	// ���� ������� ������ � ����������� ������ � ������������� �������� �� ����
	while (cur->left != nullptr)
		cur = cur->left;

	return Iterator(this, cur);
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::RIterator BST<Key_T, Data_T>::rbegin() const
{
	Node* cur = this->root;

	if (cur == nullptr)
		return RIterator(*this);

	// ���� ������� ������ � ������������ ������ � ������������� �������� �� ����
	while (cur->right != nullptr)
		cur = cur->right;

	return RIterator(this, cur);
}
//*************************************************************************************************************************************************//


//********************************************************** ���������� ������� Iterator **********************************************************//
template <class Key_T, class Data_T> BST<Key_T, Data_T>::Iterator::Iterator(const BST& tree, const Key_T& key)
{
	Node* cur = tree.root;

	this->tree = &tree;

	// ���� ������� � �������� ������
	while (cur != nullptr && key != cur->key)
	{
		if (key < cur->key)
			cur = cur->left;
		else
			cur = cur->right;
	}

	this->cur = cur;
}

template <class Key_T, class Data_T> const typename Key_T& BST<Key_T, Data_T>::Iterator::operator*() const
{
	if (this->cur == nullptr)
		throw BST_exception("�������� �� ����������");
	else
		return this->cur->key;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::Iterator& BST<Key_T, Data_T>::Iterator::operator++()
{
	if (this->cur == nullptr)
		throw BST_exception("�������� �� ����������");

	// ���� � ������� ���� ���� ������ �������, �� ��������� � ����
	if (this->cur->right != nullptr)
	{
		this->cur = this->cur->right;
		// ���� � ����� ������� ���� ����� �����, �� �������� �� ��� �� �����
		while (this->cur->left != nullptr)
			this->cur = this->cur->left;
	}
	// ����� ���� ���������� ������ ��������
	else
	{
		Node* left_par = nullptr;		// ��������� ����� ��������
		Node* cur = this->tree->root;	// ������� ���� ������

		while (cur->key != this->cur->key)
		{
			if (this->cur->key < cur->key)
			{
				left_par = cur;
				cur = cur->left;
			}
			else
				cur = cur->right;
		}

		this->cur = left_par;
	}

	return *this;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::Iterator& BST<Key_T, Data_T>::Iterator::operator--()
{
	if (this->cur == nullptr)
		throw BST_exception("�������� �� ����������");

	// ���� � ������� ���� ���� ����� �������, �� ��������� � ����
	if (this->cur->left != nullptr)
	{
		this->cur = this->cur->left;
		// ���� � ����� ������� ���� ������ �����, �� �������� �� ��� �� �����
		while (this->cur->right != nullptr)
			this->cur = this->cur->right;
	}
	// ����� ���� ���������� ������� ��������
	else
	{
		Node* right_par = nullptr;		// ��������� ������ ��������
		Node* cur = this->tree->root;	// ������� ���� ������

		while (cur->key != this->cur->key)
		{
			if (this->cur->key < cur->key)
				cur = cur->left;
			else
			{
				right_par = cur;
				cur = cur->right;
			}
		}

		this->cur = right_par;
	}

	return *this;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::Iterator BST<Key_T, Data_T>::Iterator::operator++(int)
{
	Iterator temp = *this;

	++(*this);

	return temp;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::Iterator BST<Key_T, Data_T>::Iterator::operator--(int)
{
	Iterator temp = *this;

	--(*this);

	return temp;
}

template <class Key_T, class Data_T> typename bool BST<Key_T, Data_T>::Iterator::operator==(const Iterator& iter) const
{
	return this->tree == iter.tree && this->cur == iter.cur;
}

template <class Key_T, class Data_T> typename bool BST<Key_T, Data_T>::Iterator::operator!=(const Iterator& iter) const
{
	return !(this->tree == iter.tree && this->cur == iter.cur);
}

template <class Key_T, class Data_T> const typename BST<Key_T, Data_T>::Iterator& BST<Key_T, Data_T>::Iterator::operator=(const Iterator& iter)
{
	if (this == &iter)
		return *this;

	this->tree = iter.tree;
	this->cur = iter.cur;

	return *this;
}
//*************************************************************************************************************************************************//


//********************************************************** ���������� ������� RIterator *********************************************************//
template <class Key_T, class Data_T> BST<Key_T, Data_T>::RIterator::RIterator(const BST& tree, const Key_T& key)
{
	Node* cur = tree.root;

	this->tree = &tree;

	// ���� ������� � �������� ������
	while (cur != nullptr && key != cur->key)
	{
		if (key < cur->key)
			cur = cur->left;
		else
			cur = cur->right;
	}

	this->cur = cur;
}

template <class Key_T, class Data_T> const typename Key_T& BST<Key_T, Data_T>::RIterator::operator*() const
{
	if (this->cur == nullptr)
		throw BST_exception("�������� �� ����������");
	else
		return this->cur->key;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::RIterator& BST<Key_T, Data_T>::RIterator::operator++()
{
	if (this->cur == nullptr)
		throw BST_exception("�������� �� ����������");

	// ���� � ������� ���� ���� ����� �������, �� ��������� � ����
	if (this->cur->left != nullptr)
	{
		this->cur = this->cur->left;
		// ���� � ����� ������� ���� ������ �����, �� �������� �� ��� �� �����
		while (this->cur->right != nullptr)
			this->cur = this->cur->right;
	}
	// ����� ���� ���������� ������� ��������
	else
	{
		Node* right_par = nullptr;		// ��������� ������ ��������
		Node* cur = this->tree->root;	// ������� ���� ������

		while (cur->key != this->cur->key)
		{
			if (this->cur->key < cur->key)
				cur = cur->left;
			else
			{
				right_par = cur;
				cur = cur->right;
			}
		}

		this->cur = right_par;
	}

	return *this;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::RIterator& BST<Key_T, Data_T>::RIterator::operator--()
{
	if (this->cur == nullptr)
		throw BST_exception("�������� �� ����������");

	// ���� � ������� ���� ���� ������ �������, �� ��������� � ����
	if (this->cur->right != nullptr)
	{
		this->cur = this->cur->right;
		// ���� � ����� ������� ���� ����� �����, �� �������� �� ��� �� �����
		while (this->cur->left != nullptr)
			this->cur = this->cur->left;
	}
	// ����� ���� ���������� ������ ��������
	else
	{
		Node* left_par = nullptr;		// ��������� ����� ��������
		Node* cur = this->tree->root;	// ������� ���� ������

		while (cur->key != this->cur->key)
		{
			if (this->cur->key < cur->key)
			{
				left_par = cur;
				cur = cur->left;
			}
			else
				cur = cur->right;
		}

		this->cur = left_par;
	}

	return *this;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::RIterator BST<Key_T, Data_T>::RIterator::operator++(int)
{
	RIterator temp = *this;

	++(*this);

	return temp;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::RIterator BST<Key_T, Data_T>::RIterator::operator--(int)
{
	RIterator temp = *this;

	--(*this);

	return temp;
}

template <class Key_T, class Data_T> typename bool BST<Key_T, Data_T>::RIterator::operator==(const RIterator& riter) const
{
	return this->tree == riter.tree && this->cur == riter.cur;
}

template <class Key_T, class Data_T> typename bool BST<Key_T, Data_T>::RIterator::operator!=(const RIterator& riter) const
{
	return !(this->tree == riter.tree && this->cur == riter.cur);
}

template <class Key_T, class Data_T> const typename BST<Key_T, Data_T>::RIterator& BST<Key_T, Data_T>::RIterator::operator=(const RIterator& riter)
{
	if (this == &riter)
		return *this;

	this->tree = riter.tree;
	this->cur = riter.cur;

	return *this;
}
//*************************************************************************************************************************************************//


#endif
