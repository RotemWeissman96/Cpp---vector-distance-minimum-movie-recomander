
// don't change those includes
#include "RSUser.h"

#include "RecommenderSystem.h"


// implement your cpp code here
RSUser::RSUser(const std::string& name,const rank_map& map,
               std::shared_ptr<RecommenderSystem>& rs)
{
  this->rs = rs;
  this->user_name = name;
  this->pref_map = map;
}


void RSUser::add_movie_to_rs(const std::string &name, int year,
                     const std::vector<double> &features,
                     double rate)
{
  sp_movie p_movie = this->rs->get_movie (name, year);
  if (p_movie == nullptr)
  {
    p_movie = this->rs->add_movie (name, year, features);
  }
  this->pref_map.insert(std::pair<sp_movie , double> (p_movie, rate));
}

sp_movie RSUser::get_recommendation_by_content() const
{
  return this->rs->recommend_by_content (*this);
}

sp_movie RSUser::get_recommendation_by_cf(int k) const
{
  return this->rs->recommend_by_cf(*this, k);
}

double RSUser::get_prediction_score_for_movie(
                              const std::string& name, int year, int k) const
{
  std::shared_ptr<Movie> movie = std::make_shared<Movie>(name, year);
  return this->rs->predict_movie_score (*this, movie, k);
}

std::ostream& operator <<(std::ostream& os, const RSUser& user)
{
  std::cout << "name: " << user.get_name() << std::endl;
  std::cout << *user.rs << std::endl;
  return os;
}
//testing