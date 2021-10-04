// Graph.h
#pragma once
#ifndef _INC_GRAPH_
#define _INC_GRAPH_
// Подключаемые заголовочные файлы
#include <vector>
#include <limits>
#include "GraphForm.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "Graph_exception.h"


//*************************************************************** Класс графа *********************************************************************//
template <class Vertex_T, class Edge_T>
class Graph
{

	template <typename T> using vector = std::vector<T>;
	using GraphForm = GraphForm<Vertex_T, Edge_T>;
	using EdgeIteratorForm = typename GraphForm::EdgeIteratorForm;
	using OutEdgeIteratorForm = typename GraphForm::OutEdgeIteratorForm;
	using MatrixGraph = MatrixGraph<Vertex_T, Edge_T>;
	using MatrixEdgeIterator = typename MatrixGraph::MatrixEdgeIterator;
	using MatrixOutEdgeIterator = typename MatrixGraph::MatrixOutEdgeIterator;
	using ListGraph = ListGraph<Vertex_T, Edge_T>;
	using ListEdgeIterator = typename ListGraph::ListEdgeIterator;
	using ListOutEdgeIterator = typename ListGraph::ListOutEdgeIterator;

	vector<Vertex_T*> vectorVertex;		// Вектор указателей на вершины графа
	GraphForm* form;					// Указатель на форму графа
	bool directed;						// Ориентирован ли граф
	GraphFormType type;					// Тип формы графа(L-граф или M-граф)

	// Получить номер вершины в векторе
	size_t _getVertexIndex(const Vertex_T& vertex) const;

public:

	// Конструктор графа по умолчанию(пустой неориентированный L-граф)
	Graph();
	// Конструктор графа с параметрами: vertices - числом вершин, directed - ориентированностью
	// и form - формой представления графа
	Graph(size_t vertices, bool directed, GraphFormType form);
	// Конструктор графа с параметрами: vertices - числом вершин, edges - числом случайных рёбер
	// directed - ориентированностью и form - формой представления графа
	Graph(size_t vertices, size_t edges, bool directed, GraphFormType form);
	// Конструктор копирования графов
	Graph(const Graph& graph);
	// Деструктор графа
	~Graph();

	// Оператор присваивания графов
	const Graph& operator=(const Graph& graph);

	// Получить число вершин графа
	size_t getVertexCount() const { return this->vectorVertex.size(); }
	// Получить число рёбер графа
	size_t getEdgesCount() const { return this->form->getEdgesCount(); }
	// Проверка графа на ориентированность
	bool isDirected() const { return this->directed; }
	// Получить форму графа(L-граф или M-граф)
	GraphFormType getFormType() const { return this->type; }
	// Получить коэффициент насыщенности графа
	double getK() const;

	// Проверить, есть ли у графа вершина, заданная дескриптором
	bool isHasVertex(const Vertex_T& vertex) const { return _getVertexIndex(vertex) != -1; }
	// Проверить, есть ли у графа ребро с заданнными дескрипторами входной и выходной вершин
	bool isHasEdge(const Vertex_T& outV, const Vertex_T& inV) const;

	// Получить дескриптор вершины по имени, в заданном дескрипторе
	const Vertex_T& getVertex(const Vertex_T& vertex) const;
	// Получить дескриптор ребра, заданного дескрипторами выходной и входной вершин
	const Edge_T& getEdge(const Vertex_T& outV, const Vertex_T& inV) const;

	// Установить параметры вершины oldV, заданные в дескрипторе вершины newV
	void setVertex(const Vertex_T& oldV, const Vertex_T& newV);
	// Установить параметры ребра oldE, заданные в дескрипторе ребра newE
	void setEdge(const Edge_T& oldE, const Edge_T& newE);

	// Преобразовать граф в L-форму
	void toListGraph();
	// Преобразовать граф в M-форму
	void toMatrixGraph();

	// Добавить вершину в граф, с заданным дескриптором. Возврат - ссылка на добавленную вершину
	const Vertex_T& insertVertex(const Vertex_T& vertex);
	// Удалить вершину из графа, заданной дескриптором
	void deleteVertex(const Vertex_T& vertex);
	// Добавить ребро графа, заданного дескрипторами выходной и входной вершин. Возврат - ссылка на добавленное ребро
	const Edge_T& insertEdge(const Vertex_T& outV, const Vertex_T& inV);
	// Удалить ребро графа, заданного дескрипторами выходной и входной вершин
	void deleteEdge(const Vertex_T& outV, const Vertex_T& inV);

	// Вывести структуру графа в консоль
	void print() const { this->form->print(); }

	// Класс итератора вершин
	class VertexIterator
	{

		const Graph* graph;	// Указатель на текущий граф
		size_t cur;			// Индекс текущей вершины

	public:

		// Конструктор итератора вершин по умолчанию
		VertexIterator() : graph(nullptr), cur(-1) {}
		// Конструктор итератора вершин с ссылкой на граф
		VertexIterator(const Graph& graph, bool toBegin = true);
		// Конструктор копирования итератора вершин
		VertexIterator(const VertexIterator& iter) : graph(iter.graph), cur(iter.cur) {}

		// Получить дескриптор вершины, на которую в данный момент установлен итератор
		Vertex_T& operator*() const;

		// Префиксный оператор инкрементирования - переход к следующей вершине графа
		VertexIterator& operator++();
		// Постфиксный оператор инкрементирования
		VertexIterator operator++(int);

		// Оператор равенства итераторов
		bool operator==(const VertexIterator& iter) const;
		// Оператор неравенства итераторов
		bool operator!=(const VertexIterator& iter) const;

		// Оператор присваивания итераторов
		const VertexIterator& operator=(const VertexIterator& iter);

		// Возвращает итератор, установленный на начальную вершину
		static VertexIterator begin(const Graph& graph) { return VertexIterator(graph, true); }
		// Возвращает итератор, установленный на конец
		static VertexIterator end(const Graph& graph) { return VertexIterator(graph, false); }

	};

	// Класс итератора рёбер
	class EdgeIterator
	{

		const Graph* graph;		// Указатель на текущий граф
		EdgeIteratorForm* form;	// Указатель на сам итератор

	public:

		// Конструктор итератора рёбер по умолчанию
		EdgeIterator() : graph(nullptr), form(nullptr) {}
		// Конструктор итератора рёбер с параметрами: graph - ссылкой на текущий граф и toBegin - установкой на начало
		EdgeIterator(const Graph& graph, bool toBegin = true);
		// Конструктор копирования итераторов рёбер
		EdgeIterator(const EdgeIterator& iter);
		// Деструктор итератора рёбер
		~EdgeIterator() { delete this->form; }

		// Получить дескриптор ребра, на который в данный момент установлен итератор
		Edge_T& operator*() const;

		// Префиксный оператор инкрементирования - переход к следующему ребру графа
		EdgeIterator& operator++();
		// Постфиксный оператор инкрементирования
		EdgeIterator operator++(int);

		// Оператор равенства итераторов
		bool operator==(const EdgeIterator& iter) const;
		// Оператор неравенства итераторов
		bool operator!=(const EdgeIterator& iter) const;

		// Оператор присваивания итераторов
		const EdgeIterator& operator=(const EdgeIterator& iter);

		// Возвращает итератор, установленный на начальное ребро
		static EdgeIterator begin(const Graph& graph) { return EdgeIterator(graph, true); }
		// Возвращает итератор, установленный на конец
		static EdgeIterator end(const Graph& graph) { return EdgeIterator(graph, false); }

	};

	// Класс итератора исходящих рёбер
	class OutEdgeIterator
	{

		const Graph* graph;			// Указатель на текущий граф
		OutEdgeIteratorForm* form;	// Указатель на сам итератор

	public:

		// Конструктор итератора исходящих рёбер по умолчанию
		OutEdgeIterator() : graph(nullptr), form(nullptr) {}
		// Конструктор итератора исходящих рёбер с параметрами: graph - ссылкой на текущий граф,
		// vertex - вершиной и toBegin - установкой на начало
		OutEdgeIterator(const Graph& graph, const Vertex_T& vertex, bool toBegin = true);
		// Конструктор копирования итераторов исходящих рёбер
		OutEdgeIterator(const OutEdgeIterator& iter);
		// Деструктор итератора исходящих рёбер
		~OutEdgeIterator() { delete this->form; }

		// Получить дескриптор ребра, на который в данный момент установлен итератор
		Edge_T& operator*() const;

		// Префиксный оператор инкрементирования - переход к следующему ребру графа
		OutEdgeIterator& operator++();
		// Постфиксный оператор инкрементирования
		OutEdgeIterator operator++(int);

		// Оператор равенства итераторов
		bool operator==(const OutEdgeIterator& iter);
		// Оператор неравенства итераторов
		bool operator!=(const OutEdgeIterator& iter);

		// Оператор присваивания итераторов
		const OutEdgeIterator& operator=(const OutEdgeIterator& iter);
		
		// Возвращает итератор, установленный на начальное ребро
		static OutEdgeIterator begin(const Graph& graph, const Vertex_T& vertex) { return OutEdgeIterator(graph, vertex, true); }
		// Возвращает итератор, установленный на конец
		static OutEdgeIterator end(const Graph& graph, const Vertex_T& vertex) { return OutEdgeIterator(graph, vertex, false); }

	};

};
//*************************************************************************************************************************************************//


//**************************************************** Реализация методов Graph *******************************************************************//
// private-методы:
template <class Vertex_T, class Edge_T>
typename size_t Graph<Vertex_T, Edge_T>::_getVertexIndex(const Vertex_T& vertex) const
{
	// Проходим по вектору указателей на вершины в поиске вершины, равной vertex
	for (size_t i = 0, end = getVertexCount(); i < end; i++)
		if (vertex == *this->vectorVertex[i])
			return i;

	return -1;
}


// public-методы:
template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::Graph()
{
	// По умолчанию создаём пустой неориентированный L-граф

	this->vectorVertex = vector<Vertex_T*>();
	this->directed = false;
	this->type = GraphFormType::L_GRAPH;
	this->form = new ListGraph(this->vectorVertex, this->directed);
}

template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::Graph(size_t vertices, bool directed, GraphFormType form)
{
	// Создаём граф c количеством вершин, равным "vertices", имена которых будут:
	// "0", "1", ... , "vertices - 1". Данные являются случайными значением

	this->vectorVertex = vector<Vertex_T*>();
	this->directed = directed;
	this->type = form;
	
	// Создаём форму графа в зависимости от заданного параметра
	if (this->type == GraphFormType::L_GRAPH)
		this->form = new ListGraph(this->vectorVertex, this->directed);
	else
		this->form = new MatrixGraph(this->vectorVertex, this->directed);

	// Добавляем заданное число вершин
	for (size_t i = 0; i < vertices; i++)
	{
		Vertex_T* vertex = new Vertex_T(std::to_string(i), rand());

		this->vectorVertex.push_back(vertex);
		this->form->insertVertex(i);
	}
}

template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::Graph(size_t vertices, size_t edges, bool directed, GraphFormType form): Graph(vertices, directed, form)
{
	size_t size = getVertexCount();

	// Проверяем корректность параметра количества рёбер. А именно если их больше, чем возможно
	// для данного графа с заданным числом вершин, то корректируем этот параметр
	if (this->directed && (edges > size * (size - 1)))
		edges = size * (size - 1);
	else if (!this->directed && (edges > static_cast<size_t>(size * (size - 1) / 2)))
		edges = static_cast<size_t>(size * (size - 1) / 2);

	size_t outV = 0, inV = 0;

	// В качестве сида устанавливаем текущее время
	srand(static_cast<unsigned int>(time(nullptr)));

	// Генерируем edges случайных рёбер
	for (size_t i = 0; i < edges; i++)
	{
		outV = rand() % size;	// Случайно выбираем вершину исхода
		inV = rand() % size;	// Случайно выбираем вершину входа

		// Генерируем случайный числа до тех пор, пока полученного ребра не будет в графе
		// и оно не будет образовывать петлю
		while (this->form->isHasEdge(outV, inV) || outV == inV)
		{
			outV = rand() % size;
			inV = rand() % size;
		}

		// Как сгенерировали новую пару вершин, то добавляем ребро
		const Edge_T& edge = insertEdge(*this->vectorVertex[outV], *this->vectorVertex[inV]);

		// Устанавливаем случайный вес и данные
		setEdge(edge, Edge_T(edge.getVertexOut(), edge.getVertexIn(), rand() % 10 + 1, rand()));
	}	
}

template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::Graph(const Graph& graph)
{
	// Копируем вектор вершин, создаём новые вершины - копии вершин из graph
	this->vectorVertex = vector<Vertex_T*>(graph.vectorVertex.size(), nullptr);
	for (size_t i = 0, size = graph.vectorVertex.size(); i < size; i++)
		this->vectorVertex[i] = new Vertex_T(*graph.vectorVertex[i]);

	this->directed = graph.directed;
	this->type = graph.type;

	// Создаём форму графа, аналогичную форме graph
	if (this->type == GraphFormType::L_GRAPH)
		this->form = new ListGraph(dynamic_cast<const ListGraph&>(*graph.form), this->vectorVertex);
	else
		this->form = new MatrixGraph(dynamic_cast<const MatrixGraph&>(*graph.form), this->vectorVertex);
}

template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::~Graph()
{
	// Проходим по вектору, удаляя его вершины
	for (size_t i = 0, size = this->vectorVertex.size(); i < size; i++)
		delete this->vectorVertex[i];

	// Очищаем вектор и удаляем форму графа
	this->vectorVertex.clear();
	delete this->form;
}

template <class Vertex_T, class Edge_T>
const typename Graph<Vertex_T, Edge_T>& Graph<Vertex_T, Edge_T>::operator=(const Graph& graph)
{
	if (this == &graph)
		return *this;

	// Проходим по вектору, удаляя его вершины
	for (size_t i = 0, size = this->vectorVertex.size(); i < size; i++)
		delete this->vectorVertex[i];

	// Очищаем вектор и удаляем форму графа
	this->vectorVertex.clear();

	// Копируем вектор вершин, создаём новые вершины - копии вершин из graph
	this->vectorVertex = vector<Vertex_T*>(graph.vectorVertex.size(), nullptr);
	for (size_t i = 0, size = graph.vectorVertex.size(); i < size; i++)
		this->vectorVertex[i] = new Vertex_T(*graph.vectorVertex[i]);

	this->directed = graph.directed;

	// Если типы графов совпадают, то вызываем для них оператор присваивания
	if (this->type == graph.type)
		*this->form = *graph.form;
	// Если не совпадают
	else
	{
		// То сначала удаляем форму старого графа
		delete this->form;	
		this->type = graph.type;

		// А затем создаём новую форму - копию формы graph
		if (graph.type == GraphFormType::L_GRAPH)
			this->form = new ListGraph(dynamic_cast<const ListGraph&>(*graph.form), this->vectorVertex);
		else
			this->form = new MatrixGraph(dynamic_cast<const MatrixGraph&>(*graph.form), this->vectorVertex);
	}
	
	return *this;
}

template <class Vertex_T, class Edge_T>
typename double Graph<Vertex_T, Edge_T>::getK() const
{
	size_t size = getVertexCount();			// Текущее число вершин
	size_t max_edges = size * (size - 1);	// Максимальное число рёбер для этих вершин(исключая петли)

	// В случае неориентированного графа максимальное число рёбер в два раза меньше
	if (!this->directed)
		max_edges = max_edges / 2;

	// В вырожденном случае возвращаем NaN
	if (max_edges == 0)
		return std::numeric_limits<double>::quiet_NaN();
	else
		return static_cast<double>(getEdgesCount()) / static_cast<double>(max_edges);
}

template <class Vertex_T, class Edge_T>
typename bool Graph<Vertex_T, Edge_T>::isHasEdge(const Vertex_T& outV, const Vertex_T& inV) const
{
	// Сначала получаем индексы заданных вершин
	size_t outV_index = _getVertexIndex(outV);
	size_t inV_index = _getVertexIndex(inV);

	// Если какой-то из вершин нет, то выбрасываем исключение
	if (outV_index == -1)
		throw Graph_exception("вершины исхода с таким именем нет в графе");
	if (inV_index == -1)
		throw Graph_exception("вершины входа с таким именем нет в графе");

	return this->form->isHasEdge(outV_index, inV_index);
}

template <class Vertex_T, class Edge_T>
const typename Vertex_T& Graph<Vertex_T, Edge_T>::getVertex(const Vertex_T& vertex) const
{
	// Сначала получаем индекс вершины в графе
	size_t index = _getVertexIndex(vertex);

	// Если её нет в нём, то выбрасываем исключение
	if (index == -1)
		throw Graph_exception("вершины с таким именем нет в графе");

	return *this->vectorVertex[index];
}

template <class Vertex_T, class Edge_T>
const typename Edge_T& Graph<Vertex_T, Edge_T>::getEdge(const Vertex_T& outV, const Vertex_T& inV) const
{
	// Сначала получаем индексы заданных вершин
	size_t outV_index = _getVertexIndex(outV);
	size_t inV_index = _getVertexIndex(inV);

	// Если какой-то из вершин нет, то выбрасываем исключение
	if (outV_index == -1)
		throw Graph_exception("вершины исхода с таким именем нет в графе");
	if (inV_index == -1)
		throw Graph_exception("вершины входа с таким именем нет в графе");

	return this->form->getEdge(outV_index, inV_index);
}

template <class Vertex_T, class Edge_T>
typename void Graph<Vertex_T, Edge_T>::setVertex(const Vertex_T& oldV, const Vertex_T& newV)
{
	// Сначала получаем индекс вершины графа, где необходимо изменить данные
	long oldV_index = _getVertexIndex(oldV);
	long newV_index = _getVertexIndex(newV);

	// Проверяем есть ли эта вершина в графе
	if (oldV_index == -1)
		throw Graph_exception("вершины с таким именем нет в графе");

	// Проверяем нет ли уже вершины с новым задаваемым именем
	if (newV_index != -1 && newV_index != oldV_index)
		throw Graph_exception("вершина с таким именем уже есть в графе");

	*this->vectorVertex[oldV_index] = newV;
}

template <class Vertex_T, class Edge_T>
typename void Graph<Vertex_T, Edge_T>::setEdge(const Edge_T& oldE, const Edge_T& newE)
{
	// Сначала получаем индексы заданных вершин
	size_t outV_index = _getVertexIndex(oldE.getVertexOut());
	size_t inV_index = _getVertexIndex(oldE.getVertexIn());

	// Если какой-то из вершин нет, то выбрасываем исключение
	if (outV_index == -1)
		throw Graph_exception("вершины исхода с таким именем нет в графе");
	if (inV_index == -1)
		throw Graph_exception("вершины входа с таким именем нет в графе");

	// Устанавливаем новые данные в ребре, если оно есть
	this->form->getEdge(outV_index, inV_index) = newE;

	// Если граф неориентированный, то устанавливаем новые данные также и в противоположном ребре
	if (!this->directed)
		this->form->getEdge(inV_index, outV_index) = newE;
}

template <class Vertex_T, class Edge_T>
typename void Graph<Vertex_T, Edge_T>::toListGraph()
{
	if (this->type == GraphFormType::L_GRAPH)
		return;
	
	// Создаём L-форму данного графа
	ListGraph* l_graph = new ListGraph(this->vectorVertex, this->directed);
	// Устанавливаем итератор рёбер на начало
	auto iter = MatrixEdgeIterator::begin(dynamic_cast<MatrixGraph&>(*this->form));
	// Также определяем конец итерации рёбер M-графа
	auto end = MatrixEdgeIterator::end(dynamic_cast<MatrixGraph&>(*this->form));

	// Сначала изменяем форму L-графа, добавляя в неё вершины
	for (size_t i = 0, end = getVertexCount(); i < end; i++)
		l_graph->insertVertex(i);

	// Затем обходим все рёбра M-графа, получаем их, находим индексы вершин, которые 
	// данные рёбра соединяют и добавляем аналогичные рёбра в L-форму
	for (; iter != end; ++iter)
	{
		size_t outV = _getVertexIndex((*iter).getVertexOut());
		size_t inV = _getVertexIndex((*iter).getVertexIn());

		// Создаём новое ребро, и связываем его с вершинами outV и inV
		Edge_T* edge = new Edge_T(*this->vectorVertex[outV], *this->vectorVertex[inV]);

		// Пытаемся считать вес текущего ребра и перенести его в новое
		try { edge->setWeight((*iter).getWeight()); } 
		catch (Graph_exception&) {}

		// Пытаемся считать данные текущего ребра и перенести их в новое
		try { edge->setData((*iter).getData()); }
		catch (Graph_exception&) {}

		l_graph->insertEdge(outV, inV, edge);

		// Если граф неориентированный, то добавляем также противоположное ребро
		if (!this->directed)
		{
			// Создаём противоположное ребро, и связываем его с вершинами inV и outV
			Edge_T* reverse_edge = new Edge_T(*this->vectorVertex[inV], *this->vectorVertex[outV]);

			// Пытаемся считать вес текущего ребра и перенести его в противоположное ребро
			try { reverse_edge->setWeight((*iter).getWeight()); }
			catch (Graph_exception&) {}

			// Пытаемся считать данные текущего ребра и перенести их в противоположное ребро
			try { reverse_edge->setData((*iter).getData()); }
			catch (Graph_exception&) {}

			l_graph->insertEdge(inV, outV, reverse_edge);
		}
	}

	// Удаляем старую M-форму графа, и вместо неё в указатель на форму записываем
	// полученную L-форму
	delete this->form;
	this->form = l_graph;
	this->type = GraphFormType::L_GRAPH;
}

template <class Vertex_T, class Edge_T>
typename void Graph<Vertex_T, Edge_T>::toMatrixGraph()
{
	size_t size = getVertexCount();

	if (this->type == GraphFormType::M_GRAPH)
		return;

	// Создаём M-форму данного графа
	MatrixGraph* m_graph = new MatrixGraph(this->vectorVertex, this->directed);
	// Устанавливаем итератор рёбер на начало
	auto iter = ListEdgeIterator::begin(dynamic_cast<ListGraph&>(*this->form));
	// Также определяем конец итерации рёбер M-графа
	auto end = ListEdgeIterator::end(dynamic_cast<ListGraph&>(*this->form));

	// Сначала изменяем форму M-графа, добавляя в неё вершины
	for (size_t i = 0, end = size; i < end; i++)
		m_graph->insertVertex(i);

	// Обходим все рёбра L-графа, получаем их, находим индексы вершин, которые 
	// данные рёбра соединяют и добавляем аналогичные рёбра в M-форму
	for (; iter != end; ++iter)
	{
		size_t outV = _getVertexIndex((*iter).getVertexOut());
		size_t inV = _getVertexIndex((*iter).getVertexIn());

		// Создаём новое ребро, и связываем его с вершинами outV и inV
		Edge_T* edge = new Edge_T(*this->vectorVertex[outV], *this->vectorVertex[inV]);

		// Пытаемся считать вес текущего ребра и перенести его в новое
		try { edge->setWeight((*iter).getWeight()); }
		catch (Graph_exception&) {}

		// Пытаемся считать данные текущего ребра и перенести их в новое
		try { edge->setData((*iter).getData()); }
		catch (Graph_exception&) {}

		m_graph->insertEdge(outV, inV, edge);

		// Если граф неориентированный, то добавляем также противоположное ребро
		if (!this->directed)
		{
			// Создаём противоположное ребро, и связываем его с вершинами inV и outV
			Edge_T* reverse_edge = new Edge_T(*this->vectorVertex[inV], *this->vectorVertex[outV]);

			// Пытаемся считать вес текущего ребра и перенести его в противоположное ребро
			try { reverse_edge->setWeight((*iter).getWeight()); }
			catch (Graph_exception&) {}

			// Пытаемся считать данные текущего ребра и перенести их в противоположное ребро
			try { reverse_edge->setData((*iter).getData()); }
			catch (Graph_exception&) {}

			m_graph->insertEdge(inV, outV, reverse_edge);
		}
	}

	// Удаляем старую L-форму графа, и вместо неё в указатель на форму записываем
	// полученную M-форму
	delete this->form;
	this->form = m_graph;
	this->type = GraphFormType::M_GRAPH;
}

template <class Vertex_T, class Edge_T>
const typename Vertex_T& Graph<Vertex_T, Edge_T>::insertVertex(const Vertex_T& vertex)
{
	// Проверяем нет ли уже в графе вершины с тем же именем, что и в vertex
	if (_getVertexIndex(vertex) != -1)
		throw Graph_exception("вершина с таким именем уже есть в графе");

	// Если нет, то добавляем её в вектор вершин и в форму графа
	size_t index = getVertexCount();
	
	// Создаём вершину
	Vertex_T* new_vertex = new Vertex_T(vertex);

	// Добавляем указатель на неё в вектор
	this->vectorVertex.push_back(new_vertex);
	this->form->insertVertex(index);

	return *this->vectorVertex.back();
}

template <class Vertex_T, class Edge_T>
typename void Graph<Vertex_T, Edge_T>::deleteVertex(const Vertex_T& vertex)
{
	size_t index = _getVertexIndex(vertex);

	// Проверяем есть ли в графе вершина с заданным именем, что и в vertex
	if (index == -1)
		throw Graph_exception("вершины с таким именем нет в графе");

	// Если вершина в графе есть, то сначала удаляем исходящие из неё рёбра
	// и входящие в неё рёбра, и в целом её из формы графа
	this->form->deleteVertex(index);

	// Удаляем вершину
	delete this->vectorVertex[index];

	// Затем удаляем указатель на неё в векторе
	this->vectorVertex.erase(this->vectorVertex.begin() + index);
}

template <class Vertex_T, class Edge_T>
const typename Edge_T& Graph<Vertex_T, Edge_T>::insertEdge(const Vertex_T& outV, const Vertex_T& inV)
{
	// Сначала получаем индексы заданных вершин
	size_t outV_index = _getVertexIndex(outV);
	size_t inV_index = _getVertexIndex(inV);

	// Если какой-то из вершин нет, то выбрасываем исключение
	if (outV_index == -1)
		throw Graph_exception("вершины исхода с таким именем нет в графе");
	if (inV_index == -1)
		throw Graph_exception("вершины входа с таким именем нет в графе");

	// Если ребро является петлёй, то выбрасываем исключение
	if (outV_index == inV_index)
		throw Graph_exception("в простом графе не могут быть петли");

	// Создаём ребро с заданными вершинами исхода и входа
	Edge_T* edge = new Edge_T(*this->vectorVertex[outV_index], *this->vectorVertex[inV_index]);

	// Пытаемся добавить данное ребро
	try { this->form->insertEdge(outV_index, inV_index, edge); }
	// Если данное ребро уже было в графе, то удаляем созданное ребро и выбрасываем пойманное исключение
	catch (Graph_exception&) { delete edge; throw; }

	// Если граф не ориентированный, то добавляем ещё противоположное ребро
	if (!this->directed)
	{
		Edge_T* reverse_edge = new Edge_T(*this->vectorVertex[inV_index], *this->vectorVertex[outV_index]);

		this->form->insertEdge(inV_index, outV_index, reverse_edge);
	}

	return *edge;
}

template <class Vertex_T, class Edge_T>
typename void Graph<Vertex_T, Edge_T>::deleteEdge(const Vertex_T& outV, const Vertex_T& inV)
{
	// Сначала получаем индексы заданных вершин
	size_t outV_index = _getVertexIndex(outV);
	size_t inV_index = _getVertexIndex(inV);

	// Если какой-то из вершин нет, то выбрасываем исключение
	if (outV_index == -1)
		throw Graph_exception("вершины исхода с таким именем нет в графе");
	if (inV_index == -1)
		throw Graph_exception("вершины входа с таким именем нет в графе");

	// Пытаемся удалить ребро
	this->form->deleteEdge(outV_index, inV_index);

	// Если успешно удалили и данный граф неориентированный, то удаляем также противоположное ребро
	if (!this->directed)
		this->form->deleteEdge(inV_index, outV_index);
}
//*************************************************************************************************************************************************//


//************************************************* Реализация методов VertexIterator *************************************************************//
// public-методы:
template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::VertexIterator::VertexIterator(const Graph& graph, bool toBegin)
{
	this->graph = &graph;

	// Если изначально решили не устанавливать итератор
	if (!toBegin || this->graph->getVertexCount() == 0)
		this->cur = -1;
	else
		this->cur = 0;
}

template <class Vertex_T, class Edge_T>
typename Vertex_T& Graph<Vertex_T, Edge_T>::VertexIterator::operator*() const
{
	if (this->cur == -1)
		throw Graph_exception("итератор не установлен");

	return *this->graph->vectorVertex[this->cur];
}

template <class Vertex_T, class Edge_T>
typename Graph<Vertex_T, Edge_T>::VertexIterator& Graph<Vertex_T, Edge_T>::VertexIterator::operator++()
{
	if (this->cur == -1)
		throw Graph_exception("итератор не установлен");

	this->cur++;
	if (this->cur >= this->graph->getVertexCount())
		this->cur = -1;

	return *this;
}

template <class Vertex_T, class Edge_T>
typename Graph<Vertex_T, Edge_T>::VertexIterator Graph<Vertex_T, Edge_T>::VertexIterator::operator++(int)
{
	VertexIterator temp = *this;

	++(*this);

	return temp;
}

template <class Vertex_T, class Edge_T>
const typename Graph<Vertex_T, Edge_T>::VertexIterator& Graph<Vertex_T, Edge_T>::VertexIterator::operator=(const VertexIterator& iter)
{
	if (this == &iter)
		return *this;

	this->graph = iter.graph;
	this->cur = iter.cur;

	return *this;
}

template <class Vertex_T, class Edge_T>
typename bool Graph<Vertex_T, Edge_T>::VertexIterator::operator==(const VertexIterator& iter) const
{
	return this->graph == iter.graph && this->cur == iter.cur;
}

template <class Vertex_T, class Edge_T>
typename bool Graph<Vertex_T, Edge_T>::VertexIterator::operator!=(const VertexIterator& iter) const
{
	return !(this->graph == iter.graph && this->cur == iter.cur);
}
//*************************************************************************************************************************************************//


//**************************************************** Реализация методов EdgeIterator ************************************************************//
// public-методы:
template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::EdgeIterator::EdgeIterator(const Graph& graph, bool toBegin)
{
	this->graph = &graph;
	
	// В зависимости от формы графа создаём итератор рёбер
	if (this->graph->getFormType() == GraphFormType::L_GRAPH)
		this->form = new ListEdgeIterator(dynamic_cast<ListGraph&>(*graph.form), toBegin);
	else
		this->form = new MatrixEdgeIterator(dynamic_cast<MatrixGraph&>(*graph.form), toBegin);
}

template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::EdgeIterator::EdgeIterator(const EdgeIterator& iter)
{
	this->graph = iter.graph;

	// Если копируемый итератор не был создан
	if (iter.form == nullptr)
	{
		this->form = nullptr;

		return;
	}

	// В зависимости от формы графа создаём копию итератора
	if (this->graph->getFormType() == GraphFormType::L_GRAPH)
		this->form = new ListEdgeIterator(dynamic_cast<const ListEdgeIterator&>(*iter.form));
	else
		this->form = new MatrixEdgeIterator(dynamic_cast<const MatrixEdgeIterator&>(*iter.form));
}

template <class Vertex_T, class Edge_T>
typename Edge_T& Graph<Vertex_T, Edge_T>::EdgeIterator::operator*() const
{
	if (this->form == nullptr)
		throw Graph_exception("итератор не установлен");

	return *(*this->form);
}

template <class Vertex_T, class Edge_T>
typename Graph<Vertex_T, Edge_T>::EdgeIterator& Graph<Vertex_T, Edge_T>::EdgeIterator::operator++()
{
	if (this->form == nullptr)
		throw Graph_exception("итератор не установлен");

	++(*this->form);

	return *this;
}

template <class Vertex_T, class Edge_T>
typename Graph<Vertex_T, Edge_T>::EdgeIterator Graph<Vertex_T, Edge_T>::EdgeIterator::operator++(int)
{
	EdgeIterator temp = *this;

	++(*this);

	return temp;
}

template <class Vertex_T, class Edge_T>
typename bool Graph<Vertex_T, Edge_T>::EdgeIterator::operator==(const EdgeIterator& iter) const
{
	if (this->graph != iter.graph)
		return false;
	else if (this->form == nullptr && iter.form == nullptr)
		return true;
	else if (this->form != nullptr && iter.form == nullptr || this->form == nullptr && iter.form != nullptr)
		return false;
	else
		return *this->form == *iter.form;
}

template <class Vertex_T, class Edge_T>
typename bool Graph<Vertex_T, Edge_T>::EdgeIterator::operator!=(const EdgeIterator& iter) const
{
	if (this->graph != iter.graph)
		return true;
	else if (this->form == nullptr && iter.form == nullptr)
		return false;
	else if (this->form != nullptr && iter.form == nullptr || this->form == nullptr && iter.form != nullptr)
		return true;
	else
		return *this->form != *iter.form;
}

template <class Vertex_T, class Edge_T>
const typename Graph<Vertex_T, Edge_T>::EdgeIterator& Graph<Vertex_T, Edge_T>::EdgeIterator::operator=(const EdgeIterator& iter)
{
	if (this == &iter)
		return *this;

	// Если копируемый итератор не был создан
	if (iter.form == nullptr)
	{
		this->graph = iter.graph;

		if (this->form != nullptr)
			delete this->form;

		this->form = nullptr;

		return *this;
	}

	// Получаем тип формы копируемого графа
	GraphFormType type = iter.graph->getFormType();
	
	// Если итератор ещё не был создан
	if (this->graph == nullptr || this->form == nullptr)
	{
		this->graph = iter.graph;

		// В зависимости от формы графа создаём копию итератора
		if (type == GraphFormType::L_GRAPH)
			this->form = new ListEdgeIterator(dynamic_cast<const ListEdgeIterator&>(*iter.form));
		else
			this->form = new MatrixEdgeIterator(dynamic_cast<const MatrixEdgeIterator&>(*iter.form));

		return *this;
	}

	// Если этот тип совпадает с типом данного графа, то просто вызываем оператор присваивания для данных итераторов
	if (this->graph->getFormType() == type)
		*this->form = *iter.form;
	// Если же типы не совпадают
	else
	{
		// Сначала удаляем данный итератор
		delete this->form;

		// В зависимости от формы графа создаём копию итератора
		if (type == GraphFormType::L_GRAPH)
			this->form = new ListEdgeIterator(dynamic_cast<const ListEdgeIterator&>(*iter.form));
		else
			this->form = new MatrixEdgeIterator(dynamic_cast<const MatrixEdgeIterator&>(*iter.form));
	}

	this->graph = iter.graph;

	return *this;
}
//*************************************************************************************************************************************************//


//************************************************** Реализация методов OutEdgeIterator ***********************************************************//
// public-методы:
template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::OutEdgeIterator::OutEdgeIterator(const Graph& graph, const Vertex_T& vertex, bool toBegin)
{
	this->graph = &graph;

	// Сначала ищем вершину в графе
	size_t index = this->graph->_getVertexIndex(vertex);

	// Если вершины нет, то выбрасываем исключение
	if (index == -1)
		throw Graph_exception("вершины с таким именем нет в графе");
	
	// Затем в зависимости от типа графа создаём итератор
	if (graph.getFormType() == GraphFormType::L_GRAPH)
		this->form = new ListOutEdgeIterator(dynamic_cast<const ListGraph&>(*graph.form), index, toBegin);
	else
		this->form = new MatrixOutEdgeIterator(dynamic_cast<const MatrixGraph&>(*graph.form), index, toBegin);
}

template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::OutEdgeIterator::OutEdgeIterator(const OutEdgeIterator& iter)
{
	this->graph = iter.graph;
	
	// Если копируемый итератор не был создан
	if (iter.form == nullptr)
	{
		this->form = nullptr;

		return;
	}

	// В зависимости от типа графа создаём копию итератора
	if (iter.graph->getFormType() == GraphFormType::L_GRAPH)
		this->form = new ListOutEdgeIterator(dynamic_cast<const ListOutEdgeIterator&>(*iter.form));
	else
		this->form = new MatrixOutEdgeIterator(dynamic_cast<const MatrixOutEdgeIterator&>(*iter.form));
}

template <class Vertex_T, class Edge_T>
typename Edge_T& Graph<Vertex_T, Edge_T>::OutEdgeIterator::operator*() const
{
	if (this->form == nullptr)
		throw Graph_exception("итератор не установлен");

	return *(*this->form);
}

template <class Vertex_T, class Edge_T>
typename Graph<Vertex_T, Edge_T>::OutEdgeIterator& Graph<Vertex_T, Edge_T>::OutEdgeIterator::operator++()
{
	if (this->form == nullptr)
		throw Graph_exception("итератор не установлен");

	++(*this->form);

	return *this;
}

template <class Vertex_T, class Edge_T>
typename Graph<Vertex_T, Edge_T>::OutEdgeIterator Graph<Vertex_T, Edge_T>::OutEdgeIterator::operator++(int)
{
	OutEdgeIterator temp = *this;

	++(*this);

	return temp;
}

template <class Vertex_T, class Edge_T>
typename bool Graph<Vertex_T, Edge_T>::OutEdgeIterator::operator==(const OutEdgeIterator& iter)
{
	if (this->graph != iter.graph)
		return false;
	else if (this->form == nullptr && iter.form == nullptr)
		return true;
	else if (this->form != nullptr && iter.form == nullptr || this->form == nullptr && iter.form != nullptr)
		return false;
	else
		return *this->form == *iter.form;
}

template <class Vertex_T, class Edge_T>
typename bool Graph<Vertex_T, Edge_T>::OutEdgeIterator::operator!=(const OutEdgeIterator& iter)
{
	if (this->graph != iter.graph)
		return true;
	else if (this->form == nullptr && iter.form == nullptr)
		return false;
	else if (this->form != nullptr && iter.form == nullptr || this->form == nullptr && iter.form != nullptr)
		return true;
	else
		return *this->form != *iter.form;
}

template <class Vertex_T, class Edge_T>
const typename Graph<Vertex_T, Edge_T>::OutEdgeIterator& Graph<Vertex_T, Edge_T>::OutEdgeIterator::operator=(const OutEdgeIterator& iter)
{
	if (this == &iter)
		return *this;

	// Если копируемый итератор не был создан
	if (iter.form == nullptr)
	{
		this->graph = iter.graph;

		if (this->form != nullptr)
			delete this->form;

		this->form = nullptr;

		return *this;
	}

	// Получаем тип формы копируемого графа
	GraphFormType type = iter.graph->getFormType();

	// Если итератор ещё не был создан
	if (this->graph == nullptr || this->form == nullptr)
	{
		this->graph = iter.graph;

		// В зависимости от формы графа создаём копию итератора
		if (type == GraphFormType::L_GRAPH)
			this->form = new ListOutEdgeIterator(dynamic_cast<const ListOutEdgeIterator&>(*iter.form));
		else
			this->form = new MatrixOutEdgeIterator(dynamic_cast<const MatrixOutEdgeIterator&>(*iter.form));

		return *this;
	}

	// Если тип данного графа совпадает с типом копируемого графа, то вызываем оператор присваивания для этих итераторов
	if (this->graph->getFormType() == type)
		*this->form = *iter.form;
	// Если же типы не совпадают
	else
	{
		// Сначала удаляем данный итератор
		delete this->form;

		// В зависимости от формы графа создаём копию итератора
		if (type == GraphFormType::L_GRAPH)
			this->form = new ListOutEdgeIterator(dynamic_cast<const ListOutEdgeIterator&>(*iter.form));
		else
			this->form = new MatrixOutEdgeIterator(dynamic_cast<const MatrixOutEdgeIterator&>(*iter.form));
	}

	this->graph = iter.graph;

	return *this;
}
//*************************************************************************************************************************************************//


#endif
