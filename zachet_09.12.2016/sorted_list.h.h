//
// Created by ulyanin on 09.12.16.
//

#ifndef ZACHET_09_12_2016_SORTED_LIST_H_H
#define ZACHET_09_12_2016_SORTED_LIST_H_H

class Node
{
public:
    Node(int v)
            : val(v)
            , Next(nullptr)
    { }

    int val;
    Node * Next;
private:
    std::mutex mut_;
};

class sorted_list {
public:
    void insert( int x );
    bool erase( int x );
    bool has( int x ) const;
private:
    Node * head;
};

sorted_list::sorted_list()
{

}

#endif //ZACHET_09_12_2016_SORTED_LIST_H_H
