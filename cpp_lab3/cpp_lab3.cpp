// cpp_lab3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <unordered_set>

template <typename T>
class BinaryTree {
private:
	struct Node {
		T data;
		Node* left;
		Node* right;
		int size = 1;

		Node(T value) : data(value), left(nullptr), right(nullptr) {}
	};

	Node* root;
	bool debug = false;

	void insert(Node*& node, T value) {
		if (node == nullptr) {
			node = new Node(value);
		}
		else if (value < node->data) {
			insert(node->left, value);
			node->size++;
		}
		else {
			insert(node->right, value);
			node->size++;
		}
	}

	bool search(Node* node, T value) {
		if (node == nullptr) {
			return false;
		}
		if (node->data == value) {
			return true;
		}
		else if (value < node->data) {
			return search(node->left, value);
		}
		else {
			return search(node->right, value);
		}
	}

	void inOrder(Node* node) {
		if (node != nullptr) {
			inOrder(node->left);
			std::cout << node->data << " ";
			inOrder(node->right);
		}
	}

	void clear(Node* node) {
		if (node != nullptr) {
			clear(node->left);
			clear(node->right);
			delete node;
		}
	}

	Node* copy(Node* node) {
		if (node == nullptr) {
			return nullptr;
		}
		Node* newNode = new Node(node->data);
		newNode->left = copy(node->left);
		newNode->right = copy(node->right);
		return newNode;
	}

public:
	BinaryTree() : root(nullptr) {}

	bool operator<(const BinaryTree<T>& another) const {
		return (this->root ? this->root->size : 0) < (another.root ? another.root->size : 0);
	}

	bool operator>(const BinaryTree<T>& another) const {
		return (this->root ? this->root->size : 0) > (another.root ? another.root->size : 0);
	}

	bool operator==(const BinaryTree<T>& another) const {
		return (this->root ? this->root->size : 0) == (another.root ? another.root->size : 0);
	}

	int getSize() const
	{
		return (root ? root->size : 0);
	}

	BinaryTree(const BinaryTree& other) {
		if (debug)
			std::cout << "Copy Constructor" << std::endl;
		root = copy(other.root);
	}

	BinaryTree(BinaryTree&& other) noexcept : root(other.root) {
		if (debug)
			std::cout << "Move Constructor" << std::endl;
		other.root = nullptr;
	}

	BinaryTree& operator=(const BinaryTree& other) {
		if (debug)
			std::cout << "Assignment Operator" << std::endl;
		if (this != &other) {
			clear(root);
			root = copy(other.root);
		}
		return *this;
	}

	BinaryTree& operator=(BinaryTree&& other) noexcept {
		if (debug)
			std::cout << "Move Assignment Operator" << std::endl;
		if (this != &other) {
			clear(root);
			root = other.root;
			other.root = nullptr;
		}
		return *this;
	}

	void insert(T value) {
		if (debug)
			std::cout << "insert" << std::endl;
		insert(root, value);
	}

	bool search(T value) {
		if (debug)
			std::cout << "search" << std::endl;
		return search(root, value);
	}

	void inOrder() {
		if (debug)
			std::cout << "inOrder" << std::endl;
		inOrder(root);
		std::cout << std::endl;
	}

	void inOrder(Node* node, std::vector<T>& values) const {
		if (node != nullptr) {
			inOrder(node->left, values);
			values.push_back(node->data);
			inOrder(node->right, values);
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const BinaryTree& tree) {
		tree.print(os, tree.root);
		return os;
	}

	void print(std::ostream& os, Node* node) const {
		if (node != nullptr) {
			print(os, node->left);
			os << node->data << " ";
			print(os, node->right);
		}
	}

	std::vector<T> getValues() const {
		std::vector<T> values;
		inOrder(root, values);
		return values;
	}

	void setDebug(bool debug)
	{
		this->debug = debug;
	}

	~BinaryTree() {
		if (debug)
			std::cout << "Destructor" << std::endl;

		clear(root);
	}
};

// Специализация std::hash для BinaryTree
namespace std {
	template <>
	struct hash<BinaryTree<int>> {
		size_t operator()(const BinaryTree<int>& tree) const {
			size_t hashValue = 0;
			std::vector<int> values = tree.getValues(); // Получаем значения дерева

			for (const auto& value : values) {
				hashValue ^= std::hash<int>()(value) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
			}

			return hashValue;
		}
	};
}

int main()
{
	// Инициализация генератора случайных чисел
	srand(static_cast<unsigned int>(time(0)));

	// Генерация случайного размера вектора от 500 до 1000
	int size = rand() % 501 + 500; // Случайное число от 500 до 1000
	std::vector<BinaryTree<int>> v1(size);

	// Заполнение вектора случайными значениями
	for (int i = 0; i < size; ++i) {
		BinaryTree<int> tree;
		tree.setDebug(false);

		int size_tree = rand() % 11 + 2; // Случайное число от 2 до 10
		for (int j = 0; j < size_tree; ++j) { // Заполняем каждое дерево 10 случайными значениями
			tree.insert(rand() % 100); // Случайное значение от 0 до 99
		}
		v1[i] = std::move(tree); // Перемещение дерева в вектор
	}

	// Определение позиций b и e
	int b = size - 200; // Начальная позиция (последние 200 элементов)
	int e = size - 1;   // Конечная позиция

	// Проверка на допустимость значений b и e
	if (b < 0) {
		std::cout << "b не может быть меньше 0. Устанавливаем b в 0." << std::endl;
		b = 0;
	}
	if (e >= size) {
		std::cout << "e не может быть больше или равным размеру вектора. Устанавливаем e в " << size - 1 << "." << std::endl;
		e = size - 1;
	}

	// Создание вектора v2 и копирование элементов из v1
	std::vector<BinaryTree<int>> v2(v1.begin() + b, v1.begin() + e + 1);

	// Определение n в диапазоне от 20 до 50
	int n1 = rand() % 31 + 20; // Случайное число от 20 до 50

	// Сортировка вектора v1 по размеру деревьев
	std::sort(v1.begin(), v1.end(), [](const BinaryTree<int>& a, const BinaryTree<int>& b) {
		return a > b; // Сравнение по размеру
		});

	// Создание списка list1 и добавление первых n наибольших деревьев
	std::list<BinaryTree<int>> list1;
	for (int i = 0; i < n1 && i < v1.size(); ++i) {
		list1.push_back(std::move(v1[i])); // Перемещение деревьев в список
	}

	int n2 = rand() % 31 + 20; // Случайное число от 20 до 50

	// Использование partial_sort для нахождения n наименьших деревьев
	std::partial_sort(v2.begin(), v2.begin() + n2, v2.end(), [](const BinaryTree<int>& a, const BinaryTree<int>& b) {
		return a < b; // Сравнение по размеру
		});

	// Создание списка list2 и добавление первых n наименьших деревьев
	std::list<BinaryTree<int>> list2(v2.begin(), v2.begin() + n2);

	// Удаление перемещенных элементов из векторов v1 и v2
	v1.erase(v1.begin(), v1.begin() + n1); // Удаляем элементы из v1
	v1.shrink_to_fit();
	v2.erase(v2.begin(), v2.begin() + n2); // Удаляем элементы из v2
	v2.shrink_to_fit();

	// 1. Вычисление среднего значения
	int totalSize = 0;
	for (const auto& tree : list1) {
		totalSize += tree.getSize(); // Суммируем размеры деревьев
	}
	double averageSize = static_cast<double>(totalSize) / list1.size(); // Среднее значение

	// 2. Перегруппировка элементов списка
	std::list<BinaryTree<int>> greaterThanAverage;
	std::list<BinaryTree<int>> lessThanOrEqualToAverage;

	for (const auto& tree : list1) {
		if (tree.getSize() > averageSize) {
			greaterThanAverage.push_back(tree); // Дерево больше среднего
		}
		else {
			lessThanOrEqualToAverage.push_back(tree); // Дерево меньше или равно среднему
		}
	}

	// Объединяем списки: сначала элементы больше среднего, затем остальные
	greaterThanAverage.splice(greaterThanAverage.end(), lessThanOrEqualToAverage);

	// Теперь greaterThanAverage содержит все деревья, большие среднего значения, в начале
	list1 = std::move(greaterThanAverage); // Перемещаем обратно в list1

	// 1. Удаление элементов с нечётным размером
	list2.remove_if([](const BinaryTree<int>& tree) {
		return  (tree.getSize() % 2 != 0); // Условие для удаления
		});

	std::unordered_set< BinaryTree<int>> unique_tree;

	for (const auto& tree : v1)
	{
		unique_tree.insert(tree);
	}

	for (const auto& tree : v2)
	{
		unique_tree.insert(tree);
	}

	std::vector< BinaryTree<int>> v3(unique_tree.begin(), unique_tree.end());

	// 1. Определяем размеры списков
	size_t size1 = list1.size();
	size_t size2 = list2.size();

	// 2. Находим количество элементов, которые нужно удалить
	size_t n = 0; // Установите n в нужное значение
	if (size1 > size2) {
		n = size1 - size2; // Удаляем из list1
		auto it = list1.begin();
		std::advance(it, n); // Перемещаем итератор на n позиций вперед
		list1.erase(list1.begin(), it); // Удаляем первые n элементов
	}
	else if (size2 > size1) {
		n = size2 - size1; // Удаляем из list2
		auto it = list2.begin();
		std::advance(it, n); // Перемещаем итератор на n позиций вперед
		list2.erase(list2.begin(), it); // Удаляем первые n элементов
	}

	// 3. Создаем список list3 для хранения пар
	std::list<std::pair<BinaryTree<int>, BinaryTree<int>>> list3;

	// 4. Формируем пары из элементов list1 и list2
	auto it1 = list1.begin();
	auto it2 = list2.begin();
	while (it1 != list1.end() && it2 != list2.end()) {
		list3.emplace_back(*it1, *it2); // Добавляем пару в list3
		++it1;
		++it2;
	}

	// Создание вектора для хранения пар
	std::vector<std::pair<BinaryTree<int>, BinaryTree<int>>> v4;

	// Определяем минимальный размер между v1 и v2
	size_t minSize = std::min(v1.size(), v2.size());

	// Формируем пары из элементов v1 и v2
	for (size_t i = 0; i < minSize; ++i) {
		v4.emplace_back(v1[i], v2[i]); // Добавляем пару в v3
	}
}

