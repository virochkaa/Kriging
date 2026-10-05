# Surrogates — пункт 1: X и y

Проект содержит только два класса и `main.cpp`. Обработки исключений и проверки входных данных нет: предполагается, что размерность равна 1, 2 или 3, а вход содержит корректные числа.

* `include/Dataset.h` — хранение `Matrix X`, `Vector y`, получение точки, координаты или отклика по индексу, доступ к полной `X` и `y`.
* `include/TextReader.h` — преобразование потока или строки текста в `Dataset`, с ветвлением по размерности.
* `src/main.cpp` — пример с 10 000 точками Куэтта из `data/couette_10000.txt`.

Входной текст должен содержать повторяющиеся группы из `d` координат и одного отклика:
* `d=1`: `x1 response`
* `d=2`: `x1 x2 response`
* `d=3`: `x1 x2 x3 response`

Разделители: пробелы, табуляция, переводы строки, `,`, `;`. Десятичный разделитель — точка. Группы могут быть на отдельных строках или в одной строке. Если данные придут в ином порядке или с метаданными, достаточно будет изменить `TextReader`, не меняя `Dataset`.

Пример чтения уже имеющейся текстовой строки:

```cpp
Dataset data = TextReader::read("0 0 0.0\n0.01 0 0.0\n0 0.01 0.001", 2);
const Matrix& X = data.getX();
const Vector& y = data.getY();
const Vector& point = data.getPoint(1);
double yCoord = data.getCoordinate(1, 1);
double response = data.getResponse(1);
```

Если нужен *отдельный изменяемый экземпляр* `X` или `y`, а не ссылка на данные объекта:

```cpp
Matrix X = data.getX();
Vector y = data.getY();
```

Можно также создавать набор из уже подготовленных контейнеров:

```cpp
Dataset data(2, {{0,0}, {0.01,0}}, {0,0});
```

Для исходного CSV с заголовком также подойдет этот же ридер:

```cpp
std::ifstream input("data/couette_10000.csv");
Dataset data = TextReader::read(input, 2, true); // skipHeader = true
```

Сборка из корневой папки проекта:

```bash
g++ -std=c++17 -Iinclude src/main.cpp -o surrogates
./surrogates
```
