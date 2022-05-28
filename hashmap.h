

#if !defined(HASH_H)
#define HASH_H 1

#define DEF_CAP 3

template <typename K, typename V>
struct Node
{
    K key;
    V value;
    bool is_added;
};

template <typename K, typename V>
struct Bucket
{
    int capacity;
    int size;
    Node<K, V>(*nodes[]); // zero-sized array(size assigned at runtime) must be end of struct
};

template <typename K, typename V>
struct HashmapRaw
{
    Bucket<K, V>(*buckets[]); // zero-sized array(size assigned at runtime) must be end of struct
};

template <typename K, typename V>
class Hashmap
{
private:
    int capacity;
    HashmapRaw<K, V>(*hashmap_raw);
    int get_hash(K);
    void extend_array(int);

public:
    explicit Hashmap();
    bool add_item(K, V);
    bool remove_item(K);
    V get_item(K);
};

static int make_prime(int);

#include "hashmap.tpp"

#endif // HASH_H
