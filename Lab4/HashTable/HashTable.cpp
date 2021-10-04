// HashTable.cpp


//*************************************************** Подключаемые заголовочные файлы *************************************************************//
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "HashTable.h"
//*************************************************************************************************************************************************//


//****************************************************** Используемые объекты из std **************************************************************//

using std::cin;
using std::cout;
using std::endl;
using std::string;

//*************************************************************************************************************************************************//


//********************************************************* Прототипы используемых функций ********************************************************//

// Получить целое значение из стандартного потока ввода
int get_int();
// Получить число с плавающей точкой из стандартного потока ввода
float get_float();
// Получить слово с заглавными буквами кириллицы из стандартного потока ввода
string get_word();
// Проверка на то, что в строке содержаться только заглавные буквы кириллицы
bool is_cyrillic(const string& str);
// Получить случайное слово с заглавными буквами кириллицы
string get_rand_word(short unsigned int start_len = 5, short unsigned int end_len = 16);
// Поиск ближайшей степени двойки большей, чем num
size_t clp2(size_t num);

//*************************************************************************************************************************************************//


//********************************************* Пространство имён тестирования хеш-таблицы ********************************************************//
namespace TestHash
{
    // Конвертация ключа key в число
    unsigned long conversion(const string& key);
    // Хеш-функция для ключа key
    size_t hash(const string& key, size_t table_size);
    // Функция тестирования хеш-функции
    void TestHashFunction(size_t table_size);

    unsigned long conversion(const string& key)
    {
        // Важной составляющей данного алгоритма является арифметическое переполнение, которое происходит практически всегда.
        // В качестве возвращаемого типа был выбран 32-битный безнаковый тип - unsigned int, поскольку результирующее число 
        // после преобразования не должно быть слишком большим.

        static const unsigned char base = 32; // Число букв в русском алфавите без буквы Ё 

        unsigned int sum = 0;

        for (auto& iter : key)
            sum = sum * base + (static_cast<unsigned char>(iter) - static_cast<unsigned char>('А') + 1);

        return sum;
    }

    size_t hash(const string& key, size_t table_size)
    {
        // Золотое сечение
        static const double phi = 0.61803398874989484820;
        // Преобразуем ключ к числу num
        unsigned int num = conversion(key);
        // Дробная часть проиведения num * phi
        double temp = num * phi - static_cast<unsigned int>(num * phi);

        // Возвращаем целую часть произведения temp * size
        return static_cast<size_t>(table_size * temp);
    }

    void TestHashFunction(size_t table_size)
    {
        // Отношение размера выборки ключей к размеру таблицы
        static const unsigned short a = 20;
        // Используемая переменная под ключ
        string key;
        // Вектор числа ключей для каждого значения хеша в хеш-таблице  
        std::vector<size_t> keys(table_size);

        cout << "Тестирование хеш-функции началось..." << endl;

        // Устанавливаем сид на текущее значение времени
        srand(static_cast<unsigned int>(time(nullptr)));

        // Генерируем случайные ключи, хешируем их и подсчитываем количество ключей с одинаковыми хеш-значениями
        for (size_t i = 0, end = a * table_size; i < end; i++)
        {
            key = get_rand_word();
            keys[hash(key, table_size)]++;
        }

        // Находим значение критерия хи-квадрат
        double xi = 0;

        for (size_t i = 0; i < table_size; i++)
            xi += (keys[i] - a) * (keys[i] - a);
        xi = xi / a;

        cout << "Тестирование хеш-функции завершено." << endl;

        // Выводим полученные данные статистические данные
        cout << "Полученные статистические данные:";
        cout << "Нижняя теоретическая оценка(m - sqrt(m)): " << table_size - sqrt(table_size) << "." << endl;
        cout << "Значение критерия хи-квадрат: " << xi << "." << endl;
        cout << "Верхняя теоретическая оценка(m + sqrt(m)): " << table_size + sqrt(table_size) << "." << endl;
    }

    void TestHashTable(size_t table_size, TableType type, double alpha)
    {
        HashTable<string, int> table(table_size, type);         // Хеш-таблица для тестирования
        double ins = 0, del = 0, src = 0;                       // Счётчики трудоёмкости операций вставки, удаления и поиска
        size_t count = static_cast<size_t>(table_size * alpha); // Число элементов в хеш-таблице
        std::vector<string> keys(count);                        // Копии ключей в хеш-таблицы
        string key;                                             // Используемая переменная под ключ

        cout << "Тестирование трудоёмкости операций для хеш-таблицы с заданными параметрами началось..." << endl;

        cout << "Генерируется хеш-таблица..." << endl;

        // Устанавливаем сид на текущее значение времени
        srand(static_cast<unsigned int>(time(nullptr)));

        // Генерируем хеш-таблицу с "count" случайными ключами
        for (size_t i = 0; i < count; i++)
        {
            try
            {
                key = get_rand_word();
                table.append(key, 1);
                keys[i] = key;
            }
            catch (Hash_exception&) { i--; }
        }

        cout << "Хеш-таблица успешно сгенерированна." << endl;

        cout << "Число элементов в хеш-таблице до испытаний: " << table.getCount() << "." << endl;

        cout << "Начинается тестирование операций вставки, удаления и поиска для данной хеш-таблицы..." << endl;

        // Генерируем поток операций вставки, удаления и поиска
        for (size_t i = 0, end = count / 2; i < end; i++)
        {
            // 10% операций - промахи
            if (i % 10 == 0)
            {
                // Неудачное удаление элемента

                // Генерируем ключ, которого нет списке ключей и в хеш-таблице
                for (key = get_rand_word(); std::find(keys.begin(), keys.end(), key) != keys.end(); key = get_rand_word());
                // Пытаемся удалить элемент с этим ключом
                try { table.remove(key); }
                catch (Hash_exception&) {}
                // Считываем число элементарных операций при неуспешном удалении 
                del += table.getLastOp();

                // Неудачный поиск элемента

                // Генерируем ключ, которого нет списке ключей и в хеш-таблице
                for (key = get_rand_word(); std::find(keys.begin(), keys.end(), key) != keys.end(); key = get_rand_word());
                // Пытаемся его найти
                try { table[key]; }
                catch (Hash_exception&) {}
                // Считываем число элементарных операций при неуспешном поиске
                src += table.getLastOp();

                // Неудачное добавление элемента
                
                // Выбираем случайный ключ из хеш-таблицы
                key = keys[rand() % count];
                // Пытаемся снова добавить элемент с таким ключом в хеш-таблицу
                try { table.append(key, 1); }
                catch (Hash_exception&) {}
                // Считываем число элементарных операций при неуспешном добавлении
                ins += table.getLastOp();
            }
            // Остальные 90% операций успешные
            else
            {
                size_t index = rand() % count;  // Выбираем случайный ключ из списка ключей

                // Успешное удаление элемента

                table.remove(keys[index]);
                del += table.getLastOp();

                // Успешное добавление элемента

                // Генерируем ключ, которого нет списке ключей и в хеш-таблице
                for (key = get_rand_word(); std::find(keys.begin(), keys.end(), key) != keys.end(); key = get_rand_word());
                table.append(key, 1);
                keys[index] = key;
                ins += table.getLastOp();

                // Успешный поиск элемента

                table[keys[rand() % count]];
                src += table.getLastOp();
            }
        }

        cout << "Тестирование операций вставки, удаления и поиска для данной хеш-таблицы завершено." << endl;
        cout << "Число элементов в хеш-таблице после испытаний: " << table.getCount() << "." << endl;

        cout << "Теоретические и экспериментальные оценки трудоёмкости операций вставки, удаления и поиска:" << endl;

        cout << "Оценки операции вставки:" << endl;
        cout << "Теоретическая оценка: ";
        cout.precision(3);
        if (type == TableType::SEPARATE_CHAINING)
            cout << 1 + alpha << "." << endl;
        else
            cout << 0.5 + 0.05 / (1 - alpha) + 0.45 * (1 / ((1 - alpha) * (1 - alpha))) << "." << endl;
        cout.precision(3);
        cout << "Экспериментальная оценка: " << ins / (count / static_cast<double>(2)) << "." << endl;

        cout << "Оценки операции удаления:" << endl;
        cout << "Теоретическая оценка: ";
        cout.precision(3);
        if (type == TableType::SEPARATE_CHAINING)
            cout << 0.1 * (1 + alpha) + 0.9 * (1 + alpha / 2) << "." << endl;
        else
            cout << 0.5 + 0.45 / (1 - alpha) + 0.05 * (1 / ((1 - alpha) * (1 - alpha))) << "." << endl;
        cout.precision(3);
        cout << "Экспериментальная оценка: " << del / (count / static_cast<double>(2)) << "." << endl;

        cout << "Оценки операции поиска:" << endl;
        cout << "Теоретическая оценка: ";
        cout.precision(3);
        if (type == TableType::SEPARATE_CHAINING)
            cout << 0.1 * (1 + alpha) + 0.9 * (1 + alpha / 2) << "." << endl;
        else
            cout << 0.5 + 0.45 / (1 - alpha) + 0.05 * (1 / ((1 - alpha) * (1 - alpha))) << "." << endl;
        cout.precision(3);
        cout << "Экспериментальная оценка: " << src / (count / static_cast<double>(2)) << "." << endl;
    }
}
//*************************************************************************************************************************************************//


int main()
{
    HashTable<string, int> table;                   // Используемая хеш-таблица
    constexpr unsigned short MAX_SIZE = 1024;       // Максимальный задаваемый размер хеш-таблицы
    size_t MAX_COUNT = MAX_SIZE;                    // Максимальное число элементов в хеш-таблице
    int size = 0;                                   // Вводимый размер хеш-таблицы
    TableType type = TableType::SEPARATE_CHAINING;  // Выбираемый тип хеш-таблицы
    int choose = 0;                                 // Выбираемый пункт меню

    // Устанавливаем кодировку ввода на windows-1251
    SetConsoleCP(1251);
    // Устанавливаем кодировку вывода на windows-1251
    SetConsoleOutputCP(1251);

    cout << "\t\t\t\t\tПрограмма \"АТД Хеш-таблица\"." << endl << endl;

    cout << "Выберите тип хеш-таблицы:" << endl;
    cout << "\t1: Хеш-таблица с цепочками коллизий." << endl;
    cout << "\t2: Хеш-таблица с открытой адресацией." << endl;
    choose = get_int();
    while (choose < 1 || choose > 2)
    {
        cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
        choose = get_int();
    }
    switch (choose)
    {
    case 1: break;  // По умолчанию этот тип хеш-таблицы уже выбран
    case 2: type = TableType::OPEN_ADDRESSING; break;
    }

    cout << "Введите размер хеш-таблицы(не больше " << MAX_SIZE << "):" << endl;
    size = get_int();
    while (size < 0 || size > MAX_SIZE)
    {
        cout << "Ошибка: введённый размер меньше 0 или превышает " << MAX_SIZE << ". Повторите ввод." << endl;
        size = get_int();
    }

    cout << "Выберите тип заполнения хеш-таблицы:" << endl;
    cout << "\t1: Пустая хеш-таблица." << endl;
    cout << "\t2: Хеш-таблица со случайными ключами." << endl;
    choose = get_int();
    while (choose < 1 || choose > 2)
    {
        cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
        choose = get_int();
    }
    table = HashTable<string, int>(size, type); // Создаём пустую хеш-таблицу размера size и типа type
    size = clp2(size);
    switch (choose)
    {
    case 1: break;  // Хеш-таблица изначально пуста
    
    case 2:
    {
        HashTable<string, int> temp = table;
        size_t count = 0;

        srand(static_cast<unsigned int>(time(nullptr)));
        
        if (type == TableType::SEPARATE_CHAINING)
            MAX_COUNT = size * 20;
        else
            MAX_COUNT = size;

        cout << "Введите количество элементов в хеш-таблице(не больше " << MAX_COUNT << "):" << endl;
        count = get_int();
        while (count < 0 || count > MAX_COUNT)
        {
            cout << "Ошибка: введённый размер меньше 0 или превышает " << MAX_COUNT << ". Повторите ввод." << endl;
            count = get_int();
        }
        for (size_t i = 0; i < count; i++)
        {
            try { temp.append(get_rand_word(1, 3), rand()); }
            catch (Hash_exception&) { i--; }
        }

        table = temp;

        break;
    }
    }
    cout << "Хеш-таблица успешно создана." << endl << endl;

    bool isExit = false;
    while (!isExit)
    {
        cout << "Меню программы \"АТД Хеш-таблица\":" << endl;
        cout << "\t1: Получить размер хеш-таблицы." << endl;
        cout << "\t2: Получить количество элементов в текущей хеш-таблице." << endl;
        cout << "\t3: Вывести тип текущей хеш-таблицы." << endl;
        cout << "\t4: Получить количество переборов в хеш-таблице на последней операции вставки, удаления или поиска." << endl;
        cout << "\t5: Получить элемент хеш-таблицы по ключу." << endl;
        cout << "\t6: Установить новое значение элемента хеш-таблицы по ключу." << endl;
        cout << "\t7: Проверка хеш-таблицы на пустоту." << endl;
        cout << "\t8: Очистить хеш-таблицу." << endl;
        cout << "\t9: Добавить элемент в хеш-таблицу с заданным ключом и данными." << endl;
        cout << "\t10: Удалить элемент из хеш-таблицы по ключу." << endl;
        cout << "\t11: Вывести хеш-таблицу." << endl;
        cout << "\t12: Перейти в раздел управления \"Итератор\"." << endl;
        cout << "\t13: Произвести тестирование хеш-функции." << endl;
        cout << "\t14: Произвести тестирование оценок трудоёмкости основных операций для хеш-таблицы." << endl;
        cout << "\t15: Выйти из программы." << endl;

        cout << "Выберите пункт меню:" << endl;
        choose = get_int();
        while (choose < 1 || choose > 15)
        {
            cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
            choose = get_int();
        }

        cout << endl;

        switch (choose)
        {
        case 1:
        {
            cout << "Размер хеш-таблицы: " << table.getSize() << "." << endl;
            
            break;
        }

        case 2:
        {
            cout << "Количество элементов в текущей хеш-таблице: " << table.getCount() << "." << endl;
            
            break;
        }

        case 3:
        {
            cout << "Тип текущей хеш-таблицы: ";

            if (table.getType() == TableType::SEPARATE_CHAINING)
                cout << "\"Хеш-таблица с цепочками коллизий\"." << endl;
            else
                cout << "\"Хеш-таблица с открытой адресацией\"." << endl;
            
            break;
        }

        case 4:
        {
            cout << "Количество переборов в хеш-таблице на предыдущей операции вставки, удаления или поиска: "
                << table.getLastOp() << "." << endl;
            
            break;
        }

        case 5:
        {
            cout << "Введите ключ:" << endl;
            string key = get_word();

            try
            {
                cout << "Элемент хеш-таблицы с данным ключом: " << table[key] << "." << endl;
            }
            catch (Hash_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }

            break;
        }

        case 6:
        {
            cout << "Введите ключ:" << endl;
            string key = get_word();
            cout << "Введите элемент:" << endl;
            int val = get_int();

            try
            {
                table[key] = val;
                cout << "Элемент успешно установлен." << endl;
            }
            catch (Hash_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }

            break;
        }

        case 7:
        {
            if (!table.isEmpty())
                cout << "Хеш-таблица содержит элементы." << endl;
            else
                cout << "Хеш-таблица пуста." << endl;
            
            break;
        }

        case 8:
        {
            table.clear();
            cout << "Хеш-таблица успешно очищена." << endl;
            
            break;
        }

        case 9:
        {
            if (table.getSize() + 1 > MAX_SIZE)
            {
                cout << "Ошибка: превышено максимальное допустимое число элементов в хеш-таблице: " << MAX_COUNT << "." << endl;

                break;
            }

            cout << "Введите ключ:" << endl;
            string key = get_word();
            cout << "Введите элемент:" << endl;
            int val = get_int();

            try
            {
                table.append(key, val);
                cout << "Элемент с данным ключом успешно добавлен в хеш-таблицу." << endl;
            }
            catch (Hash_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }

            break;
        }

        case 10:
        {
            cout << "Введите ключ:" << endl;
            string key = get_word();

            try
            {
                table.remove(key);
                cout << "Элемент с заданным ключом успешно удалён из хеш-таблицы." << endl;
            }
            catch (Hash_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }

            break;
        }

        case 11:
        {
            cout << "Структура текущей хеш-таблицы:" << endl;
            if (table.getType() == TableType::SEPARATE_CHAINING)
                cout << "Хеш-таблица с цепочками коллизий:" << endl;
            else
                cout << "Хеш-таблица с открытой адресацией:" << endl;
            table.print();
            
            break;
        }
        case 12:
        {
            HashTable<string, int>::Iterator iter(table);

            cout << "Раздел \"Итератор\":" << endl << endl;

            cout << "Выберите тип создаваемого итератора:" << endl;
            cout << "\t1: Неустановленный." << endl;
            cout << "\t2: Установленный на элемент с определённым ключом." << endl;
            choose = get_int();
            while (choose < 1 || choose > 2)
            {
                cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
                choose = get_int();
            }
            switch (choose)
            {
            case 1: break; // Итератор не установлен на конкретный элемент по умолчанию
            
            case 2:
            {
                cout << "Введите ключ:" << endl;
                string key = get_word();

                iter = HashTable<string, int>::Iterator(table, key);
                
                if (iter == table.end())
                    cout << "Итератор перешёл в состояние \"неустановлен\"." << endl;
                else
                    cout << "Итератор установлен на элемент с заданным ключом." << endl;
                
                break;
            }
            }
            cout << "Итератор успешно создан." << endl << endl;

            bool isExit = false;
            while (!isExit)
            {
                cout << "Меню \"Итератор\":" << endl;
                cout << "\t1: Установить итератор по заданному ключу." << endl;
                cout << "\t2: Получить элемент итератора." << endl;
                cout << "\t3: Инкрементировать итератор." << endl;
                cout << "\t4: Выйти из данного раздела." << endl;

                cout << "Выберите пункт меню:" << endl;
                choose = get_int();
                while (choose < 1 || choose > 4)
                {
                    cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
                    choose = get_int();
                }

                cout << endl;

                switch (choose)
                {
                case 1:
                {
                    cout << "Введите ключ:" << endl;
                    string key = get_word();

                    iter = HashTable<string, int>::Iterator(table, key);
                    
                    if (iter == table.end())
                        cout << "Итератор перешёл в состояние \"неустановлен\"." << endl;
                    else
                        cout << "Итератор установлен на элемент с заданным ключом." << endl;
                    
                    break;
                }

                case 2:
                {
                    try
                    {
                        cout << "Ключ элемента дерева, на который в данный момент установлен итератор: " << *iter << "." << endl;
                    }
                    catch (Hash_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }

                    break;
                }

                case 3:
                {
                    try
                    {
                        iter++;
                        
                        if (iter == table.end())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\"." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (Hash_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }

                    break;
                }

                case 4:
                {
                    cout << "Выходим из раздела \"Итератор\"..." << endl;
                    isExit = true;
                    
                    break;
                }
                }
                cout << endl;
            }

            break;
        }
        case 13:
        {
            const size_t MIN_SIZE = 8;
            const size_t MAX_SIZE = 8192;

            size_t size;

            cout << "Введите размер хеш-таблицы(не меньший " << MIN_SIZE << " и не больший " << MAX_SIZE << "):"<< endl;
            size = get_int();
            while (size < MIN_SIZE || size > MAX_SIZE)
            {
                cout << "Ошибка: введённый размер меньше " << MIN_SIZE << " или превышает " << MAX_SIZE << ". Повторите ввод." << endl;
                size = get_int();
            }

            TestHash::TestHashFunction(size);

            break;
        }
        case 14:
        {
            const size_t MIN_SIZE = 8;
            const size_t MAX_SIZE = 8192;

            size_t size;
            TableType type = TableType::SEPARATE_CHAINING;
            double alpha;

            cout << "Выберите тип хеш-таблицы:" << endl;
            cout << "\t1: Хеш-таблица с цепочками коллизий." << endl;
            cout << "\t2: Хеш-таблица с открытой адресацией." << endl;
            choose = get_int();
            while (choose < 1 || choose > 2)
            {
                cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
                choose = get_int();
            }
            switch (choose)
            {
            case 1: break;  // По умолчанию этот тип хеш-таблицы уже выбран
            case 2: type = TableType::OPEN_ADDRESSING; break;
            }

            cout << "Введите размер хеш-таблицы(не меньший " << MIN_SIZE << " и не больший " << MAX_SIZE << "):" << endl;
            size = get_int();
            while (size < MIN_SIZE || size > MAX_SIZE)
            {
                cout << "Ошибка: введённый размер меньше " << MIN_SIZE << " или превышает " << MAX_SIZE << ". Повторите ввод." << endl;
                size = get_int();
            }

            cout << "Введите коэффициент заполнения хеш-таблицы(коэффициент альфа)." << endl;
            if (type == TableType::SEPARATE_CHAINING)
            {
                cout << "Для хеш-таблицы с цепочками коллизий он должен лежать в следующих пределах: 0.1 <= alpha <= 10." << endl;

                alpha = get_float();
                while (0.1 < alpha && alpha > 10.0)
                {
                    cout << "Ошибка: введённый коэффициент заполнения не лежит в указанных пределах." << endl;
                    alpha = get_float();
                }
            }
            else
            {
                cout << "Для хеш-таблицы с открытой адресацией он должен лежать в следующий пределах: 0.1 <= alpha <= 1." << endl;

                alpha = get_float();
                while (0.1 < alpha && alpha > 1.0)
                {
                    cout << "Ошибка: введённый коэффициент заполнения не лежит в указанных пределах." << endl;
                    alpha = get_float();
                }
            }

            TestHash::TestHashTable(size, type, alpha);

            break;
        }

        case 15:
        {
            cout << "Выходим из программы..." << endl;
            isExit = true;
            
            break;
        }
        }
        cout << endl;
    }

    cout << "Программа успешно завершила свою работу. Нажмите Enter для выхода из программы...";
    cin.get();

    return 0;
}

int get_int()
{
    using std::invalid_argument;
    using std::out_of_range;

    string buf;
    int val;

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

float get_float()
{
    using std::invalid_argument;
    using std::out_of_range;

    string buf;
    float val;

    cout << ">>> ";
    getline(cin, buf);

    while (true)
    {
        try
        {
            val = stof(buf);
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

string get_word()
{
    string result;

    cout << ">>> ";
    cin >> result;
    cin.ignore(32767, '\n');

    while (!is_cyrillic(result))
    {
        cout << "Ошибка: введённое первое слово должно содержать только заглавные буквы кириллицы." << endl;
        cout << ">>> ";
        cin >> result;
        cin.ignore(32767, '\n');
    }

    return result;
}

bool is_cyrillic(const string& str)
{
    for (auto iter : str)
    {
        if (!(static_cast<unsigned char>('А') <= static_cast<unsigned char>(iter)
            && static_cast<unsigned char>(iter) <= static_cast <unsigned char>('Я'))
            )
            return false;
    }
    return true;
}

string get_rand_word(short unsigned int start_len, short unsigned int end_len)
{
    string result;                              // Результирующая строка
    static const short base = 32;               // Число букв в русском алфавите без буквы Ё
    unsigned char let = 0;                      // Случайно генерируемая заглавная буква русского алфавита 
    short len = rand() % end_len + start_len;   // Рандомная генерация длины строки

    if (len > end_len)
        len = end_len;

    for (short i = 0; i < len; i++)
    {
        let = static_cast<unsigned char>('А') + (rand() % base);
        result.push_back(let);
    }

    return result;
}

size_t clp2(size_t num)
{
    size_t result = 1;

    for (; result < num; result <<= 1);

    return result;
}
