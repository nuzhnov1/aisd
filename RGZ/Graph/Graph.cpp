// Graph.cpp


//*************************************************** Подключаемые заголовочные файлы *************************************************************//
#include <Windows.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include "Task1.h"
#include "Task2.h"
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
// Получить строку
string get_string();

//*************************************************************************************************************************************************//


int main()
{
    using Vertex_T = Vertex<string, int>;
    using Edge_T = Edge<Vertex_T, int, int>;
    using Graph_T = Graph<Vertex_T, Edge_T>;
    using V_Iterator = Graph_T::VertexIterator;
    using E_Iterator = Graph_T::EdgeIterator;
    using OE_Iterator = Graph_T::OutEdgeIterator;

    Graph_T graph;                              // Используемый граф
    constexpr unsigned short MAX_VERTICES = 20; // Максимальное число вершин в графе
    int choose = 0;                             // Выбираемый пункт меню

    // Устанавливаем кодировку ввода на windows-1251
    SetConsoleCP(1251);
    // Устанавливаем кодировку вывода на windows-1251
    SetConsoleOutputCP(1251);

    cout << "\t\t\t\t\tПрограмма \"АТД Простой граф\"." << endl << endl;

    cout << "Выберите тип создаваемого графа:" << endl;
    cout << "\t1: Пустой неориентированный L-граф." << endl;
    cout << "\t2: Граф с \"V\" вершинами без рёбер." << endl;
    cout << "\t3: Граф с \"V\" вершинами и \"E\" случайными рёбрами." << endl;
    choose = get_int();
    while (choose < 1 || choose > 3)
    {
        cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
        choose = get_int();
    }

    cout << endl;

    switch (choose)
    {
    case 1: break;  // По умолчанию этот граф уже создан

    case 2: case 3:
    {
        GraphFormType type = GraphFormType::L_GRAPH;
        bool directed = false;
        int vertex_count = 0;
        int edges_count = 0;
        int sub_choose = 0;

        if (choose == 2)
            cout << "Создание графа с \"V\" вершинами без рёбер." << endl;
        else
            cout << "Создание графа с \"V\" вершинами и с \"E\" случайными рёбрами." << endl;

        cout << "Выберите тип формы графа(L-форма или M-форма):" << endl;
        cout << "\t1: L-форма." << endl;
        cout << "\t2: M-форма." << endl;
        sub_choose = get_int();
        while (sub_choose < 1 || sub_choose > 2)
        {
            cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
            sub_choose = get_int();
        }
        if (sub_choose == 1)
            type = GraphFormType::L_GRAPH;
        else
            type = GraphFormType::M_GRAPH;

        cout << "Выберите тип графа(ориентированный или неориентированный):" << endl;
        cout << "\t1: Ориентированный граф." << endl;
        cout << "\t2: Неориентированный граф." << endl;
        sub_choose = get_int();
        while (sub_choose < 1 || sub_choose > 2)
        {
            cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
            sub_choose = get_int();
        }
        if (sub_choose == 1)
            directed = true;
        else
            directed = false;

        cout << "Введите количество вершин \"V\" графа(не больше " << MAX_VERTICES << "):" << endl;
        vertex_count = get_int();
        while (vertex_count < 0 || vertex_count > MAX_VERTICES)
        {
            cout << "Ошибка: введённое количество вершин меньше 0 или больше " << MAX_VERTICES << ". Повторите ввод." << endl;
            vertex_count = get_int();
        }

        if (choose == 3)
        {
            cout << "Введите количество рёбер \"E\" графа." << endl;
            edges_count = get_int();
            while (edges_count < 0)
            {
                cout << "Ошибка: число рёбер не может быть отрицательным. Повторите ввод." << endl;
                edges_count = get_int();
            }
        }

        if (choose == 2)
            graph = Graph_T(vertex_count, directed, type);
        else
            graph = Graph_T(vertex_count, edges_count, directed, type);

        break;
    }
    }

    cout << "Простой граф успешно создан." << endl << endl;

    bool isExit = false;
    while (!isExit)
    {
        cout << "Меню программы \"АТД Простой граф\":" << endl;
        cout << "\t1: Получить число вершин в графе." << endl;
        cout << "\t2: Получить число рёбер в графе." << endl;
        cout << "\t3: Получить тип направленности графа." << endl;
        cout << "\t4: Получить тип формы представления графа." << endl;
        cout << "\t5: Получить коэффициент насыщения графа." << endl;
        cout << "\t6: Преобразовать граф к L-форме." << endl;
        cout << "\t7: Преобразовать граф к M-форме." << endl;
        cout << "\t8: Проверить наличие вершины в графе." << endl;
        cout << "\t9: Проверить наличие ребра в графе." << endl;
        cout << "\t10: Вставить вершину в граф." << endl;
        cout << "\t11: Удалить вершину из графа." << endl;
        cout << "\t12: Вставить ребро в граф." << endl;
        cout << "\t13: Удалить ребро из графа." << endl;
        cout << "\t14: Вывести граф." << endl;
        cout << "\t15: Перейти в раздел управления \"Операции над вершиной графа\"." << endl;
        cout << "\t16: Перейти в раздел управления \"Операции над рёбром графа\"." << endl;
        cout << "\t17: Перейте в раздел управления \"Итератор вершин графа\"." << endl;
        cout << "\t18: Перейти в раздел управления \"Итератор рёбер графа\"." << endl;
        cout << "\t19: Перейти в раздел управления \"Итератор исходящих рёбер вершины графа\"." << endl;
        cout << "\t20: Задача 1. Сформировать список рёбер на основе двухпроходного эйлерова цикла." << endl;
        cout << "\t21: Задача 2. Определить диаметр и путь, соответствующему этому диаметру, на основе алгоритма Флойда." << endl;
        cout << "\t22: Выйти из программы." << endl;

        cout << "Выберите пункт меню:" << endl;
        choose = get_int();
        while (choose < 1 || choose > 22)
        {
            cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
            choose = get_int();
        }

        cout << endl;

        switch (choose)
        {
        case 1:
        {
            cout << "Число вершин в графе: " << graph.getVertexCount() << "." << endl;
            
            break;
        }

        case 2:
        {
            cout << "Число рёбер в графе: " << graph.getEdgesCount() << "." << endl;
            
            break;
        }

        case 3:
        {
            cout << "Тип направленности графа: ";

            if (graph.isDirected())
                cout << "ориентированный граф." << endl;
            else
                cout << "неориентированный граф." << endl;
            
            break;
        }

        case 4:
        {
            cout << "Тип формы представления графа: ";

            if (graph.getFormType() == GraphFormType::L_GRAPH)
                cout << "L-граф." << endl;
            else
                cout << "M-граф." << endl;

            break;
        }

        case 5:
        {
            cout << "Коэффициент насыщения графа: " << graph.getK() << "." << endl;

            break;
        }

        case 6:
        {
            if (graph.getFormType() == GraphFormType::L_GRAPH)
                cout << "Граф уже представлен в L-форме." << endl;
            else
            {
                graph.toListGraph();
                cout << "Граф успешно перестроен." << endl;
            }

            break;
        }

        case 7:
        {
            if (graph.getFormType() == GraphFormType::M_GRAPH)
                cout << "Граф уже представлен в M-форме." << endl;
            else
            {
                graph.toMatrixGraph();
                cout << "Граф успешно перестроен." << endl;
            }

            break;
        }

        case 8:
        {
            Vertex_T vertex;
            string name;

            cout << "Введите имя вершины:" << endl;
            name = get_string();
            vertex.setName(name);

            if (graph.isHasVertex(vertex))
                cout << "Граф содержит вершину с таким именем." << endl;
            else
                cout << "Граф не содержит вершины с таким именем." << endl;

            break;
        }

        case 9:
        {
            Vertex_T outV, inV;
            string name_outV, name_inV;

            cout << "Введите имя вершины исхода:" << endl;
            name_outV = get_string();
            outV.setName(name_outV);

            cout << "Введите имя вершины входа:" << endl;
            name_inV = get_string();
            inV.setName(name_inV);

            try
            {
                if (graph.isHasEdge(outV, inV))
                    cout << "Граф содержит ребро с такими вершинами." << endl;
                else
                    cout << "Граф не содержит ребро с такими вершинами." << endl;
            }
            catch (Graph_exception& exception) 
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }

            break;
        }

        case 10:
        {
            Vertex_T vertex;
            int choose = 0;

            if (graph.getVertexCount() >= MAX_VERTICES)
            {
                cout << "Ошибка: достигнут лимит на количество вершин в графе, равный "
                    << MAX_VERTICES << " вершин." << endl;

                break;
            }

            cout << "Выберите тип задания вершины:" << endl;
            cout << "\t1: Задание имени вершины." << endl;
            cout << "\t2: Задание имени и данных вершины." << endl;
            choose = get_int();
            while (choose < 1 || choose > 2)
            {
                cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
                choose = get_int();
            }

            cout << endl;

            switch (choose)
            {
            case 1: case 2:
            {
                string name;

                cout << "Введите имя вершины:" << endl;
                name = get_string();
                vertex.setName(name);

                if (choose == 2)
                {
                    int data = 0;

                    cout << "Введите данные вершины:" << endl;
                    data = get_int();
                    vertex.setData(data);
                }

                try
                {
                    vertex = graph.insertVertex(vertex);
                    cout << "Вершина была успешно вставлена в граф." << endl;
                }
                catch (Graph_exception& exception)
                {
                    cout << "Ошибка: " << exception.what() << "." << endl;
                }

                break;
            }
            }

            break;
        }

        case 11:
        {
            Vertex_T vertex;
            string name;

            cout << "Введите имя удаляемой вершины." << endl;

            name = get_string();
            vertex.setName(name);

            try
            {
                graph.deleteVertex(vertex);
                cout << "Вершина с заданным именем успешно удалена из графа." << endl;
            }
            catch (Graph_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }

            break;
        }

        case 12:
        {
            Vertex_T outV, inV;
            string outV_name, inV_name;
            int choose = 0;

            cout << "Выберите тип задания ребра:" << endl;
            cout << "\t1: Задание ребра без установления веса и данных." << endl;
            cout << "\t2: Задание ребра с установлением веса." << endl;
            cout << "\t3: Задание ребра с установлением веса и данных." << endl;
            choose = get_int();
            while (choose < 1 || choose > 3)
            {
                cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
                choose = get_int();
            }

            cout << endl;

            cout << "Введите имя вершины исхода:" << endl;
            outV_name = get_string();
            outV.setName(outV_name);

            cout << "Введите имя вершины входа:" << endl;
            inV_name = get_string();
            inV.setName(inV_name);

            switch (choose)
            {
            case 1:
            {
                try
                {
                    graph.insertEdge(outV, inV);
                    cout << "Ребро с заданными вершинами исхода и входа успешно вставлено в граф." << endl;
                }
                catch (Graph_exception& exception)
                {
                    cout << "Ошибка: " << exception.what() << "." << endl;
                }

                break;
            }

            case 2: case 3:
            {
                int weight = 0;

                cout << "Введите вес ребра:" << endl;
                weight = get_int();

                if (choose == 2)
                {
                    try
                    {
                        Edge_T tmp(outV, inV, weight);
                        const Edge_T& edge = graph.insertEdge(outV, inV);

                        graph.setEdge(edge, tmp);

                        cout << "Ребро с заданными параметрами успешно вставлено в граф." << endl;
                    }
                    catch (Graph_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }
                }
                else
                {
                    int data = 0;

                    cout << "Введите данные ребра:" << endl;
                    data = get_int();

                    try
                    {
                        Edge_T tmp(outV, inV, weight, data);
                        const Edge_T& edge = graph.insertEdge(outV, inV);

                        graph.setEdge(edge, tmp);

                        cout << "Ребро с заданными параметрами успешно вставлено в граф." << endl;
                    }
                    catch (Graph_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }
                }

                break;
            }
            }

            break;
        }

        case 13:
        {
            Vertex_T outV, inV;
            string outV_name, inV_name;

            cout << "Введите имя вершины исхода:" << endl;
            outV_name = get_string();
            outV.setName(outV_name);

            cout << "Введите имя вершины входа:" << endl;
            inV_name = get_string();
            inV.setName(inV_name);

            try
            {
                graph.deleteEdge(outV, inV);
                cout << "Ребро с заданными вершинами исхода и входа успешно удалено из графа." << endl;
            }
            catch (Graph_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }

            break;
        }

        case 14:
        {
            cout << "Выведенный граф:" << endl;

            graph.print();

            break;
        }

        case 15:
        {
            int choose = 0;
            bool isExit = false;

            while (!isExit)
            {
                cout << "Меню \"Операции над вершиной графа\"." << endl << endl;

                cout << "\t1: Прочитать данные существующей вершины." << endl;
                cout << "\t2: Изменить поле имени сушествующей вершины." << endl;
                cout << "\t3: Изменить поле данных существующей вершины." << endl;
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
                    Vertex_T vertex;
                    string name;

                    cout << "Введите имя существующей вершины:" << endl;
                    name = get_string();
                    vertex.setName(name);

                    try
                    {
                        vertex = graph.getVertex(vertex);
                    }
                    catch (Graph_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                        
                        break;
                    }

                    cout << "Данные вершины:" << endl;
                    
                    cout << "Поле имени вершины: " << vertex.getName() << "." << endl;
                   
                    cout << "Поле данных вершины: "; 
                    try { cout << vertex.getData() << "." << endl; }
                    catch (Graph_exception&) { cout << "?." << endl; }

                    break;
                }

                case 2: case 3:
                {
                    Vertex_T vertex;
                    string old_name;

                    cout << "Введите имя существующей вершины:" << endl;
                    old_name = get_string();
                    try
                    {
                        vertex = graph.getVertex(Vertex_T(old_name, 1));
                    }
                    catch (Graph_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                        
                        break;
                    }

                    if (choose == 3)
                    {
                        int data = 0;

                        cout << "Введите новые данные вершины:" << endl;
                        data = get_int();
                        vertex.setData(data);
                    }
                    else
                    {
                        string new_name;

                        cout << "Введите новое имя вершины:" << endl;
                        new_name = get_string();
                        vertex.setName(new_name);
                    }

                    try
                    {
                        graph.setVertex(Vertex_T(old_name, 1), vertex);
                        cout << "Данные вершины успешно изменены." << endl;
                    }
                    catch (Graph_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }

                    break;
                }

                case 4:
                {
                    cout << "Выходим из раздела \"Операции над вершиной графа\"." << endl;
                    isExit = true;

                    break;
                }
                }

                cout << endl;
            }

            break;
        }

        case 16:
        {
            int choose = 0;
            bool isExit = false;

            while (!isExit)
            {
                cout << "Меню \"Операции над ребром графа\"." << endl << endl;

                cout << "\t1: Прочитать данные существующего ребра." << endl;
                cout << "\t2: Изменить поле веса сушествующего ребра." << endl;
                cout << "\t3: Изменить поле данных существующего ребра." << endl;
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
                    Vertex_T outV, inV;
                    Edge_T edge(outV, inV);
                    string outV_name, inV_name;

                    cout << "Введите имя вершины исхода:" << endl;
                    outV_name = get_string();
                    outV.setName(outV_name);

                    cout << "Введите имя вершины входа:" << endl;
                    inV_name = get_string();
                    inV.setName(inV_name);

                    try
                    {
                        edge = graph.getEdge(outV, inV);
                    }
                    catch (Graph_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;

                        break;
                    }

                    cout << "Данные ребра:" << endl;
                    
                    cout << "Поле веса ребра: "; 
                    try { cout << edge.getWeight() << "." << endl; }
                    catch (Graph_exception&) { cout << "?." << endl; }
                    
                    cout << "Поле данных вершины: "; 
                    try { cout << edge.getData() << "." << endl; }
                    catch (Graph_exception&) { cout << "?." << endl; }

                    break;
                }

                case 2: case 3:
                {
                    Vertex_T outV, inV;
                    Edge_T edge(outV, inV);
                    string outV_name, inV_name;

                    cout << "Введите имя вершины исхода:" << endl;
                    outV_name = get_string();
                    outV.setName(outV_name);

                    cout << "Введите имя вершины входа:" << endl;
                    inV_name = get_string();
                    inV.setName(inV_name);

                    try
                    {
                        edge = graph.getEdge(outV, inV);
                    }
                    catch (Graph_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;

                        break;
                    }

                    if (choose == 3)
                    {
                        int data = 0;

                        cout << "Введите новые данные ребра:" << endl;
                        data = get_int();
                        edge.setData(data);
                    }
                    else
                    {
                        int weight = 0;

                        cout << "Введите новый вес ребра:" << endl;
                        weight = get_int();
                        edge.setWeight(weight);
                    }

                    try
                    {
                        graph.setEdge(Edge_T(outV, inV), edge);
                        cout << "Данные ребра успешно изменены." << endl;
                    }
                    catch (Graph_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }

                    break;
                }

                case 4:
                {
                    cout << "Выходим из раздела \"Операции над ребром графа\"." << endl;
                    isExit = true;

                    break;
                }
                }

                cout << endl;
            }

            break;
        }

        case 17:
        {
            V_Iterator iter(graph, false);
            
            int choose = 0;
            bool isExit = false;

            cout << "Итератор вершин успешно создан. Внимание изначально итератор находится в неустановленном состоянии!" << endl << endl;

            while (!isExit)
            {
                cout << "Меню \"Итератор вершин графа\"." << endl << endl;

                cout << "\t1: Установить итератор в начало." << endl;
                cout << "\t2: Получить всю информацию о текущей вершине." << endl;
                cout << "\t3: Перейти к следующей вершине." << endl;
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
                    iter = V_Iterator::begin(graph);

                    if (iter == V_Iterator::end(graph))
                        cout << "Ошибка: в графе нет вершин. Итератор перешёл в состояние \"неустановлен\"" << endl;
                    else
                        cout << "Итератор успешно установлен на начальную вершину." << endl;

                    break;
                }

                case 2:
                {
                    Vertex_T vertex;

                    try
                    {
                        vertex = *iter;

                        cout << "Данные текущей вершины:" << endl;
                        cout << "Поле имени: " << vertex.getName() << "." << endl;
                        cout << "Поле данных: ";
                        try { cout << vertex.getData() << "." << endl; }
                        catch (Graph_exception&) { cout << "?." << endl; }
                    }
                    catch (Graph_exception& exception)
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

                        if (iter == V_Iterator::end(graph))
                            cout << "Внимание итератор перешёл в \"неустановленное\" состояние." << endl;
                        else
                            cout << "Итератор успешно перешёл на следующую вершину." << endl;
                    }
                    catch (Graph_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }

                    break;
                }

                case 4:
                {
                    cout << "Выходим из раздела \"Итератор вершин графа\"." << endl;
                    isExit = true;

                    break;
                }
                }

                cout << endl;
            }

            break;
        }

        case 18:
        {
            E_Iterator iter(graph, false);

            int choose = 0;
            bool isExit = false;

            cout << "Итератор рёбер успешно создан. Внимание изначально итератор находится в неустановленном состоянии!" << endl << endl;

            while (!isExit)
            {
                cout << "Меню \"Итератор рёбер графа\"." << endl << endl;

                cout << "\t1: Установить итератор в начало." << endl;
                cout << "\t2: Получить всю информацию о текущем ребре." << endl;
                cout << "\t3: Перейти к следующему ребру." << endl;
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
                    iter = E_Iterator::begin(graph);

                    if (iter == E_Iterator::end(graph))
                        cout << "Ошибка: в графе нет рёбер. Итератор перешёл в состояние \"неустановлен\"" << endl;
                    else
                        cout << "Итератор успешно установлен на начальное ребро." << endl;

                    break;
                }

                case 2:
                {
                    try
                    {
                        Edge_T edge = *iter;

                        cout << "Данные о текущем ребре:" << endl;
                        cout << "Имя вершины исхода: " << edge.getVertexOut().getName() << "." << endl;
                        cout << "Имя вершины входа: " << edge.getVertexIn().getName() << "." << endl;

                        cout << "Вес ребра: ";
                        try { cout << edge.getWeight() << "." << endl; }
                        catch (Graph_exception&) { cout << "?." << endl; }

                        cout << "Данные ребра: ";
                        try { cout << edge.getData() << "." << endl; }
                        catch (Graph_exception&) { cout << "?." << endl; }
                    }
                    catch (Graph_exception& exception)
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

                        if (iter == E_Iterator::end(graph))
                            cout << "Внимание итератор перешёл в \"неустановленное\" состояние." << endl;
                        else
                            cout << "Итератор успешно перешёл на следующее ребро." << endl;
                    }
                    catch (Graph_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }

                    break;
                }

                case 4:
                {
                    cout << "Выходим из раздела \"Итератор рёбер графа\"." << endl;
                    isExit = true;

                    break;
                }
                }

                cout << endl;
            }

            break;
        }

        case 19:
        {
            OE_Iterator iter;
            Vertex_T vertex;
            string name;
            int choose = 0;
            bool isExit = false;

            cout << "Введите имя вершины, на которую необходимо установить итератор исходящих рёбер:" << endl;
            name = get_string();
            vertex.setName(name);

            if (!graph.isHasVertex(vertex))
            {
                cout << "Ошибка: вершины с данным именем в графе нет." << endl;
                
                break;
            }
            else
            {
                iter = OE_Iterator::end(graph, vertex);

                cout << "Итератор исходящих рёбер для данной вершины создан. Внимание изначально итератор находится в неустановленном состоянии!" << endl << endl;
            }

            while (!isExit)
            {
                cout << "Меню \"Итератор исходящих рёбер вершины графа\"." << endl << endl;

                cout << "\t1: Установить итератор в начало." << endl;
                cout << "\t2: Получить всю информацию о текущем ребре." << endl;
                cout << "\t3: Перейти к следующему ребру." << endl;
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
                    iter = OE_Iterator::begin(graph, vertex);

                    if (iter == OE_Iterator::end(graph, vertex))
                        cout << "Ошибка: в графе на данной вершине нет исходящих рёбер. Итератор перешёл в состояние \"неустановлен\"" << endl;
                    else
                        cout << "Итератор успешно установлен на начальное ребро." << endl;

                    break;
                }

                case 2:
                {
                    try
                    {
                        Edge_T edge = *iter;

                        cout << "Данные о текущем ребре:" << endl;
                        cout << "Имя вершины исхода: " << edge.getVertexOut().getName() << "." << endl;
                        cout << "Имя вершины входа: " << edge.getVertexIn().getName() << "." << endl;

                        cout << "Вес ребра: ";
                        try { cout << edge.getWeight() << "." << endl; }
                        catch (Graph_exception&) { cout << "?." << endl; }

                        cout << "Данные ребра: ";
                        try { cout << edge.getData() << "." << endl; }
                        catch (Graph_exception&) { cout << "?." << endl; }
                    }
                    catch (Graph_exception& exception)
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

                        if (iter == OE_Iterator(graph, vertex))
                            cout << "Внимание итератор перешёл в \"неустановленное\" состояние." << endl;
                        else
                            cout << "Итератор успешно перешёл на следующее ребро." << endl;
                    }
                    catch (Graph_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }

                    break;
                }

                case 4:
                {
                    cout << "Выходим из раздела \"Итератор исходящих рёбер вершины графа\"." << endl;
                    isExit = true;

                    break;
                }
                }

                cout << endl;
            }

            break;
        }

        case 20:
        {
            cout << "Результат решения задачи для данного графа:" << endl;
            
            try
            {
                Task1 task(graph);
                task.restart();
                cout << task.result() << endl;
            }
            catch (Graph_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }

            break;
        }

        case 21:
        {
            cout << "Результат решения задачи для данного графа:" << endl;
            
            try
            {
                Task2 task(graph);
                task.restart();
                cout << task.result() << endl;
            }
            catch (Graph_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }

            break;
        }

        case 22:
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
        catch (std::invalid_argument)
        {
            cout << "Ошибка: неверный формат входных данных." << endl;
        }
        catch (std::out_of_range)
        {
            cout << "Ошибка: введённое число вышло из диапазона int." << endl;
        }

        cout << ">>> ";
        getline(cin, buf);
    }

    return val;
}

string get_string()
{
    string result;

    cout << ">>> ";
    getline(cin, result);

    return result;
}
