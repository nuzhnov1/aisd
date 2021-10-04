// ListGraph.h
#pragma once
#ifndef _INC_LIST_GRAPH_
#define _INC_LIST_GRAPH_
// ������������ ������������ �����
#include <iostream>
#include <vector>
#include <list>
#include "GraphForm.h"
#include "Graph_exception.h"
#include "Vertex.h"
#include "Edge.h"


//********************************************** ����� ��������� ����� ������������� ����� ********************************************************//
template <class Vertex_T, class Edge_T>
class ListGraph : public GraphForm<Vertex_T, Edge_T>
{
	
	struct Node;

	template <typename T> using vector = std::vector<T>;
	template <typename T> using list = std::list<T>;
	using list_iterator = typename list<Node>::const_iterator;
	using GraphForm = GraphForm<Vertex_T, Edge_T>;
	using EdgeIteratorForm = typename GraphForm::EdgeIteratorForm;
	using OutEdgeIteratorForm = typename GraphForm::OutEdgeIteratorForm;

	// ���� ������
	struct Node
	{

		Edge_T* edge;	// ��������� �� �����
		size_t v2;		// ����� ������ �������

		Node(Edge_T* edge, size_t v2) : edge(edge), v2(v2) {}

	};

	vector<list<Node>> matrix;	// ������ ������� ���������

	// ���� �� ����� � ��������� outV � inV � �����. � "iter" ������������ �������� �� ��� �����
	bool _isHasEdge(size_t outV, size_t inV, list_iterator* iter = nullptr) const;

public:

	// ����������� ��������� ����� ����� � �����������: ������� �� ������ ������ vectorVertex � ������������������ ����� directed
	ListGraph(const vector<Vertex_T*>& vectorVertex, bool directed) : GraphForm(vectorVertex, directed), matrix() {}
	// ����������� ����������� ��������� ����� �����
	ListGraph(const ListGraph& graph, const vector<Vertex_T*>& vectorVertex);
	// ���������� ��������� ����� �����
	~ListGraph();

	// �������� ������������ ������
	virtual const ListGraph& operator=(const GraphForm& graph) final;

	// ���� �� ����� � �����
	virtual bool isHasEdge(size_t outV, size_t inV) const final;
	// �������� �����, ����������� ������� outV � inV
	virtual Edge_T& getEdge(size_t outV, size_t inV) const final;

	// �������� ������� � ����
	virtual void insertVertex(size_t index) final;
	// ������� ������� �����
	virtual void deleteVertex(size_t index) final;
	// �������� ����� � ����, ������������ ������� outV � inV, � ��������� ����������� � edge
	virtual void insertEdge(size_t outV, size_t inV, Edge_T* edge) final;
	// ������� ����� �� �����, ������������ ������� outV � inV
	virtual void deleteEdge(size_t outV, size_t inV) final;

	// ������� ����
	virtual void print() const final;

	// ����� ��������� ���� ����� ��������� �����
	class ListEdgeIterator : public EdgeIteratorForm
	{

		const ListGraph* graph_p;	// ��������� �� ������� L-����
		size_t curV;				// ����� ������� �������
		list_iterator curE;			// �������� �������� �����

	public:

		// ����������� ��������� � �����������: ������� �� �������� ���� graph � ���������� ��������� toBegin
		ListEdgeIterator(const ListGraph& graph, bool toBegin = true);
		// ����������� ����������� ����������
		ListEdgeIterator(const ListEdgeIterator& iter) : graph_p(iter.graph_p), curV(iter.curV), curE(iter.curE) {}

		// �������� ���������� ����� �����, �� ������� � ������ ������ ��������� ��������
		virtual Edge_T& operator*() const final;

		// ���������� �������� ����������������� - ������� � ���������� ����� �����
		virtual ListEdgeIterator& operator++() final;

		// �������� ��������� ����������
		virtual bool operator==(const EdgeIteratorForm& iter) const final;
		// �������� ����������� ����������
		virtual bool operator!=(const EdgeIteratorForm& iter) const final;

		// �������� ������������ ����������
		virtual const ListEdgeIterator& operator=(const EdgeIteratorForm& iter) final;

		// ���������� ��������, ������������� �� ������
		static ListEdgeIterator begin(const ListGraph& graph) { return ListEdgeIterator(graph, true); }
		// ���������� ��������, ������������� �� �����
		static ListEdgeIterator end(const ListGraph& graph) { return ListEdgeIterator(graph, false); }
	
	};

	// ����� ��������� ��������� ���� ����� ��������� �����
	class ListOutEdgeIterator : public OutEdgeIteratorForm
	{

		const ListGraph* graph_p;	// ��������� �� ������� L-����
		size_t curV;				// ����� ������� �������
		list_iterator curE;			// �������� �������� �����

	public:

		// ����������� ��������� � �����������: ������� �� ��������� ���� graph, �������� ������� index � ���������� ��������� toBegin
		ListOutEdgeIterator(const ListGraph& graph, size_t index, bool toBegin = true);
		// ����������� ����������� ����������
		ListOutEdgeIterator(const ListOutEdgeIterator& iter) : graph_p(iter.graph_p), curV(iter.curV), curE(iter.curE) {}

		// �������� ���������� ����� �����, �� ������� � ������ ������ ��������� ��������
		virtual Edge_T& operator*() const final;

		// ���������� �������� ����������������� - ������� � ���������� ���������� ����� ������� �����
		virtual ListOutEdgeIterator& operator++() final;

		// �������� ��������� ����������
		virtual bool operator==(const OutEdgeIteratorForm& iter) const final;
		// �������� ����������� ����������
		virtual bool operator!=(const OutEdgeIteratorForm& iter) const final;

		// �������� ������������ ����������
		virtual const ListOutEdgeIterator& operator=(const OutEdgeIteratorForm& iter) final;

		// ���������� ��������, ������������� �� ������
		static ListOutEdgeIterator begin(const ListGraph& graph, size_t index) { return ListOutEdgeIterator(graph, index, true); }

		// ���������� ��������, ������������� �� �����
		static ListOutEdgeIterator end(const ListGraph& graph, size_t index) { return ListOutEdgeIterator(graph, index, false); }

	};

};
//*************************************************************************************************************************************************//


//***************************************************** ���������� ������� ListGraph **************************************************************//
// private-������:
template <class Vertex_T, class Edge_T>
typename bool ListGraph<Vertex_T, Edge_T>::_isHasEdge(size_t outV, size_t inV, list_iterator* iter) const
{
	// �������� �� ������ ��� ������� "outV" � ������ ���� � ������� �������, ������ inV
	// � "iter" ���������� �������� �� ��������� �������
	list_iterator temp_iter = this->matrix[outV].begin();
	for (auto end = this->matrix[outV].end(); temp_iter != end; temp_iter++)
		if ((*temp_iter).v2 == inV)
		{
			if (iter != nullptr)
				*iter = temp_iter;

			return true;
		}

	if (iter != nullptr)
		*iter = temp_iter;

	return false;
}


// public-������:
template <class Vertex_T, class Edge_T>
ListGraph<Vertex_T, Edge_T>::ListGraph(const ListGraph& graph, const vector<Vertex_T*>& vectorVertex): 
	GraphForm(vectorVertex, graph.directed)
{
	this->directed = graph.directed;
	this->edgesCount = graph.edgesCount;
	this->matrix = vector<list<Node>>();

	// �������� ������� ������� ���������, �������� � ������ ������ ����,
	// � ����� ���� ����, ����������� �� �� ������ ������, ��� � ���������� �����
	for (size_t i = 0, size = this->_getVertexCount(); i < size; i++)
	{
		this->matrix.push_back(list<Node>());	// ��������� ������ ������

		// �������� �� ����� � ������� ������ ���������� ������� �������
		for (const auto& node : graph.matrix[i])
		{
			size_t j = node.v2;

			// ������ �����
			Edge_T* tmp_edge = new Edge_T(*this->vectorVertex[i], *this->vectorVertex[j]);

			// �������� ���������� ��� � �����
			try { tmp_edge->setWeight(node.edge->getWeight()); }
			catch (Graph_exception&) {}

			// �������� ���������� ������ � �����
			try { tmp_edge->setData(node.edge->getData()); }
			catch (Graph_exception&) {}

			// ��������� ����� � ������
			this->matrix[i].push_back(Node(tmp_edge, j));
		}
	}
}

template <class Vertex_T, class Edge_T>
ListGraph<Vertex_T, Edge_T>::~ListGraph()
{
	// �������� �� ������� ������� ����, ������ ������ �� ���
	for (auto& listNode : this->matrix)
	{
		// �������� �� ������, ������ ������ �����
		for (const auto& node : listNode)
			if (node.edge != nullptr)
				delete node.edge;

		listNode.clear();
	}

	this->matrix.clear();
}

template <class Vertex_T, class Edge_T>
const typename ListGraph<Vertex_T, Edge_T>& ListGraph<Vertex_T, Edge_T>::operator=(const GraphForm& graph)
{
	// �������� ����������� ����� ����� � ��������� �����
	const ListGraph& l_graph = dynamic_cast<const ListGraph&>(graph);

	if (this == &l_graph)
		return *this;

	// �������� �� ������� ������� ����, ������ ������ �� ���
	for (auto& listNode : this->matrix)
	{
		// �������� �� ������, ������ ������ �����
		for (const auto& node : listNode)
			if (node.edge != nullptr)
				delete node.edge;

		listNode.clear();
	}
	this->matrix.clear();

	this->directed = l_graph.directed;
	this->edgesCount = l_graph.edgesCount;
	this->matrix = vector<list<Node>>();

	// �������� ������� ������� ���������, �������� � ������ ������ ����,
	// � ����� ���� ����, ����������� �� �� ������ ������, ��� � ���������� �����
	for (size_t i = 0, size = this->_getVertexCount(); i < size; i++)
	{
		this->matrix.push_back(list<Node>());	// ��������� ������ ������

		// �������� �� ����� � ������� ������ ���������� ������� �������
		for (const auto& node : l_graph.matrix[i])
		{
			size_t j = node.v2;

			// ������ �����
			Edge_T* tmp_edge = new Edge_T(*this->vectorVertex[i], *this->vectorVertex[j]);

			// �������� ���������� ��� � �����
			try { tmp_edge->setWeight(node.edge->getWeight()); }
			catch (Graph_exception&) {}

			// �������� ���������� ������ � �����
			try { tmp_edge->setData(node.edge->getData()); }
			catch (Graph_exception&) {}

			// ��������� ����� � ������
			this->matrix[i].push_back(Node(tmp_edge, j));
		}
	}

	return *this;
}

template <class Vertex_T, class Edge_T>
typename bool ListGraph<Vertex_T, Edge_T>::isHasEdge(size_t outV, size_t inV) const
{
	// �������� �� ������ ��� ������� outV, �������� ������� ������� ������ �� ���������� � inV
	for (const auto& edge : this->matrix[outV])
		if (edge.v2 == inV)
			return true;

	return false;
}

template <class Vertex_T, class Edge_T>
typename Edge_T& ListGraph<Vertex_T, Edge_T>::getEdge(size_t outV, size_t inV) const
{
	typename list<Node>::iterator cur{};

	// ���������, ���� �� �������� ����� � �����
	if (!_isHasEdge(outV, inV, &cur))
		throw Graph_exception("������ ����� ��� � �����");

	return (*(*cur).edge);
}

template <class Vertex_T, class Edge_T>
typename void ListGraph<Vertex_T, Edge_T>::insertVertex(size_t index)
{
	// � ������ ������� ��������� ��������� ������ ������ � ������� index
	this->matrix.insert(this->matrix.begin() + index, list<Node>());

	// ��������� ����������� ������ � �����, ���������� ������� ������ �����,
	// ������� ��� ������ ������������ �������
	for (auto& listEdge : this->matrix)
		for (auto& edge : listEdge)
			if (edge.v2 >= index)
				edge.v2++;
}

template <class Vertex_T, class Edge_T>
typename void ListGraph<Vertex_T, Edge_T>::deleteVertex(size_t index)
{
	// ������� ������� ��� ����, ��������� �� ��
	for (const auto& node : this->matrix[index])
		delete node.edge;

	this->edgesCount -= this->matrix[index].size();
	this->matrix[index].clear();
	this->matrix.erase(this->matrix.begin() + index);

	// � ����� ������� ��� �������� ����

	size_t j = 0; // ����� �������� �������� ����

	// ������-�������� ��� �������� � �������� ����, � ������� ����� �������� ������� ��������� � index.
	// ����� ������-���������� ������ ������� ���������� �������� ���� ����� ���������� j
	auto predicate = [index, &j](Node node) -> bool 
	{ 
		if (node.v2 == index) { j++; delete node.edge; return true; }
		else return false;
	};

	// ������� ��� ����, �������� � ������� ��� ������� index
	for (auto& listEdge : this->matrix)
		listEdge.remove_if(predicate);

	this->edgesCount -= j;

	// ��������� ����������� ������ � �����, �������� ������� ������ �����, ������� ��� ������
	for (auto& listEdge : this->matrix)
		for (auto& edge : listEdge)
			if (edge.v2 > index)
				--edge.v2;
}

template <class Vertex_T, class Edge_T>
typename void ListGraph<Vertex_T, Edge_T>::insertEdge(size_t outV, size_t inV, Edge_T* edge)
{
	// ���������, ���� �� �������� ����� � �����
	if (_isHasEdge(outV, inV))
		throw Graph_exception("����� ����� ��� ���� � �����");

	// ���� ����� �� ����������� �����, �� ��������� ���
	this->matrix[outV].push_front(Node(edge, inV));

	this->edgesCount++;
}

template <class Vertex_T, class Edge_T>
typename void ListGraph<Vertex_T, Edge_T>::deleteEdge(size_t outV, size_t inV)
{
	typename list<Node>::iterator cur{};

	// ���������, ���� �� �������� ����� � �����
	if (!_isHasEdge(outV, inV, &cur))
		throw Graph_exception("������ ����� ��� � �����");

	// ���� ����� ����������� �����, �� ������� ���
	Edge_T* edge = (*cur).edge;
	this->matrix[outV].erase(cur);
	delete edge;

	this->edgesCount--;
}

template <class Vertex_T, class Edge_T>
typename void ListGraph<Vertex_T, Edge_T>::print() const
{
	using std::cout;
	using std::endl;

	setlocale(LC_ALL, "Russian");

	// ���� ��� ������, �� ��������� �� ��������� ������ � �������
	if (this->matrix.size() == 0)
	{
		cout << endl;

		return;
	}

	// �������� �� ������� ������� ����
	for (size_t i = 0, end = this->matrix.size(); i < end; i++)
	{
		cout << "[" << this->vectorVertex[i]->getName() << "]: -> ";
		// �������� �� ������� ������ ����
		for (const auto& node : this->matrix[i])
		{
			cout << "{" << node.edge->getVertexIn().getName() << ", ";

			try { cout << node.edge->getWeight(); }		// �������� ��������� ��� �����
			catch (Graph_exception&) { cout << "?"; }	// ���� �� �� ����������, ������� ���� - ?
			cout << "} -> ";
		}
		cout << "null" << endl;
	}

	// ������� ���������� �� ������������
	cout << "� ���������� ������� ���������� ��� ������� ������. ����� ��� ������ ����," << endl;
	cout << "��������� �� ���� �������. � �������� ������� ������� ���������� ������� ����� �����," << endl;
	cout << "� ����� ��� ����� �����. ������ \"?\" ���������� ����, ��� ������� �� ����������." << endl;

	cout << endl;
}
//*************************************************************************************************************************************************//


//************************************************* ���������� ������� ListEdgeIterator ***********************************************************//
// public-������:
template <class Vertex_T, class Edge_T>
ListGraph<Vertex_T, Edge_T>::ListEdgeIterator::ListEdgeIterator(const ListGraph& graph, bool toBegin)
{
	this->graph_p = &graph;

	// ���� ������ ���������� �� ������������� ��������
	if (!toBegin)
	{
		this->curV = -1;

		return;
	}

	const vector<list<Node>>& matrix = this->graph_p->matrix;
	size_t size = this->graph_p->matrix.size();

	// ���� ������ �������� ������ ����, � ������������� �������� �� ������ ����� � ��
	for (this->curV = 0; this->curV < size; this->curV++)
	{
		auto end = matrix[this->curV].end(); // ����� ������

		for (this->curE = matrix[this->curV].begin(), end = matrix[this->curV].end(); this->curE != end; this->curE++)
			// ���� ������ ���� ������������, �� ������������� �������� �� ������ �����
			// ���� �� ������������, �� ��� ���� ������� ����� ������ ������� ������,
			// �� ����� ������������� �������� �� ����
			if (this->graph_p->directed || (!this->graph_p->directed && (*this->curE).v2 >= this->curV))
				return;
	}

	// ���� �������� ��� � �� ��� ����������
	this->curV = -1;
}

template <class Vertex_T, class Edge_T>
typename Edge_T& ListGraph<Vertex_T, Edge_T>::ListEdgeIterator::operator*() const
{
	if (this->curV == -1)
		throw Graph_exception("�������� �� ����������");

	return (*(*this->curE).edge);
}

template <class Vertex_T, class Edge_T>
typename ListGraph<Vertex_T, Edge_T>::ListEdgeIterator& ListGraph<Vertex_T, Edge_T>::ListEdgeIterator::operator++()
{
	const vector<list<Node>>& matrix = this->graph_p->matrix;
	size_t size = this->graph_p->matrix.size();

	if (this->curV == -1)
		throw Graph_exception("�������� �� ����������");

	// �������������� ��������
	this->curE++; 

	// ����� ������������ ��������� ��������
	while (this->curV < size)
	{
		auto end = matrix[this->curV].end(); // ����� ������

		// ��������� �� ����� �� �������� �� ������� ������
		for (; this->curE != end; this->curE++)
			// ���� �� �����, � ��� ���� ���� ������������, �� ���������� ��������
			// ����� ���������� ��������, ���� ���� �� ������������ � ����� ������� ������� ����� ������
			// ��� ����� ������ ������ �������
			if (this->graph_p->directed || (!this->graph_p->directed && (*this->curE).v2 >= this->curV))
				return *this;

		this->curV++;

		if (this->curV < size)
			this->curE = matrix[this->curV].begin();
	}

	this->curV = -1;

	return *this;
}

template <class Vertex_T, class Edge_T>
typename bool ListGraph<Vertex_T, Edge_T>::ListEdgeIterator::operator==(const EdgeIteratorForm& iter) const
{
	const ListEdgeIterator& l_iter = dynamic_cast<const ListEdgeIterator&>(iter);

	// ���� ��������� ��������� �� ������ �����
	if (this->graph_p != l_iter.graph_p)
		return false;
	// ���� ��� ��� ������������� �� ����� � ��� �� �����
	else if (this->curV == -1 && l_iter.curV == -1)
		return true;
	// ���� ��� ����������� �� ����� � ��� �� ����� �����
	else if (this->curV == l_iter.curV && this->curE == l_iter.curE)
		return true;
	// ���� ������
	else
		return false;
}

template <class Vertex_T, class Edge_T>
typename bool ListGraph<Vertex_T, Edge_T>::ListEdgeIterator::operator!=(const EdgeIteratorForm& iter) const
{
	const ListEdgeIterator& l_iter = dynamic_cast<const ListEdgeIterator&>(iter);

	// ���� ��������� ��������� �� ������ �����
	if (this->graph_p != l_iter.graph_p)
		return true;
	// ���� ��� ��� ������������� �� ����� � ��� �� �����
	else if (this->curV == -1 && l_iter.curV == -1)
		return false;
	// ���� ��� ����������� �� ����� � ��� �� ����� �����
	else if (this->curV == l_iter.curV && this->curE == l_iter.curE)
		return false;
	// ���� ������
	else
		return true;
}

template <class Vertex_T, class Edge_T>
const typename ListGraph<Vertex_T, Edge_T>::ListEdgeIterator& ListGraph<Vertex_T, Edge_T>::ListEdgeIterator::operator=(const EdgeIteratorForm& iter)
{
	const ListEdgeIterator& l_iter = dynamic_cast<const ListEdgeIterator&>(iter);

	if (this == &l_iter)
		return *this;

	this->graph_p = l_iter.graph_p;
	this->curV = l_iter.curV;
	this->curE = l_iter.curE;

	return *this;
}
//*************************************************************************************************************************************************//


//*************************************************** ���������� ������� ListOutEdgeIterator ******************************************************//
// public-������:
template <class Vertex_T, class Edge_T>
ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator::ListOutEdgeIterator(const ListGraph& graph, size_t index, bool toBegin)
{
	this->graph_p = &graph;
	this->curV = index;

	// ���� ������ ���������� �� ������������� ��������
	if (!toBegin)
	{
		this->curV = -1;

		return;
	}

	const vector<list<Node>>& matrix = this->graph_p->matrix;
	size_t size = this->graph_p->matrix.size();

	// ������������� �� ������ ����� � ������ ��������� ���� ��� ������� � ������� index 
	this->curE = matrix[this->curV].begin();

	// ���� � ������ ������� ��� ��������� ����
	if (this->curE == matrix[this->curV].end())
		this->curV = -1;
}

template <class Vertex_T, class Edge_T>
typename Edge_T& ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator::operator*() const
{
	if (this->curV == -1)
		throw Graph_exception("�������� �� ����������");

	return (*(*this->curE).edge);
}

template <class Vertex_T, class Edge_T>
typename ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator& ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator::operator++()
{
	const vector<list<Node>>& matrix = this->graph_p->matrix;
	size_t size = this->graph_p->matrix.size();

	if (this->curV == -1)
		throw Graph_exception("�������� �� ����������");

	this->curE++;

	// ��������� ������� �� �������� � ���������������� ���������
	if (this->curE == matrix[this->curV].end())
		this->curV = -1;

	return *this;
}

template <class Vertex_T, class Edge_T>
typename bool ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator::operator==(const OutEdgeIteratorForm& iter) const
{
	const ListOutEdgeIterator& l_iter = dynamic_cast<const ListOutEdgeIterator&>(iter);

	// ���� ��������� ��������� �� ������ �����
	if (this->graph_p != l_iter.graph_p)
		return false;
	// ���� ��� ��� ������������� �� ����� � ��� �� �����
	else if (this->curV == -1 && l_iter.curV == -1)
		return true;
	// ���� ��� ����������� �� ����� � ��� �� ����� �����
	else if (this->curV == l_iter.curV && this->curE == l_iter.curE)
		return true;
	// ���� ������
	else
		return false;
}

template <class Vertex_T, class Edge_T>
typename bool ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator::operator!=(const OutEdgeIteratorForm& iter) const
{
	const ListOutEdgeIterator& l_iter = dynamic_cast<const ListOutEdgeIterator&>(iter);

	// ���� ��������� ��������� �� ������ �����
	if (this->graph_p != l_iter.graph_p)
		return true;
	// ���� ��� ��� ������������� �� ����� � ��� �� �����
	else if (this->curV == -1 && l_iter.curV == -1)
		return false;
	// ���� ��� ����������� �� ����� � ��� �� ����� �����
	else if (this->curV == l_iter.curV && this->curE == l_iter.curE)
		return false;
	// ���� ������
	else
		return true;
}

template <class Vertex_T, class Edge_T>
const typename ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator& ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator::operator=(const OutEdgeIteratorForm& iter)
{
	const ListOutEdgeIterator& l_iter = dynamic_cast<const ListOutEdgeIterator&>(iter);

	if (this == &l_iter)
		return *this;

	this->graph_p = l_iter.graph_p;
	this->curV = l_iter.curV;
	this->curE = l_iter.curE;

	return *this;
}
//*************************************************************************************************************************************************//


#endif
