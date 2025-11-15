                                                                                                                                                          Бинарная куча
class MinHeap:
    def __init__(self):
        self.heap = []

    # Добавляем новый элемент в кучу
    def push(self, item):
        self.heap.append(item)
        self._bubble_up(len(self.heap)-1)

    # Вынимаем минимальный элемент
    def pop_min(self):
        if len(self.heap) == 0:
            raise IndexError('Heap is empty')
        elif len(self.heap) == 1:
            return self.heap.pop()
        else:
            min_item = self.heap[0]
            self.heap[0] = self.heap.pop()
            self._sink_down(0)
            return min_item

    # Приводит узел наверх (если надо)
    def _bubble_up(self, idx):
        parent_idx = (idx - 1) // 2
        while idx > 0 and self.heap[idx] < self.heap[parent_idx]:
            self.heap[idx], self.heap[parent_idx] = self.heap[parent_idx], self.heap[idx]
            idx = parent_idx
            parent_idx = (idx - 1) // 2

    # Пропихиваем узел вниз (если надо)
    def _sink_down(self, idx):
        length = len(self.heap)
        while True:
            left_child_idx = 2 * idx + 1
            right_child_idx = 2 * idx + 2
            smallest = idx
            if left_child_idx < length and self.heap[left_child_idx] < self.heap[smallest]:
                smallest = left_child_idx
            if right_child_idx < length and self.heap[right_child_idx] < self.heap[smallest]:
                smallest = right_child_idx
            if smallest != idx:
                self.heap[idx], self.heap[smallest] = self.heap[smallest], self.heap[idx]
                idx = smallest
            else:
                break

# Тестируем работу бинарной кучи
if __name__ == "__main__":
    min_heap = MinHeap()
    min_heap.push(5)
    min_heap.push(3)
    min_heap.push(10)
    min_heap.push(1)
    print(f'Popped minimum: {min_heap.pop_min()}')  # Должно вывести 1
    print(f'Current heap: {min_heap.heap}')
                                                                                                                                                              Биноминальная куча

class BinomialNode:
    def __init__(self, key):
        self.key = key
        self.children = []     # Список детей
        self.degree = 0        # Степень вершины

class BinomialHeap:
    def __init__(self):
        self.trees = []

    # Функция для объединения двух кучи
    def merge(self, other):
        merged_trees = sorted(self.trees + other.trees, key=lambda tree: tree.degree)
        if not merged_trees:
            return None
        res = []
        curr_degree = None
        carry = None
        for tree in merged_trees:
            if curr_degree is None or tree.degree != curr_degree:
                if carry:
                    res.append(carry)
                carry = tree
                curr_degree = tree.degree
            else:
                new_tree = BinomialNode(None)
                new_tree.children.extend([carry, tree])
                new_tree.degree = curr_degree + 1
                carry = new_tree
                curr_degree += 1
        if carry:
            res.append(carry)
        self.trees = res

    # Вставка нового элемента
    def insert(self, key):
        new_node = BinomialNode(key)
        new_heap = BinomialHeap()
        new_heap.trees.append(new_node)
        self.merge(new_heap)

    # Нахождение минимального ключа
    def find_min(self):
        if not self.trees:
            return float('inf')
        return min(tree.key for tree in self.trees)

    # Удаление минимального элемента
    def extract_min(self):
        if not self.trees:
            return None
        min_key = float('inf')
        min_tree = None
        for tree in self.trees:
            if tree.key < min_key:
                min_key = tree.key
                min_tree = tree
        self.trees.remove(min_tree)
        children = [BinomialHeap() for _ in range(len(min_tree.children))]
        for i, child in enumerate(min_tree.children):
            children[i].trees.append(child)
        for child_heap in children:
            self.merge(child_heap)
        return min_key

# Тестируем биноминальную кучу
if __name__ == "__main__":
    bin_heap = BinomialHeap()
    bin_heap.insert(5)
    bin_heap.insert(3)
    bin_heap.insert(10)
    bin_heap.insert(1)
    print(f'Minimum element: {bin_heap.find_min()}')  # Должно вывести 1
    print(f'Extracted minimum: {bin_heap.extract_min()}')  # Должно вернуть 1
    print(f'New minimum after extraction: {bin_heap.find_min()}')  # Теперь должно быть 3

                                                                                                      Фибоначчи
def fibonacci(n):
    if n in (1, 2):
        return 1
    return fibonacci(n - 1) + fibonacci(n - 2)
                                                                                                      Хеш таблицы

from collections import defaultdict
import heapq

class HashHeap:
    def __init__(self):
        # Словарь для отображения ключей к позициям в куче
        self.hash_map = {}      # {ключ: индекс_позиции_в_куче}
        # Сам массив кучи (используется стандартная библиотека heapq)
        self.heap = []
        # Дополнительный счётчик для разрешения коллизий при повторяющихся значениях
        self.counter = 0

    def push(self, key, priority):
        """Добавляем пару ключ-значение"""
        entry = (-priority, self.counter, key)  # минус перед приоритетом для максимальной кучи
        heapq.heappush(self.heap, entry)
        self.hash_map[key] = len(self.heap) - 1
        self.counter += 1

    def peek_max(self):
        """Получаем максимальный элемент (без удаления)"""
        if not self.heap:
            return None
        _, _, key = self.heap[0]
        return key

    def pop_max(self):
        """Извлекаем максимальный элемент"""
        if not self.heap:
            return None
        _, _, key = heapq.heappop(self.heap)
        del self.hash_map[key]
        return key

    def update_priority(self, key, new_priority):
        """Обновляем приоритет заданного ключа"""
        if key not in self.hash_map:
            return False
        old_entry = self.heap[self.hash_map[key]]
        new_entry = (-new_priority, old_entry[1], key)
        self.heap[self.hash_map[key]] = new_entry
        heapq.heapify(self.heap)  # Перестроим кучу заново
        return True

    def contains(self, key):
        """Проверяем наличие ключа в структуре"""
        return key in self.hash_map

    def remove(self, key):
        """Удаляем ключ из структуры"""
        if key not in self.hash_map:
            return False
        index = self.hash_map[key]
        del self.hash_map[key]
        # Замена удалённого элемента последним элементом в куче
        last_element = self.heap[-1]
        self.heap[index] = last_element
        del self.heap[-1]
        if index < len(self.heap):
            self.hash_map[last_element[2]] = index
            heapq.heapify(self.heap)
        return True


# Примеры использования:
hash_heap = HashHeap()
hash_heap.push('apple', 10)
hash_heap.push('banana', 5)
hash_heap.push('cherry', 20)
print(hash_heap.peek_max())  # Выведет cherry
print(hash_heap.pop_max())   # Выведет cherry
print(hash_heap.contains('banana'))  # Выведет True
hash_heap.update_priority('apple', 30)
print(hash_heap.peek_max())  # Выведет apple
