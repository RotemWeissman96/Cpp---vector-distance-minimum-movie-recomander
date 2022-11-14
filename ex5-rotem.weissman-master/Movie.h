
#ifndef MOVIE_H
#define MOVIE_H
#include <iostream>
#include <vector>
#include <memory>

#define HASH_START 17



class Movie;

typedef std::shared_ptr<Movie> sp_movie; // define your smart pointer

/**
 * those declarations and typedefs are given to you and should be used in
 * the exercise
 */
typedef std::size_t (*hash_func)(const sp_movie& movie);
typedef bool (*equal_func)(const sp_movie& m1,const sp_movie& m2);
std::size_t sp_movie_hash(const sp_movie& movie);
bool sp_movie_equal(const sp_movie& m1,const sp_movie& m2);

class Movie
{
 private:
    std::string movie_name;
    int year;

 public:
    /**
     * constructor
     * @param name: user_name of movie
     * @param year: year it was made
     */
    Movie(const std::string& name, int year);

    /**
     * returns the user_name of the movie
     * @return const ref to user_name of movie
     */
    std::string get_name() const {return this->movie_name;}

    /**
     * returns the year the movie was made
     * @return year movie was made
     */
    int get_year() const {return this->year;}

	/**
     * operator< for two movies
     * @param rhs: right hand side
     * @param lhs: left hand side
     * @return returns true if (rhs.year) < lhs.year or (rhs.year == lhs
     * .year & rhs.user_name == lhs.user_name) else return false
     */
    bool operator < (const Movie& other_movie) const;

    /**
     * operator<< for movie
     * @param os ostream to output info with
     * @param movie movie to output
     */
    friend std::ostream& operator <<(std::ostream& os, const Movie& movie);
};


#endif //MOVIE_H
//testing