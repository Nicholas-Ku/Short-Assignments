#include <iostream>
#include <vector>
using namespace std;

class HeapHelper {
public:
    HeapHelper() { size_ = 0; }
    HeapHelper(const vector<int> &input);
    int left_child_loc(int i) const { return 2 * i + 1; }
    int right_child_loc(int i) const { return 2 * i + 2; }
    int parent_loc(int i) const { return ((i - 1) / 2); }
    
    void add_node(const int &inserted);
    bool remove_top_node_and_get_value(int &dummy_value);
    void reheapify_downwards(const int &element_location);
    void reheapify_upwards(const int &element_location);

    void print() const;
    void continous_removal_and_print();

private:
    vector<int> heap_;
    int size_;
};

void HeapHelper::reheapify_downwards(const int &element_location)
{
    const int last_element_location = size_ - 1;
    const int current_element = heap_[element_location];
    const int left_child_location = left_child_loc(element_location);
    const int right_child_location = right_child_loc(element_location);

    // Has both a right child and a left child
    if (right_child_location <= last_element_location) {
        const int left_child = heap_[left_child_location];
        const int right_child = heap_[right_child_location];

        if (current_element < left_child || current_element < right_child) {
            if (left_child >= right_child) {
                heap_[left_child_location] = current_element;
                heap_[element_location] = left_child;
                reheapify_downwards(left_child_location);
            }
            else {
                heap_[right_child_location] = current_element;
                heap_[element_location] = right_child;
                reheapify_downwards(right_child_location);
            }
        }
    }

    // Only has left child
    else if (left_child_location <= last_element_location) {
        const int left_child = heap_[left_child_location];

        if (current_element < left_child) {
            heap_[left_child_location] = current_element;
            heap_[element_location] = left_child;
            reheapify_downwards(left_child_location);
        }
    }
    return;
}

HeapHelper::HeapHelper(const vector<int> &input)
{
    heap_ = input;
    size_ = input.size();

    const int last_element = size_ - 1;
    for (int traversal = last_element; traversal != -1; traversal--) {
        reheapify_downwards(traversal);
    }

}

bool HeapHelper::remove_top_node_and_get_value(int &dummy_value)
{
    if (size_ <= 0) return false;

    if (size_ == 1) {
        dummy_value = heap_[0];
        size_--;
        return true;
    }

    else {
        dummy_value = heap_[0];
        heap_[0] = heap_[size_ - 1];
        size_--;
        reheapify_downwards(0);
        return true;
    }
}

void HeapHelper::reheapify_upwards(const int &element_location)
{
    if (element_location == 0) return;

    const int parent_location = parent_loc(element_location);
    const int parent = heap_[parent_location];
    const int element = heap_[element_location];

    if (element > parent) {
        heap_[element_location] = parent;
        heap_[parent_location] = element;
        reheapify_upwards(parent_location);
    }
    return;
}

void HeapHelper::add_node(const int &inserted)
{
    size_++;
    heap_.push_back(inserted);
    reheapify_upwards(size_ - 1);
}

void HeapHelper::print() const
{
    for (int i = 0; i < size_; i++) {
        cout << heap_[i] << endl;
    }
}

void HeapHelper::continous_removal_and_print()
{
    const int amount_to_remove = size_;
    for (int i = 0; i < amount_to_remove; i++) {
        cout << "Iteration Removal: " << i+1 << endl;
        int dummy_int = 0;
        remove_top_node_and_get_value(dummy_int);
        cout << "Removed: " << dummy_int << endl;
        print();
        cout << endl;
    }
}


int main()
{
    vector<int> input_dummy;
    for (int i = 0; i < 5; i++) {
        input_dummy.push_back(i);
    }

    cout << "Initial Vector: " << endl;
    for (int i = 0; i < input_dummy.size(); i++) {
        cout << input_dummy[i] << endl;
    }

    HeapHelper max_heap(input_dummy);
    cout << "Max Heap Vector: " << endl;
    max_heap.print();

    HeapHelper max_heap2(input_dummy);
    max_heap2.add_node(5);
    cout << "Max Heap Vector after insertion: " << endl;
    max_heap2.print();

    int dummy_int = 0;
    max_heap2.remove_top_node_and_get_value(dummy_int);
    cout << "Max Heap Vector after removal: " << endl;
    cout << "Removed value: " << dummy_int << endl;
    cout << "Rest of Max Heap Vector: " << endl;
    max_heap2.print();

    cout << "Test 1: " << endl;
    vector<int> test_vector_1 = { 0, 3, 9, 1, 5, 4, 18, 21 };
    HeapHelper test_heap_1(test_vector_1);
    test_heap_1.print();
    test_heap_1.continous_removal_and_print();
}
