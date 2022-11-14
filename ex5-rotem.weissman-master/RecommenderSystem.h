#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H
#include <set>
#include <map>
#include "RSUser.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

#define NOT_SEEN -1

template< typename T >
struct shared_ptr_comparator {
    bool operator()
            (const std::shared_ptr<T> &a, const std::shared_ptr<T> &b) const {
      return std::less<T>()(*a, *b);
    }
};

class RSUser;
class RecommenderSystem
{
  private:
    std::map< sp_movie , std::vector<double>, shared_ptr_comparator<Movie> >
        data;
    std::set<std::shared_ptr<Movie>> get_all_unwatched(const RSUser& user)
                                                                         const;
    std::set<std::shared_ptr<Movie>> get_all_watched(const RSUser& user) const;
  public:
    RecommenderSystem() { };

    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie
        (const std::string& name,int year,const std::vector<double>& features);


    /**
     * a function that calculates the movie with highest score based on movie
     * features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const RSUser& user);

    /**
     * a function that calculates the movie with highest predicted score
     * based on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const RSUser& user, int k);


    /**
     * Predict a user rating for a movie given argument using item cf
     * procedure with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const RSUser &user, const sp_movie &movie,
												  int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name user_name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;


    friend std::ostream& operator <<(std::ostream& os, const RecommenderSystem&
    rs);

};


#endif //RECOMMENDERSYSTEM_H
//testing