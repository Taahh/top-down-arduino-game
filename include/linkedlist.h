#ifndef LINKEDLIST_H
#define LINKEDLIST_H
template <typename T>
struct LinkedNode
{
    T dataByte;
    LinkedNode *next;
};

template <typename T>
class LinkedList
{
public:
    ~LinkedList()
    {
        LinkedNode<T> *dummy = head;
        while (dummy != nullptr)
        {
            LinkedNode<T> *next = dummy->next;
            delete dummy;
            dummy = next;
        }
    }
    LinkedList(LinkedNode<T> *head = nullptr) : head(head), tail(head), nodesSize(0) {}
    LinkedNode<T> *head;
    LinkedNode<T> *tail;
    void push(T byte);
    T *pop();
    T *peek();
    int size();

private:
    int nodesSize;
};

template <typename T>
void LinkedList<T>::push(T byte)
{
    this->nodesSize++;
    if (head == nullptr)
    {
        this->head = new LinkedNode<T>();
        this->head->dataByte = byte;
        this->tail = head;
        return;
    }

    this->tail->next = new LinkedNode<T>();
    this->tail->next->dataByte = byte;
    this->tail = this->tail->next;
}

template <typename T>
T *LinkedList<T>::pop()
{
    if (head == nullptr)
    {
        return nullptr;
    }
    T value = head->dataByte;
    this->nodesSize--;
    if (this->nodesSize == 0)
    {
        head = nullptr;
        tail = nullptr;
        return &value;
    }
    this->head = this->head->next;
    return &value;
}

template <typename T>
T *LinkedList<T>::peek()
{
    if (head == nullptr)
    {
        return nullptr;
    }
    T value = head->dataByte;
    return &value;
}

template <typename T>
int LinkedList<T>::size()
{
    return this->nodesSize;
}

#endif