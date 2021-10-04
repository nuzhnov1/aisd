// HashOA.h
#pragma once
#ifndef _INC_HASH_OA_
#define _INC_HASH_OA_
// ������������ ������������ �����
#include <iostream>
#include "AbstractHash.h"
#include "Hash_exception.h"


//**************************************************** ���-������� � �������� ���������� **********************************************************//
template <class Key_T, class Data_T>
class HashOA : public AbstractHash<Key_T, Data_T>
{

	using AbstractHash = AbstractHash<Key_T, Data_T>;
	using AbstractIterator = typename AbstractHash::AbstractIterator;

	// �����-������������ ��������� ������ ���-�������
	enum class Status
	{
		BUSY,		// ������
		DELETED,	// �������� ����� �������� �� �� ��������
		FREE		// ��������
	};

	// ��������� ������ ���-�������
	struct Cell
	{
		Key_T key {};		// ����
		Data_T data {};		// ������
		Status state {};	// ���������
	};

	Cell* cells; // ������������ ������ ����� ���-�������

	// �������� ������������
	size_t _linearHash(const Key_T& key, size_t i) const { return (HashOA::_hash(key) + i) % this->size; }
	// ����������� ���-������
	void _copy(const HashOA& table);

public:

	class IteratorOA;

	// ����������� ���-������� � �������� ���������� � �������� ��������
	HashOA(size_t size);
	// ����������� ����������� ���-������� � �������� ����������
	HashOA(const HashOA& table);
	// ���������� ���-������� � �������� ����������
	virtual ~HashOA() { delete[] this->cells; }

	// �������� ������� � �������� �� �����
	virtual Data_T& operator[](const Key_T& key) final;
	// �������� ������������ ���-������ � ��������� ��������
	virtual const HashOA& operator=(const AbstractHash& table) final;

	// �������� ��� ���-�������
	virtual TableType getType() const final { return TableType::OPEN_ADDRESSING; }
	
	// ������� ���-������� c �������� ����������
	virtual void clear() final;
	// �������� ������� � ���-������� � �������� ���������� �� ��������� ����� key � ������� data
	virtual void append(const Key_T& key, const Data_T& data) final;
	// ������� ������� �� ���-������� � �������� ���������� �� ����� key
	virtual void remove(const Key_T& key) final;
	
	// ����� ���-������� � ��������� ��������
	virtual void print() const final;
	
	// ��������� ��������� �� ������ ���-�������
	IteratorOA begin() const;
	// ��������� ��������� � "���������������" ���������
	IteratorOA end() const { return IteratorOA(*this); }

	// ����� ��������� ���-������� � �������� ����������
	class IteratorOA : public AbstractIterator
	{

		friend class HashOA;

		const HashOA* table;	// ��������� �� ���-�������
		size_t pos;				// ����� �������� ��������

		// private-����������� ��������� � �����������: table - ��������� �� ���-������� � ��������� ��������,
		// pos - ������� ������� ������� � node - ���������� �� ������� ������� � �������
		IteratorOA(const HashOA* table, size_t pos) : table(table) {}

	public:

		// ����������� ��������� �� ���������
		IteratorOA() : table(nullptr), pos(-1) {}
		// ����������� � ������� �� ���-�������
		IteratorOA(const HashOA& table) : table(&table), pos(-1) {}
		// ����������� � ������� �� ���-������� � ������ ��������
		IteratorOA(const HashOA& table, const Key_T& key);
		// ����������� ����������� ���������
		IteratorOA(const IteratorOA& iter) : table(iter.table), pos(iter.pos) {}

		// �������� "���������������" - ��������� �������� �������� ���-������� �� ������
		virtual const Key_T& operator*() const final;

		// �������� ����������� �����������������
		virtual IteratorOA& operator++() final;

		// �������� ��������� ����������
		virtual bool operator==(const typename AbstractIterator& iter) const final;
		// �������� ����������� ����������
		virtual bool operator!=(const typename AbstractIterator& iter) const final;
		
		// �������� ������������ ����������
		virtual const IteratorOA& operator=(const typename AbstractIterator& iter) final;
	};

};
//*************************************************************************************************************************************************//


//**************************************************** ���������� ������� HashOA ******************************************************************//
// private-������:
template <class Key_T, class Data_T> typename void HashOA<Key_T, Data_T>::_copy(const HashOA& table)
{
	for (size_t i = 0; i < table.size; i++)
		this->cells[i] = table.cells[i];

	this->size = table.size;
	this->count = table.count;
	this->last_op = 0;
}


// public-������:
template <class Key_T, class Data_T> HashOA<Key_T, Data_T>::HashOA(size_t size)
{
	this->size = HashOA::_clp2(size);
	
	this->cells = new Cell[this->size];
	for (size_t i = 0; i < this->size; i++)
		this->cells[i].state = Status::FREE;

	this->count = 0;
	this->last_op = 0;
}

template <class Key_T, class Data_T> HashOA<Key_T, Data_T>::HashOA(const HashOA& table)
{
	this->cells = new Cell[table.size];

	_copy(table);
}

template <class Key_T, class Data_T> typename Data_T& HashOA<Key_T, Data_T>::operator[](const Key_T& key)
{
	// i - ����� �����, j - ����� ������� ������
	size_t i = 0, j = 0;

	this->last_op = 0;

	do
	{
		// �������� ���� ��� i-�� �����
		j = _linearHash(key, i);

		this->last_op++;

		// ���� ������� ������� ������ � ��� �� ������, ��� � key
		if (this->cells[j].key == key && this->cells[j].state == Status::BUSY)
			return this->cells[j].data;

		i++;
		// �������� �� �����, ���� �� �������� ������ ��������� ������ ��� �� ������ ��� �������
	} while (this->cells[j].state != Status::FREE && i < this->size);

	// ���� ��� � �� ��������� �������� � ������ key, �� ����������� ����������
	throw Hash_exception("�������� � ����� ������ ��� � ���-�������");
}

template <class Key_T, class Data_T> const typename HashOA<Key_T, Data_T>& HashOA<Key_T, Data_T>::operator=(const AbstractHash& table)
{
	const HashOA& table_oa = dynamic_cast<const HashOA&>(table);

	if (this == &table_oa)
		return *this;

	delete[] this->cells;

	this->cells = new Cell[table_oa.size];

	_copy(table_oa);

	return *this;
}

template <class Key_T, class Data_T> typename void HashOA<Key_T, Data_T>::clear()
{
	for (size_t i = 0; i < this->size; i++)
		this->cells[i].state = Status::FREE;

	this->count = 0;
	this->last_op = 0;
}

template <class Key_T, class Data_T> typename void HashOA<Key_T, Data_T>::append(const Key_T& key, const Data_T& data)
{
	// i - ����� �����, j - ����� ������� ������
	size_t i = 0, j = 0;
	// ������� ������ �������� ������
	long long pos = -1;
	
	this->last_op = 0;

	do 
	{
		j = _linearHash(key, i); // �������� ���� ��� i-�� �����

		this->last_op++;

		// ���� ������� ��������� �������� ������, �� ���������� � ������� � pos
		if (this->cells[j].state == Status::DELETED && pos == -1)
			pos = j;
		// ���� ��������� ������ � ��� �� ������, � ��� ���� ��� ������, �� ����������� ����������
		if (this->cells[j].state == Status::BUSY && this->cells[j].key == key)
			throw Hash_exception("������� � ����� ������ ��� ���� � ���-�������");

		i++;
	// �������� �� �����, ���� �� �������� ��������� ������ ��� �� ������ ��� �������
	} while (this->cells[j].state != Status::FREE && i < this->size);
	
	// ���� � ������� ��� ������ ������, �� ����������� ����������
	if (i == this->size && pos == -1)
		throw Hash_exception("���-������� �����������");

	// �����, ���� �� ��������� �������� ������, �� � pos ���������� ����� ������ ��������� ������
	if (pos == -1)
		pos = j;
	
	// � ���� ������, �.�. ���� ��������� �������� ������, �� ��������� ������� ������ ����
	// � �� � ��������� ������

	this->cells[pos].key = key;
	this->cells[pos].data = data;
	this->cells[pos].state = Status::BUSY;

	this->count++;
}

template <class Key_T, class Data_T> typename void HashOA<Key_T, Data_T>::remove(const Key_T& key)
{
	// i - ����� �����, j - ����� ������� ������
	size_t i = 0, j = 0;

	this->last_op = 0;

	do
	{
		// �������� ���� ��� i-�� �����
		j = _linearHash(key, i);

		this->last_op++;

		// ���� ��������� ������� ������ � ��� �� ������, �� � key, �� �������� �, ��� ��������
		if (this->cells[j].key == key && this->cells[j].state == Status::BUSY)
		{
			this->cells[j].state = Status::DELETED;
			this->count--;

			return;
		}

		i++;
	// �������� �� �����, ���� �� �������� ��������� ������ ��� �� ������ ��� �������
	} while (this->cells[j].state != Status::FREE && i < this->size);

	// ���� ��� � �� ��������� �������� � ������ key, �� ����������� ����������
	throw Hash_exception("�������� � ����� ������ ��� � ���-�������");
}

template <class Key_T, class Data_T> typename void HashOA<Key_T, Data_T>::print() const
{
	using std::cout;
	using std::endl;

	for (size_t i = 0; i < this->size; i++)
	{
		cout << "[" << i << "]: ";

		if (this->cells[i].state == Status::BUSY)
		{
			cout << "Busy" << ": " << this->cells[i].key;
		}
		else if (this->cells[i].state == Status::DELETED)
			cout << "Deleted";
		else
			cout << "Free";

		cout << endl;
	}
}

template <class Key_T, class Data_T> typename HashOA<Key_T, Data_T>::IteratorOA HashOA<Key_T, Data_T>::begin() const
{
	size_t i = 0;

	// �������� �� ���� ���-������� � ������ ������ ������� ������
	for (; i < this->size; i++)
		if (this->cells[i].state == Status::BUSY)
			// ���� ����� ����� ������, �� ������������� �������� �� �� � ���������� ���
			return IteratorOA(this, i);

	// ���� �� ����� ����� ������, �� ���������� ��������������� ��������
	return IteratorOA(*this);

}
//*************************************************************************************************************************************************//


//***************************************************** ���������� ������� ������ IteratorOA ******************************************************//
// public-������:
template <class Key_T, class Data_T> HashOA<Key_T, Data_T>::IteratorOA::IteratorOA(const HashOA& table, const Key_T& key)
{
	this->table = &table;

	// ���� � ���-������� table ������� � ������ key

	// i - ����� �����, j - ����� ������� ������
	size_t i = 0, j = 0;

	do
	{
		// �������� ���� ��� i-�� �����
		j = table._linearHash(key, i);

		// ���� ������� ������� ������ � ��� �� ������, ��� � key
		if (table.cells[j].key == key && table.cells[j].state == Status::BUSY)
		{
			this->pos = j;

			return;
		}

		i++;
		// �������� �� �����, ���� �� �������� ������ ��������� ������ ��� �� ������ ��� �������
	} while (table.cells[j].state != Status::FREE && i < table.size);

	// ���� ��� � �� ��������� �������� � ������ key
	this->pos = -1;
}

template <class Key_T, class Data_T> const typename Key_T& HashOA<Key_T, Data_T>::IteratorOA::operator*() const
{
	if (this->pos == -1)
		throw Hash_exception("�������� �� ����������");
	else
		return this->table->cells[this->pos].key;
}

template <class Key_T, class Data_T> typename HashOA<Key_T, Data_T>::IteratorOA& HashOA<Key_T, Data_T>::IteratorOA::operator++()
{
	if (this->pos == -1)
		throw Hash_exception("�������� �� ����������");

	for (this->pos++; this->pos < this->table->size; this->pos++)
		if (this->table->cells[this->pos].state == Status::BUSY)
			return *this;

	this->pos = -1;

	return *this;
}

template <class Key_T, class Data_T> typename bool HashOA<Key_T, Data_T>::IteratorOA::operator==(const AbstractIterator& iter) const
{
	const IteratorOA& iter_oa = dynamic_cast<const IteratorOA&>(iter);

	return this->table == iter_oa.table && this->pos == iter_oa.pos;
}

template <class Key_T, class Data_T> typename bool HashOA<Key_T, Data_T>::IteratorOA::operator!=(const typename AbstractIterator& iter) const
{
	const IteratorOA& iter_oa = dynamic_cast<const IteratorOA&>(iter);

	return !(this->table == iter_oa.table && this->pos == iter_oa.pos);
}

template <class Key_T, class Data_T> const typename HashOA<Key_T, Data_T>::IteratorOA& HashOA<Key_T, Data_T>::IteratorOA::operator=(const AbstractIterator& iter)
{
	const IteratorOA& iter_oa = dynamic_cast<const IteratorOA&>(iter);

	this->table = iter_oa.table;
	this->pos = iter_oa.pos;

	return *this;
}
//*************************************************************************************************************************************************//


#endif
