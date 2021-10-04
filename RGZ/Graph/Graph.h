// Graph.h
#pragma once
#ifndef _INC_GRAPH_
#define _INC_GRAPH_
// ������������ ������������ �����
#include <vector>
#include <limits>
#include "GraphForm.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "Graph_exception.h"


//*************************************************************** ����� ����� *********************************************************************//
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

	vector<Vertex_T*> vectorVertex;		// ������ ���������� �� ������� �����
	GraphForm* form;					// ��������� �� ����� �����
	bool directed;						// ������������ �� ����
	GraphFormType type;					// ��� ����� �����(L-���� ��� M-����)

	// �������� ����� ������� � �������
	size_t _getVertexIndex(const Vertex_T& vertex) const;

public:

	// ����������� ����� �� ���������(������ ����������������� L-����)
	Graph();
	// ����������� ����� � �����������: vertices - ������ ������, directed - ������������������
	// � form - ������ ������������� �����
	Graph(size_t vertices, bool directed, GraphFormType form);
	// ����������� ����� � �����������: vertices - ������ ������, edges - ������ ��������� ����
	// directed - ������������������ � form - ������ ������������� �����
	Graph(size_t vertices, size_t edges, bool directed, GraphFormType form);
	// ����������� ����������� ������
	Graph(const Graph& graph);
	// ���������� �����
	~Graph();

	// �������� ������������ ������
	const Graph& operator=(const Graph& graph);

	// �������� ����� ������ �����
	size_t getVertexCount() const { return this->vectorVertex.size(); }
	// �������� ����� ���� �����
	size_t getEdgesCount() const { return this->form->getEdgesCount(); }
	// �������� ����� �� �����������������
	bool isDirected() const { return this->directed; }
	// �������� ����� �����(L-���� ��� M-����)
	GraphFormType getFormType() const { return this->type; }
	// �������� ����������� ������������ �����
	double getK() const;

	// ���������, ���� �� � ����� �������, �������� ������������
	bool isHasVertex(const Vertex_T& vertex) const { return _getVertexIndex(vertex) != -1; }
	// ���������, ���� �� � ����� ����� � ���������� ������������� ������� � �������� ������
	bool isHasEdge(const Vertex_T& outV, const Vertex_T& inV) const;

	// �������� ���������� ������� �� �����, � �������� �����������
	const Vertex_T& getVertex(const Vertex_T& vertex) const;
	// �������� ���������� �����, ��������� ������������� �������� � ������� ������
	const Edge_T& getEdge(const Vertex_T& outV, const Vertex_T& inV) const;

	// ���������� ��������� ������� oldV, �������� � ����������� ������� newV
	void setVertex(const Vertex_T& oldV, const Vertex_T& newV);
	// ���������� ��������� ����� oldE, �������� � ����������� ����� newE
	void setEdge(const Edge_T& oldE, const Edge_T& newE);

	// ������������� ���� � L-�����
	void toListGraph();
	// ������������� ���� � M-�����
	void toMatrixGraph();

	// �������� ������� � ����, � �������� ������������. ������� - ������ �� ����������� �������
	const Vertex_T& insertVertex(const Vertex_T& vertex);
	// ������� ������� �� �����, �������� ������������
	void deleteVertex(const Vertex_T& vertex);
	// �������� ����� �����, ��������� ������������� �������� � ������� ������. ������� - ������ �� ����������� �����
	const Edge_T& insertEdge(const Vertex_T& outV, const Vertex_T& inV);
	// ������� ����� �����, ��������� ������������� �������� � ������� ������
	void deleteEdge(const Vertex_T& outV, const Vertex_T& inV);

	// ������� ��������� ����� � �������
	void print() const { this->form->print(); }

	// ����� ��������� ������
	class VertexIterator
	{

		const Graph* graph;	// ��������� �� ������� ����
		size_t cur;			// ������ ������� �������

	public:

		// ����������� ��������� ������ �� ���������
		VertexIterator() : graph(nullptr), cur(-1) {}
		// ����������� ��������� ������ � ������� �� ����
		VertexIterator(const Graph& graph, bool toBegin = true);
		// ����������� ����������� ��������� ������
		VertexIterator(const VertexIterator& iter) : graph(iter.graph), cur(iter.cur) {}

		// �������� ���������� �������, �� ������� � ������ ������ ���������� ��������
		Vertex_T& operator*() const;

		// ���������� �������� ����������������� - ������� � ��������� ������� �����
		VertexIterator& operator++();
		// ����������� �������� �����������������
		VertexIterator operator++(int);

		// �������� ��������� ����������
		bool operator==(const VertexIterator& iter) const;
		// �������� ����������� ����������
		bool operator!=(const VertexIterator& iter) const;

		// �������� ������������ ����������
		const VertexIterator& operator=(const VertexIterator& iter);

		// ���������� ��������, ������������� �� ��������� �������
		static VertexIterator begin(const Graph& graph) { return VertexIterator(graph, true); }
		// ���������� ��������, ������������� �� �����
		static VertexIterator end(const Graph& graph) { return VertexIterator(graph, false); }

	};

	// ����� ��������� ����
	class EdgeIterator
	{

		const Graph* graph;		// ��������� �� ������� ����
		EdgeIteratorForm* form;	// ��������� �� ��� ��������

	public:

		// ����������� ��������� ���� �� ���������
		EdgeIterator() : graph(nullptr), form(nullptr) {}
		// ����������� ��������� ���� � �����������: graph - ������� �� ������� ���� � toBegin - ���������� �� ������
		EdgeIterator(const Graph& graph, bool toBegin = true);
		// ����������� ����������� ���������� ����
		EdgeIterator(const EdgeIterator& iter);
		// ���������� ��������� ����
		~EdgeIterator() { delete this->form; }

		// �������� ���������� �����, �� ������� � ������ ������ ���������� ��������
		Edge_T& operator*() const;

		// ���������� �������� ����������������� - ������� � ���������� ����� �����
		EdgeIterator& operator++();
		// ����������� �������� �����������������
		EdgeIterator operator++(int);

		// �������� ��������� ����������
		bool operator==(const EdgeIterator& iter) const;
		// �������� ����������� ����������
		bool operator!=(const EdgeIterator& iter) const;

		// �������� ������������ ����������
		const EdgeIterator& operator=(const EdgeIterator& iter);

		// ���������� ��������, ������������� �� ��������� �����
		static EdgeIterator begin(const Graph& graph) { return EdgeIterator(graph, true); }
		// ���������� ��������, ������������� �� �����
		static EdgeIterator end(const Graph& graph) { return EdgeIterator(graph, false); }

	};

	// ����� ��������� ��������� ����
	class OutEdgeIterator
	{

		const Graph* graph;			// ��������� �� ������� ����
		OutEdgeIteratorForm* form;	// ��������� �� ��� ��������

	public:

		// ����������� ��������� ��������� ���� �� ���������
		OutEdgeIterator() : graph(nullptr), form(nullptr) {}
		// ����������� ��������� ��������� ���� � �����������: graph - ������� �� ������� ����,
		// vertex - �������� � toBegin - ���������� �� ������
		OutEdgeIterator(const Graph& graph, const Vertex_T& vertex, bool toBegin = true);
		// ����������� ����������� ���������� ��������� ����
		OutEdgeIterator(const OutEdgeIterator& iter);
		// ���������� ��������� ��������� ����
		~OutEdgeIterator() { delete this->form; }

		// �������� ���������� �����, �� ������� � ������ ������ ���������� ��������
		Edge_T& operator*() const;

		// ���������� �������� ����������������� - ������� � ���������� ����� �����
		OutEdgeIterator& operator++();
		// ����������� �������� �����������������
		OutEdgeIterator operator++(int);

		// �������� ��������� ����������
		bool operator==(const OutEdgeIterator& iter);
		// �������� ����������� ����������
		bool operator!=(const OutEdgeIterator& iter);

		// �������� ������������ ����������
		const OutEdgeIterator& operator=(const OutEdgeIterator& iter);
		
		// ���������� ��������, ������������� �� ��������� �����
		static OutEdgeIterator begin(const Graph& graph, const Vertex_T& vertex) { return OutEdgeIterator(graph, vertex, true); }
		// ���������� ��������, ������������� �� �����
		static OutEdgeIterator end(const Graph& graph, const Vertex_T& vertex) { return OutEdgeIterator(graph, vertex, false); }

	};

};
//*************************************************************************************************************************************************//


//**************************************************** ���������� ������� Graph *******************************************************************//
// private-������:
template <class Vertex_T, class Edge_T>
typename size_t Graph<Vertex_T, Edge_T>::_getVertexIndex(const Vertex_T& vertex) const
{
	// �������� �� ������� ���������� �� ������� � ������ �������, ������ vertex
	for (size_t i = 0, end = getVertexCount(); i < end; i++)
		if (vertex == *this->vectorVertex[i])
			return i;

	return -1;
}


// public-������:
template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::Graph()
{
	// �� ��������� ������ ������ ����������������� L-����

	this->vectorVertex = vector<Vertex_T*>();
	this->directed = false;
	this->type = GraphFormType::L_GRAPH;
	this->form = new ListGraph(this->vectorVertex, this->directed);
}

template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::Graph(size_t vertices, bool directed, GraphFormType form)
{
	// ������ ���� c ����������� ������, ������ "vertices", ����� ������� �����:
	// "0", "1", ... , "vertices - 1". ������ �������� ���������� ���������

	this->vectorVertex = vector<Vertex_T*>();
	this->directed = directed;
	this->type = form;
	
	// ������ ����� ����� � ����������� �� ��������� ���������
	if (this->type == GraphFormType::L_GRAPH)
		this->form = new ListGraph(this->vectorVertex, this->directed);
	else
		this->form = new MatrixGraph(this->vectorVertex, this->directed);

	// ��������� �������� ����� ������
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

	// ��������� ������������ ��������� ���������� ����. � ������ ���� �� ������, ��� ��������
	// ��� ������� ����� � �������� ������ ������, �� ������������ ���� ��������
	if (this->directed && (edges > size * (size - 1)))
		edges = size * (size - 1);
	else if (!this->directed && (edges > static_cast<size_t>(size * (size - 1) / 2)))
		edges = static_cast<size_t>(size * (size - 1) / 2);

	size_t outV = 0, inV = 0;

	// � �������� ���� ������������� ������� �����
	srand(static_cast<unsigned int>(time(nullptr)));

	// ���������� edges ��������� ����
	for (size_t i = 0; i < edges; i++)
	{
		outV = rand() % size;	// �������� �������� ������� ������
		inV = rand() % size;	// �������� �������� ������� �����

		// ���������� ��������� ����� �� ��� ���, ���� ����������� ����� �� ����� � �����
		// � ��� �� ����� ������������ �����
		while (this->form->isHasEdge(outV, inV) || outV == inV)
		{
			outV = rand() % size;
			inV = rand() % size;
		}

		// ��� ������������� ����� ���� ������, �� ��������� �����
		const Edge_T& edge = insertEdge(*this->vectorVertex[outV], *this->vectorVertex[inV]);

		// ������������� ��������� ��� � ������
		setEdge(edge, Edge_T(edge.getVertexOut(), edge.getVertexIn(), rand() % 10 + 1, rand()));
	}	
}

template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::Graph(const Graph& graph)
{
	// �������� ������ ������, ������ ����� ������� - ����� ������ �� graph
	this->vectorVertex = vector<Vertex_T*>(graph.vectorVertex.size(), nullptr);
	for (size_t i = 0, size = graph.vectorVertex.size(); i < size; i++)
		this->vectorVertex[i] = new Vertex_T(*graph.vectorVertex[i]);

	this->directed = graph.directed;
	this->type = graph.type;

	// ������ ����� �����, ����������� ����� graph
	if (this->type == GraphFormType::L_GRAPH)
		this->form = new ListGraph(dynamic_cast<const ListGraph&>(*graph.form), this->vectorVertex);
	else
		this->form = new MatrixGraph(dynamic_cast<const MatrixGraph&>(*graph.form), this->vectorVertex);
}

template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::~Graph()
{
	// �������� �� �������, ������ ��� �������
	for (size_t i = 0, size = this->vectorVertex.size(); i < size; i++)
		delete this->vectorVertex[i];

	// ������� ������ � ������� ����� �����
	this->vectorVertex.clear();
	delete this->form;
}

template <class Vertex_T, class Edge_T>
const typename Graph<Vertex_T, Edge_T>& Graph<Vertex_T, Edge_T>::operator=(const Graph& graph)
{
	if (this == &graph)
		return *this;

	// �������� �� �������, ������ ��� �������
	for (size_t i = 0, size = this->vectorVertex.size(); i < size; i++)
		delete this->vectorVertex[i];

	// ������� ������ � ������� ����� �����
	this->vectorVertex.clear();

	// �������� ������ ������, ������ ����� ������� - ����� ������ �� graph
	this->vectorVertex = vector<Vertex_T*>(graph.vectorVertex.size(), nullptr);
	for (size_t i = 0, size = graph.vectorVertex.size(); i < size; i++)
		this->vectorVertex[i] = new Vertex_T(*graph.vectorVertex[i]);

	this->directed = graph.directed;

	// ���� ���� ������ ���������, �� �������� ��� ��� �������� ������������
	if (this->type == graph.type)
		*this->form = *graph.form;
	// ���� �� ���������
	else
	{
		// �� ������� ������� ����� ������� �����
		delete this->form;	
		this->type = graph.type;

		// � ����� ������ ����� ����� - ����� ����� graph
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
	size_t size = getVertexCount();			// ������� ����� ������
	size_t max_edges = size * (size - 1);	// ������������ ����� ���� ��� ���� ������(�������� �����)

	// � ������ ������������������ ����� ������������ ����� ���� � ��� ���� ������
	if (!this->directed)
		max_edges = max_edges / 2;

	// � ����������� ������ ���������� NaN
	if (max_edges == 0)
		return std::numeric_limits<double>::quiet_NaN();
	else
		return static_cast<double>(getEdgesCount()) / static_cast<double>(max_edges);
}

template <class Vertex_T, class Edge_T>
typename bool Graph<Vertex_T, Edge_T>::isHasEdge(const Vertex_T& outV, const Vertex_T& inV) const
{
	// ������� �������� ������� �������� ������
	size_t outV_index = _getVertexIndex(outV);
	size_t inV_index = _getVertexIndex(inV);

	// ���� �����-�� �� ������ ���, �� ����������� ����������
	if (outV_index == -1)
		throw Graph_exception("������� ������ � ����� ������ ��� � �����");
	if (inV_index == -1)
		throw Graph_exception("������� ����� � ����� ������ ��� � �����");

	return this->form->isHasEdge(outV_index, inV_index);
}

template <class Vertex_T, class Edge_T>
const typename Vertex_T& Graph<Vertex_T, Edge_T>::getVertex(const Vertex_T& vertex) const
{
	// ������� �������� ������ ������� � �����
	size_t index = _getVertexIndex(vertex);

	// ���� � ��� � ��, �� ����������� ����������
	if (index == -1)
		throw Graph_exception("������� � ����� ������ ��� � �����");

	return *this->vectorVertex[index];
}

template <class Vertex_T, class Edge_T>
const typename Edge_T& Graph<Vertex_T, Edge_T>::getEdge(const Vertex_T& outV, const Vertex_T& inV) const
{
	// ������� �������� ������� �������� ������
	size_t outV_index = _getVertexIndex(outV);
	size_t inV_index = _getVertexIndex(inV);

	// ���� �����-�� �� ������ ���, �� ����������� ����������
	if (outV_index == -1)
		throw Graph_exception("������� ������ � ����� ������ ��� � �����");
	if (inV_index == -1)
		throw Graph_exception("������� ����� � ����� ������ ��� � �����");

	return this->form->getEdge(outV_index, inV_index);
}

template <class Vertex_T, class Edge_T>
typename void Graph<Vertex_T, Edge_T>::setVertex(const Vertex_T& oldV, const Vertex_T& newV)
{
	// ������� �������� ������ ������� �����, ��� ���������� �������� ������
	long oldV_index = _getVertexIndex(oldV);
	long newV_index = _getVertexIndex(newV);

	// ��������� ���� �� ��� ������� � �����
	if (oldV_index == -1)
		throw Graph_exception("������� � ����� ������ ��� � �����");

	// ��������� ��� �� ��� ������� � ����� ���������� ������
	if (newV_index != -1 && newV_index != oldV_index)
		throw Graph_exception("������� � ����� ������ ��� ���� � �����");

	*this->vectorVertex[oldV_index] = newV;
}

template <class Vertex_T, class Edge_T>
typename void Graph<Vertex_T, Edge_T>::setEdge(const Edge_T& oldE, const Edge_T& newE)
{
	// ������� �������� ������� �������� ������
	size_t outV_index = _getVertexIndex(oldE.getVertexOut());
	size_t inV_index = _getVertexIndex(oldE.getVertexIn());

	// ���� �����-�� �� ������ ���, �� ����������� ����������
	if (outV_index == -1)
		throw Graph_exception("������� ������ � ����� ������ ��� � �����");
	if (inV_index == -1)
		throw Graph_exception("������� ����� � ����� ������ ��� � �����");

	// ������������� ����� ������ � �����, ���� ��� ����
	this->form->getEdge(outV_index, inV_index) = newE;

	// ���� ���� �����������������, �� ������������� ����� ������ ����� � � ��������������� �����
	if (!this->directed)
		this->form->getEdge(inV_index, outV_index) = newE;
}

template <class Vertex_T, class Edge_T>
typename void Graph<Vertex_T, Edge_T>::toListGraph()
{
	if (this->type == GraphFormType::L_GRAPH)
		return;
	
	// ������ L-����� ������� �����
	ListGraph* l_graph = new ListGraph(this->vectorVertex, this->directed);
	// ������������� �������� ���� �� ������
	auto iter = MatrixEdgeIterator::begin(dynamic_cast<MatrixGraph&>(*this->form));
	// ����� ���������� ����� �������� ���� M-�����
	auto end = MatrixEdgeIterator::end(dynamic_cast<MatrixGraph&>(*this->form));

	// ������� �������� ����� L-�����, �������� � �� �������
	for (size_t i = 0, end = getVertexCount(); i < end; i++)
		l_graph->insertVertex(i);

	// ����� ������� ��� ���� M-�����, �������� ��, ������� ������� ������, ������� 
	// ������ ���� ��������� � ��������� ����������� ���� � L-�����
	for (; iter != end; ++iter)
	{
		size_t outV = _getVertexIndex((*iter).getVertexOut());
		size_t inV = _getVertexIndex((*iter).getVertexIn());

		// ������ ����� �����, � ��������� ��� � ��������� outV � inV
		Edge_T* edge = new Edge_T(*this->vectorVertex[outV], *this->vectorVertex[inV]);

		// �������� ������� ��� �������� ����� � ��������� ��� � �����
		try { edge->setWeight((*iter).getWeight()); } 
		catch (Graph_exception&) {}

		// �������� ������� ������ �������� ����� � ��������� �� � �����
		try { edge->setData((*iter).getData()); }
		catch (Graph_exception&) {}

		l_graph->insertEdge(outV, inV, edge);

		// ���� ���� �����������������, �� ��������� ����� ��������������� �����
		if (!this->directed)
		{
			// ������ ��������������� �����, � ��������� ��� � ��������� inV � outV
			Edge_T* reverse_edge = new Edge_T(*this->vectorVertex[inV], *this->vectorVertex[outV]);

			// �������� ������� ��� �������� ����� � ��������� ��� � ��������������� �����
			try { reverse_edge->setWeight((*iter).getWeight()); }
			catch (Graph_exception&) {}

			// �������� ������� ������ �������� ����� � ��������� �� � ��������������� �����
			try { reverse_edge->setData((*iter).getData()); }
			catch (Graph_exception&) {}

			l_graph->insertEdge(inV, outV, reverse_edge);
		}
	}

	// ������� ������ M-����� �����, � ������ �� � ��������� �� ����� ����������
	// ���������� L-�����
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

	// ������ M-����� ������� �����
	MatrixGraph* m_graph = new MatrixGraph(this->vectorVertex, this->directed);
	// ������������� �������� ���� �� ������
	auto iter = ListEdgeIterator::begin(dynamic_cast<ListGraph&>(*this->form));
	// ����� ���������� ����� �������� ���� M-�����
	auto end = ListEdgeIterator::end(dynamic_cast<ListGraph&>(*this->form));

	// ������� �������� ����� M-�����, �������� � �� �������
	for (size_t i = 0, end = size; i < end; i++)
		m_graph->insertVertex(i);

	// ������� ��� ���� L-�����, �������� ��, ������� ������� ������, ������� 
	// ������ ���� ��������� � ��������� ����������� ���� � M-�����
	for (; iter != end; ++iter)
	{
		size_t outV = _getVertexIndex((*iter).getVertexOut());
		size_t inV = _getVertexIndex((*iter).getVertexIn());

		// ������ ����� �����, � ��������� ��� � ��������� outV � inV
		Edge_T* edge = new Edge_T(*this->vectorVertex[outV], *this->vectorVertex[inV]);

		// �������� ������� ��� �������� ����� � ��������� ��� � �����
		try { edge->setWeight((*iter).getWeight()); }
		catch (Graph_exception&) {}

		// �������� ������� ������ �������� ����� � ��������� �� � �����
		try { edge->setData((*iter).getData()); }
		catch (Graph_exception&) {}

		m_graph->insertEdge(outV, inV, edge);

		// ���� ���� �����������������, �� ��������� ����� ��������������� �����
		if (!this->directed)
		{
			// ������ ��������������� �����, � ��������� ��� � ��������� inV � outV
			Edge_T* reverse_edge = new Edge_T(*this->vectorVertex[inV], *this->vectorVertex[outV]);

			// �������� ������� ��� �������� ����� � ��������� ��� � ��������������� �����
			try { reverse_edge->setWeight((*iter).getWeight()); }
			catch (Graph_exception&) {}

			// �������� ������� ������ �������� ����� � ��������� �� � ��������������� �����
			try { reverse_edge->setData((*iter).getData()); }
			catch (Graph_exception&) {}

			m_graph->insertEdge(inV, outV, reverse_edge);
		}
	}

	// ������� ������ L-����� �����, � ������ �� � ��������� �� ����� ����������
	// ���������� M-�����
	delete this->form;
	this->form = m_graph;
	this->type = GraphFormType::M_GRAPH;
}

template <class Vertex_T, class Edge_T>
const typename Vertex_T& Graph<Vertex_T, Edge_T>::insertVertex(const Vertex_T& vertex)
{
	// ��������� ��� �� ��� � ����� ������� � ��� �� ������, ��� � � vertex
	if (_getVertexIndex(vertex) != -1)
		throw Graph_exception("������� � ����� ������ ��� ���� � �����");

	// ���� ���, �� ��������� � � ������ ������ � � ����� �����
	size_t index = getVertexCount();
	
	// ������ �������
	Vertex_T* new_vertex = new Vertex_T(vertex);

	// ��������� ��������� �� �� � ������
	this->vectorVertex.push_back(new_vertex);
	this->form->insertVertex(index);

	return *this->vectorVertex.back();
}

template <class Vertex_T, class Edge_T>
typename void Graph<Vertex_T, Edge_T>::deleteVertex(const Vertex_T& vertex)
{
	size_t index = _getVertexIndex(vertex);

	// ��������� ���� �� � ����� ������� � �������� ������, ��� � � vertex
	if (index == -1)
		throw Graph_exception("������� � ����� ������ ��� � �����");

	// ���� ������� � ����� ����, �� ������� ������� ��������� �� �� ����
	// � �������� � �� ����, � � ����� � �� ����� �����
	this->form->deleteVertex(index);

	// ������� �������
	delete this->vectorVertex[index];

	// ����� ������� ��������� �� �� � �������
	this->vectorVertex.erase(this->vectorVertex.begin() + index);
}

template <class Vertex_T, class Edge_T>
const typename Edge_T& Graph<Vertex_T, Edge_T>::insertEdge(const Vertex_T& outV, const Vertex_T& inV)
{
	// ������� �������� ������� �������� ������
	size_t outV_index = _getVertexIndex(outV);
	size_t inV_index = _getVertexIndex(inV);

	// ���� �����-�� �� ������ ���, �� ����������� ����������
	if (outV_index == -1)
		throw Graph_exception("������� ������ � ����� ������ ��� � �����");
	if (inV_index == -1)
		throw Graph_exception("������� ����� � ����� ������ ��� � �����");

	// ���� ����� �������� �����, �� ����������� ����������
	if (outV_index == inV_index)
		throw Graph_exception("� ������� ����� �� ����� ���� �����");

	// ������ ����� � ��������� ��������� ������ � �����
	Edge_T* edge = new Edge_T(*this->vectorVertex[outV_index], *this->vectorVertex[inV_index]);

	// �������� �������� ������ �����
	try { this->form->insertEdge(outV_index, inV_index, edge); }
	// ���� ������ ����� ��� ���� � �����, �� ������� ��������� ����� � ����������� ��������� ����������
	catch (Graph_exception&) { delete edge; throw; }

	// ���� ���� �� ���������������, �� ��������� ��� ��������������� �����
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
	// ������� �������� ������� �������� ������
	size_t outV_index = _getVertexIndex(outV);
	size_t inV_index = _getVertexIndex(inV);

	// ���� �����-�� �� ������ ���, �� ����������� ����������
	if (outV_index == -1)
		throw Graph_exception("������� ������ � ����� ������ ��� � �����");
	if (inV_index == -1)
		throw Graph_exception("������� ����� � ����� ������ ��� � �����");

	// �������� ������� �����
	this->form->deleteEdge(outV_index, inV_index);

	// ���� ������� ������� � ������ ���� �����������������, �� ������� ����� ��������������� �����
	if (!this->directed)
		this->form->deleteEdge(inV_index, outV_index);
}
//*************************************************************************************************************************************************//


//************************************************* ���������� ������� VertexIterator *************************************************************//
// public-������:
template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::VertexIterator::VertexIterator(const Graph& graph, bool toBegin)
{
	this->graph = &graph;

	// ���� ���������� ������ �� ������������� ��������
	if (!toBegin || this->graph->getVertexCount() == 0)
		this->cur = -1;
	else
		this->cur = 0;
}

template <class Vertex_T, class Edge_T>
typename Vertex_T& Graph<Vertex_T, Edge_T>::VertexIterator::operator*() const
{
	if (this->cur == -1)
		throw Graph_exception("�������� �� ����������");

	return *this->graph->vectorVertex[this->cur];
}

template <class Vertex_T, class Edge_T>
typename Graph<Vertex_T, Edge_T>::VertexIterator& Graph<Vertex_T, Edge_T>::VertexIterator::operator++()
{
	if (this->cur == -1)
		throw Graph_exception("�������� �� ����������");

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


//**************************************************** ���������� ������� EdgeIterator ************************************************************//
// public-������:
template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::EdgeIterator::EdgeIterator(const Graph& graph, bool toBegin)
{
	this->graph = &graph;
	
	// � ����������� �� ����� ����� ������ �������� ����
	if (this->graph->getFormType() == GraphFormType::L_GRAPH)
		this->form = new ListEdgeIterator(dynamic_cast<ListGraph&>(*graph.form), toBegin);
	else
		this->form = new MatrixEdgeIterator(dynamic_cast<MatrixGraph&>(*graph.form), toBegin);
}

template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::EdgeIterator::EdgeIterator(const EdgeIterator& iter)
{
	this->graph = iter.graph;

	// ���� ���������� �������� �� ��� ������
	if (iter.form == nullptr)
	{
		this->form = nullptr;

		return;
	}

	// � ����������� �� ����� ����� ������ ����� ���������
	if (this->graph->getFormType() == GraphFormType::L_GRAPH)
		this->form = new ListEdgeIterator(dynamic_cast<const ListEdgeIterator&>(*iter.form));
	else
		this->form = new MatrixEdgeIterator(dynamic_cast<const MatrixEdgeIterator&>(*iter.form));
}

template <class Vertex_T, class Edge_T>
typename Edge_T& Graph<Vertex_T, Edge_T>::EdgeIterator::operator*() const
{
	if (this->form == nullptr)
		throw Graph_exception("�������� �� ����������");

	return *(*this->form);
}

template <class Vertex_T, class Edge_T>
typename Graph<Vertex_T, Edge_T>::EdgeIterator& Graph<Vertex_T, Edge_T>::EdgeIterator::operator++()
{
	if (this->form == nullptr)
		throw Graph_exception("�������� �� ����������");

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

	// ���� ���������� �������� �� ��� ������
	if (iter.form == nullptr)
	{
		this->graph = iter.graph;

		if (this->form != nullptr)
			delete this->form;

		this->form = nullptr;

		return *this;
	}

	// �������� ��� ����� ����������� �����
	GraphFormType type = iter.graph->getFormType();
	
	// ���� �������� ��� �� ��� ������
	if (this->graph == nullptr || this->form == nullptr)
	{
		this->graph = iter.graph;

		// � ����������� �� ����� ����� ������ ����� ���������
		if (type == GraphFormType::L_GRAPH)
			this->form = new ListEdgeIterator(dynamic_cast<const ListEdgeIterator&>(*iter.form));
		else
			this->form = new MatrixEdgeIterator(dynamic_cast<const MatrixEdgeIterator&>(*iter.form));

		return *this;
	}

	// ���� ���� ��� ��������� � ����� ������� �����, �� ������ �������� �������� ������������ ��� ������ ����������
	if (this->graph->getFormType() == type)
		*this->form = *iter.form;
	// ���� �� ���� �� ���������
	else
	{
		// ������� ������� ������ ��������
		delete this->form;

		// � ����������� �� ����� ����� ������ ����� ���������
		if (type == GraphFormType::L_GRAPH)
			this->form = new ListEdgeIterator(dynamic_cast<const ListEdgeIterator&>(*iter.form));
		else
			this->form = new MatrixEdgeIterator(dynamic_cast<const MatrixEdgeIterator&>(*iter.form));
	}

	this->graph = iter.graph;

	return *this;
}
//*************************************************************************************************************************************************//


//************************************************** ���������� ������� OutEdgeIterator ***********************************************************//
// public-������:
template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::OutEdgeIterator::OutEdgeIterator(const Graph& graph, const Vertex_T& vertex, bool toBegin)
{
	this->graph = &graph;

	// ������� ���� ������� � �����
	size_t index = this->graph->_getVertexIndex(vertex);

	// ���� ������� ���, �� ����������� ����������
	if (index == -1)
		throw Graph_exception("������� � ����� ������ ��� � �����");
	
	// ����� � ����������� �� ���� ����� ������ ��������
	if (graph.getFormType() == GraphFormType::L_GRAPH)
		this->form = new ListOutEdgeIterator(dynamic_cast<const ListGraph&>(*graph.form), index, toBegin);
	else
		this->form = new MatrixOutEdgeIterator(dynamic_cast<const MatrixGraph&>(*graph.form), index, toBegin);
}

template <class Vertex_T, class Edge_T>
Graph<Vertex_T, Edge_T>::OutEdgeIterator::OutEdgeIterator(const OutEdgeIterator& iter)
{
	this->graph = iter.graph;
	
	// ���� ���������� �������� �� ��� ������
	if (iter.form == nullptr)
	{
		this->form = nullptr;

		return;
	}

	// � ����������� �� ���� ����� ������ ����� ���������
	if (iter.graph->getFormType() == GraphFormType::L_GRAPH)
		this->form = new ListOutEdgeIterator(dynamic_cast<const ListOutEdgeIterator&>(*iter.form));
	else
		this->form = new MatrixOutEdgeIterator(dynamic_cast<const MatrixOutEdgeIterator&>(*iter.form));
}

template <class Vertex_T, class Edge_T>
typename Edge_T& Graph<Vertex_T, Edge_T>::OutEdgeIterator::operator*() const
{
	if (this->form == nullptr)
		throw Graph_exception("�������� �� ����������");

	return *(*this->form);
}

template <class Vertex_T, class Edge_T>
typename Graph<Vertex_T, Edge_T>::OutEdgeIterator& Graph<Vertex_T, Edge_T>::OutEdgeIterator::operator++()
{
	if (this->form == nullptr)
		throw Graph_exception("�������� �� ����������");

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

	// ���� ���������� �������� �� ��� ������
	if (iter.form == nullptr)
	{
		this->graph = iter.graph;

		if (this->form != nullptr)
			delete this->form;

		this->form = nullptr;

		return *this;
	}

	// �������� ��� ����� ����������� �����
	GraphFormType type = iter.graph->getFormType();

	// ���� �������� ��� �� ��� ������
	if (this->graph == nullptr || this->form == nullptr)
	{
		this->graph = iter.graph;

		// � ����������� �� ����� ����� ������ ����� ���������
		if (type == GraphFormType::L_GRAPH)
			this->form = new ListOutEdgeIterator(dynamic_cast<const ListOutEdgeIterator&>(*iter.form));
		else
			this->form = new MatrixOutEdgeIterator(dynamic_cast<const MatrixOutEdgeIterator&>(*iter.form));

		return *this;
	}

	// ���� ��� ������� ����� ��������� � ����� ����������� �����, �� �������� �������� ������������ ��� ���� ����������
	if (this->graph->getFormType() == type)
		*this->form = *iter.form;
	// ���� �� ���� �� ���������
	else
	{
		// ������� ������� ������ ��������
		delete this->form;

		// � ����������� �� ����� ����� ������ ����� ���������
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
