
#include "Movie.h"

#define HASH_START 17
#define RES_MULT 31

/**
 * hash function used for a unordered_map (implemented for you)
 * @param movie shared pointer to movie
 * @return an integer for the hash map
 */
std::size_t sp_movie_hash (const sp_movie &movie)
{
  std::size_t res = HASH_START;
  res = res * RES_MULT + std::hash<std::string> () (movie->get_name ());
  res = res * RES_MULT + std::hash<int> () (movie->get_year ());
  return res;
}

/**
 * equal function used for an unordered_map (implemented for you)
 * @param m1
 * @param m2
 * @return true if the year and name are equal else false
 */

bool sp_movie_equal (const sp_movie &m1, const sp_movie &m2)
{
  return !(*m1 < *m2) && !(*m2 < *m1);
}

/**
 *
 * @param p1
 * @param p2
 * @return true if rank of p1 is bigger than the rank of p2.
 */
bool sp_pair_equal (const std::pair<double, sp_movie>& p1,
                    const std::pair<double, sp_movie>& p2)
{
  return p1.first < p2.first;
}

/**
 *
 * @param m1
 * @param m2
 * @return true if the movie that m1 contain is smaller the m2.
 */
bool sp_movie_order (const sp_movie &m1, const sp_movie &m2)
{
  return *m1 < *m2;
}

Movie::Movie (const std::string &name, int year)
{
  _name = name;
  _year = year;
}

std::string Movie::get_name () const
{
  return _name;
}

int Movie::get_year () const
{
  return _year;
}

bool Movie::operator< (const Movie &comp_movie) const
{
  if (_year == comp_movie.get_year ())
    {
      if (_name < comp_movie.get_name ())
        {
          return true;
        }
    }
  return _year < comp_movie.get_year ();
}

std::ostream &operator<< (std::ostream &s, Movie &m)
{
  s << m.get_name () << " (" << m.get_year () << ")" << std::endl;
  return s;
}
