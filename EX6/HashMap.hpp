
#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_

#include "vector"
#include "stdexcept"

#define RUN_MSG "invalid key"
#define LOAD_FAC 2
#define INIT_HASH 16
#define MAX_LOAD 0.75
#define MIN_LOAD 0.25
#define NON_INDEX -1
#define MIN_CAP 1


template<typename KeyT, typename ValueT>
class HashMap {

 public:

  typedef std::pair<KeyT, ValueT> HashNode;
  typedef std::vector<HashNode> Bucket;

  class ConstIterator; //forward declaration
  typedef ConstIterator const_iterator;

  /**
   *
   * @return the end iterator of the last bucket. return as const iterator.
   */
  ConstIterator get_last () const
  {
    typename Bucket::iterator last_node;
    for (int i = 0; i < _capacity; i++)
      {
        if (_hash_arr[i].size () > 0)
          {
            last_node = _hash_arr[i].end ();
          }
      }
    return ConstIterator(last_node, *this);
  }

  /**
   *
   * @return the begin iterator of the first bucket. return as const iterator.
   */
  ConstIterator begin () const
  {
    int index = NON_INDEX;
    for (int i = 0; i < _capacity; i++)
      {
        if (_hash_arr[i].size () > 0)
          {
            index = i;
            break;
          }
      }
    if (index == NON_INDEX)
      {
        return end();
      }
    return ConstIterator (_hash_arr[index].begin (), *this);
  }

  ConstIterator end () const
  {
    return get_last();
  }

  ConstIterator cbegin () const
  {
    return begin();
  }

  ConstIterator cend () const
  {
    return get_last();
  }

  /********************************************
   *****      hash_map_constructors       *****
   ********************************************/
  HashMap ()
  {
    _capacity = INIT_HASH;
    _size = 0;
    _hash_arr = new Bucket[_capacity]{}; //dynamic array in size of capacity

    for (int i = 0; i < _capacity; i++)
      {_hash_arr[i] = Bucket ();}

  }

  HashMap (const std::vector<KeyT> key_vec,const std::vector<ValueT> value_vec)
  {
    if (key_vec.size () != value_vec.size ())
      {
        throw std::length_error (RUN_MSG);
      }
    _size = 0;
    _capacity = INIT_HASH;

    _hash_arr = new Bucket[_capacity]{}; //dynamic array in size of capacity

    for (int i = 0; i < _capacity; i++)
      {_hash_arr[i] = Bucket ();}

    for (unsigned long int i = 0; i < key_vec.size (); i++)
      {insert_initial (key_vec[i], value_vec[i]);}
  }

  HashMap (const HashMap<KeyT, ValueT> &hash_map_to_copy)
  {
    _capacity = hash_map_to_copy._capacity;
    _size = hash_map_to_copy._size;

    _hash_arr = new Bucket[_capacity]{}; //dynamic array in size of capacity

    for (int i = 0; i < _capacity; i++)
      {_hash_arr[i] = Bucket (hash_map_to_copy._hash_arr[i]);}

  }

  /********************************************
   *****      hash_map_destructor       *****
   ********************************************/
  virtual ~HashMap ()
  {
    delete[] _hash_arr;
  }

  /********************************************
   *****      hash_map_functions          *****
   ********************************************/

  /**
   *
   * @return size of the hash map
   */
  int size () const
  {
    return _size;
  }

  /**
 *
 * @return capacity of the hash map
 */
  int capacity () const
  {
    return _capacity;
  }

  /**
   * increase the capacity of the hash map.
   * happens if the load factor is overload.
   */
  void increase_func_map ()
  {

    auto *new_alloc_hash =
        new Bucket[_capacity * LOAD_FAC]{};
    _capacity = _capacity * LOAD_FAC;

    for (int i = 0; i < _capacity / LOAD_FAC; i++)
      {
        int vector_size = _hash_arr[i].size ();
        for (int j = 0; j < vector_size; j++)
          {
            help_insert (new_alloc_hash,
                         _hash_arr[i][j].first,
                         _hash_arr[i][j].second);
          }
      }
    delete[] _hash_arr;
    _hash_arr = new_alloc_hash;
  }

  /**
 * decrease the capacity of the hash map.
 * happens if the current load factor is under the min require load factor.
 */
  void decrease_hash_map ()
  {
    int save_capacity = _capacity;
    while (_capacity > MIN_CAP && get_load_factor () < MIN_LOAD)
      { _capacity = _capacity / LOAD_FAC; }

    auto *new_alloc_hash = new Bucket[_capacity]{};

    for (int i = 0; i < save_capacity; i++)
      {
        for (unsigned long int j = 0; j < _hash_arr[i].size (); j++)
          {
            help_insert (new_alloc_hash,
                         _hash_arr[i][j].first,
                         _hash_arr[i][j].second);
          }
        _hash_arr[i].clear ();
      }

    delete[] _hash_arr;
    _hash_arr = new_alloc_hash;
  }

  /**
   *
   * @param hash_arr get the hash map.
   * @param key the relevant key.
   * @param value the relevant value.
   * @return false if the key exist in the map. if doesnt exist return true
   * and insert the key and the value to the relevant bucket.
   */
  bool help_insert (Bucket *hash_arr,
                    const KeyT &key, const ValueT &value)
  {
    int hash_map_bucket = std::hash<KeyT> () (key);
    int capacity_mod = hash_map_bucket & (_capacity - 1);

    for (unsigned long int j = 0; j < hash_arr[capacity_mod].size (); j++)
      {
        if (key == hash_arr[capacity_mod][j].first)
          { return false;}
      }
    hash_arr[capacity_mod].emplace_back (HashNode (key, value));

    return true;
  }

  /**
 *
 * @param hash_arr get the hash map.
 * @param key the relevant key.
 * @param value the relevant value.
 * @return false if the key exist in the map and change the keys value.
 * if doesnt exist return true and insert the key and the value to the
 * relevant bucket.
 */
  bool help_insert_initial (Bucket *hash_arr,
                            const KeyT &key, const ValueT &value)
  {
    int hash_map_bucket = std::hash<KeyT> () (key);
    int capacity_mod = hash_map_bucket & (_capacity - 1);

    for (unsigned long int j = 0; j < hash_arr[capacity_mod].size (); j++)
      {
        if (key == _hash_arr[capacity_mod][j].first)
          {
            _hash_arr[capacity_mod][j].second = value;
            return false;
          }
      }
    hash_arr[capacity_mod].emplace_back (HashNode (key, value));

    return true;
  }

    /**
  *
  * @param key the relevant key.
  * @param value the relevant value.
  * @return false if the key exist in the map and change the keys value.
  * if doesnt exist return true and insert the key and the value to the
  * relevant bucket.
  */
  bool
  insert_initial (const KeyT &key, const ValueT &value)
  {
    if (!help_insert_initial (_hash_arr, key, value))
      { return false;}

    _size++;
    if (get_load_factor () > MAX_LOAD)
      {
        increase_func_map ();
      }

    return true;
  }

  /**
 *
 * @param key the relevant key.
 * @param value the relevant value.
 * @return false if the key exist in the map. if doesnt exist return true
 * and insert the key and the value to the relevant bucket.
 */
  bool
  insert (const KeyT &key, const ValueT &value)
  {
    if (!help_insert (_hash_arr, key, value))
      { return false;}

    _size++;

    if (get_load_factor () > MAX_LOAD)
      { increase_func_map ();}

    return true;

  }

  /**
   *
   * @return true if the hash map is empty and false if it doesnt.
   */
  bool empty () const
  {
    return _size == 0;
  }

  /**
   *
   * @param key to check if the key is in the hash map.
   * @return true if it does and false if it doesnt.
   */
  bool contains_key (const KeyT &key) const
  {
    int hash_map_bucket = std::hash<KeyT> () (key);
    int capacity_mod = hash_map_bucket & (_capacity - 1);

    for (auto vec_pair : _hash_arr[capacity_mod])
      {
        if (key == vec_pair.first)
          { return true;}
      }
    return false;
  }

  /**
   *
   * @param key to check the value of the relevant key.
   * @return the value of the relevant key and throw an error if there
   * is no matching key.
   */
  ValueT &at (const KeyT &key) const
  {
    if (contains_key (key))
      {
        int hash_map_bucket = std::hash<KeyT> () (key);
        int capacity_mod = hash_map_bucket & (_capacity - 1);

        for (unsigned long int j = 0; j < _hash_arr[capacity_mod].size (); j++)
          {
            if (key == _hash_arr[capacity_mod][j].first)
              { return _hash_arr[capacity_mod][j].second; }
          }
      }

    throw std::out_of_range (RUN_MSG);
  }

  /**
   *
   * @param key to erase.
   * @return true if the key is exist and erased and false if it doesnt exist.
   */
  virtual bool erase (const KeyT &key)
  {
    if (!contains_key (key))
      { return false; }

    int hash_map_bucket = std::hash<KeyT> () (key);
    unsigned long int index;
    int capacity_mod = hash_map_bucket & (_capacity - 1);

    for (unsigned long int j = 0; j < _hash_arr[capacity_mod].size (); j++)
      {
        if (key == _hash_arr[capacity_mod][j].first)
          {
            index = j;
            break;
          }
      }
    _hash_arr[capacity_mod].erase (_hash_arr[capacity_mod].begin () + index);

    _size--;

    if (get_load_factor () < MIN_LOAD)
      { decrease_hash_map (); }

    return true;
  }

  double get_load_factor () const
  {
    return  (double) _size / _capacity;
  }

  /**
   *
   * @param key to check the size of his bucket.
   * @return the size of the bucket that includes the key.
   */
  int bucket_size (const KeyT &key) const
  {
    if (!contains_key (key))
      { throw std::out_of_range (RUN_MSG); }

    int hash_map_bucket = std::hash<KeyT> () (key);
    int capacity_mod = hash_map_bucket & (_capacity - 1);

    return _hash_arr[capacity_mod].size ();
  }

  /**
   *
   * @param key to check the index of the bucket.
   * @return the index of that bucket that contain the key. throw an error
   * if the key doesnt exist.
   */
  int bucket_index (const KeyT &key) const
  {
    if (!contains_key (key))
      { throw std::out_of_range (RUN_MSG); }

    int hash_map_bucket = std::hash<KeyT> () (key);
    return hash_map_bucket & (_capacity - 1);
  }

  /**
   * clear the all hash map from arguments.
   */
  void clear ()
  {
    for (int i = 0; i < _capacity; i++)
      { _hash_arr[i].clear ();}
    _size = 0;
  }

  /**
   *
   * @param key to return his matching value.
   * @return the matching value of garbage value if the key doesnt exist.
   */
  ValueT operator[] (const KeyT &key) const
  {
    if (!contains_key (key))
      {
        ValueT return_value;
        return return_value;
      }
    return at (key);
  }

  /**
   *
   * @param key to insert his matching value.
   * @return insert a garbage value to the relevant place with the key if
   * doesnt exist. and than return the value that needs to change.
   * if the key exist return just the value that needs to change.
   */
  ValueT &operator[] (const KeyT &key)
  {
    if (!contains_key (key))
      {
        insert (key, ValueT ());
        return at (key);
      }
    return at (key);
  }

  /********************************************
   *****      hash_map_operators          *****
   ********************************************/
  HashMap<KeyT, ValueT> &operator= (const HashMap<KeyT, ValueT> &rhs_hash_map)
  {
    if (this == &rhs_hash_map)
      {
        return (*this);
      }

    delete[] this->_hash_arr;

    _capacity = rhs_hash_map._capacity;
    _size = rhs_hash_map._size;

    _hash_arr = new Bucket[_capacity]{};
    for (int i = 0; i < _capacity; i++)
      {
        _hash_arr[i] = Bucket ();
        _hash_arr[i] = rhs_hash_map._hash_arr[i];
      }

    return *this;
  }

  bool operator== (const HashMap<KeyT, ValueT> &rhs_hash_map) const
  {
    if(_size != rhs_hash_map.size())
      {return false;}

    for (int i = 0; i < _capacity; i++)
      {
        for (unsigned long int j = 0; j < _hash_arr[i].size(); j++)
          {
            if ((!rhs_hash_map.contains_key (_hash_arr[i][j].first)) ||
            _hash_arr[i][j].second != rhs_hash_map.at
            (_hash_arr[i][j].first))
              { return false;}
          }
      }
    return true;
  }

  bool operator!= (const HashMap<KeyT, ValueT> &rhs_hash_map) const
  {
    bool equality_flag = (*this == rhs_hash_map);
    return !equality_flag;
  }

  /********************************************
 *****        const_iter_class         *****
 ********************************************/

  class ConstIterator {
   private:

    typename Bucket::iterator hash_node_ptr;
    const HashMap<KeyT, ValueT> *_hash_arr_iter;

   public:
   /********************************************
    *****        const_iter_typedefs       *****
    ********************************************/

    typedef std::pair<KeyT, ValueT> value_type;
    typedef const std::pair<KeyT, ValueT> &reference;
    typedef const std::pair<KeyT, ValueT> *pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    /********************************************
     *****        const_iter_constructor    *****
     ********************************************/

    ConstIterator (const typename Bucket::iterator&
                   ptr,const HashMap<KeyT, ValueT> &hash_map)
        :hash_node_ptr (ptr), _hash_arr_iter (&hash_map)
    {}

    /********************************************
    *****        const_iter_operators       *****
    ********************************************/

    reference operator* () const
    { return *hash_node_ptr; }

    typename Bucket::iterator operator-> ()
    { return hash_node_ptr; }

    friend bool operator== (const ConstIterator &iter_a, const ConstIterator
    &iter_b)
    { return iter_a.hash_node_ptr == iter_b.hash_node_ptr; };
    friend bool operator!= (const ConstIterator &iter_a, const ConstIterator
    &iter_b)
    { return iter_a.hash_node_ptr != iter_b.hash_node_ptr; };

    ConstIterator &operator++ ()
    {
      int bucket_check_index = _hash_arr_iter->bucket_index
          (hash_node_ptr->first);
      Bucket& temp_bucket = _hash_arr_iter->_hash_arr[bucket_check_index];

      for (unsigned long int i = 0; i < temp_bucket.size (); i++)
        {
          if (temp_bucket[i].first == hash_node_ptr->first)
            {
              if ((i + 1) < temp_bucket.size ())
                {
                  hash_node_ptr = temp_bucket.begin () + 1 + i;
                  return *this;
                }
            }
        }
      for (int j = bucket_check_index + 1; j < _hash_arr_iter->_capacity; j++)
        {
          if (_hash_arr_iter->_hash_arr[j].size () > 0)
            {
              hash_node_ptr = _hash_arr_iter->_hash_arr[j].begin ();
              return *this;
            }
        }
      hash_node_ptr = _hash_arr_iter->get_last ().hash_node_ptr;
      return *this;
    }

    const_iterator operator++ (int)
    {
      const_iterator tmp (hash_node_ptr, *_hash_arr_iter);
      ++(*this);
      return tmp;
    }

  };

 protected:

  int _capacity;
  int _size;
  Bucket *_hash_arr;
};

#endif //_HASHMAP_HPP_
