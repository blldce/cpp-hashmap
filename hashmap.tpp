

#include "hashmap.h"

template <typename K, typename V>
Hashmap<K, V>::Hashmap() : capacity(make_prime(DEF_CAP))
{
    this->hashmap_raw = (HashmapRaw<K, V>(*)) new HashmapRaw<K, V> + (this->capacity * sizeof(Bucket<K, V>));

    // initialize buckets
    for (int i = 0; i < this->capacity; i++)
    {
        this->hashmap_raw->buckets[i] = (struct Bucket<K, V>(*)) new Bucket<K, V> + (this->capacity * sizeof(Node<K, V>));
        this->hashmap_raw->buckets[i]->capacity = this->capacity;
        this->hashmap_raw->buckets[i]->size = 0;
        for (int j = 0; j < this->hashmap_raw->buckets[i]->capacity; j++) // initialize nodes to zeroed
        {
            this->hashmap_raw->buckets[i]->nodes[j] = (Node<K, V>(*)) new Node<K, V>;
            this->hashmap_raw->buckets[i]->nodes[j]->is_added = 0;
        }
    }
}

template <typename K, typename V>
int Hashmap<K, V>::get_hash(K key)
{
    return key % this->capacity;
}

template <typename K, typename V>
bool Hashmap<K, V>::add_item(K key, V value)
{
    int index = this->get_hash(key);

    Bucket<K, V>(*bucket) = this->hashmap_raw->buckets[index];

    if (bucket->size < bucket->capacity)
    {
        index = 0;
        while (index < bucket->capacity)
        {
            Node<K, V>(*node) = bucket->nodes[index];
            if (node->is_added == 0) // check its empty
            {
                node->key = key;
                node->value = value;
                node->is_added = true;
                bucket->size++;
                return true;
            }
            if (node->key == key) // update value, associate with key
            {
                node->value = value;
                return true;
            }
            index++;
        }
    } // capacity excess, extend array and return here!
    else
    {
        this->extend_array(index);
        return this->add_item(key, value);
    }
    return false;
}

template <typename K, typename V>
bool Hashmap<K, V>::remove_item(K key)
{
    int index = this->get_hash(key);
    Bucket<K, V>(*bucket_ptr) = this->hashmap_raw->buckets[index];
    index = 0;
    while (index < bucket_ptr->capacity)
    {
        if (bucket_ptr->nodes[index]->key == key) // update value, associate with key
        {
            bucket_ptr->nodes[index]->is_added = false;
            bucket_ptr->size--;
            // no need to freed, we'll use this node again.
            return true;
        }
        index++;
    }
    return false;
}

template <typename K, typename V>
V Hashmap<K, V>::get_item(K key)
{
    int index = this->get_hash(key);
    Bucket<K, V>(*bucket_ptr) = this->hashmap_raw->buckets[index];
    index = 0;
    while (index < bucket_ptr->capacity)
    {
        if (bucket_ptr->nodes[index]->key == key)
            return bucket_ptr->nodes[index]->value;
        index++;
    }
    return static_cast<V>(0);
}

template <typename K, typename V>
void Hashmap<K, V>::extend_array(int index)
{
    Bucket<K, V>(*bucket_ptr) = this->hashmap_raw->buckets[index];

    int old_cap = bucket_ptr->capacity;
    bucket_ptr->capacity = make_prime(old_cap * 2);
    index = 0;
    while (index < bucket_ptr->capacity)
    {
        Node<K, V>(*new_node) = (Node<K, V>(*)) new Node<K, V>;
        new_node->is_added = false;
        if (index < old_cap)
        {
            Node<K, V>(*old_node) = bucket_ptr->nodes[index];
            new_node->key = old_node->key;
            new_node->value = old_node->value;
            new_node->is_added = old_node->is_added;
            delete old_node;
        }
        bucket_ptr->nodes[index] = new_node;
        index++;
    }
}

static int make_prime(int number)
{
    if (number <= 2) // prime numbers must be positive
        return 2;    // return min prime number

    int count = 2; // starting from index 2
    while (1)      // endless loop to reach number
    {
        if (number % count == 0) // not prime, increase & call it again!
            return make_prime(++number);
        count++;
        if (count == number)
            return number;
    }
    return number;
}
