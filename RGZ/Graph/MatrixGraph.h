// MatrixGraph.h
#pragma once
#ifndef _INC_MATRIX_GRAPH_
#define _INC_MATRIX_GRAPH_
// Подключаемые заголовочные файлы
#include <iostream>
#include <iomanip>
#include <locale>
#include "GraphForm.h"
#include "Graph_exception.h"


//********************************************** Класс матричной формы представления графа ********************************************************//
template <class Vertex_T, class Edge_T>
class MatrixGraph: public GraphForm<Vertex_T, Edge_T>
{
	
	template <typename T> using vector = std::vector<T>;
	using GraphForm = GraphForm<Vertex_T, Edge_T>;
	using EdgeIteratorForm = typename GraphForm::EdgeIteratorForm;
	using OutEdgeIteratorForm = typename GraphForm::OutEdgeIteratorForm;

	vector<vector<Edge_T*>> matrix;	// Матрица смежности

public:

	// Конструктор матричной формы графа с параметрами: ссылкой на вектор вершин vectorVertex и ориентированностью графа directed
	MatrixGraph(const vector<Vertex_T*>& vectorVertex, bool directed) : GraphForm(vectorVertex, directed), matrix() {}
	// Конструктор копирования матричной формы графа
	MatrixGraph(const MatrixGraph& graph, const vector<Vertex_T*>& vectorVertex);
	// Деструктор матричной формы графа
	~MatrixGraph();

	// Оператор присваивания графов
	virtual const MatrixGraph& operator=(const GraphForm& graph) final;

	// Есть ли ребро в графе
	virtual bool isHasEdge(size_t outV, size_t inV) const final { return this->matrix[outV][inV] != nullptr; }
	// Получить ребро, соединающее вершины outV и inV
	virtual Edge_T& getEdge(size_t outV, size_t inV) const final;

	// Добавить вершину в граф
	virtual void insertVertex(size_t index) final;
	// Удалить вершину графа
	virtual void deleteVertex(size_t index) final;
	// Добавить ребро в граф, соединающего вершины outV и inV, с заданными параметрами в edge
	virtual void insertEdge(size_t outV, size_t inV, Edge_T* edge) final;
	// Удалить ребро из графа, соединающего вершины outV и inV
	virtual void deleteEdge(size_t outV, size_t inV) final;

	// Вывести граф
	virtual void print() const final;

	// Класс итератора рёбер графа матричной формы
	class MatrixEdgeIterator : public EdgeIteratorForm
	{

		const MatrixGraph* graph_p;	// Указатель на текущий M-граф
		size_t curI, curJ;			// Номер строки и столбца текущего ребра в матрице смежности

	public:

		// Конструктор итератора с параметрами: ссылкой на матричный граф graph и состоянием итератора toBegin
		MatrixEdgeIterator(const MatrixGraph& graph, bool toBegin = true);
		// Конструктор копирования итераторов
		MatrixEdgeIterator(const MatrixEdgeIterator& iter) : graph_p(iter.graph_p), curI(iter.curI), curJ(iter.curJ) {}

		// Получить дескриптор ребра графа, на который в данный момент указывает итератор
		virtual Edge_T& operator*() const final;

		// Префиксный оператор инкрементирования - переход к следующему ребру графа
		virtual MatrixEdgeIterator& operator++() final;

		// Оператор равенства итераторов
		virtual bool operator==(const EdgeIteratorForm& iter) const final;
		// Оператор неравенства итераторов
		virtual bool operator!=(const EdgeIteratorForm& iter) const final;

		// Оператор присваивания итераторов
		virtual const MatrixEdgeIterator& operator=(const EdgeIteratorForm& iter) final;

		// Возвращает итератор, установленный на начало
		static MatrixEdgeIterator begin(const MatrixGraph& graph) { return MatrixEdgeIterator(graph, true); }
		// Возвращает итератор, установленный на конец
		static MatrixEdgeIterator end(const MatrixGraph& graph) { return MatrixEdgeIterator(graph, false); }

	};

	// Класс итератора исходящих рёбер графа матричной формы
	class MatrixOutEdgeIterator : public OutEdgeIteratorForm
	{

		const MatrixGraph* graph_p;	// Указатель на текущий M-граф
		size_t curI, curJ;			// Номер строки и столбца текущего ребра в матрице смежности

	public:

		// Конструктор итератора с параметрами: ссылкой на матричный граф graph, индексом вершины index и состоянием итератора toBegin
		MatrixOutEdgeIterator(const MatrixGraph& graph, size_t index, bool toBegin = true);
		// Конструктор копирования итераторов
		MatrixOutEdgeIterator(const MatrixOutEdgeIterator& iter) : graph_p(iter.graph_p), curI(iter.curI), curJ(iter.curJ) {}

		// Получить дескриптор ребра графа, на который в данный момент указывает итератор
		virtual Edge_T& operator*() const final;

		// Префиксный оператор инкрементирования - переход к следующему ребру графа
		virtual MatrixOutEdgeIterator& operator++() final;

		// Оператор равенства итераторов
		virtual bool operator==(const OutEdgeIteratorForm& iter) const final;
		// Оператор неравенства итераторов
		virtual bool operator!=(const OutEdgeIteratorForm& iter) const final;

		// Оператор присваивания итераторов
		virtual const MatrixOutEdgeIterator& operator=(const OutEdgeIteratorForm& iter) final;

		// Возвращает итератор, установленный на начало
		static MatrixOutEdgeIterator begin(const MatrixGraph& graph, size_t index) { return MatrixOutEdgeIterator(graph, index, true); }
		// Возвращает итератор, установленный на конец
		static MatrixOutEdgeIterator end(const MatrixGraph& graph, size_t index) { return MatrixOutEdgeIterator(graph, index, false); }

	};

};
//*************************************************************************************************************************************************//


//***************************************************** Реализация методов MatrixGraph ************************************************************//
// public-методы
template <class Vertex_T, class Edge_T>
MatrixGraph<Vertex_T, Edge_T>::MatrixGraph(const MatrixGraph& graph, const vector<Vertex_T*>& vectorVertex):
	GraphForm(vectorVertex, graph.directed)
{
	this->directed = graph.directed;
	this->edgesCount = graph.edgesCount;
	this->matrix = vector<vector<Edge_T*>>();

	// В матрице смежности для каждой ячейки создаём ребро, которое связывает
	// те же номера вершин, что и в копируемом графе
	for (size_t i = 0, size = this->_getVertexCount(); i < size; i++)
	{
		// Добавляем пустую строку размера size
		this->matrix.push_back(vector<Edge_T*>(size, nullptr));

		for (size_t j = 0; j < size; j++)
			if (graph.matrix[i][j] != nullptr)
			{
				// Создаём ребро
				this->matrix[i][j] = new Edge_T(*this->vectorVertex[i], *this->vectorVertex[j]);

				// Пытаемся установить вес в ребро
				try { this->matrix[i][j]->setWeight(graph.matrix[i][j]->getWeight()); }
				catch (Graph_exception&) {}

				// Пытаемся установить данные в ребро
				try { this->matrix[i][j]->setData(graph.matrix[i][j]->getData()); }
				catch (Graph_exception&) {}
			}
	}
}

template <class Vertex_T, class Edge_T>
MatrixGraph<Vertex_T, Edge_T>::~MatrixGraph()
{
	// Проходим по матрице смежности и удаляем рёбра графа
	for (auto& vectorEdge : this->matrix)
	{
		for (const auto& Edge : vectorEdge)
			if (Edge != nullptr)
				delete Edge;
		
		vectorEdge.clear();
	}

	this->matrix.clear();
}

template <class Vertex_T, class Edge_T>
const typename MatrixGraph<Vertex_T, Edge_T>& MatrixGraph<Vertex_T, Edge_T>::operator=(const GraphForm& graph)
{
	// Приводим абстрактную форму графа к матричной форме
	const MatrixGraph& m_graph = dynamic_cast<const MatrixGraph&>(graph);

	if (this == &m_graph)
		return *this;

	// Проходим по матрице смежности и удаляем рёбра графа
	for (auto& vectorEdge : this->matrix)
	{
		for (const auto& Edge : vectorEdge)
			if (Edge != nullptr)
				delete Edge;

		vectorEdge.clear();
	}

	this->matrix.clear();

	this->directed = m_graph.directed;
	this->edgesCount = m_graph.edgesCount;
	this->matrix = vector<vector<Edge_T*>>();

	// В матрице смежности для каждой ячейки создаём ребро, которое связывает
	// те же номера вершин, что и в копируемом графе
	for (size_t i = 0, size = this->_getVertexCount(); i < size; i++)
	{
		// Добавляем пустую строку размера size
		this->matrix.push_back(vector<Edge_T*>(size, nullptr));

		for (size_t j = 0; j < size; j++)
			if (m_graph.matrix[i][j] != nullptr)
			{
				// Создаём ребро
				this->matrix[i][j] = new Edge_T(*this->vectorVertex[i], *this->vectorVertex[j]);

				// Пытаемся установить вес в ребро
				try { this->matrix[i][j]->setWeight(m_graph.matrix[i][j]->getWeight()); }
				catch (Graph_exception&) {}

				// Пытаемся установить данные в ребро
				try { this->matrix[i][j]->setData(m_graph.matrix[i][j]->getData()); }
				catch (Graph_exception&) {}
			}
	}

	return *this;
}

template <class Vertex_T, class Edge_T>
typename Edge_T& MatrixGraph<Vertex_T, Edge_T>::getEdge(size_t outV, size_t inV) const
{
	// Проверяем, есть ли заданное ребро в графе
	if (this->matrix[outV][inV] == nullptr)
		throw Graph_exception("такого ребра нет в графе");

	return *(this->matrix[outV][inV]);
}

template <class Vertex_T, class Edge_T>
typename void MatrixGraph<Vertex_T, Edge_T>::insertVertex(size_t index)
{
	// Создаём столбец матрицы
	for (auto& vector : this->matrix)
		vector.insert(vector.begin() + index, nullptr);

	// Создаём новую строку матрицы
	this->matrix.insert(this->matrix.begin() + index, vector<Edge_T*>(this->matrix.size() + 1, nullptr));
}

template <class Vertex_T, class Edge_T>
typename void MatrixGraph<Vertex_T, Edge_T>::deleteVertex(size_t index)
{
	// Сначала удаляем все исходящие рёбра - строку index матрицы смежности
	for (const auto& edge : this->matrix[index])
		if (edge != nullptr)
		{
			this->edgesCount--;
			delete edge;
		}
	this->matrix.erase(this->matrix.begin() + index);

	// А затем удаляем все входящие рёбра - столбец index матрицы смежности
	for (auto& vectorEdge : this->matrix)
	{
		if (vectorEdge[index] != nullptr)
		{
			this->edgesCount--;
			delete vectorEdge[index];
		}

		vectorEdge.erase(vectorEdge.begin() + index);
	}
}

template <class Vertex_T, class Edge_T>
typename void MatrixGraph<Vertex_T, Edge_T>::insertEdge(size_t outV, size_t inV, Edge_T* edge)
{
	// Проверяем есть ли ребро в графе
	if (this->matrix[outV][inV] != nullptr)
		throw Graph_exception("такое ребро уже есть в графе");

	// Если нет, то добавляем его в граф
	this->matrix[outV][inV] = edge;

	this->edgesCount++;
}

template <class Vertex_T, class Edge_T>
typename void MatrixGraph<Vertex_T, Edge_T>::deleteEdge(size_t outV, size_t inV)
{
	// Проверяем есть ли ребро в графе
	if (this->matrix[outV][inV] == nullptr)
		throw Graph_exception("такого ребра нет в графе");

	// Если ребро принадлежит графу, то удаляем его
	delete this->matrix[outV][inV];
	this->matrix[outV][inV] = nullptr;

	this->edgesCount--;
}

template <class Vertex_T, class Edge_T>
typename void MatrixGraph<Vertex_T, Edge_T>::print() const
{
	using std::cout;
	using std::endl;
	using std::setw;

	setlocale(LC_ALL, "Russian");

	// Если нет вершин, то переходим на следующую строку и выходим
	if (this->matrix.size() == 0)
	{
		cout << endl;

		return;
	}

	// Вывод номеров столбцов
	cout << "     ";
	for (auto& vertex : this->vectorVertex)
		cout << std::setw(4) << vertex->getName();
	cout << endl;
	// Вывод верхней линии
	cout << "_____";
	for (size_t i = 0, size = this->matrix.size(); i < size; i++)
		cout << "____";
	cout << endl;

	// Вывод номеров строк и весов рёбер
	for (size_t i = 0, end = this->matrix.size(); i < end; i++)
	{
		cout << " " << std::setw(2) << this->vectorVertex[i]->getName() << " |";
		for (const auto& edge : this->matrix[i])
			if (edge != nullptr)
			{
				cout << std::setw(4);
				try { cout << edge->getWeight(); }			// Пытаемся обратиться к весу ребра
				catch (Graph_exception&) { cout << "?"; }	// Если он не установлен, выводим знак - ?
			}
			else
				cout << std::setw(4) << "inf";	// Если ребра нет, то вес равен бесконечности
		cout << endl;
	}
	
	// Выводим нижнюю линию
	cout << "_____";
	for (size_t i = 0, size = this->matrix.size(); i < size; i++)
		cout << "____";
	cout << endl;

	// Выводим информацию об обозначениях
	cout << "Знаком \"?\" обозначены рёбра, вес которых не установлен." << endl;
	cout << "Строка \"inf\" означает то, что ребра между вершинами нет." << endl;
}
//*************************************************************************************************************************************************//


//************************************************** Реализация методов MatrixEdgeIterator ********************************************************//
// public-методы:
template <class Vertex_T, class Edge_T>
MatrixGraph<Vertex_T, Edge_T>::MatrixEdgeIterator::MatrixEdgeIterator(const MatrixGraph& graph, bool toBegin)
{
	this->graph_p = &graph;

	// Если решили изначально не устанавливать итератор
	if (!toBegin)
	{
		this->curI = -1;
		this->curJ = -1;

		return;
	}

	// Проходим по матрице смежности в поисках первого ребра
	size_t end_row = this->graph_p->matrix.size();
	for (this->curI = 0; this->curI < end_row; this->curI++)
	{
		// Если граф неориентированный, то обходим только нижний треугольник матрицы относительно
		// главной диагонали включительно
		size_t end_col = (this->graph_p->directed) ? end_row : this->curI + 1;

		for (this->curJ = 0; this->curJ < end_col; this->curJ++)
			if (this->graph_p->matrix[this->curI][this->curJ] != nullptr)
				return;
	}

	// Если итератор так и не был установлен
	this->curI = -1;
	this->curJ = -1;
}

template <class Vertex_T, class Edge_T>
typename Edge_T& MatrixGraph<Vertex_T, Edge_T>::MatrixEdgeIterator::operator*() const
{
	if (this->curI == -1 || this->curJ == -1)
		throw Graph_exception("итератор не установлен");

	return *(this->graph_p->matrix[this->curI][this->curJ]);
}

template <class Vertex_T, class Edge_T>
typename MatrixGraph<Vertex_T, Edge_T>::MatrixEdgeIterator& MatrixGraph<Vertex_T, Edge_T>::MatrixEdgeIterator::operator++()
{
	if (this->curI == -1 || this->curJ == -1)
		throw Graph_exception("итератор не установлен");

	// Проходим по матрице смежности
	for (size_t end_row = this->graph_p->matrix.size(); this->curI < end_row; this->curI++)
	{
		// Если граф неориентированный, то доходим только до главной диагонали включительно
		size_t end_col = (this->graph_p->directed) ? end_row : this->curI + 1;

		// Проходим по текущей строке и ищем первую ненулевую ячейку
		for (this->curJ++; this->curJ < end_col; this->curJ++)
			if (this->graph_p->matrix[this->curI][this->curJ] != nullptr)
				return *this;

		this->curJ = -1;
	}

	this->curI = -1;

	return *this;
}

template <class Vertex_T, class Edge_T>
typename bool MatrixGraph<Vertex_T, Edge_T>::MatrixEdgeIterator::operator==(const EdgeIteratorForm& iter) const
{
	const MatrixEdgeIterator& m_iter = dynamic_cast<const MatrixEdgeIterator&>(iter);

	return this->graph_p == m_iter.graph_p && this->curI == m_iter.curI && this->curJ == m_iter.curJ;
}

template <class Vertex_T, class Edge_T>
typename bool MatrixGraph<Vertex_T, Edge_T>::MatrixEdgeIterator::operator!=(const EdgeIteratorForm& iter) const
{
	const MatrixEdgeIterator& m_iter = dynamic_cast<const MatrixEdgeIterator&>(iter);

	return !(this->graph_p == m_iter.graph_p && this->curI == m_iter.curI && this->curJ == m_iter.curJ);
}

template <class Vertex_T, class Edge_T>
const typename MatrixGraph<Vertex_T, Edge_T>::MatrixEdgeIterator& MatrixGraph<Vertex_T, Edge_T>::MatrixEdgeIterator::operator=(const EdgeIteratorForm& iter)
{
	const MatrixEdgeIterator& m_iter = dynamic_cast<const MatrixEdgeIterator&>(iter);

	if (this == &m_iter)
		return *this;

	this->graph_p = m_iter.graph_p;
	this->curI = m_iter.curI;
	this->curJ = m_iter.curJ;

	return *this;
}
//*************************************************************************************************************************************************//


//************************************************* Реализация методов MatrixOutEdgeIterator ******************************************************//
// public-методы:
template <class Vertex_T, class Edge_T>
MatrixGraph<Vertex_T, Edge_T>::MatrixOutEdgeIterator::MatrixOutEdgeIterator(const MatrixGraph& graph, size_t index, bool toBegin)
{
	this->graph_p = &graph;
	this->curI = index;

	// Если решили изначально не устанавливать итератор
	if (!toBegin)
	{
		this->curJ = -1;

		return;
	}

	// Проходим по вектору исходящих рёбер в матрице смежности в поиске первого ребра
	size_t size = this->graph_p->matrix.size();
	for (this->curJ = 0; this->curJ < size; this->curJ++)
		if (this->graph_p->matrix[this->curI][this->curJ] != nullptr)
			return;

	// Если у данной вершины нет исходящих рёбер
	this->curJ = -1;
}

template <class Vertex_T, class Edge_T>
typename Edge_T& MatrixGraph<Vertex_T, Edge_T>::MatrixOutEdgeIterator::operator*() const
{
	if (this->curI == -1 || this->curJ == -1)
		throw Graph_exception("итератор не установлен");

	return *(this->graph_p->matrix[this->curI][this->curJ]);
}

template <class Vertex_T, class Edge_T>
typename MatrixGraph<Vertex_T, Edge_T>::MatrixOutEdgeIterator& MatrixGraph<Vertex_T, Edge_T>::MatrixOutEdgeIterator::operator++()
{
	if (this->curI == -1 || this->curJ == -1)
		throw Graph_exception("итератор не установлен");

	// Инкрементируем curJ, и проходим дальше по строке в поиске следующего ребра
	size_t size = this->graph_p->matrix.size();
	for (this->curJ++; this->curJ < size; this->curJ++)
		if (this->graph_p->matrix[this->curI][this->curJ] != nullptr)
			return *this;

	this->curJ = -1;

	return *this;
}

template <class Vertex_T, class Edge_T>
typename bool MatrixGraph<Vertex_T, Edge_T>::MatrixOutEdgeIterator::operator==(const typename OutEdgeIteratorForm& iter) const
{
	const MatrixOutEdgeIterator& m_iter = dynamic_cast<const MatrixOutEdgeIterator&>(iter);

	return this->graph_p == m_iter.graph_p && this->curI == m_iter.curI && this->curJ == m_iter.curJ;
}

template <class Vertex_T, class Edge_T>
typename bool MatrixGraph<Vertex_T, Edge_T>::MatrixOutEdgeIterator::operator!=(const typename OutEdgeIteratorForm& iter) const
{
	const MatrixOutEdgeIterator& m_iter = dynamic_cast<const MatrixOutEdgeIterator&>(iter);

	return !(this->graph_p == m_iter.graph_p && this->curI == m_iter.curI && this->curJ == m_iter.curJ);
}

template <class Vertex_T, class Edge_T>
const typename MatrixGraph<Vertex_T, Edge_T>::MatrixOutEdgeIterator& MatrixGraph<Vertex_T, Edge_T>::MatrixOutEdgeIterator::operator=(const typename OutEdgeIteratorForm& iter)
{
	const MatrixOutEdgeIterator& m_iter = dynamic_cast<const MatrixOutEdgeIterator&>(iter);

	if (this == &m_iter)
		return *this;

	this->graph_p = m_iter.graph_p;
	this->curI = m_iter.curI;
	this->curJ = m_iter.curJ;

	return *this;
}
//*************************************************************************************************************************************************//


#endif
