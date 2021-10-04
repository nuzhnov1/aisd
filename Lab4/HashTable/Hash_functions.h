// Hash_functions.h
#pragma once
#ifndef _INC_HASH_FUNCTIONS_
#define _INC_HASH_FUNCTIONS


//*********************************************** ������� ������� ����������� �������� ��������� ����� ********************************************//
extern unsigned int conversion(const std::string& val);
extern unsigned int conversion(const unsigned long long& val);
//************************************************************************************************************************************************//


template <class T> size_t mul_hash(const T& val, const size_t size)
{
	// ������� �������
	const double phi = 0.61803398874989484820;
	// ����������� ���� � ����� num
	unsigned int num = conversion(val);
	// ������� ����� ����������� num * phi
	double temp = num * phi - static_cast<unsigned int>(num * phi);

	// ���������� ����� ����� ������������ temp * size
	return static_cast<size_t>(size * temp);
}

template <class T> size_t mod_hash(const T& val, const size_t size)
{
	// ����������� ���� � ����� num
	unsigned int num = conversion(val);

	// ���������� ������� �� ������� num �� size
	return static_cast<size_t>(val % size);
}


#endif
