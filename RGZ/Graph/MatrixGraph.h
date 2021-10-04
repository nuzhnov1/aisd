// MatrixGraph.h
#pragma once
#ifndef _INC_MATRIX_GRAPH_
#define _INC_MATRIX_GRAPH_
// ������������ ������������ �����
#include <iostream>
#include <iomanip>
#include <locale>
#include "GraphForm.h"
#include "Graph_exception.h"


//********************************************** ����� ��������� ����� ������������� ����� ********************************************************//
template <class Vertex_T, class Edge_T>
class MatrixGraph: public GraphForm<Vertex_T, Edge_T>
{
	
	template <typename T> using vector = std::vector<T>;
	using GraphForm = GraphForm<Vertex_T, Edge_T>;
	using EdgeIteratorForm = typename GraphForm::EdgeIteratorForm;
	using OutEdgeIteratorForm = typename GraphForm::OutEdgeIteratorForm;

	vector<vector<Edge_T*>> matrix;	// ������� ���������

public:

	// ����������� ��������� ����� ����� � �����������: ������� �� ������ ������ vectorVertex � ������������������ ����� directed
	MatrixGraph(const vector<Vertex_T*>& vectorVertex, bool directed) : GraphForm(vectorVertex, directed), matrix() {}
	// ����������� ����������� ��������� ����� �����
	MatrixGraph(const MatrixGraph& graph, const vector<Vertex_T*>& vectorVertex);
	// ���������� ��������� ����� �����
	~MatrixGraph();

	// �������� ������������ ������
	virtual const MatrixGraph& operator=(const GraphForm& graph) final;

	// ���� �� ����� � �����
	virtual bool isHasEdge(size_t outV, size_t inV) const final { return this->matrix[outV][inV] != nullptr; }
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
	class MatrixEdgeIterator : public EdgeIteratorForm
	{

		const MatrixGraph* graph_p;	// ��������� �� ������� M-����
		size_t curI, curJ;			// ����� ������ � ������� �������� ����� � ������� ���������

	public:

		// ����������� ��������� � �����������: ������� �� ��������� ���� graph � ���������� ��������� toBegin
		MatrixEdgeIterator(const MatrixGraph& graph, bool toBegin = true);
		// ����������� ����������� ����������
		MatrixEdgeIterator(const MatrixEdgeIterator& iter) : graph_p(iter.graph_p), curI(iter.curI), curJ(iter.curJ) {}

		// �������� ���������� ����� �����, �� ������� � ������ ������ ��������� ��������
		virtual Edge_T& operator*() const final;

		// ���������� �������� ����������������� - ������� � ���������� ����� �����
		virtual MatrixEdgeIterator& operator++() final;

		// �������� ��������� ����������
		virtual bool operator==(const EdgeIteratorForm& iter) const final;
		// �������� ����������� ����������
		virtual bool operator!=(const EdgeIteratorForm& iter) const final;

		// �������� ������������ ����������
		virtual const MatrixEdgeIterator& operator=(const EdgeIteratorForm& iter) final;

		// ���������� ��������, ������������� �� ������
		static MatrixEdgeIterator begin(const MatrixGraph& graph) { return MatrixEdgeIterator(graph, true); }
		// ���������� ��������, ������������� �� �����
		static MatrixEdgeIterator end(const MatrixGraph& graph) { return MatrixEdgeIterator(graph, false); }

	};

	// ����� ��������� ��������� ���� ����� ��������� �����
	class MatrixOutEdgeIterator : public OutEdgeIteratorForm
	{

		const MatrixGraph* graph_p;	// ��������� �� ������� M-����
		size_t curI, curJ;			// ����� ������ � ������� �������� ����� � ������� ���������

	public:

		// ����������� ��������� � �����������: ������� �� ��������� ���� graph, �������� ������� index � ���������� ��������� toBegin
		MatrixOutEdgeIterator(const MatrixGraph& graph, size_t index, bool toBegin = true);
		// ����������� ����������� ����������
		MatrixOutEdgeIterator(const MatrixOutEdgeIterator& iter) : graph_p(iter.graph_p), curI(iter.curI), curJ(iter.curJ) {}

		// �������� ���������� ����� �����, �� ������� � ������ ������ ��������� ��������
		virtual Edge_T& operator*() const final;

		// ���������� �������� ����������������� - ������� � ���������� ����� �����
		virtual MatrixOutEdgeIterator& operator++() final;

		// �������� ��������� ����������
		virtual bool operator==(const OutEdgeIteratorForm& iter) const final;
		// �������� ����������� ����������
		virtual bool operator!=(const OutEdgeIteratorForm& iter) const final;

		// �������� ������������ ����������
		virtual const MatrixOutEdgeIterator& operator=(const OutEdgeIteratorForm& iter) final;

		// ���������� ��������, ������������� �� ������
		static MatrixOutEdgeIterator begin(const MatrixGraph& graph, size_t index) { return MatrixOutEdgeIterator(graph, index, true); }
		// ���������� ��������, ������������� �� �����
		static MatrixOutEdgeIterator end(const MatrixGraph& graph, size_t index) { return MatrixOutEdgeIterator(graph, index, false); }

	};

};
//*************************************************************************************************************************************************//


//***************************************************** ���������� ������� MatrixGraph ************************************************************//
// public-������
template <class Vertex_T, class Edge_T>
MatrixGraph<Vertex_T, Edge_T>::MatrixGraph(const MatrixGraph& graph, const vector<Vertex_T*>& vectorVertex):
	GraphForm(vectorVertex, graph.directed)
{
	this->directed = graph.directed;
	this->edgesCount = graph.edgesCount;
	this->matrix = vector<vector<Edge_T*>>();

	// � ������� ��������� ��� ������ ������ ������ �����, ������� ���������
	// �� �� ������ ������, ��� � � ���������� �����
	for (size_t i = 0, size = this->_getVertexCount(); i < size; i++)
	{
		// ��������� ������ ������ ������� size
		this->matrix.push_back(vector<Edge_T*>(size, nullptr));

		for (size_t j = 0; j < size; j++)
			if (graph.matrix[i][j] != nullptr)
			{
				// ������ �����
				this->matrix[i][j] = new Edge_T(*this->vectorVertex[i], *this->vectorVertex[j]);

				// �������� ���������� ��� � �����
				try { this->matrix[i][j]->setWeight(graph.matrix[i][j]->getWeight()); }
				catch (Graph_exception&) {}

				// �������� ���������� ������ � �����
				try { this->matrix[i][j]->setData(graph.matrix[i][j]->getData()); }
				catch (Graph_exception&) {}
			}
	}
}

template <class Vertex_T, class Edge_T>
MatrixGraph<Vertex_T, Edge_T>::~MatrixGraph()
{
	// �������� �� ������� ��������� � ������� ���� �����
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
	// �������� ����������� ����� ����� � ��������� �����
	const MatrixGraph& m_graph = dynamic_cast<const MatrixGraph&>(graph);

	if (this == &m_graph)
		return *this;

	// �������� �� ������� ��������� � ������� ���� �����
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

	// � ������� ��������� ��� ������ ������ ������ �����, ������� ���������
	// �� �� ������ ������, ��� � � ���������� �����
	for (size_t i = 0, size = this->_getVertexCount(); i < size; i++)
	{
		// ��������� ������ ������ ������� size
		this->matrix.push_back(vector<Edge_T*>(size, nullptr));

		for (size_t j = 0; j < size; j++)
			if (m_graph.matrix[i][j] != nullptr)
			{
				// ������ �����
				this->matrix[i][j] = new Edge_T(*this->vectorVertex[i], *this->vectorVertex[j]);

				// �������� ���������� ��� � �����
				try { this->matrix[i][j]->setWeight(m_graph.matrix[i][j]->getWeight()); }
				catch (Graph_exception&) {}

				// �������� ���������� ������ � �����
				try { this->matrix[i][j]->setData(m_graph.matrix[i][j]->getData()); }
				catch (Graph_exception&) {}
			}
	}

	return *this;
}

template <class Vertex_T, class Edge_T>
typename Edge_T& MatrixGraph<Vertex_T, Edge_T>::getEdge(size_t outV, size_t inV) const
{
	// ���������, ���� �� �������� ����� � �����
	if (this->matrix[outV][inV] == nullptr)
		throw Graph_exception("������ ����� ��� � �����");

	return *(this->matrix[outV][inV]);
}

template <class Vertex_T, class Edge_T>
typename void MatrixGraph<Vertex_T, Edge_T>::insertVertex(size_t index)
{
	// ������ ������� �������
	for (auto& vector : this->matrix)
		vector.insert(vector.begin() + index, nullptr);

	// ������ ����� ������ �������
	this->matrix.insert(this->matrix.begin() + index, vector<Edge_T*>(this->matrix.size() + 1, nullptr));
}

template <class Vertex_T, class Edge_T>
typename void MatrixGraph<Vertex_T, Edge_T>::deleteVertex(size_t index)
{
	// ������� ������� ��� ��������� ���� - ������ index ������� ���������
	for (const auto& edge : this->matrix[index])
		if (edge != nullptr)
		{
			this->edgesCount--;
			delete edge;
		}
	this->matrix.erase(this->matrix.begin() + index);

	// � ����� ������� ��� �������� ���� - ������� index ������� ���������
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
	// ��������� ���� �� ����� � �����
	if (this->matrix[outV][inV] != nullptr)
		throw Graph_exception("����� ����� ��� ���� � �����");

	// ���� ���, �� ��������� ��� � ����
	this->matrix[outV][inV] = edge;

	this->edgesCount++;
}

template <class Vertex_T, class Edge_T>
typename void MatrixGraph<Vertex_T, Edge_T>::deleteEdge(size_t outV, size_t inV)
{
	// ��������� ���� �� ����� � �����
	if (this->matrix[outV][inV] == nullptr)
		throw Graph_exception("������ ����� ��� � �����");

	// ���� ����� ����������� �����, �� ������� ���
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

	// ���� ��� ������, �� ��������� �� ��������� ������ � �������
	if (this->matrix.size() == 0)
	{
		cout << endl;

		return;
	}

	// ����� ������� ��������
	cout << "     ";
	for (auto& vertex : this->vectorVertex)
		cout << std::setw(4) << vertex->getName();
	cout << endl;
	// ����� ������� �����
	cout << "_____";
	for (size_t i = 0, size = this->matrix.size(); i < size; i++)
		cout << "____";
	cout << endl;

	// ����� ������� ����� � ����� ����
	for (size_t i = 0, end = this->matrix.size(); i < end; i++)
	{
		cout << " " << std::setw(2) << this->vectorVertex[i]->getName() << " |";
		for (const auto& edge : this->matrix[i])
			if (edge != nullptr)
			{
				cout << std::setw(4);
				try { cout << edge->getWeight(); }			// �������� ���������� � ���� �����
				catch (Graph_exception&) { cout << "?"; }	// ���� �� �� ����������, ������� ���� - ?
			}
			else
				cout << std::setw(4) << "inf";	// ���� ����� ���, �� ��� ����� �������������
		cout << endl;
	}
	
	// ������� ������ �����
	cout << "_____";
	for (size_t i = 0, size = this->matrix.size(); i < size; i++)
		cout << "____";
	cout << endl;

	// ������� ���������� �� ������������
	cout << "������ \"?\" ���������� ����, ��� ������� �� ����������." << endl;
	cout << "������ \"inf\" �������� ��, ��� ����� ����� ��������� ���." << endl;
}
//*************************************************************************************************************************************************//


//************************************************** ���������� ������� MatrixEdgeIterator ********************************************************//
// public-������:
template <class Vertex_T, class Edge_T>
MatrixGraph<Vertex_T, Edge_T>::MatrixEdgeIterator::MatrixEdgeIterator(const MatrixGraph& graph, bool toBegin)
{
	this->graph_p = &graph;

	// ���� ������ ���������� �� ������������� ��������
	if (!toBegin)
	{
		this->curI = -1;
		this->curJ = -1;

		return;
	}

	// �������� �� ������� ��������� � ������� ������� �����
	size_t end_row = this->graph_p->matrix.size();
	for (this->curI = 0; this->curI < end_row; this->curI++)
	{
		// ���� ���� �����������������, �� ������� ������ ������ ����������� ������� ������������
		// ������� ��������� ������������
		size_t end_col = (this->graph_p->directed) ? end_row : this->curI + 1;

		for (this->curJ = 0; this->curJ < end_col; this->curJ++)
			if (this->graph_p->matrix[this->curI][this->curJ] != nullptr)
				return;
	}

	// ���� �������� ��� � �� ��� ����������
	this->curI = -1;
	this->curJ = -1;
}

template <class Vertex_T, class Edge_T>
typename Edge_T& MatrixGraph<Vertex_T, Edge_T>::MatrixEdgeIterator::operator*() const
{
	if (this->curI == -1 || this->curJ == -1)
		throw Graph_exception("�������� �� ����������");

	return *(this->graph_p->matrix[this->curI][this->curJ]);
}

template <class Vertex_T, class Edge_T>
typename MatrixGraph<Vertex_T, Edge_T>::MatrixEdgeIterator& MatrixGraph<Vertex_T, Edge_T>::MatrixEdgeIterator::operator++()
{
	if (this->curI == -1 || this->curJ == -1)
		throw Graph_exception("�������� �� ����������");

	// �������� �� ������� ���������
	for (size_t end_row = this->graph_p->matrix.size(); this->curI < end_row; this->curI++)
	{
		// ���� ���� �����������������, �� ������� ������ �� ������� ��������� ������������
		size_t end_col = (this->graph_p->directed) ? end_row : this->curI + 1;

		// �������� �� ������� ������ � ���� ������ ��������� ������
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


//************************************************* ���������� ������� MatrixOutEdgeIterator ******************************************************//
// public-������:
template <class Vertex_T, class Edge_T>
MatrixGraph<Vertex_T, Edge_T>::MatrixOutEdgeIterator::MatrixOutEdgeIterator(const MatrixGraph& graph, size_t index, bool toBegin)
{
	this->graph_p = &graph;
	this->curI = index;

	// ���� ������ ���������� �� ������������� ��������
	if (!toBegin)
	{
		this->curJ = -1;

		return;
	}

	// �������� �� ������� ��������� ���� � ������� ��������� � ������ ������� �����
	size_t size = this->graph_p->matrix.size();
	for (this->curJ = 0; this->curJ < size; this->curJ++)
		if (this->graph_p->matrix[this->curI][this->curJ] != nullptr)
			return;

	// ���� � ������ ������� ��� ��������� ����
	this->curJ = -1;
}

template <class Vertex_T, class Edge_T>
typename Edge_T& MatrixGraph<Vertex_T, Edge_T>::MatrixOutEdgeIterator::operator*() const
{
	if (this->curI == -1 || this->curJ == -1)
		throw Graph_exception("�������� �� ����������");

	return *(this->graph_p->matrix[this->curI][this->curJ]);
}

template <class Vertex_T, class Edge_T>
typename MatrixGraph<Vertex_T, Edge_T>::MatrixOutEdgeIterator& MatrixGraph<Vertex_T, Edge_T>::MatrixOutEdgeIterator::operator++()
{
	if (this->curI == -1 || this->curJ == -1)
		throw Graph_exception("�������� �� ����������");

	// �������������� curJ, � �������� ������ �� ������ � ������ ���������� �����
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
