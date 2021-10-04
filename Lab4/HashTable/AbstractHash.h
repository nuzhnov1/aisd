// AbstarctHash.h
#pragma once
#ifndef _INC_ABSTRACT_HASH_
#define _INC_ABSTRACT_HASH_
// ������������ ������������ �����
#include "Hash_functions.h"
#include "Hash_exception.h"


//******************************************************* �����-������������ ����� ���-������� ****************************************************//
enum class TableType
{
	SEPARATE_CHAINING,	// ������� ��������
	OPEN_ADDRESSING		// �������� ���������
};
//************************************************************************************************************************************************//


//**************************************************** ����������� ����� ���-������� **************************************************************//
template <class Key_T, class Data_T>
class AbstractHash
{

protected:

	size_t size;			// ������ ���-�������
	size_t count;			// ����� ��������� � ���-�������
	mutable size_t last_op;	// ����� ����, ����������� ��������� ���������
	
	// ����������������� ������� �����������
	size_t _hash(const Key_T& key) const { return mul_hash<Key_T>(key, this->size); }
	// ����� ��������� ������� ������ �������, ��� num
	size_t _clp2(size_t num) const;							

public:
	
	// ����������� ����������� ����� ���-������� ��-���������
	AbstractHash(): size(0), count(0), last_op(0) {}
	// ����������� ����������
	virtual ~AbstractHash() {}
	
	// �������� ������� � �������� �� �����
	virtual Data_T& operator[](const Key_T& key) = 0;
	// �������� ������������ ���-������
	virtual const AbstractHash& operator=(const AbstractHash& table) = 0;

	// �������� ��� ���-�������
	virtual TableType getType() const = 0;		
	// �������� ������ ���-�������
	size_t getSize() const { return this->size; }
	// �������� ����� ��������� � ���-�������
	size_t getCount() const { return this->count; }
	// �������� ����� ���� �� ��������� ��������
	size_t getLastOp() const { return this->last_op; }
	// ��������� ���-������� �� �������
	bool isEmpty() const { return this->count == 0; }

	// ������� ���-�������
	virtual void clear() = 0;
	// �������� ������� � ���-������� � �������� ������ key � ������� data
	virtual void append(const Key_T& key, const Data_T& data) = 0;
	// ������� ������� �� ���-������� �� ����� key
	virtual void remove(const Key_T& key) = 0;
	
	// ����� ���-�������
	virtual void print() const = 0;								

	class AbstractIterator
	{

	public:

		// ����������� ����������
		virtual ~AbstractIterator() {}

		// �������� "���������������" - ��������� �������� �������� ���-������� �� ������
		virtual const Key_T& operator*() const = 0;
		
		// �������� ����������� �����������������
		virtual AbstractIterator& operator++() = 0;	
		
		// �������� ������������ ����������
		virtual bool operator==(const AbstractIterator& iter) const = 0;
		// �������� ����������� ����������
		virtual bool operator!=(const AbstractIterator& iter) const = 0;
		
		virtual const AbstractIterator& operator=(const AbstractIterator& iter) = 0;
		// �������� ��������� ����������
	};

};
//*************************************************************************************************************************************************//


//************************************************************* ���������� ������� AbstractHash ***************************************************//
// protected-������:
template <class Key_T, class Data_T> typename size_t AbstractHash<Key_T, Data_T>::_clp2(size_t num) const
{
	size_t result = 1;

	for (; result < num; result <<= 1);

	return result;
}
//*************************************************************************************************************************************************//


#endif
