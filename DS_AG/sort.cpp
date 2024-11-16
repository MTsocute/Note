// #include <bits/stdc++.h>
// #include "include/stdc++.h"
#include <iostream>
using namespace std;

constexpr static int MAXHEAP_MODE = 0;
constexpr static int MIN_HEAP_MODE = 1;

using namespace std;

template<class T>
void print_all(const vector<T> &v) {
    std::cout << "Content: \n";
    std::for_each(v.begin(), v.end(), [](auto val) {
        std::cout << val << " ";
    });
    std::cout << std::endl;
}

class SortLib {
public:
    explicit SortLib() = default;

    SortLib(const SortLib &) = delete;

    SortLib &operator=(const SortLib &) = delete;

    template<class T>
    void quickSort(vector<T> &arr, int low, int high) {
        if (low < high) {
            const int mid = my_partition(arr, low, high);
            quickSort(arr, low, mid - 1);
            quickSort(arr, mid + 1, high);
        }
    }

    template<class T>
    void heapSort(vector<T> &arr) {
        this->buildMaxHeap(arr);
        for (int i = arr.size() - 1; i > 0; --i) {
            // 动态的堆调整的范围
            swap(arr[0], arr[i]);
            this->heapAdjust(arr, 0, i);
        }
    }

    template<class T>
    void mergeSort(vector<T> &arr, int low, int high) {
        int mid = low + (high - low) / 2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);

        merge(arr, low, mid, high);
    }

    template<class T>
    void selectSort(vector<T> &arr) {
        for (int i = 0; i < arr.size() - 1; ++i) {
            int min = i;
            for (int j = i + 1; j < arr.size(); ++j)
                if (arr[j] < arr[min]) min = j;
            if (min != i) std::swap(arr[i], arr[min]);
        }
    }

    template<class T>
    void insertSort(vector<T> &arr) {
        for (int i = 1; i < arr.size(); ++i) {
            T cur = arr[i];
            int j = i - 1;
            while (j >= 0 && cur < arr[j]) {
                arr[j + 1] = arr[j]; // 所有元素不断的往后走
                j--;
            }
            // 离开了循环的时候不在原来的位置，所以 j + 1 到原来的位置
            arr[j + 1] = cur;
        }
    }

    template<class T>
    void insertSort_half(vector<T> &arr) {
        for (int i = 1; i < arr.size(); ++i) {
            T cur = arr[i];
            int low = 0;
            int high = i - 1;
            while (low <= high) {
                int mid = low + (high - low) / 2;
                // 右边太大了，应该去左边找
                if (arr[mid] >= cur) high = mid - 1;
                else low = mid + 1;
            }
            // 最后落在 low 这个位置
            // low 右边的元素都移动到右边去
            int j = i - 1;
            for (; j >= low; --j) arr[j + 1] = arr[j];
            arr[j + 1] = cur;
        }
    }

private:
    template<class T>
    void merge(vector<T> &arr, const int low, const int high) {
        vector<T> temp;
        for (const auto &e: arr) temp.emplace_back(e);

        const int mid = low + (high - low) / 2;

        int k = low;
        int i = low;
        int j = mid + 1;

        while (i <= mid && j <= high) {
            if (temp[low] <= temp[j]) arr[k++] = temp[i++];
            else arr[k++] = temp[j++];
        }

        while (i <= mid) { arr[k++] = temp[i++]; }
        while (j <= high) { arr[k++] = temp[j++]; }
    }

    template<class T>
    int my_partition(vector<T> &arr, int low, int high) {
        int randomIndex = std::rand() % (high - low + 1) + low;
        T Pivot = arr[randomIndex];

        std::swap(arr[randomIndex], arr[low]); // 将Pivot放到低位

        while (low < high) {
            while (low < high && arr[high] >= Pivot) --high;
            arr[low] = arr[high]; // 移动比Pivot小的元素到低位
            while (low < high && arr[low] <= Pivot) ++low;
            arr[high] = arr[low]; // 移动比Pivot大的元素到高位
        }

        arr[low] = Pivot; // 将Pivot放回正确位置
        return low; // 返回Pivot的最终位置
    }

    // 第二个参数是决定调整的范围的，非常重要
    template<class T>
    void heapAdjust(vector<T> &arr, int k, const int heapSize) {
        T k_val = arr[k];
        // 如果需要调整就一直下坠
        // 从当前节点的左孩子往下走
        for (int left = 2 * k + 1; left < heapSize; left = 2 * left + 1) {
            // 如果存在右孩子且右孩子比左孩子大
            if (left + 1 < heapSize && arr[left] < arr[left + 1]) ++left;

            // 分支更大结束当前分支
            if (arr[left] <= k_val) break;
            // 孩子比分支节点大，交换并下坠调整，
            arr[k] = arr[left];
            // 从节点位置哪里开始调整
            k = left;
        }
        // 根节点回到原来的位置
        arr[k] = k_val;
    }

    template<class T>
    void buildMaxHeap(vector<T> &arr) {
        // 从最后一个非叶子节点开始调整堆，逐步建立大根堆
        for (int k = arr.size() / 2 - 1; k >= 0; --k)
            this->heapAdjust(arr, k, arr.size());
    }

    // merge
};

int main() {
    // std::srand(static_cast<unsigned int>(std::time(nullptr))); // 设置随机种子
    vector<int> v = {19, 3, 2, 44, 5, 6, 7, 8, 9, 10};

    SortLib demo;

    // demo.quickSort(v, 0, v.size() - 1); // 确保使用 static_cast 转换为 int
    // demo.heapSort(v);
    // demo.heapSort(v);
    // demo.selectSort(v);
    demo.insertSort(v);
    // demo.insertSort_half(v);

    print_all(v);

    return 0;
}
