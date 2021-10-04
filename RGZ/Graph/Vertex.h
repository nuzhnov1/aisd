// Vertex.h
#pragma once
#ifndef _INC_VERTEX_
#define _INC_VERTEX_
// Подключаемые заголовочные файлы
#include "Graph_exception.h"


//********************************************** Класс исключений вершины графа *******************************************************************//
class Vertex_exception: public Graph_exception
{
	
	using string = std::string;

public:

	Vertex_exception(const string& error) : Graph_exception(error) {}

};
//*************************************************************************************************************************************************//


//**************************************************** Класс вершины графа ************************************************************************//
template <class Name_T, class Data_T> 
class Vertex
{

	Name_T name;	// Имя вершины
	Data_T data;	// Данные вершины

	bool isNameSet;	// Инициализировано ли поле имени
	bool isDataSet;	// Инициализировано ли поле данных

public:

	// Конструктор вершины по умолчанию
	Vertex() : name(), data(), isNameSet(false), isDataSet(false) {}
	// Конструктор вершины с параметрами: именем name и данными data
	Vertex(const Name_T& name, const Data_T& data) :
		name(name), data(data), isNameSet(true), isDataSet(true) {}
	// Конструктор копирования вершины
	Vertex(const Vertex& vertex) :
		name(vertex.name), data(vertex.data),
		isNameSet(vertex.isNameSet), isDataSet(vertex.isDataSet) {}

	// Оператор равенства вершин
	bool operator==(const Vertex& vertex) const;
	// Оператор неравенства вершин
	bool operator!=(const Vertex& vertex) const;
	// Оператор присваивания вершин
	const Vertex& operator=(const Vertex& vertex);

	// Установить имя вершины
	void setName(const Name_T& name) { this->name = name; isNameSet = true; }
	// Установить данные вершины
	void setData(const Data_T& data) { this->data = data; isDataSet = true; }

	// Получить имя вершины
	const Name_T& getName() const;
	// Получить данные вершины
	const Data_T& getData() const;

};
//*************************************************************************************************************************************************//


//******************************************************** Реализация методов Vertex **************************************************************//
// public-методы
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
		throw Vertex_exception("поле имени не установлено");
}

template <class Name_T, class Data_T>
const typename Data_T& Vertex<Name_T, Data_T>::getData() const
{
	if (this->isDataSet)
		return this->data;
	else
		throw Vertex_exception("поле данных не установлено");
}
//*************************************************************************************************************************************************//


#endif
