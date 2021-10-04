// Vertex.h
#pragma once
#ifndef _INC_VERTEX_
#define _INC_VERTEX_
// ������������ ������������ �����
#include "Graph_exception.h"


//********************************************** ����� ���������� ������� ����� *******************************************************************//
class Vertex_exception: public Graph_exception
{
	
	using string = std::string;

public:

	Vertex_exception(const string& error) : Graph_exception(error) {}

};
//*************************************************************************************************************************************************//


//**************************************************** ����� ������� ����� ************************************************************************//
template <class Name_T, class Data_T> 
class Vertex
{

	Name_T name;	// ��� �������
	Data_T data;	// ������ �������

	bool isNameSet;	// ���������������� �� ���� �����
	bool isDataSet;	// ���������������� �� ���� ������

public:

	// ����������� ������� �� ���������
	Vertex() : name(), data(), isNameSet(false), isDataSet(false) {}
	// ����������� ������� � �����������: ������ name � ������� data
	Vertex(const Name_T& name, const Data_T& data) :
		name(name), data(data), isNameSet(true), isDataSet(true) {}
	// ����������� ����������� �������
	Vertex(const Vertex& vertex) :
		name(vertex.name), data(vertex.data),
		isNameSet(vertex.isNameSet), isDataSet(vertex.isDataSet) {}

	// �������� ��������� ������
	bool operator==(const Vertex& vertex) const;
	// �������� ����������� ������
	bool operator!=(const Vertex& vertex) const;
	// �������� ������������ ������
	const Vertex& operator=(const Vertex& vertex);

	// ���������� ��� �������
	void setName(const Name_T& name) { this->name = name; isNameSet = true; }
	// ���������� ������ �������
	void setData(const Data_T& data) { this->data = data; isDataSet = true; }

	// �������� ��� �������
	const Name_T& getName() const;
	// �������� ������ �������
	const Data_T& getData() const;

};
//*************************************************************************************************************************************************//


//******************************************************** ���������� ������� Vertex **************************************************************//
// public-������
template <class Name_T, class Data_T>
typename bool Vertex<Name_T, Data_T>::operator==(const Vertex& vertex) const
{
	return (this->isNameSet && vertex.isNameSet && this->name == vertex.name);
}

template <class Name_T, class Data_T>
typename bool Vertex<Name_T, Data_T>::operator!=(const Vertex& vertex) const
{
	return !(this->isNameSet && vertex.isNameSet && this->name == vertex.name);
}

template <class Name_T, class Data_T>
typename const Vertex<Name_T, Data_T>& Vertex<Name_T, Data_T>::operator=(const Vertex& vertex)
{
	if (this == &vertex)
		return *this;

	this->name = vertex.name;
	this->data = vertex.data;
	this->isNameSet = vertex.isNameSet;
	this->isDataSet = vertex.isDataSet;

	return *this;
}

template <class Name_T, class Data_T>
const typename Name_T& Vertex<Name_T, Data_T>::getName() const
{
	if (this->isNameSet)
		return this->name;
	else
		throw Vertex_exception("���� ����� �� �����������");
}

template <class Name_T, class Data_T>
const typename Data_T& Vertex<Name_T, Data_T>::getData() const
{
	if (this->isDataSet)
		return this->data;
	else
		throw Vertex_exception("���� ������ �� �����������");
}
//*************************************************************************************************************************************************//


#endif
