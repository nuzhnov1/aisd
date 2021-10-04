// Edge.h
#pragma once
#ifndef _INC_EDGE_
#define _INC_EDGE_
// ������������ ������������ �����
#include "Graph_exception.h"


//*********************************************** ����� ���������� ����� ����� ********************************************************************//
class Edge_exception: public Graph_exception 
{
	
	using string = std::string;

public:

	Edge_exception(const string& error) : Graph_exception(error) {}

};
//*************************************************************************************************************************************************//


//**************************************************** ����� ����� ����� **************************************************************************//
template <class Vertex_T, class Weight_T, class Data_T>
class Edge
{

	const Vertex_T& outV;	// ��������� �� ���������� �������, ������ ����� �������
	const Vertex_T& inV;	// ��������� �� ���������� �������, ���� ����� ������

	Weight_T weight;	// ��� �����
	Data_T data;		// ������ �����

	bool isWeightSet;	// ���������������� �� ���� ����
	bool isDataSet;		// ���������������� �� ���� ������

public:

	// ����������� ����� �� � �����������: ������������� ������� ������ outV � ������� ����� inV
	Edge(const Vertex_T& outV, const Vertex_T& inV) :
		outV(outV), inV(inV), weight(), data(),
		isWeightSet(false), isDataSet(false) {}
	// ����������� ����� � �����������: ������������� ������� ������ outV � ������� ����� inV � ����� ����� weight
	Edge(const Vertex_T& outV, const Vertex_T& inV, const Weight_T& weight) :
		outV(outV), inV(inV), weight(weight), data(),
		isWeightSet(true), isDataSet(false) {}
	// ����������� ����� � �����������: ������������� ������� ������ outV � ������� ����� inV, ����� ����� weight � ������� data
	Edge(const Vertex_T& outV, const Vertex_T& inV, const Weight_T& weight, const Data_T& data) :
		outV(outV), inV(inV), weight(weight), data(data),
		isWeightSet(true), isDataSet(true) {}
	// ����������� ����������� ����
	Edge(const Edge& edge) : outV(edge.outV), inV(edge.inV), weight(edge.weight),
		data(edge.data), isWeightSet(edge.isWeightSet), isDataSet(edge.isDataSet) {}

	// �������� ������������ ����
	const Edge& operator=(const Edge& edge);

	// ��������� ����������� ������� ������
	const Vertex_T& getVertexOut() const { return this->outV; }
	// ��������� ����������� ������� �����
	const Vertex_T& getVertexIn() const { return this->inV; }

	// ���������� ��� �����
	void setWeight(const Weight_T& weight) { this->weight = weight; isWeightSet = true; }
	// ���������� ������ �����
	void setData(const Data_T& data) { this->data = data; isDataSet = true; }

	// �������� ��� �����
	const Weight_T& getWeight() const;
	// �������� ������ �����
	const Data_T& getData() const;

};
//*************************************************************************************************************************************************//


//******************************************************** ���������� ������� Edge ****************************************************************//
// public-������
template <class Vertex_T, class Weight_T, class Data_T>
typename const Edge<Vertex_T, Weight_T, Data_T>& Edge<Vertex_T, Weight_T, Data_T>::operator=(const Edge& edge)
{
	if (this == &edge)
		return *this;

	this->weight = edge.weight;
	this->data = edge.data;
	this->isWeightSet = edge.isWeightSet;
	this->isDataSet = edge.isDataSet;

	return *this;
}

template <class Vertex_T, class Weight_T, class Data_T> 
const typename Weight_T& Edge<Vertex_T, Weight_T, Data_T>::getWeight() const
{
	if (this->isWeightSet)
		return this->weight;
	else
		throw Edge_exception("���� ���� �� �����������");
}

template <class Vertex_T, class Weight_T, class Data_T>
const typename Data_T& Edge<Vertex_T, Weight_T, Data_T>::getData() const
{
	if (this->isDataSet)
		return this->data;
	else
		throw Edge_exception("���� ������ �� �����������");
}
//*************************************************************************************************************************************************//


#endif
