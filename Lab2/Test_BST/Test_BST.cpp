// Test_BST.cpp


//*************************************************** Подключаемые заголовочные файлы *************************************************************//
#include <iostream>
#include <string> 
#include <math.h>
#include <time.h>
#include <locale.h>
#include "../BST/BST.h"
//*************************************************************************************************************************************************//


//******************************************************** Используемые объекты из std ************************************************************//

using std::cout;
using std::cin;
using std::endl;

//*************************************************************************************************************************************************//


//************************************************************* Псевдонимы типов ******************************************************************//

typedef unsigned long long INT_64;

//*************************************************************************************************************************************************//


//************************************************************ Прототипы функций ******************************************************************//

// Функция установки первого случайного числа от часов компьютера
void sRand();
// Функция генерации случайного числа  линейным конгруэнтным генератором Xi+1 =(a * Xi + c) % arr 
//habr.com/ru/post/208178 
INT_64 LineRand();
// Тестирование случайного дерева, размера n
void test_rand(size_t n);
// Тестирование вырожденного дерева, размера n
// delta - интервал между последовательно возрастающими числами
void test_ord(size_t n, INT_64 delta);
// Получить целое число
INT_64 get_val();

//*************************************************************************************************************************************************//


static INT_64 RRand = 15750;


int main()
{
    constexpr size_t MAX_SIZE = 1000000; // Максимально допустимый размер дерева
    INT_64 start, end, step;

    setlocale(LC_ALL, "Russian");

    cout << "\t\t\tПрограмма тестирования операций вставки, удаления и поиска в BST-дереве." << endl << endl;

    cout << "Введите начальное значение размерности дерева(не больше " << MAX_SIZE << "): " << endl;
    start = get_val();
    while (start > MAX_SIZE)
    {
        cout << "Ошибка: введённое значение начальной размерности превышает допустимое максимальное значение размерности дерева.";
        cout << "Повторите ввод." << endl;
        start = get_val();
    }
    cout << "Введите конечное значение размерности дерева(не больше " << MAX_SIZE << "): " << endl;
    end = get_val();
    while (end > MAX_SIZE)
    {
        cout << "Ошибка: введённое значение начальной размерности превышает допустимое максимальное значение размерности дерева.";
        cout << "Повторите ввод." << endl;
        end = get_val();
    }
    cout << "Введите шаг для размерности дерева." << endl;
    step = get_val();

    cout << "Тестирования началось..." << endl;
    for (INT_64 i = start; i <= end; i += step)
    {
        cout << "Результаты тестирования случайного дерева размерностью " << i << ":" << endl;
        test_rand(static_cast<size_t>(i));
        cout << "Результаты тестирования вырожденного дерева размерностью " << i << ":" << endl;
        test_ord(static_cast<size_t>(i), 1000000);
        cout << endl;
    }
    cout << "Тестирования окончено." << endl;
    cout << "Программа успешно завершила свою работу. Нажмите Enter для выхода из программы...";
    cin.get();

    return 0;
}

void sRand()
{ 
    srand(static_cast<unsigned int>(time(0)));
    RRand = static_cast<INT_64>(rand()); 
}

INT_64 LineRand()
{
    // Константы генератора rand
    const INT_64 mRand = static_cast<INT_64>((1 << 63) - 1);
    const INT_64 aRand = 6364136223846793005;
    const INT_64 cRand = 1442695040888963407;

	INT_64 y1, y2;

	y1 = (aRand * RRand + cRand) % mRand;
	y2 = (aRand * y1 + cRand) % mRand;
	RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 & 0xFFFFFFFF00000000LL) >> 32;

	return RRand;
}

void test_rand(size_t n)
{ 
    BST<INT_64, int> tree;          // Создание дерева для 64 – разрядных ключей типа INT_64
    INT_64* arr = new INT_64[n];    // Массив для ключей, которые присутствуют в дереве
         
    sRand(); // Установка первого случайного числа

    for (size_t i = 0; i < n; i++)  // Заполнение дерева и массива элементами со случайными ключами 
    {
        arr[i] = LineRand();
        tree.append(arr[i], 1);
    }

    // Вывод размера дерева до теста 
    cout << "Размерность дерева до испытаний: " << tree.getSize() << "." << endl;

    // Обнуление счётчиков трудоёмкости вставки, удаления и поиска 
    double I = 0;
    double D = 0;
    double S = 0;

    // Генерация потока операций, 10% - промахи операций 
    for (size_t i = 0; i < n / 2; i++)
        if (i % 10 == 0) // 10% промахов 
        {
            try { tree.remove(LineRand()); }
            catch (BST_exception&) {}
            D += tree.getLastOp();

            try { tree.append(arr[LineRand() % n], 1); }
            catch (BST_exception&) {}
            I += tree.getLastOp();

            try { tree.getItemByKey(LineRand()); }
            catch (BST_exception&) {}
            S += tree.getLastOp();
        }
        else // 90% успешных операций 
        {
            INT_64 index = LineRand() % n;

            try { tree.remove(arr[index]); }
            catch (BST_exception&) {}
            D += tree.getLastOp();

            INT_64 key = LineRand();
            
            try { tree.append(key, 1); }
            catch (BST_exception&) {}
            I += tree.getLastOp();
            arr[index] = key;

            try { tree.getItemByKey(arr[LineRand() % n]); }
            catch (BST_exception&) {}
            S += tree.getLastOp();
        } // Конец теста 

    // Вывод результатов:

    // Вывод размера дерева после теста
    cout << "Размерность дерева после испытаний: " << tree.getSize() << "." << endl;
    
    // Теоретической оценки трудоёмкости операций BST
    cout << "Теоретическая оценка(1.39 * log2(n)): " << 1.39 * (log(static_cast<double>(n)) / log(2.0)) << "." << endl;
    // Экспериментальной оценки трудоёмкости вставки
    cout << "Среднее количество переборов на одну вставку: " << I / (n / static_cast<double>(2)) << "." << endl;
    // Экспериментальной оценки трудоёмкости удаления
    cout << "Среднее количество переборов на одно удаление: " << D / (n / static_cast<double>(2)) << "." << endl;
    // Экспериментальной оценки трудоёмкости поиска
    cout << "Среднее количество переборов на один поиск: " << S / (n / static_cast<double>(2)) << "." << endl;
    
    // Освобождение памяти массива arr[]
    delete[] arr;
}

// Тестирование вырожденного BST-дерева
void test_ord(size_t n, INT_64 delta)
{
    BST< INT_64, int > tree;        // Создание дерева для 64 – разрядных ключей типа INT_64  
    INT_64* arr = new INT_64[n];    // Массив для ключей, которые присутствуют в дереве

    sRand(); // Установка первого случайного числа

    // Заполнение дерева и массива элементами с возрастающими чётными ключами на интервале [0, delta, 2 * delta, ... ,delta * n] 
    for (size_t i = 0; i < n; i++)
    {
        arr[i] = i * delta;
        tree.append(arr[i], 1);
    }

    // Вывод размера дерева до теста
    cout << "Размерность дерева до испытаний: " << tree.getSize() << "." << endl;

    // Обнуление счётчиков трудоёмкости вставки, удаления и поиска
    double I = 0;
    double D = 0;
    double S = 0;

    // Генерация потока операций, 10% - промахи операций 
    for (size_t i = 0; i < n / 2; i++)
    {
        if (i % 10 == 0)  // 10% промахов 
        {
            INT_64 key = LineRand() % (delta * n);

            key += ~(key % 2); // Случайный нечётный ключ 
            try { tree.remove(key); }
            catch (BST_exception&) {}
            D += tree.getLastOp();

            try { tree.append(arr[LineRand() % n], 1); }
            catch (BST_exception&) {}
            I += tree.getLastOp();

            key = LineRand() % (delta * n);
            key += ~(key % 2); // Случайный нечётный ключ 
            try { tree.getItemByKey(key); }
            catch (BST_exception&) {}
            S += tree.getLastOp();
        }
        else  // 90% успешных операций 
        {
            INT_64 index = rand() % n;

            try { tree.remove(arr[index]); }
            catch (BST_exception&) {}
            D += tree.getLastOp();

            INT_64 key = LineRand() % (delta * n);

            key += key % 2;  // Случайный чётный ключ 
            try { tree.append(key, 1); }
            catch (BST_exception&) {}
            I += tree.getLastOp();
            arr[index] = key;

            try { tree.getItemByKey(arr[LineRand() % n]); }
            catch (BST_exception&) {}
            S += tree.getLastOp();
        }
    }

    // Вывод результатов:

    // Вывод размера дерева после теста
    cout << "Размерность дерева после испытаний: " << tree.getSize() << "." << endl;
    // Теоретической оценки трудоёмкости операций BST
    cout << "Теоретическая оценка(n / 2): " << n / 2 << "." << endl;
    // Экспериментальной оценки трудоёмкости вставки
    cout << "Среднее количество переборов на одну вставку: " << I / (n / static_cast<double>(2)) << "." << endl;
    // Экспериментальной оценки трудоёмкости удаления
    cout << "Среднее количество переборов на одно удаление: " << D / (n / static_cast<double>(2)) << "." << endl;
    // Экспериментальной оценки трудоёмкости поиска
    cout << "Среднее количество переборов на один поиск: " << S / (n / static_cast<double>(2)) << "." << endl;

    // Освобождение памяти массива arr[]
    delete[] arr;
}

INT_64 get_val()
{
    using std::string;
    using std::invalid_argument;
    using std::out_of_range;

    string buf;
    INT_64 val;

    cout << ">>> ";
    getline(cin, buf);

    while (true)
    {
        try
        {
            val = stoi(buf);
            break;
        }
        catch (invalid_argument)
        {
            cout << "Ошибка: неверный формат входных данных." << endl;
        }
        catch (out_of_range)
        {
            cout << "Ошибка: введённое число вышло из диапазона int." << endl;
        }

        cout << ">>> ";
        getline(cin, buf);
    }

    return val;
}
