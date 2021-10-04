// Edge.h
#pragma once
#ifndef _INC_EDGE_
#define _INC_EDGE_
// Подключаемые заголовочные файлы
#include "Graph_exception.h"


//*********************************************** Класс исключений ребра графа ********************************************************************//
class Edge_exception: public Graph_exception 
{
	
	using string = std::string;

public:

	Edge_exception(const string& error) : Graph_exception(error) {}

};
//*************************************************************************************************************************************************//


//**************************************************** Класс ребра графа **************************************************************************//
template <class Vertex_T, class Weight_T, class Data_T>
class Edge
{

	const Vertex_T& outV;	// Указатель на дескриптор вершины, откуда ребро выходит
	const Vertex_T& inV;	// Указатель на дескриптор вершины, куда ребро входит

	Weight_T weight;	// Вес ребра
	Data_T data;		// Данные ребра

	bool isWeightSet;	// Инициализировано ли поле веса
	bool isDataSet;		// Инициализировано ли поле данных

public:

	// Конструктор ребра по с параметрами: дескрипторами вершины исхода outV и вершины входа inV
	Edge(const Vertex_T& outV, const Vertex_T& inV) :
		outV(outV), inV(inV), weight(), data(),
		isWeightSet(false), isDataSet(false) {}
	// Конструктор ребра с параметрами: дескрипторами вершины исхода outV и вершины входа inV и весом ребра weight
	Edge(const Vertex_T& outV, const Vertex_T& inV, const Weight_T& weight) :
		outV(outV), inV(inV), weight(weight), data(),
		isWeightSet(true), isDataSet(false) {}
	// Конструктор ребра с параметрами: дескрипторами вершины исхода outV и вершины входа inV, весом ребра weight и данными data
	Edge(const Vertex_T& outV, const Vertex_T& inV, const Weight_T& weight, const Data_T& data) :
		outV(outV), inV(inV), weight(weight), data(data),
		isWeightSet(true), isDataSet(true) {}
	// Конструктор копирования рёбер
	Edge(const Edge& edge) : outV(edge.outV), inV(edge.inV), weight(edge.weight),
		data(edge.data), isWeightSet(edge.isWeightSet), isDataSet(edge.isDataSet) {}

	// Оператор присваивания рёбер
	const Edge& operator=(const Edge& edge);

	// Получение дескриптора вершины исхода
	const Vertex_T& getVertexOut() const { return this->outV; }
	// Получение дескриптора вершины входа
	const Vertex_T& getVertexIn() const { return this->inV; }

	// Установить вес ребра
	void setWeight(const Weight_T& weight) { this->weight = weight; isWeightSet = true; }
	// Установить данные ребра
	void setData(const Data_T& data) { this->data = data; isDataSet = true; }

	// Получить вес ребра
	const Weight_T& getWeight() const;
	// Получить данные ребра
	const Data_T& getData() const;

};
//*************************************************************************************************************************************************//


//******************************************************** Реализация методов Edge ****************************************************************//
// public-методы
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
		throw Edge_exception("поле веса не установлено");
}

template <class Vertex_T, class Weight_T, class Data_T>
const typename Data_T& Edge<Vertex_T, Weight_T, Data_T>::getData() const
{
	if (this->isDataSet)
		return this->data;
	else
		throw Edge_exception("поле данных не установлено");
}
//*************************************************************************************************************************************************//


#endif
