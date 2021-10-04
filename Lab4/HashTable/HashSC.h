// HashSC.h
#pragma once
#ifndef _INC_HASH_SC_
#define _INC_HASH_SC_
// ������������ ������������ �����
#include <iostream>
#include "AbstractHash.h"
#include "Hash_exception.h"


//**************************************************** ���-������� � ��������� �������� ***********************************************************//
template <class Key_T, class Data_T>
class HashSC : public AbstractHash<Key_T, Data_T>
{
	
	class Chain;

	using AbstractHash = AbstractHash<Key_T, Data_T>;
	using AbstractIterator = typename AbstractHash::AbstractIterator;
	using Node = typename Chain::Node;

	// ����� ������� ��������
	class Chain
	{

		friend class HashSC;

		// ��������� ���� �������
		struct Node
		{
			Key_T key;		// ����
			Data_T data;	// ������
			Node* next;		// ��������� ����

			Node(const Key_T& key, const Data_T& data, Node* next = nullptr)
				: key(key), data(data), next(next) {}
		};

		Node* head;				// ������ �������
		mutable size_t last_op;	// ���������� ���������� ��������� �� ��������� ��������				

	public:

		// ����������� ������� �������� ��-���������
		Chain() : head(nullptr), last_op(0) {}
		// ����������� ����������� ������� ��������
		Chain(const Chain& �hain);
		// ���������� ������� ��������
		~Chain();							

		// �������� ���������� ���������� �� ��������� ��������
		size_t getLastOp() const { return this->last_op; }
		
		// ����� ���� �� ����� � ������� ��������
		Node* search(const Key_T& key) const;
		
		// ���������� ���� � ������� ��������
		void append(const Node& node);
		// �������� ���� �� ������� �������� �� ��������� �����
		void remove(const Key_T& key);
		
		// ����� ������� ��������
		void print() const;

	};

	Chain** chains; // ������������ ������ ������� ��������

	// ����������� ���-������
	void _copy(const HashSC& src);

public:

	class IteratorSC;

	// ����������� ���-������� � ��������� �������� � �������� ��������
	HashSC(size_t size);
	// ����������� ����������� ���-������� � ��������� ��������
	HashSC(const HashSC& table);
	// ���������� ���-������� � ��������� ��������
	virtual ~HashSC();

	// �������� ������� � �������� �� �����
	virtual Data_T& operator[](const Key_T& key) final;
	// �������� ������������ ���-������ � ��������� ��������
	virtual const HashSC& operator=(const AbstractHash& table) final;

	// �������� ��� ���-�������
	virtual TableType getType() const final { return TableType::SEPARATE_CHAINING; }
	
	// ������� ���-������� c ��������� ��������
	virtual void clear() final;
	// �������� ������� � ���-������� � ��������� �������� �� ��������� ����� key � ������� data
	virtual void append(const Key_T& key, const Data_T& data) final;
	// ������� ������� �� ���-������� � ��������� �������� �� ����� key
	virtual void remove(const Key_T& key) final;
	
	// ����� ���-������� � ��������� ��������
	virtual void print() const final;

	// ��������� ��������� �� ������ ���-�������
	IteratorSC begin() const;
	// ��������� ��������� � "���������������" ���������
	IteratorSC end() const { return IteratorSC(*this); }

	// ����� ��������� ���-������� � ��������� ��������
	class IteratorSC : public AbstractIterator
	{

		friend class HashSC;

		const HashSC* table;	// ��������� �� ���-�������
		size_t pos;				// ����� ������� �������
		const Node* cur;		// ��������� �� ������� ������� � �������

		// private-����������� ��������� � �����������: table - ��������� �� ���-������� � ��������� ��������,
		// pos - ������� ������� ������� � node - ���������� �� ������� ������� � �������
		IteratorSC(const HashSC* table, size_t pos, const Node* node) : table(table), pos(pos), cur(node) {}

	public:

		// ����������� ��������� �� ���������
		IteratorSC() : table(nullptr), pos(-1), cur(nullptr) {}
		// ����������� ��������� � ������� �� ���-�������
		IteratorSC(const HashSC& table) : table(&table), pos(-1), cur(nullptr) {}
		// ����������� ��������� � ������� �� ���-������� � � ������
		IteratorSC(const HashSC& table, const Key_T& key);
		// ����������� ����������� ���������
		IteratorSC(const IteratorSC& iter) : table(iter.table), pos(iter.pos), cur(iter.cur) {}

		// �������� "���������������" - ��������� �������� �������� ���-������� �� ������
		virtual const Key_T& operator*() const final;

		// �������� ����������� �����������������
		virtual IteratorSC& operator++() final;

		// �������� ��������� ����������
		virtual bool operator==(const AbstractIterator& iter) const final;
		// �������� ����������� ����������
		virtual bool operator!=(const AbstractIterator& iter) const final;

		// �������� ������������ ����������
		virtual const IteratorSC& operator=(const AbstractIterator& iter) final;

	};

};
//*************************************************************************************************************************************************//


//*************************************************** ���������� ������� Chain ********************************************************************//
// public-������:
template <class Key_T, class Data_T> HashSC<Key_T, Data_T>::Chain::Chain(const Chain& chain)
{
	Node* node_copy = chain.head;

	// ������� ������� chain, �������� � ���� � ������
	if (node_copy != nullptr)
	{
		Node* node_this = this->head = new Node(*node_copy);

		for (node_copy = node_copy->next; node_copy != nullptr; node_this = node_this->next, node_copy = node_copy->next)
			node_this->next = new Node(*node_copy);
	}
	else
		this->head = nullptr;

	this->last_op = 0;
}

template <class Key_T, class Data_T> HashSC<Key_T, Data_T>::Chain::~Chain()
{
	Node* cur = this->head;
	Node* prev = nullptr;

	// ������� �������, ������ � ����
	while (cur != nullptr)
	{
		prev = cur;
		cur = cur->next;

		delete prev;
	}
}

template <class Key_T, class Data_T> typename HashSC<Key_T, Data_T>::Chain::Node* HashSC<Key_T, Data_T>::Chain::search(const Key_T& key) const
{
	Node* cur = this->head;

	this->last_op = 0;

	// ������� ������� �� �� �������� ���� � ������, ������ key, ��� ���� �� ����� �� �����
	for (; cur != nullptr && cur->key != key; cur = cur->next, this->last_op++);

	return cur;
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::Chain::append(const Node& node)
{
	// ��������� ���� �� ��� ������� � ����� ������ � �������
	if (search(node.key) != nullptr)
		throw Hash_exception("������� � ����� ������ ��� ���� � ���-�������");

	// ���� ���, �� ��������� � ������ �������
	Node* temp = this->head;
	this->head = new Node(node);
	this->head->next = temp;
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::Chain::remove(const Key_T& key)
{
	Node* cur = this->head;
	Node* prev = nullptr;
	
	this->last_op = 0;

	// ������� ������� �� �� �������� ���� � ������, ������ key, ��� ���� �� ����� �� �����
	for (; cur != nullptr && cur->key != key; prev = cur, cur = cur->next, this->last_op++);

	// ���� ����� �� ����� �������, ������ � ��� ��� ���� � ������, ������ key
	if (cur == nullptr)
		throw Hash_exception("�������� � ����� ������ ��� � ���-�������");
	// ���� ����� �� ����� ������, �.�. �� ����� �� �����, �� ��������� ������� � ������, ������ key
	else
	{
		// ���� ������� �������� �������, �� �������� ��������� ������� ���������� ��������� �������
		if (prev == nullptr)
			this->head = this->head->next;
		// ����� ��������� ������� ���������� ����
		else
			prev->next = cur->next;

		delete cur;
	}
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::Chain::print() const
{
	using std::cout;
	using std::endl;

	for (Node* cur = this->head; cur != nullptr; cur = cur->next)
		cout << cur->key << " -> ";

	cout << "null" << endl;
}
//*************************************************************************************************************************************************//


//*************************************************** ���������� ������� HashSC *******************************************************************//
// private-������:
template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::_copy(const HashSC& table)
{
	for (size_t i = 0; i < table.size; i++)
		if (table.chains[i] == nullptr)
			this->chains[i] = nullptr;
		else
			this->chains[i] = new Chain(*table.chains[i]);

	this->size = table.size;
	this->count = table.count;
	this->last_op = 0;
}


// public-������:
template <class Key_T, class Data_T> HashSC<Key_T, Data_T>::HashSC(size_t size)
{
	this->size = HashSC::_clp2(size);
	this->chains = new Chain*[this->size]();
	this->count = 0;
	this->last_op = 0;
}

template <class Key_T, class Data_T> HashSC<Key_T, Data_T>::HashSC(const HashSC& table)
{
	this->chains = new Chain * [table.size];

	_copy(table);
}

template <class Key_T, class Data_T> HashSC<Key_T, Data_T>::~HashSC()
{
	clear();

	delete[] this->chains;
}

template <class Key_T, class Data_T> typename Data_T& HashSC<Key_T, Data_T>::operator[](const Key_T& key)
{
	size_t i = HashSC::_hash(key);

	this->last_op = 1;

	// ���� �� ������� ���� � ������� ������ ��� ���������
	if (this->chains[i] == nullptr)
		throw Hash_exception("�������� � ����� ������ ��� � ���-�������");

	// ���� ����, ���� �� ���� �������
	Node* node = this->chains[i]->search(key);

	this->last_op += this->chains[i]->getLastOp();

	if (node == nullptr)
		throw Hash_exception("�������� � ����� ������ ��� � ���-�������");
	else
		return node->data;
}

template <class Key_T, class Data_T> const typename HashSC<Key_T, Data_T>& HashSC<Key_T, Data_T>::operator=(const AbstractHash& table)
{
	const HashSC& table_sc = dynamic_cast<const HashSC&>(table);

	if (this == &table_sc)
		return *this;

	clear();

	delete[] this->chains;
	
	this->chains = new Chain * [table_sc.size];

	_copy(table_sc);

	return *this;
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::clear()
{
	for (size_t i = 0; i < this->size; i++)
	{
		if (this->chains[i] != nullptr)
		{
			delete this->chains[i];

			this->chains[i] = nullptr;
		}
	}
	
	this->count = 0;
	this->last_op = 0;
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::append(const Key_T& key, const Data_T& data)
{
	size_t i = HashSC::_hash(key);

	this->last_op = 1;

	// ���� �� ������� ���� ��� ������ ���������
	if (this->chains[i] == nullptr)
	{
		// ������ �������
		this->chains[i] = new Chain();
		// � ��������� � �� �������
		this->chains[i]->append(Node(key, data, nullptr));
		this->count++;
	}
	// ���� ���� ���� �� ���� �������
	else
	{
		// �������� �������� � ��� ������� ������ �������
		try
		{
			this->chains[i]->append(Node(key, data, nullptr));
			// ������� ��������
			this->last_op += this->chains[i]->getLastOp();
			this->count++;
		}
		// ���� �� ����� ���������� � ������� �������� ����������, �� � last_op ���������� ����� ��������
		// � ����� ����������� ��� ����������
		catch (Hash_exception&)
		{
			this->last_op += this->chains[i]->getLastOp();
			
			throw;
		}
	}
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::remove(const Key_T& key)
{
	size_t i = HashSC::_hash(key);

	this->last_op = 1;

	// ���� �� ������� ���� ��� ������ ���������
	if (this->chains[i] == nullptr)
		throw Hash_exception("�������� � ����� ������ ��� � ���-�������");

	// ���� ���� ���� �� ���� �������, �� �������� ����� ������� � ������ key � ������� ���
	try
	{
		this->chains[i]->remove(key);
		// ������� �������
		this->last_op += this->chains[i]->getLastOp();
		// ���� � ������� �� �������� ���������, �� ������� �
		if (this->chains[i]->head == nullptr)
		{
			
			delete this->chains[i];

			this->chains[i] = nullptr;
		}
		this->count--;
	}
	// ���� �� ����� �������� �� ������� �������� ����������, �� � last_op ���������� ����� ��������
	// � ����� ����������� ��� ����������
	catch (Hash_exception&)
	{
		this->last_op += this->chains[i]->getLastOp();
		
		throw;
	}
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::print() const
{
	using std::cout;
	using std::endl;

	for (size_t i = 0; i < this->size; i++)
	{
		cout << "[" << i << "]: ";

		if (this->chains[i] == nullptr)
			cout << "null" << endl;
		else
			this->chains[i]->print();
	}
}

template <class Key_T, class Data_T> typename HashSC<Key_T, Data_T>::IteratorSC HashSC<Key_T, Data_T>::begin() const
{
	// ���� � ���-������� ��� ���������
	if (this->count == 0)
		return IteratorSC(*this);

	// ���� ������ �������� �������
	size_t i = 0;

	for (; i < this->size; i++)
		if (this->chains[i] != nullptr)
			break;

	return IteratorSC(this, i, this->chains[i]->head);
}
//*************************************************************************************************************************************************//


//**************************************************** ���������� ������� ������ IteratorSC *******************************************************//
// public-������:
template <class Key_T, class Data_T> HashSC<Key_T, Data_T>::IteratorSC::IteratorSC(const HashSC& table, const Key_T& key)
{
	this->table = &table;
	// ���� ����� �������
	size_t& i = this->pos = table._hash(key);

	// ���� �� ������� ���� ��� ���������
	if (table.chains[i] == nullptr)
	{
		this->pos = -1;
		this->cur = nullptr;

		return;
	}

	// ���� ���� ���� �� ����, �� �������� ����� ������� � ���� ������� � ������, ������ key
	this->cur = table.chains[i]->search(key);

	// ���� ������� � ������ ������ �� ��� ������
	if (this->cur == nullptr)
		this->pos = -1;
}

template <class Key_T, class Data_T> const typename Key_T& HashSC<Key_T, Data_T>::IteratorSC::operator*() const
{
	if (this->cur == nullptr)
		throw Hash_exception("�������� �� ����������");
	else
		return this->cur->key;
}

template <class Key_T, class Data_T> typename HashSC<Key_T, Data_T>::IteratorSC& HashSC<Key_T, Data_T>::IteratorSC::operator++()
{
	if (this->cur == nullptr)
		throw Hash_exception("�������� �� ����������");

	// ���� ��������� ������� � ������� ����, �� ��������� � ����
	if (this->cur->next != nullptr)
		this->cur = this->cur->next;
	// ���� ��� ���
	else
	{
		// ��������� � ��������� �������
		size_t& i = this->pos;
		i++;

		// ���� ������ �������� �������
		for (; i < this->table->size; i++)
			if (this->table->chains[i] != nullptr)
				break;

		// ���� ��������� ������� ���, �� �������� ��������� � ��������������� ���������
		if (i == this->table->size)
		{
			this->pos = -1;
			this->cur = nullptr;
		}
		// ���� ����, �� ������� ������� ����� �������� ��������� ��������� �������
		else
			this->cur = this->table->chains[i]->head;
	}

	return *this;
}

template <class Key_T, class Data_T> typename bool HashSC<Key_T, Data_T>::IteratorSC::operator==(const AbstractIterator& iter) const
{
	const typename HashSC<Key_T, Data_T>::IteratorSC& iter_sc = dynamic_cast<const HashSC<Key_T, Data_T>::IteratorSC&>(iter);

	return this->table == iter_sc.table && this->pos == iter_sc.pos && this->cur == iter_sc.cur;
}

template <class Key_T, class Data_T> typename bool HashSC<Key_T, Data_T>::IteratorSC::operator!=(const AbstractIterator& iter) const
{
	const typename HashSC<Key_T, Data_T>::IteratorSC& iter_sc = dynamic_cast<const HashSC<Key_T, Data_T>::IteratorSC&>(iter);

	return !(this->table == iter_sc.table && this->pos == iter_sc.pos && this->cur == iter_sc.cur);
}

template <class Key_T, class Data_T> const typename HashSC<Key_T, Data_T>::IteratorSC& HashSC<Key_T, Data_T>::IteratorSC::operator=(const AbstractIterator& iter)
{
	const typename HashSC<Key_T, Data_T>::IteratorSC& iter_sc = dynamic_cast<const HashSC<Key_T, Data_T>::IteratorSC&>(iter);

	if (this == &iter_sc)
		return *this;

	this->table = iter_sc.table;
	this->pos = iter_sc.pos;
	this->cur = iter_sc.cur;

	return *this;
}
//*************************************************************************************************************************************************//


#endif
