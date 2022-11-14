#ifndef USER_H
#define USER_H
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include "Movie.h"
#include "RecommenderSystem.h"

class RecommenderSystem;
typedef std::unordered_map<sp_movie, double, hash_func, equal_func> rank_map;

class RSUser
{
 private:
    std::string user_name;
    rank_map pref_map;
    std::shared_ptr<RecommenderSystem> rs;

 public:
	/**
	 * Constructor for the class
	 */
	RSUser(const std::string& name,const rank_map& map,
           std::shared_ptr<RecommenderSystem>& rs);

	/**
	 * a getter for the user's user_name
	 * @return the username
	 */
    std::string get_name() const {return this->user_name;}

	/**
	 * function for adding a movie to the DB
	 * @param movie a Movie object
	 * @param features a vector of the movie's features
	 * @param rate the user rate for this movie
	 * @return true for success, false otherwise
	 */
	void add_movie_to_rs(const std::string &name, int year,
                         const std::vector<double> &features,
                         double rate);


    /**
     * a getter for the ranks map
     * @return
     */
    rank_map get_ranked()  const {return this->pref_map;}

	/**
	 * returns a recommendation according to the movie's content
	 * @return recommendation
	 */
	sp_movie get_recommendation_by_content() const;

	/**
	 * returns a recommendation according to the similarity
	 * recommendation method
	 * @param k the number of the most similar movies to calculate by
	 * @return recommendation
	 */
    sp_movie get_recommendation_by_cf(int k) const;

	/**
	 * predicts the score for a given movie
	 * @param name the user_name of the movie
	 * @param year the year the movie was created
	 * @param k the parameter which represents the number of
	 * the most similar movies to predict the score by
	 * @return predicted score for the given movie
	 */
	double get_prediction_score_for_movie
                              (const std::string& name, int year, int k) const;

	/**
	 * output stream operator
	 * @param os the output stream
	 * @param user the user
	 * @return output stream
	 */

    friend std::ostream& operator <<(std::ostream& os, const RSUser& user);
};



#endif //USER_H
//testing