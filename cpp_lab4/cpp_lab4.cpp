#include <iostream>
#include <unordered_map>
#include <cmath>
#include <iterator>
#include <functional>

template<typename T>
class SparseVector {
private:
    std::unordered_map<size_t, T> data;
    size_t size;

public:
    SparseVector(size_t size) : size(size) {}

    T& operator[](size_t index) {
        return data[index];
    }

    size_t getSize() const {
        return size;
    }

    // Функция для применения переданной функции к каждому элементу матрицы
    void applyFunction(const std::function<T(T)>& func) {
        for (auto& [key, value] : data) {
            value = func(value);
        }
    }

    SparseVector<T> operator+(const SparseVector<T>& other) const {
        SparseVector<T> result(size);
        for (const auto& [index, value] : data) {
            result[index] = value + other.data.at(index);
        }
        for (const auto& [index, value] : other.data) {
            if (data.find(index) == data.end()) {
                result[index] = value;
            }
        }
        return result;
    }

    T dot(const SparseVector<T>& other) const {
        T result = 0;
        for (const auto& [index, value] : data) {
            if (other.data.find(index) != other.data.end()) {
                result += value * other.data.at(index);
            }
        }
        return result;
    }

    // Унарная операция: например, нормализация
    SparseVector<T> normalize() const {
        T norm = std::sqrt(this->dot(*this));
        SparseVector<T> result(size);
        for (const auto& [index, value] : data) {
            result[index] = value / norm;
        }
        return result;
    }

};

template<typename T>
std::ostream& operator<<(std::ostream& os, const SparseVector<T>& vec) {
    for (const auto& [index, value] : vec.data) {
        os << "Index: " << index << ", Value: " << value << "\n";
    }
    return os;
}


template<typename T>
class SparseMatrix {

    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, const SparseMatrix<T>& mat);

private:
    std::unordered_map<std::pair<size_t, size_t>, T, std::hash<std::pair<size_t, size_t>>> data;
    size_t rows, cols;

public:
    SparseMatrix(size_t rows, size_t cols) : rows(rows), cols(cols) {}

    T& operator()(size_t row, size_t col) {
        return data[{row, col}];
    }

    // Оператор сравнения
    bool operator==(const SparseMatrix<T>& other) const {
        if (rows != other.rows || cols != other.cols) {
            return false; // Разные размеры
        }

        // Сравниваем элементы
        for (const auto& [key, value] : data) {
            if (other.data.find(key) == other.data.end() || other.data.at(key) != value) {
                return false; // Элемент не найден или значения не равны
            }
        }

        // Проверяем, что все элементы другой матрицы равны нулю
        for (const auto& [key, value] : other.data) {
            if (data.find(key) == data.end()) {
                if (value != 0) {
                    return false; // Элемент не найден в текущей матрице, но в другой он не равен нулю
                }
            }
        }

        return true; // Матрицы равны
    }

    // Оператор присваивания
    SparseMatrix<T>& operator=(const SparseMatrix<T>& other) {
        if (this != &other) { // Проверка на самоприсваивание
            rows = other.rows;
            cols = other.cols;
            data = other.data; // Копируем данные
        }
        return *this; // Возвращаем ссылку на текущий объект
    }

    SparseMatrix<T> operator+(const SparseMatrix<T>& other) const {
        SparseMatrix<T> result(rows, cols);
        for (const auto& [key, value] : data) {
            result(key.first, key.second) = value + other.data.at(key);
        }
        for (const auto& [key, value] : other.data) {
            if (data.find(key) == data.end()) {
                result(key.first, key.second) = value;
            }
        }
        return result;
    }

    // Оператор вычитания
    SparseMatrix<T> operator-(const SparseMatrix<T>& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrices must have the same dimensions for subtraction.");
        }

        SparseMatrix<T> result(rows, cols);

        // Вычитаем элементы
        for (const auto& [key, value] : data) {
            result(key.first, key.second) = value - other.data.at(key);
        }

        // Обрабатываем элементы, которые есть только в другой матрице
        for (const auto& [key, value] : other.data) {
            if (data.find(key) == data.end()) {
                result(key.first, key.second) = -value; // Если элемента нет в текущей матрице, считаем его равным нулю
            }
        }

        return result;
    }

    // Перегрузка оператора деления для матрицы на скаляр
    SparseMatrix<T> operator/(T scalar) const {
        if (scalar == 0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }

        SparseMatrix<T> result(rows, cols);
        for (const auto& [key, value] : data) {
            result(key.first, key.second) = value / scalar;
        }
        return result;
    }

    // Перегрузка оператора деления для матрицы на скаляр
    SparseMatrix<T> operator*(T scalar) const {
        if (scalar == 0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }

        SparseMatrix<T> result(rows, cols);
        for (const auto& [key, value] : data) {
            result(key.first, key.second) = value * scalar;
        }
        return result;
    }

    // Функция для применения переданной функции к каждому элементу матрицы
    void applyFunction(const std::function<T(T)>& func) {
        for (auto& [key, value] : data) {
            value = func(value);
        }
    }

    SparseMatrix<T> transpose() const {
        SparseMatrix<T> result(cols, rows);
        for (const auto& [key, value] : data) {
            result(key.second, key.first) = value;
        }
        return result;
    }

    SparseVector<T> operator*(const SparseVector<T>& vec) const {
        SparseVector<T> result(rows);
        for (const auto& [key, value] : data) {
            result[key.first] += value * vec[key.second];
        }
        return result;
    }

    // Перегрузка оператора умножения для матриц
    SparseMatrix<T> operator*(const SparseMatrix<T>& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Matrix dimensions must agree for multiplication.");
        }

        SparseMatrix<T> result(rows, other.cols);

        for (const auto& [key1, value1] : data) {
            size_t row1 = key1.first;
            size_t col1 = key1.second;

            for (const auto& [key2, value2] : other.data) {
                size_t row2 = key2.first;
                size_t col2 = key2.second;

                if (col1 == row2) {
                    result(row1, col2) += value1 * value2;
                }
            }
        }

        return result;
    }

    SparseMatrix<T> power_int(int exponent) const {
        if (rows != cols) {
            throw std::invalid_argument("Matrix must be square for exponentiation.");
        }
        SparseMatrix<T> result(rows, cols);
        // Инициализация единичной матрицы
        for (size_t i = 0; i < rows; ++i) {
            result(i, i) = 1;
        }
        SparseMatrix<T> base = *this;
        while (exponent > 0) {
            if (exponent % 2 == 1) {
                result = result * base;
            }
            base = base * base;
            exponent /= 2;
        }
        return result;
    }

    SparseMatrix<T> inverse() {
        if (rows != cols) {
            throw std::invalid_argument("Matrix must be square for inversion.");
        }

        SparseMatrix<T> result(rows, cols);
        // Инициализация единичной матрицы
        for (size_t i = 0; i < rows; ++i) {
            result(i, i) = 1;
        }

        SparseMatrix<T> augmented(rows, cols * 2);
        // Создание расширенной матрицы
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                augmented(i, j) = (*this)(i, j);
                if (j == i) {
                    augmented(i, j + cols) = 1; // Добавление единичной матрицы
                }
                else {
                    augmented(i, j + cols) = 0;
                }
            }
        }

        // Прямой ход Гаусса
        for (size_t i = 0; i < rows; ++i) {
            // Поиск максимального элемента в столбце
            size_t maxRow = i;
            for (size_t k = i + 1; k < rows; ++k) {
                if (std::abs(augmented(k, i)) > std::abs(augmented(maxRow, i))) {
                    maxRow = k;
                }
            }
            // Обмен текущей строки с максимальной
            for (size_t k = 0; k < cols * 2; ++k) {
                std::swap(augmented(i, k), augmented(maxRow, k));
            }

            // Приведение к верхнетреугольному виду
            for (size_t k = i + 1; k < rows; ++k) {
                T factor = augmented(k, i) / augmented(i, i);
                for (size_t j = i; j < cols * 2; ++j) {
                    augmented(k, j) -= factor * augmented(i, j);
                }
            }
        }

        // Обратный ход
        for (size_t i = rows; i-- > 0;) {
            T divisor = augmented(i, i);
            if (divisor == 0) {
                throw std::runtime_error("Matrix is singular and cannot be inverted.");
            }
            for (size_t j = 0; j < cols * 2; ++j) {
                augmented(i, j) /= divisor;
            }
            for (size_t k = 0; k < i; ++k) {
                T factor = augmented(k, i);
                for (size_t j = 0; j < cols * 2; ++j) {
                    augmented(k, j) -= factor * augmented(i, j);
                }
            }
        }

        // Извлечение обратной матрицы
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result(i, j) = augmented(i, j + cols);
            }
        }

        return result;
    }   


    SparseMatrix<T> power(double p, int approxOrder = 100)  {
        if (rows != cols) {
            throw std::invalid_argument("Matrix must be square to compute power.");
        }

        SparseMatrix<T> logA = log(approxOrder);
        SparseMatrix<T> pLogA = logA * p;

        return pLogA.exp(approxOrder);
    }

    SparseMatrix<T> log(int approxOrder = 100)  {
        if (rows != cols) {
            throw std::invalid_argument("Matrix must be square to compute the logarithm.");
        }

        SparseMatrix<T> I = SparseMatrix<T>(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            I(i, i) = 1; // Создание единичной матрицы
        }

        if (*this == I) {
            return SparseMatrix<T>(rows, cols); // Возвращаем нулевую матрицу
        }

        if (frobeniusNorm(*this) >= 1) {
            throw std::invalid_argument("Frobenius matrix norm must be less than 1.");
        }

        SparseMatrix<T> A_minus_I = *this - I;
        SparseMatrix<T> result(rows, cols);
        SparseMatrix<T> term = A_minus_I;

        for (int n = 1; n <= approxOrder; ++n) {
            if (n > 1) {
                term = term * A_minus_I;
            }
            if (n % 2 == 0) {
                result = result - term / static_cast<T>(n);
            }
            else {
                result = result + term / static_cast<T>(n);
            }
        }

        return result;
    }

    double frobeniusNorm(const SparseMatrix<T>& matrix)  {
        double norm = 0.0;
        for (const auto& [key, value] : matrix.data) {
            norm += std::pow(std::abs(value), 2);
        }
        return std::sqrt(norm);
    }

    SparseMatrix<T> exp(int approxOrder = 100)  {
        if (rows != cols) {
            throw std::invalid_argument("Matrix must be square to compute the exponential.");
        }

        SparseMatrix<T> result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            result(i, i) = 1; // Создание единичной матрицы
        }

        SparseMatrix<T> term = *this;
        result = result + term;

        for (size_t n = 2; n < approxOrder; ++n) {
            term = term * *this;
            term = term / static_cast<T>(n);
            result = result + term;
        }

        return result;
    }

};

// Хеш-функция для пар
namespace std {
    template <>
    struct hash<std::pair<size_t, size_t>> {
        size_t operator()(const std::pair<size_t, size_t>& p) const {
            return hash<size_t>()(p.first) ^ hash<size_t>()(p.second);
        }
    };
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const SparseMatrix<T>& mat) {
    for (const auto& [key, value] : mat.data) {
        os << "Row: " << key.first << ", Col: " << key.second << ", Value: " << value << "\n";
    }
    return os;
}


int main()
{
    SparseMatrix<double> mat(4, 4);
    mat(0, 0) = 1;
    mat(0, 1) = 2;
    mat(1, 0) = 3;
    mat(3, 3) = 4;
    mat(3, 0) = 1;
    mat(3, 1) = 2;
    mat(2, 2) = 3;

    std::cout << "Matrix:\n" << mat << "\n";

    SparseMatrix<double> result = mat.power_int(2);
    std::cout << "Matrix raised to the power of 2:\n" << result << "\n";

    std::cout << "Transpose Mat:\n" << mat.transpose() << "\n";

    std::cout << "Mult Mat:\n" << mat * mat << "\n";

    std::cout << "Inverse Mat:\n" << mat.inverse() << "\n";

    //std::cout << "Matrix raised to the power of 2:\n" << mat.power(2.0) << "\n";

    return 0;
}
