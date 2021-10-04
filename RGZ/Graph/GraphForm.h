// GraphForm.h
#pragma once
#ifndef _INC_GRAPH_FORM_
#define _INC_GRAPH_FORM_
// Подключаемые заголовочные файлы
#include <vector>


//**************************************************** Класс-перечисление форм графа **************************************************************//
enum class GraphFormType
{
	M_GRAPH,	// M-граф
	L_GRAPH		// L-граф
};
//*************************************************************************************************************************************************//


//**************************************************** Класс формы представления графа ************************************************************//
template <class Vertex_T, class Edge_T>
class GraphForm
{

	template <typename T> using vector = std::vector<T>;

protected:

	const vector<Vertex_T*>& vectorVertex;	// Ссылка на вектор указателей на вершины графа
	size_t edgesCount;						// Количество рёбер графа
	bool directed;							// Является ли граф ориентированным

	// Получить число вершин в графе
	size_t _getVertexCount() const { return this->vectorVertex.size(); }

public:

	// Конструктор формы графа с параметрами: ссылкой на вектор вершин vectorVertex и ориентированностью графа directed
	GraphForm(const vector<Vertex_T*>& vectorVertex, bool directed) :
		vectorVertex(vectorVertex), edgesCount(0), directed(directed) {}
	// Виртуальный деструктор для формы графа
	virtual ~GraphForm() {}

	// Оператор присваивания графов
	virtual const GraphForm& operator=(const GraphForm& graph) = 0;

	// Получить число рёбер в графе
	size_t getEdgesCount() const;
	// Есть ли ребро в графе
	virtual bool isHasEdge(size_t outV, size_t inV) const = 0;
	// Получить ребро, соединающее вершины outV и inV
	virtual Edge_T& getEdge(size_t outV, size_t inV) const = 0;

	// Добавить вершину в граф
	virtual void insertVertex(size_t index) = 0;
	// Удалить вершину графа
	virtual void deleteVertex(size_t index) = 0;
	// Добавить ребро в граф, соединающего вершины outV и inV, с заданными параметрами в edge
	virtual void insertEdge(size_t outV, size_t inV, Edge_T* edge) = 0;
	// Удалить ребро из графа, соединающего вершины outV и inV
	virtual void deleteEdge(size_t outV, size_t inV) = 0;

	// Вывести граф
	virtual void print() const = 0;

	// Класс формы итератора рёбер
	class EdgeIteratorForm
	{

	public:

		// Виртуальный деструктор формы итератора рёбер
		virtual ~EdgeIteratorForm() {}

		// Получить дескриптор ребра графа, на который в данный момент указывает итератор
		virtual Edge_T& operator*() const = 0;

		// Префиксный оператор инкрементирования - переход к следующему ребру графа
		virtual EdgeIteratorForm& operator++() = 0;

		// Оператор равенства итераторов
		virtual bool operator==(const EdgeIteratorForm& iter) const = 0;
		// Оператор неравенства итераторов
		virtual bool operator!=(const EdgeIteratorForm& iter) const = 0;

		// Оператор присваивания итераторов
		virtual const EdgeIteratorForm& operator=(const EdgeIteratorForm& iter) = 0;

	};

	// Класс формы итератора исходящих вершин
	class OutEdgeIteratorForm
	{

	public:

		// Виртуальный деструктор формы итератора исходящий рёбер
		virtual ~OutEdgeIteratorForm() {}

		// Получить дескриптор ребра графа, на который в данный момент указывает итератор
		virtual Edge_T& operator*() const = 0;

		// Префиксный оператор инкрементирования - переход к следующему ребру графа
		virtual OutEdgeIteratorForm& operator++() = 0;

		// Оператор равенства итераторов
		virtual bool operator==(const OutEdgeIteratorForm& iter) const = 0;
		// Оператор неравенства итераторов
		virtual bool operator!=(const OutEdgeIteratorForm& iter) const = 0;

		// Оператор присваивания итераторов
		virtual const OutEdgeIteratorForm& operator=(const OutEdgeIteratorForm& iter) = 0;

	};

};
//*************************************************************************************************************************************************//


//****************************************************** Реализация методов ListGraph *************************************************************//
template <class Vertex_T, class Edge_T>
typename size_t GraphForm<Vertex_T, Edge_T>::getEdgesCount() const 
{
	if (this->directed)
		return this->edgesCount;
	else
		return static_cast<size_t>(this->edgesCount / 2);
}
//*************************************************************************************************************************************************//


#endif
