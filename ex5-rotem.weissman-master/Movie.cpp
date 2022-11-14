#include "Movie.h"
#define HASH_START 17
#define RES_MULT 31

/**
 * hash function used for a unordered_map (implemented for you)
 * @param movie shared pointer to movie
 * @return an integer for the hash map
 */
std::size_t sp_movie_hash(const sp_movie& movie){
    std::size_t res = HASH_START;
    res = res * RES_MULT + std::hash<std::string>()(movie->get_name());
    res = res * RES_MULT + std::hash<int>()(movie->get_year());
    return res;
}

/**
 * equal function used for an unordered_map (implemented for you)
 * @param m1
 * @param m2
 * @return true if the year and user_name are equal else false
 */
bool sp_movie_equal(const sp_movie& m1,const sp_movie& m2){
    return !(*m1 < *m2) && !(*m2 < *m1);
}

Movie::Movie(const std::string& name, int year)
{ // simple constructor
  this->year = year;
  this->movie_name = name;
}

bool Movie::operator < (const Movie& other_movie) const
/**
 * decide witch movie is "smaller" by the year and then by name
 * @param other_movie
 * @return true if lhs is smaller
 */
{
  if (this->year < other_movie.year)
  {
    return true;
  }
  if (this->year == other_movie.year &&
      0 > this->movie_name.compare (other_movie.movie_name))
  {
    return true;
  }
  return false;
}

std::ostream& operator <<(std::ostream& os, const Movie& movie)
/**
 * prints the Movie class to cout
 * @param os
 * @param movie
 * @return
 */
{
  std::cout << movie.movie_name << " (" << movie.year << ")" << std::endl;
  return os;
}
//testing