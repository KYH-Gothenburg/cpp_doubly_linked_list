#include <iostream>
#include <optional>
#include <exception>

using namespace std;

template <typename T>
class Node {
public:
    Node() : data_(T()), next_(nullptr), prev_(nullptr) {}
    explicit Node(T data) : data_(data), next_(nullptr), prev_(nullptr) {}

    Node<T>* GetNext() { return next_; }
    void SetNext(Node<T>* next) { next_ = next; }
    Node<T>* GetPrev() { return prev_; }
    void SetPrev(Node<T>* prev) { prev_ = prev; }

    T GetData() { return data_; }
    void SetData(T data) { data_ = data; }
private:
    T data_;
    Node* next_;
    Node* prev_;
};


template <typename T>
class LinkedList {
public:
    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
    LinkedList(const initializer_list<T> &initList) : head_(nullptr), tail_(nullptr), size_(0){
        for(auto value : initList) {
            PushTail(value);
        }

    }
    LinkedList(const LinkedList<T>&) = delete;
    LinkedList<T>& operator=(const LinkedList<T>) = delete;

    ~LinkedList() {
        ClearList();
    }

    void ClearList() {
        Node<T>* current = head_;
        while(head_) {
            current = head_->GetNext();
            delete head_;
            head_ = current;
        }
        tail_ = nullptr;
        size_ = 0;
    }

    void PushHead(T data) {
        // is list empty?
        if(!head_) {
            head_ = new Node<T>(data);
            tail_ = head_;
            size_++;
            return;
        }

        head_->SetPrev(new Node<T>(data));
        head_->GetPrev()->SetNext(head_);
        head_ = head_->GetPrev();
        size_++;
    }

    optional<T> PopHead() {
        if(!head_) {
            return nullopt;
        }
        if(head_ == tail_) {
            T data = head_->GetData();
            delete head_;
            head_ = tail_ = nullptr;
            size_--;
            return data;
        }
        T data = head_->GetData();
        head_ = head_->GetNext();
        delete head_->GetPrev();
        head_->SetPrev(nullptr);
        size_--;
        return data;
    }

    void PushTail(T data) {
        // is the list empty?
        if(!tail_) {
            tail_ = new Node<T>(data);
            head_ = tail_;
            size_++;
            return;
        }
        tail_->SetNext(new Node<T>(data));
        tail_->GetNext()->SetPrev(tail_);
        tail_ = tail_->GetNext();
        size_++;
    }

    optional<T> PopTail() {
        if(!tail_) {
            return nullopt;
        }
        if(head_ == tail_) {
            T data = tail_->GetData();
            delete tail_;
            tail_ = head_ = nullptr;
            size_--;
            return data;
        }
        T data = tail_->GetData();
        tail_ = tail_->GetPrev();
        delete tail_->GetNext();
        tail_->SetNext(nullptr);
        size_--;
        return data;
    }

    void PushAt(int index, T data) {
        if(index > size_) {
            throw out_of_range("Index out of range");
        }
        if(index == 0) {
            PushHead(data);
            return;
        }
        if(index == size_) {
            PushTail(data);
            return;
        }
        Node<T>* current = &GetAt_(index-1);

        auto newNode = new Node<T>(data);
        newNode->SetNext(current->GetNext());
        newNode->SetPrev(current);
        current->SetNext(newNode);
        newNode->GetNext()->SetPrev(newNode);
        size_++;
    }

    optional<T> PopAt(int index) {
        if(index >= size_) {
            throw out_of_range("Index out of range");
        }
        if(index == 0) {
            return PopHead();
        }
        if(index == size_ - 1) {
            return PopTail();
        }
        Node<T>* current = &GetAt_(index);

        T data = current->GetData();
        current->GetPrev()->SetNext(current->GetNext());
        current->GetNext()->SetPrev(current->GetPrev());
        delete current;
        size_--;
        return data;
    }

    T operator[](int index) {
        return GetAt_(index).GetData();
    }

    int size() {
        return size_;
    }
private:
    Node<T>& GetAt_(int index){
        if(index >= size_) {
            throw out_of_range("Index out of range");
        }
        if(index == 0) {
            return *head_;
        }
        if(index == size_ - 1) {
            return *tail_;
        }
        if(index <= size_ / 2) {
            // index is in first half of list
            Node<T>* current = head_;
            for(int i = 0; i < index; i++) {
                current = current->GetNext();
            }
            return *current;
        }
        else {
            // index is in second half of list
            Node<T>* current = tail_;
            for(int i = size_-1; i > size_ / 2 + 1; i--) {
                current = current->GetPrev();
            }
            return *current;

        }

    }

    Node<T>* head_;
    Node<T>* tail_;
    int size_;
};


int main() {

    LinkedList<int> theList{1, 2, 3, 4, 5, 6};

    theList.PopAt(2);
    theList.PushAt(2, 56);
    for (int i = 0; i < theList.size(); i++)
        cout << theList[i] << endl;
    // 10, 9, 8, 7

//    theList.PushHead(10);
//    theList.PushHead(9);
//    theList.PushHead(8);
//    theList.PushHead(7);

    // 0  1  2   3
    // 7, 8, 9, 10
    //cout << theList.PopAt(1).value() << endl;
    //cout << "------" << endl;
//    auto value = theList.PopTail();
//    while(value.has_value()) {
//        cout << value.value() << endl;
//        value = theList.PopTail();
//    }

    return 0;
}
