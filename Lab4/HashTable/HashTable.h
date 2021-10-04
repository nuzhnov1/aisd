// HashTable.h
#pragma once
#ifndef _INC_HASH_TABLE_
#define _INC_HASH_TABLE_
// ������������ ������������ �����
#include "AbstractHash.h"
#include "HashSC.h"
#include "HashOA.h"


//************************************************* ���������������� ����� ���-������� ************************************************************//
template <class Key_T, class Data_T>
class HashTable
{

	using AbstractHash = AbstractHash<Key_T, Data_T>;
	using AbstractIterator = typename AbstractHash::AbstractIterator;
	using HashSC = HashSC<Key_T, Data_T>;
	using IteratorSC = typename HashSC::IteratorSC;
	using HashOA = HashOA<Key_T, Data_T>;
	using IteratorOA = typename HashOA::IteratorOA;

	AbstractHash* tab_p; // ��������� �� ����������� ���-�������

public:

	class Iterator;

	// ����������� ���-������� �� ���������
	HashTable() : HashTable(0) {}
	// ����������� ���-������� � �����������: �������� size � ����� ������� type
	HashTable(size_t size, TableType type = TableType::SEPARATE_CHAINING);
	// ����������� ����������� ���-������
	HashTable(const HashTable& table);
	// ���������� ���-������
	~HashTable() { delete this->tab_p; }

	// �������� ������� � �������� �� �����
	Data_T& operator[](const Key_T& key) { return (*this->tab_p)[key]; }
	const Data_T& operator[](const Key_T& key) const { return const_cast<HashTable&>(*this)[key]; }
	// �������� ������������ ���-������
	const HashTable& operator=(const HashTable& table);
	
	// �������� ��� ���-�������
	TableType getType() const { return this->tab_p->getType(); }
	// �������� ������ ���-�������
	size_t getSize() const { return this->tab_p->getSize(); }
	// �������� ����� ��������� � ���-�������
	size_t getCount() const { return this->tab_p->getCount(); }
	// �������� ����� ��������� �� ��������� ��������
	size_t getLastOp() const { return this->tab_p->getLastOp(); }
	// �������� ���-������� �� �������
	bool isEmpty() const { return this->tab_p->isEmpty(); }

	// ������ �������� ���-������� �� �����
	const Data_T& getItemByKey(const Key_T& key) const { return const_cast<HashTable&>(*this)[key]; }
	// ������ �������� �� ��������� �����
	void setItemByKey(const Key_T& key, const Data_T& data) { (*this)[key] = data; }

	// ������� ���-�������
	void clear() { this->tab_p->clear(); }
	// �������� � �������� ������ � ������� � ���-�������
	void append(const Key_T& key, const Data_T& data) { this->tab_p->append(key, data); }
	// ������� ������� � �������� ������
	void remove(const Key_T& key) { this->tab_p->remove(key); }
	
	// ������� ���-�������
	void print() const { this->tab_p->print(); }

	// ��������� ��������� �� ������ ���-�������
	Iterator begin() const;
	// ��������� ��������� � ��������������� ���������
	Iterator end() const;

	// ����� ��������� ���-�������
	class Iterator
	{
		friend class HashTable;

		AbstractIterator* iter_p;	// ��������� �� ����������� ��������
		TableType type;				// ��� �������, �� ������� ���������� ��������

		// private-����������� ��������� � �����������: iter_p - ���������� �� ����������� ��������
		// � type - ����� ���-�������
		Iterator(typename AbstractIterator* iter_p, TableType type) : iter_p(iter_p), type(type) {}

	public:

		// ����������� ��������� �� ���������
		Iterator() : iter_p(nullptr), type(TableType::SEPARATE_CHAINING) {}
		// ����������� ��������� � ������� �� ���-�������
		Iterator(const HashTable& table);
		// ����������� ��������� � �����������: table - ������ �� ���-�������, key - ����
		Iterator(const HashTable& table, const Key_T& key);
		// ����������� ����������� ���������
		Iterator(const Iterator& iter);
		// ���������� ���������
		~Iterator();

		// �������� "���������������" - ��������� �������� �������� ���-������� �� ������
		const Key_T& operator*() const;

		// �������� ����������� �����������������
		Iterator& operator++();
		// �������� ������������ �����������������
		Iterator operator++(int);

		// �������� ��������� ����������
		bool operator==(const Iterator& iter) const;
		// �������� ����������� ����������
		bool operator!=(const Iterator& iter) const;

		// �������� ������������ ����������
		const Iterator& operator=(const Iterator& iter);

	};

};
//*************************************************************************************************************************************************//


//*************************************************** ���������� ������� HashTable ****************************************************************//
//public-������:
template <class Key_T, class Data_T> HashTable<Key_T, Data_T>::HashTable(size_t size, TableType type)
{
	// � ����������� �� ��������� ���� ������� ������ ���-������� � ��������� ��������
	// ��� � �������� ����������

	if (type == TableType::SEPARATE_CHAINING)
		this->tab_p = new HashSC(size);
	else if (type == TableType::OPEN_ADDRESSING)
		this->tab_p = new HashOA(size);
	else
		throw Hash_exception("����������� ��� ���-�������");
}

template <class Key_T, class Data_T> HashTable<Key_T, Data_T>::HashTable(const HashTable& table)
{
	// � ����������� �� ���� ������� table ������ ���-������� � ��������� ��������
	// ��� � �������� ����������, ���������� table

	if (table.tab_p->getType() == TableType::SEPARATE_CHAINING)
		this->tab_p = new HashSC(dynamic_cast<const HashSC&>(*table.tab_p));
	else
		this->tab_p = new HashOA(dynamic_cast<const HashOA&>(*table.tab_p));
}

template <class Key_T, class Data_T> const typename HashTable<Key_T, Data_T>& HashTable<Key_T, Data_T>::operator=(const HashTable& table)
{
	if (this == &table)
		return *this;

	// ���� ���� ������ ���������, �� �������� ����������� �������� ������������
	if (table.tab_p->getType() == this->tab_p->getType())
		*this->tab_p = *table.tab_p;
	// ����� ������� ������� ������� � ������ ����� ������� table
	else
	{
		delete this->tab_p;

		if (table.tab_p->getType() == TableType::SEPARATE_CHAINING)
			this->tab_p = new HashSC(dynamic_cast<const HashSC&>(*table.tab_p));
		else
			this->tab_p = new HashOA(dynamic_cast<const HashOA&>(*table.tab_p));
	}

	return *this;
}

template <class Key_T, class Data_T> typename HashTable<Key_T, Data_T>::Iterator HashTable<Key_T, Data_T>::begin() const
{
	// � ����������� �� ���� ������� ������ �������� IteratorSC ��� IteratorOA

	if (this->tab_p->getType() == TableType::SEPARATE_CHAINING)
	{
		IteratorSC* iter_sc = new IteratorSC(dynamic_cast<const HashSC&>(*this->tab_p).begin());

		return Iterator(iter_sc, TableType::SEPARATE_CHAINING);
	}
	else 
	{
		IteratorOA* iter_oa = new IteratorOA(dynamic_cast<const HashOA&>(*this->tab_p).begin());

		return Iterator(iter_oa, TableType::OPEN_ADDRESSING);
	}
}

template <class Key_T, class Data_T> typename HashTable<Key_T, Data_T>::Iterator HashTable<Key_T, Data_T>::end() const
{
	// � ����������� �� ���� ������� ������ �������� IteratorSC ��� IteratorOA

	if (this->tab_p->getType() == TableType::SEPARATE_CHAINING)
	{
		IteratorSC* iter_sc = new IteratorSC(dynamic_cast<const HashSC&>(*this->tab_p).end());

		return Iterator(iter_sc, TableType::SEPARATE_CHAINING);
	}
	else
	{
		IteratorOA* iter_oa = new IteratorOA(dynamic_cast<const HashOA&>(*this->tab_p).end());

		return Iterator(iter_oa, TableType::OPEN_ADDRESSING);
	}
}
//*************************************************************************************************************************************************//


//*************************************************** ���������� ������� Iterator *****************************************************************//
//public-������:
template <class Key_T, class Data_T> HashTable<Key_T, Data_T>::Iterator::Iterator(const HashTable& table)
{
	// ���������� ��� �������
	this->type = table.tab_p->getType();

	// �������� ������� ���� ������ ����������� ��������
	if (this->type == TableType::SEPARATE_CHAINING)
		this->iter_p = new IteratorSC(dynamic_cast<const HashSC&>(*table.tab_p));
	else
		this->iter_p = new IteratorOA(dynamic_cast<const HashOA&>(*table.tab_p));
}

template <class Key_T, class Data_T> HashTable<Key_T, Data_T>::Iterator::Iterator(const HashTable& table, const Key_T& key)
{
	// ���������� ��� �������
	this->type = table.tab_p->getType();

	// �������� ������� ���� ������ ����������� ��������
	if (this->type == TableType::SEPARATE_CHAINING)
		this->iter_p = new IteratorSC(dynamic_cast<const HashSC&>(*table.tab_p), key);
	else
		this->iter_p = new IteratorOA(dynamic_cast<const HashOA&>(*table.tab_p), key);
}

template <class Key_T, class Data_T> HashTable<Key_T, Data_T>::Iterator::Iterator(const Iterator& iter)
{
	// ���������� ��� �������
	this->type = iter.type;

	// �������� ������� ���� ������ ����������� ��������
	if (this->type == TableType::SEPARATE_CHAINING)
		this->iter_p = new IteratorSC(dynamic_cast<const IteratorSC&>(*iter.iter_p));
	else
		this->iter_p = new IteratorOA(dynamic_cast<const IteratorOA&>(*iter.iter_p));
}

template <class Key_T, class Data_T> HashTable<Key_T, Data_T>::Iterator::~Iterator()
{
	if (this->iter_p != nullptr)
		delete this->iter_p;
}

template <class Key_T, class Data_T> const typename Key_T& HashTable<Key_T, Data_T>::Iterator::operator*() const
{
	if (this->iter_p == nullptr)
		throw Hash_exception("�������� �� ����������");
	else
		return *(*this->iter_p);
}

template <class Key_T, class Data_T> typename HashTable<Key_T, Data_T>::Iterator& HashTable<Key_T, Data_T>::Iterator::operator++()
{
	if (this->iter_p == nullptr)
		throw Hash_exception("�������� �� ����������");
	else
		++(*this->iter_p);

	return *this;
}

template <class Key_T, class Data_T> typename HashTable<Key_T, Data_T>::Iterator HashTable<Key_T, Data_T>::Iterator::operator++(int)
{
	Iterator temp = *this;

	++(*this);

	return temp;
}

template <class Key_T, class Data_T> typename bool HashTable<Key_T, Data_T>::Iterator::operator==(const Iterator& iter) const
{
	if (this->iter_p == nullptr && iter.iter_p == nullptr)
		return true;
	else if ((this->iter_p != nullptr && iter.iter_p == nullptr) || (this->iter_p == nullptr && iter.iter_p != nullptr))
		return false;
	else if (this->type != iter.type)
		return false;
	else
		return (*this->iter_p == *iter.iter_p);
}

template <class Key_T, class Data_T> typename bool HashTable<Key_T, Data_T>::Iterator::operator!=(const Iterator& iter) const
{
	if (this->iter_p == nullptr && iter.iter_p == nullptr)
		return false;
	else if ((this->iter_p != nullptr && iter.iter_p == nullptr) || (this->iter_p == nullptr && iter.iter_p != nullptr))
		return true;
	else if (this->type != iter.type)
		return true;
	else
		return (*this->iter_p == *iter.iter_p);
}

template <class Key_T, class Data_T> const typename HashTable<Key_T, Data_T>::Iterator& HashTable<Key_T, Data_T>::Iterator::operator=(const Iterator& iter)
{
	if (this == &iter)
		return *this;

	// ���� ������ �������� �� ��� ������
	if (this->iter_p == nullptr)
	{
		// ���� iter ��� ������
		if (iter.iter_p != nullptr)
			// ����� �������� ������ �������� � ����������� �� ���� �������, �� ������� �� ����������
			if (iter.type == TableType::SEPARATE_CHAINING)
				this->iter_p = new IteratorSC(dynamic_cast<const IteratorSC&>(*iter.iter_p));
			else
				this->iter_p = new IteratorOA(dynamic_cast<const IteratorOA&>(*iter.iter_p));

		this->type = iter.type;

		return *this;
	}

	// ���� �������� iter �� ��� ������
	if (iter.iter_p == nullptr)
	{
		delete this->iter_p;

		this->iter_p = nullptr;
		this->type = iter.type;

		return *this;
	}

	// ���� ���� ���������� ���������, �� �������� ����������� �������� ������������
	if (this->type == iter.type)
		*this->iter_p = *iter.iter_p;
	// ����� ������� ������� �������� � ������ �����, ���������� ��������� iter
	else
	{
		delete this->iter_p;

		this->type = iter.type;

		if (this->type == TableType::SEPARATE_CHAINING)
			this->iter_p = new IteratorSC(dynamic_cast<const IteratorSC&>(*iter.iter_p));
		else
			this->iter_p = new IteratorOA(dynamic_cast<const IteratorOA&>(*iter.iter_p));
	}

	return *this;
}
//*************************************************************************************************************************************************//


#endif
