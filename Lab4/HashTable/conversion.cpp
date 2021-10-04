// conversion.cpp


// ������������ ������������ �����
#include <string>


unsigned int conversion(const std::string& val)
{
	// ������ ������������ ������� ��������� �������� �������������� ������������, ������� ���������� ����������� ������.
	// � �������� ������������� ���� ��� ������ 32-������ ���������� ��� - unsigned int, ��������� �������������� ����� 
	// ����� �������������� �� ������ ���� ������� �������.

	const unsigned char base = 32; // ����� ���� � ������� �������� ��� ����� � 

	unsigned int sum = 0;

	for (auto& iter : val)
		sum = sum * base + (static_cast<unsigned char>(iter) - static_cast<unsigned char>('�') + 1);

	return sum;
}

unsigned int conversion(const unsigned long long& val)
{
	return static_cast<unsigned int>(val);
}
