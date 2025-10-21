#pragma once

//---������� ��� �������������� ��������� � �������� ����
template<typename T>
void heapify(std::vector<T>& arr, size_t n, size_t i) {
	int largest = i;						//---�������������� ���������� ������� ��� ������
	const int left = 2 * i + 1;				//---����� �������� �������
	const int right = 2 * i + 2;			//---������ �������� �������

	//---���� ����� �������� ������� ������ �����
	if (left < n && arr[left] > arr[largest])
	{
		largest = left;
	}
	//---���� ������ �������� ������� ������ �����
	if (right < n && arr[right]> arr[largest])
	{
		largest = right;
	}
	//---���� ���������� ������� �� ������
	if (largest != i)
	{
		std::swap(arr[i], arr[largest]);
		//---���������� ����������� � �������� ���� ���������� ���������
		heapify(arr, n, largest);
	}
}

//---Heap ����������
template<typename T>
void heapSort(std::vector<T>& arr) {
	
	size_t size = arr.size();
	if (size <= 1) return;
	
	
	//---���������� ���� (�������������� ������)
	for (int i = size / 2 - 1; i >= 0; i--)
	{
		heapify(arr, size, i);
	}
	//---���� �� ������ ��������� �������� �� ����
	for (int i = size - 1; i > 0; i--)
	{
		//---���������� ������� ������ � �����
		std::swap(arr[0], arr[i]);
		//---�������� heapify �� ����������� ����
		heapify(arr, i, 0);
	}
}