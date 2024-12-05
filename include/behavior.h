// #ifndef BEHAVIOR_H
// #define BEHAVIOR_H

// #include "linkedlist.h"

// extern LinkedList<Behavior *> *behaviors;
// class Behavior
// {
// public:
//     Behavior()
//     {
//         behaviors->push(this);
//     }
//     virtual void setup();
//     virtual void update();
// };

// #endif

// void behaviorInit()
// {
//     if (behaviors->size() > 0)
//     {
//         LinkedNode<Behavior *> *dummy = behaviors->head;
//         while (dummy != nullptr)
//         {
//             Behavior *behavior = dummy->dataByte;
//             behavior->setup();
//             dummy = dummy->next;
//         }
//     }
// }

// void behaviorUpdate()
// {
//     if (behaviors->size() > 0)
//     {
//         LinkedNode<Behavior *> *dummy = behaviors->head;
//         while (dummy != nullptr)
//         {
//             Behavior *behavior = dummy->dataByte;
//             behavior->setup();
//             dummy = dummy->next;
//         }
//     }
// }

// void addBehavior(Behavior *behavior)
// {
//     behaviors->push(behavior);
// }