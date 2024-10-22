#include <bits/stdc++.h>

using namespace std;


class SortLib {
public:
    explicit SortLib(const vector<int> && v) {
        for (const auto& i : v)
            this->arr.push_back(i);
    }

    SortLib(const SortLib &) = delete;
    SortLib &operator=(const SortLib &) = delete;

    template<class T>
    static void quickSort(vector<T> &arr, int low, int high) {
        if (low < high) {
            int mid = my_partition(arr, low, high);
            quickSort(arr, low, mid - 1);
            quickSort(arr, mid + 1, high);
        }
    }

    friend 

private:
    vector<int> arr;
    template<class T>
    constexpr int my_partition(vector<T> &arr, int low, int high) {
        // 选择随机Pivot
        int randomIndex = rand() % (high - low + 1) + low;
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
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // 设置随机种子

    vector<int> v = {19, 3, 2, 44, 5, 6, 7, 8, 9, 10};

    SortLib::quickSort(v, 0, static_cast<int>(v.size() - 1));

    std::for_each(v.begin(), v.end(), [](auto val) {
        std::cout << val << " ";
    });
    std::cout << std::endl;

    return 0;
}
