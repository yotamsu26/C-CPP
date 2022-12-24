
#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_

#include "HashMap.hpp"

#define ERASE_ERR "invalid key"

/**
 * class that inherit  invalid argument class and throw an error.
 */
class InvalidKey : public std::invalid_argument {
 public:
  explicit InvalidKey (const char *input_msg) : std::invalid_argument
                                                     (input_msg)
  {}

  explicit InvalidKey() : std::invalid_argument (ERASE_ERR) {}

};

class Dictionary : public HashMap<std::string, std::string> {
 public:

  /********************************************
 *****      Dictionary_constructors       *****
 ********************************************/

  using HashMap<std::string, std::string>::HashMap;

  ~Dictionary() override = default;
  /**
 *
 * @param key to erase.
 * @return true if the key is exist and erased and throw an error
  * if it doesnt exist.
 */
  bool erase (const std::string &key) override
  {
    if (!HashMap<std::string, std::string>::erase (key))
      {
        throw InvalidKey (ERASE_ERR);
      }
    return true;
  }

  /**
   * insert all the data that the iterator contain.
   * @param begin_iter the iterator to the beginning.
   * @param end_iter the iterator to the end.
   */
  template<typename IterT>
  void update (IterT begin_iter, IterT end_iter)
  {
    while (begin_iter != end_iter)
      {
        insert_initial (begin_iter->first, begin_iter->second);
        begin_iter++;
      }
  }
};

#endif //_DICTIONARY_HPP_
