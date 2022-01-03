#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_


#include <memory>
#include "listNode.h"
#include <stdbool.h>

template <class keyT, class dataT>
class HashTable {
    private:
        void ChangeSize(bool expand);

    public:
        int m;
        int size;
        std::shared_ptr<ListNode<keyT, dataT>>* arr;

        HashTable();
        ~HashTable();
        void Insert(keyT key, std::shared_ptr<dataT>& data);
        void Remove(keyT key);
        std::shared_ptr<dataT>& Get(keyT key);
        bool HashTable<class keyT, class dataT>::IfExists(keyT key);
};

template <class keyT, class dataT>
HashTable<class keyT, class dataT>::HashTable() : m(3), size(0) {
    arr = new std::shared_ptr<ListNode<keyT, dataT>>[m];
}

template <class keyT, class dataT>
HashTable<class keyT, class dataT>::~HashTable() {
    delete arr;
}


template <class keyT, class dataT>
std::shared_ptr<dataT>& HashTable<class keyT, class dataT>::Get(keyT key) {
    if (arr[key % m] == nullptr)
        return nullptr;
    
    std::shared_ptr<ListNode<keyT, dataT>> curr = arr[key % m];
    while (curr != nullptr) {
        if (curr->key == key)
            return curr->data;
        curr = curr->next;
    }

    return nullptr;
}

template <class keyT, class dataT>
bool HashTable<class keyT, class dataT>::IfExists(keyT key) {
    if (this->Get(key) != nullptr)
        return True;
    return False;
}

template <class keyT, class dataT>
void HashTable<class keyT, class dataT>::Insert(keyT key, std::shared_ptr<dataT>& data) {
    if (this->IfExists(key))
        return;
    
    std::shared_ptr<ListNode<keyT, dataT>> toAdd = std::shared_ptr<ListNode<keyT, dataT>>(new ListNode<keyT, dataT>(key, data));
    std::shared_ptr<ListNode<keyT, dataT>> head = arr[key % m];
    toAdd->next = head;
    arr[key % m] = std::make_shared<ListNode<keyT, dataT>>(toAdd);
    size++;

    if (size == m)
        this->ChangeSize(true);
}

template <class keyT, class dataT>
void HashTable<class keyT, class dataT>::Remove(keyT key) {
    std::shared_ptr<ListNode<keyT, dataT>> curr = arr[key % m];
    while (curr != nullptr && curr->next != nullptr) {
        if (curr->next->key == key) {
            curr->next = curr->next->next;
            break;
        }
        curr = curr->next;
    }

    size--;
    if ((double)(size / m) <= 1/9 && size != 0)
        this->ChangeSize(false);
}

template <class keyT, class dataT>
void HashTable<class keyT, class dataT>::ChangeSize(bool expand) {
    int newM = m;
    if (expand) {
        newM = m * 3;
        std::shared_ptr<ListNode<keyT, dataT>>* newArr = new std::shared_ptr<ListNode<keyT, dataT>>[newM];
    } else {
        newM = m / 3;
        std::shared_ptr<ListNode<keyT, dataT>>* newArr = new std::shared_ptr<ListNode<keyT, dataT>>[newM];
    }

    for (int i = 0; i < m; i++) {
        std::shared_ptr<ListNode<keyT, dataT>> curr = this->arr[i];
        while (curr != nullptr) {
            std::shared_ptr<ListNode<keyT, dataT>> toAdd = std::shared_ptr<ListNode<keyT, dataT>>(new ListNode<keyT, dataT>(curr));
            std::shared_ptr<ListNode<keyT, dataT>> head = newArr[curr->key % newM];
            toAdd->next = head;
            newArr[curr->key % newM] = std::make_shared<ListNode<keyT, dataT>>(toAdd);
            curr = curr->next;
        }
    }
    this->m = newM;
    delete arr;
    this->arr = newArr;
}


#endif /* HASH_TABLE_H_ */