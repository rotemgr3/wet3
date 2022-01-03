#ifndef LIST_NODE_H_
#define LIST_NODE_H_

#include <memory>

template <class keyT, class dataT>
class ListNode {
    public:
        keyT key;
        std::shared_ptr<dataT> data;
        std::shared_ptr<ListNode<dataT>> next;
        ListNode(const keyT key, std::shared_ptr<dataT>& data) : key(key), data(data), next(nullptr) {}
        ~ListNode() = default; 
        ListNode(const ListNode<keyT,dataT>& copy) = default;    
};

#endif /* LIST_NODE_H_ */