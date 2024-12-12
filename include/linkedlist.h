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

    T at(size_t n) {
        LinkedNode<T>* dummy = head;
        size_t i = 0;
        while (dummy != nullptr) {
            if (i == n) {
                return dummy->dataByte;
            }
            i++;
            dummy = dummy->next;
        }
    }

    void push(T byte)
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
    bool pop()
    {
        if (head == nullptr)
        {
            return false;
        }
        LinkedNode<T> *temp = head;
        head = head->next;
        delete temp;
        nodesSize--;
        if (head == nullptr)
        {
            tail = nullptr;
        }
        return true;
    }

    // Peek method to look at the first element without removing it
    // Returns true if successful, false if the list is empty
    bool peek(T &out) const
    {
        if (head == nullptr)
        {
            return false;
        }
        out = head->dataByte;
        return true;
    }
    int size()
    {
        return this->nodesSize;
    }

private:
    int nodesSize;
};

#endif