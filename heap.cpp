//待会会换成我们的array类型
void heapify(int *arr, int heap_len, int pos) {
    int smallest = pos;
    int left = pos * 2 + 1;
    int right = pos * 2 + 2;
    
    while(left < heap_len) {
        if(arr[smallest] > arr[left]) {
            smallest = left;
        }
        
        if(right < heap_len && arr[smallest] > arr[right]) {
            smallest = right;
        }
        
        if(smallest == pos) {
            break;
        } else {
            swap(arr[pos], arr[smallest]);
            pos = smallest;
            left = pos * 2 + 1;
            right = pos * 2 + 2;
        }
    }
}

void build_heap(int *arr, int heap_len) {
    for(int i = heap_len / 2 - 1; i >= 0; i--) {
        heapify(arr, heap_len, i);
    }
}

//差了一个堆排序接口