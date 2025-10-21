#pragma once

//---Функция для преобразования поддерева в двоичную кучу
template<typename T>
void heapify(std::vector<T>& arr, size_t n, size_t i) {
	int largest = i;						//---Инициализируем наибольший элемент как корень
	const int left = 2 * i + 1;				//---Левый дочерний элемент
	const int right = 2 * i + 2;			//---ПРавый дочерний элемент

	//---Если левый дочерний элемент больше корня
	if (left < n && arr[left] > arr[largest])
	{
		largest = left;
	}
	//---Если правый дочерний элемент больше корня
	if (right < n && arr[right]> arr[largest])
	{
		largest = right;
	}
	//---Если наибольший элемент не корень
	if (largest != i)
	{
		std::swap(arr[i], arr[largest]);
		//---Рекурсивно преобразуем в двоичную кучу затронутое поддерево
		heapify(arr, n, largest);
	}
}

//---Heap сортировка
template<typename T>
void heapSort(std::vector<T>& arr) {
	
	size_t size = arr.size();
	if (size <= 1) return;
	
	
	//---Построение кучи (перегруппируем массив)
	for (int i = size / 2 - 1; i >= 0; i--)
	{
		heapify(arr, size, i);
	}
	//---Один за другим извлекаем элементы из кучи
	for (int i = size - 1; i > 0; i--)
	{
		//---Перемещаем текущий корень в конец
		std::swap(arr[0], arr[i]);
		//---Вызываем heapify на уменьшенной куче
		heapify(arr, i, 0);
	}
}