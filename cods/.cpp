                                                                                                    Бинарная куча
#include <iostream>
#include <vector>
using namespace std;

class BinaryHeap {
private:
    vector<int> heap;
    
public:
    // Добавляет элемент в кучу
    void insert(int value) {
        heap.push_back(value);
        int i = heap.size() - 1;
        
        while(i > 0 && heap[(i - 1)/2] > heap[i]) {   // Восходящее всплытие
            swap(heap[i], heap[(i - 1)/2]);
            i = (i - 1)/2;
        }
    }
    
    // Удаляет минимальный элемент
    int extractMin() {
        if(heap.empty()) return INT_MAX;
        
        int root = heap.front();
        heap.front() = heap.back();                   // Перемещаем последний элемент на вершину
        heap.pop_back();
        
        int n = heap.size(), i = 0;
        while(true) {
            int smallest = i;
            
            int leftChild = 2 * i + 1;
            if(leftChild < n && heap[leftChild] < heap[smallest])
                smallest = leftChild;
                
            int rightChild = 2 * i + 2;
            if(rightChild < n && heap[rightChild] < heap[smallest])
                smallest = rightChild;
            
            if(smallest != i) {
                swap(heap[i], heap[smallest]);       // Спуск вниз
                i = smallest;
            } else break;
        }
        
        return root;
    }
};

// Тестирование бинарной кучи
void testBinaryHeap() {
    BinaryHeap bh;
    bh.insert(3); bh.insert(2); bh.insert(15); bh.insert(5); bh.insert(4); bh.insert(45);
    
    cout << "Extracted minimum elements:\n";
    for(int i = 0; i < 6; ++i)
        cout << bh.extractMin() << ' ';
}
                                                                                                                                      Биноминальная куча

struct BinomialNode {
    int key;
    BinomialNode* parent;
    BinomialNode* child;
    BinomialNode* sibling;
    int degree;
    
    BinomialNode(int k) : key(k), parent(nullptr), child(nullptr), sibling(nullptr), degree(0) {}
};

class BinomialHeap {
private:
    BinomialNode* head;
    
public:
    BinomialHeap() : head(nullptr) {}
    
    ~BinomialHeap() {
        clear(head);
    }
    
    // Объединение двух куч
    static BinomialHeap merge(BinomialHeap& h1, BinomialHeap& h2) {
        BinomialHeap result;
        result.head = mergeLists(h1.head, h2.head);
        result.consolidate();
        return result;
    }
    
    // Очистка памяти
    void clear(BinomialNode* node) {
        if(node == nullptr) return;
        clear(node->child);
        delete node;
    }
    
    // Поиск минимального узла
    BinomialNode* findMinimum() const {
        BinomialNode* minNode = nullptr;
        BinomialNode* current = head;
        while(current != nullptr) {
            if(minNode == nullptr || current->key < minNode->key)
                minNode = current;
            current = current->sibling;
        }
        return minNode;
    }
    
    // Извлечение минимума
    int extractMin() {
        BinomialNode* minNode = findMinimum();
        if(minNode == nullptr) throw runtime_error("Empty heap");
        
        BinomialNode* prev = nullptr;
        BinomialNode* current = head;
        while(current != minNode) {
            prev = current;
            current = current->sibling;
        }
        
        if(prev == nullptr) head = minNode->sibling;
        else prev->sibling = minNode->sibling;
        
        BinomialHeap reverseTree;
        reverseTree.head = minNode->child;
        reverse(reverseTree.head);
        
        BinomialHeap merged = merge(*this, reverseTree);
        this->head = merged.head;
        
        return minNode->key;
    }
    
    // Обратный порядок узлов (ребёнок становится родителем)
    void reverse(BinomialNode*& node) {
        BinomialNode* temp = nullptr;
        while(node != nullptr) {
            BinomialNode* next = node->sibling;
            node->sibling = temp;
            temp = node;
            node = next;
        }
        node = temp;
    }
    
    // Консолидация списка
    void consolidate() {
        size_t maxDegree = log2(getSize()) + 1;
        vector<BinomialNode*> aux(maxDegree, nullptr);
        
        BinomialNode* current = head;
        while(current != nullptr) {
            BinomialNode* next = current->sibling;
            int d = current->degree;
            
            while(aux[d] != nullptr) {
                BinomialNode* other = aux[d];
                if(other->key <= current->key) {
                    link(other, current);
                    current = other;
                } else {
                    link(current, other);
                }
                aux[d++] = nullptr;
            }
            aux[d] = current;
            current = next;
        }
        
        head = nullptr;
        for(auto it = aux.rbegin(); it != aux.rend(); ++it) {
            if(*it != nullptr) {
                (*it)->sibling = head;
                head = *it;
            }
        }
    }
    
    // Соединение двух узлов
    static void link(BinomialNode* y, BinomialNode* z) {
        y->parent = z;
        y->sibling = z->child;
        z->child = y;
        z->degree++;
    }
    
    // Объединение списков
    static BinomialNode* mergeLists(BinomialNode* firstList, BinomialNode* secondList) {
        if(firstList == nullptr) return secondList;
        if(secondList == nullptr) return firstList;
        
        BinomialNode dummyHead(-1);
        BinomialNode* tail = &dummyHead;
        
        while(firstList != nullptr && secondList != nullptr) {
            if(firstList->degree <= secondList->degree) {
                tail->sibling = firstList;
                firstList = firstList->sibling;
            } else {
                tail->sibling = secondList;
                secondList = secondList->sibling;
            }
            tail = tail->sibling;
        }
        
        if(firstList != nullptr) tail->sibling = firstList;
        else tail->sibling = secondList;
        
        return dummyHead.sibling;
    }
    
    // Размер кучи
    int getSize() const {
        int count = 0;
        BinomialNode* current = head;
        while(current != nullptr) {
            count += pow(2, current->degree);
            current = current->sibling;
        }
        return count;
    }
};

// Тестирование биноминальной кучи
void testBinomialHeap() {
    BinomialHeap b1, b2;
    b1.insert(3); b1.insert(2); b1.insert(15); b1.insert(5); b1.insert(4); b1.insert(45);
    b2.insert(8); b2.insert(10); b2.insert(12); b2.insert(20);
    
    BinomialHeap merged = BinomialHeap::merge(b1, b2);
    
    cout << "Extracted minimum elements from merged binomial heap:\n";
    for(int i = 0; i < 10; ++i)
        cout << merged.extractMin() << ' ';
}

int main() {
    testBinaryHeap();
    cout << "\n\n";
    testBinomialHeap();
    return 0;
}
                                                                                                                                                    Куча Фибоначчи

#ifndef FIBONACCI_HEAP_HPP
#define FIBONACCI_HEAP_HPP
#include <vector>
#include <list>
#include <math.h>
typedef long long int lli;
#define GOLDEN_RATIO_ROUND_DOWN 1.618
template <class V> class FibonacciHeap;
template<class T>
class Element {
protected:
    Element<T> *left = this;
    Element<T> *right = this;
    Element<T> *parent = nullptr;
    Element<T> *child = nullptr;
    T key;
    lli degree = 0;
    bool mark = false;
public:
    Element(T key) : key(key) {};
    Element(Element<T> *other) : key(other->key), degree(other->degree), mark(other->mark) {
        this->left = other->left;
        this->right = other->right;
        this->parent = other->parent;
        this->child = other->child;
    }
    T getKey() { return this->key; }
    Element<T> *getLeft() { return this->left; }
    Element<T> *getRight() { return this->right; }
    Element<T> *getChild() { return this->child; }
    Element<T> *getParent() { return this->parent; }
    bool isMarked() { return this->mark; }
    lli getDegree() { return this->degree; }
    friend class FibonacciHeap<T>;
};

template<class T>
class FibonacciHeap {
private:
    // Copy constructor and assignment operator are not implemented
    // Hiding them to avoid misusage (Rule of three)
    // Since the point here is show the algorithmic part of the data structure
    FibonacciHeap(const FibonacciHeap<T> &other);
    FibonacciHeap<T> &operator=(const FibonacciHeap<T> &other);
protected:
    Element<T> *min;
    lli n;
    lli _D(lli n) {
        return log(n)/log(GOLDEN_RATIO_ROUND_DOWN);
    }
    void _deleteAll(Element<T> *x) {
        if (x != nullptr) {
            Element<T> *y = x;
            do {
                Element<T> *z = y;
                y = y->right;
                this->_deleteAll(z->child);
                delete z;
            } while (y != x);
        }
    }
    Element<T> *_unite(Element<T> *x, Element<T> *y) {
        if (x == nullptr) {
            return y;
        } else if (y == nullptr) {
            return x;
        } else if (x->key > y->key) {
            return this->_unite(y, x);
        } else {
            Element<T> *xRight = x->left;
            Element<T> *yRight = y->left;
            x->left = yRight;
            xRight->right = y;
            y->left = xRight;
            yRight->right = x;
            return x;
        }
    }
    void _link(Element<T> *y, Element<T> *x) {
        y->left->right = y->right;
        y->right->left = y->left;
        if (x->child == nullptr) {
            x->child = y;
            y->right = y;
            y->left = y;
        } else {
            Element<T> *child = x->child;
            y->right = child;
            y->left = child->left;
            child->left->right = y;
            child->left = y;
        }
        y->parent = x;
        x->degree++;
        y->mark = false;
    }
    void _fillListWithElements(Element<T> *x, std::list<Element<T>*> &A) {
        Element<T> *last = x;
        Element<T> *w = last;
        do {
            w = w->right;
            A.push_back(w);
        } while (w != last);
    }
    void _consolidate() {
        lli D = this->_D(this->n);
        std::vector<Element<T>*> A(D + 1, nullptr);
        std::list<Element<T>*> elements;
        this->_fillListWithElements(this->min, elements);
        for (auto x : elements) {
            lli d = x->degree;
            while (A.at(d) != nullptr) {
                Element<T> *y = A.at(d);
                if (x->key > y->key) {
                    std::swap(x, y);
                }
                this->_link(y, x);
                A.at(d) = nullptr;
                d++;
            }
            A.at(d) = x;
        }
        this->min = nullptr;
        for (lli i = 0; i < (lli)A.size(); i++) {
            if (A.at(i) != nullptr) {
                A.at(i)->right = A.at(i);
                A.at(i)->left = A.at(i);
                this->min = this->_unite(this->min, A.at(i));
            }
        }
    }
    void _cut(Element<T> *x, Element<T> *y) {
        x->left->right = x->right;
        x->right->left = x->left;
        if (y->child == x) {
            if (x->right == x) {
                y->child = nullptr;
            } else {
                y->child = x->right;
            }
        }
        y->degree--;
        x->left = x;
        x->right = x;
        x->parent = nullptr;
        x->mark = false;
        this->min = this->_unite(this->min, x);
    }
    void _cascadingCut(Element<T> *y) {
        Element<T> *z = y->parent;
        if (z != nullptr) {
            if (!y->mark) {
                y->mark = true;
            } else {
                this->_cut(y, z);
                this->_cascadingCut(z);
            }
        }
    }
public:
    FibonacciHeap() {
        this->min = nullptr;
        this->n = 0;
    };
    ~FibonacciHeap() {
        if (this->min != nullptr) {
            this->_deleteAll(this->min);
        }
    }
    bool isEmpty() const {
        return this->min == nullptr;
    }
    Element<T> *insert(T key) {
        Element<T> *x = new Element<T>(key);
        this->min = this->_unite(this->min, x);
        this->n++;
        return x;
    }
    void unite(FibonacciHeap<T> *heap) {
        this->min = this->_unite(this->min, heap->min);
        this->n += heap->n;
        heap->min = nullptr;
        heap->n = 0;
    }
    T getMin() const {
        return this->min->key;
    }
    T extractMin() {
        Element<T> *z = this->min;
        if (z == nullptr) {
            throw std::invalid_argument("heap is empty");
        }
        Element<T> *x = z->child;
        if (x != nullptr) {
            Element<T> *last = x;
            do {
                x = x->right;
                x->parent = nullptr;
            } while (x != last);
            this->min = this->_unite(this->min, x);
        }
        z->left->right = z->right;
        z->right->left = z->left;
        if (z == z->right) {
            this->min = nullptr;
        } else {
            this->min = z->right;
            this->_consolidate();
        }
        this->n--;
        T min = z->key;
        delete z;
        return min;
    }
    void decreaseKey(Element<T>* x, T k) {
        if (k > x->key) {
            throw std::invalid_argument("new key is greater than current key");
        }
        x->key = k;
        Element<T> *y = x->parent;
        if (y != nullptr && x->key < y->key) {
            this->_cut(x, y);
            this->_cascadingCut(y);
        }
        if (x->key < this->min->key) {
            this->min = x;
        }
    }
    void deleteElement(Element<T> *x) {
        this->decreaseKey(x, std::numeric_limits<T>::min());
        this->extractMin();
    }
};
#endif // FIBONACCI_HEAP_HPP

                                                                                                                                                                                  Хеш таблицы

#include <iostream>
#include <list>
#include <functional>

// Структура для хранения пары ключ-значение
template<typename K, typename V>
struct HashNode {
    K key;
    V value;
    HashNode(K k, V v) : key(k), value(v) {}
};

// Класс хеш-таблицы
template<typename K, typename V>
class HashTable {
private:
    std::list<HashNode<K, V>>* table;
    size_t capacity;
    size_t size;
    
    // Простая хеш-функция
    size_t hashFunction(const K& key) {
        return std::hash<K>()(key) % capacity;
    }

public:
    HashTable(size_t cap = 10) : capacity(cap), size(0) {
        table = new std::list<HashNode<K, V>>[capacity];
    }
    
    ~HashTable() {
        delete[] table;
    }
    
    // Вставка элемента
    void insert(const K& key, const V& value) {
        size_t index = hashFunction(key);
        
        // Проверяем, существует ли уже такой ключ
        for (auto& node : table[index]) {
            if (node.key == key) {
                node.value = value; // Обновляем значение
                return;
            }
        }
        
        // Добавляем новую пару ключ-значение
        table[index].push_back(HashNode<K, V>(key, value));
        size++;
    }
    
    // Поиск значения по ключу
    V* find(const K& key) {
        size_t index = hashFunction(key);
        
        for (auto& node : table[index]) {
            if (node.key == key) {
                return &node.value;
            }
        }
        return nullptr;
    }
    
    // Удаление элемента
    void remove(const K& key) {
        size_t index = hashFunction(key);
        
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->key == key) {
                table[index].erase(it);
                size--;
                return;
            }
        }
    }
    
    // Получение размера таблицы
    size_t getSize() const {
        return size;
    }
};

int main() {
    HashTable<std::string, int> hashTable;
    
    hashTable.insert("one", 1);
    hashTable.insert("two", 2);
    hashTable.insert("three", 3);
    
    std::cout << "Value for 'one': " << *(hashTable.find("one")) << std::endl;
    
    hashTable.remove("two");
    
    std::cout << "Size of hash table: " << hashTable.getSize() << std::endl;
    
    return 0;
}
