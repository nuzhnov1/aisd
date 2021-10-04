// GraphForm.h
#pragma once
#ifndef _INC_GRAPH_FORM_
#define _INC_GRAPH_FORM_
// ������������ ������������ �����
#include <vector>


//**************************************************** �����-������������ ���� ����� **************************************************************//
enum class GraphFormType
{
	M_GRAPH,	// M-����
	L_GRAPH		// L-����
};
//*************************************************************************************************************************************************//


//**************************************************** ����� ����� ������������� ����� ************************************************************//
template <class Vertex_T, class Edge_T>
class GraphForm
{

	template <typename T> using vector = std::vector<T>;

protected:

	const vector<Vertex_T*>& vectorVertex;	// ������ �� ������ ���������� �� ������� �����
	size_t edgesCount;						// ���������� ���� �����
	bool directed;							// �������� �� ���� ���������������

	// �������� ����� ������ � �����
	size_t _getVertexCount() const { return this->vectorVertex.size(); }

public:

	// ����������� ����� ����� � �����������: ������� �� ������ ������ vectorVertex � ������������������ ����� directed
	GraphForm(const vector<Vertex_T*>& vectorVertex, bool directed) :
		vectorVertex(vectorVertex), edgesCount(0), directed(directed) {}
	// ����������� ���������� ��� ����� �����
	virtual ~GraphForm() {}

	// �������� ������������ ������
	virtual const GraphForm& operator=(const GraphForm& graph) = 0;

	// �������� ����� ���� � �����
	size_t getEdgesCount() const;
	// ���� �� ����� � �����
	virtual bool isHasEdge(size_t outV, size_t inV) const = 0;
	// �������� �����, ����������� ������� outV � inV
	virtual Edge_T& getEdge(size_t outV, size_t inV) const = 0;

	// �������� ������� � ����
	virtual void insertVertex(size_t index) = 0;
	// ������� ������� �����
	virtual void deleteVertex(size_t index) = 0;
	// �������� ����� � ����, ������������ ������� outV � inV, � ��������� ����������� � edge
	virtual void insertEdge(size_t outV, size_t inV, Edge_T* edge) = 0;
	// ������� ����� �� �����, ������������ ������� outV � inV
	virtual void deleteEdge(size_t outV, size_t inV) = 0;

	// ������� ����
	virtual void print() const = 0;

	// ����� ����� ��������� ����
	class EdgeIteratorForm
	{

	public:

		// ����������� ���������� ����� ��������� ����
		virtual ~EdgeIteratorForm() {}

		// �������� ���������� ����� �����, �� ������� � ������ ������ ��������� ��������
		virtual Edge_T& operator*() const = 0;

		// ���������� �������� ����������������� - ������� � ���������� ����� �����
		virtual EdgeIteratorForm& operator++() = 0;

		// �������� ��������� ����������
		virtual bool operator==(const EdgeIteratorForm& iter) const = 0;
		// �������� ����������� ����������
		virtual bool operator!=(const EdgeIteratorForm& iter) const = 0;

		// �������� ������������ ����������
		virtual const EdgeIteratorForm& operator=(const EdgeIteratorForm& iter) = 0;

	};

	// ����� ����� ��������� ��������� ������
	class OutEdgeIteratorForm
	{

	public:

		// ����������� ���������� ����� ��������� ��������� ����
		virtual ~OutEdgeIteratorForm() {}

		// �������� ���������� ����� �����, �� ������� � ������ ������ ��������� ��������
		virtual Edge_T& operator*() const = 0;

		// ���������� �������� ����������������� - ������� � ���������� ����� �����
		virtual OutEdgeIteratorForm& operator++() = 0;

		// �������� ��������� ����������
		virtual bool operator==(const OutEdgeIteratorForm& iter) const = 0;
		// �������� ����������� ����������
		virtual bool operator!=(const OutEdgeIteratorForm& iter) const = 0;

		// �������� ������������ ����������
		virtual const OutEdgeIteratorForm& operator=(const OutEdgeIteratorForm& iter) = 0;

	};

};
//*************************************************************************************************************************************************//


//****************************************************** ���������� ������� ListGraph *************************************************************//
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
