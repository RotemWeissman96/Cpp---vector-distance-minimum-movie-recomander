#include "RecommenderSystem.h"
#include "Movie.h"
#include <cmath>

/************************************/
//           helpers
/************************************/

double vector_distance(const std::vector<double>& vec1,
                       const std::vector<double>& vec2)
/**
 * calculate the diustance between two vectors
 * @param vec1
 * @param vec2
 * @return the distance
 */
{
  double up = 0;
  double down1 = 0;
  double down2 = 0;
  for (size_t i = 0; i < vec1.size(); i++)
  {
    up += vec1[i]*vec2[i];
  }
  for (size_t i = 0; i < vec1.size(); i++)
  {
    down1 += vec1[i]*vec1[i];
  }
  down1 = sqrt(down1);
  for (size_t i = 0; i < vec2.size(); i++)
  {
    down2 += vec2[i]*vec2[i];
  }
  down2 = sqrt(down2);
  return up/(down1*down2);
}

std::set<sp_movie> RecommenderSystem::get_all_watched
                                    (const RSUser& user) const
/**
 * get all watched movies form a user
 * @param user
 * @return a set of all watched movies of user
 */
{
  std::set<sp_movie> ret_set{};
  for (const auto& itr: user.get_ranked())
  {
    if (itr.second != -1)
    {
      ret_set.insert(itr.first);
    }
  }
  return ret_set;
}

std::set<sp_movie> RecommenderSystem::get_all_unwatched
                                    (const RSUser& user) const
/**
 * get all unwatched movied of user, get them from the rs
 * @param user
 * @return all unwatched movies
 */
{
  std::set<sp_movie> ret_set{};
  for (const auto&  itr: this->data)
  {
    if (user.get_ranked().find(itr.first) == user.get_ranked().end())
    { // if the movie is not in the user list
      ret_set.insert (itr.first);
    }
    else{
      if(user.get_ranked()[itr.first] == -1)
      {
        ret_set.insert (itr.first);
      }
    }
  }
  return ret_set;
}


bool comp_by_value(const std::pair<sp_movie , double> &pair1,
                   const std::pair<sp_movie , double> &pair2)
/**
 * compair two shared pointer of movie by the value
 * @param pair1
 * @param pair2
 * @return true if lhs is smaller
 */
{
  return pair1.second < pair2.second;
}

/************************************/
//         main functions
/************************************/


sp_movie RecommenderSystem::add_movie(const std::string& name,int year,
                                      const std::vector<double>& features)
{
  sp_movie movie = std::make_shared<Movie>(name, year);
  this->data[movie] = features;
  return movie;
}

sp_movie RecommenderSystem::recommend_by_content(const RSUser& user)
{
  double average = 0;
  std::set<sp_movie> seen_set = this->get_all_watched (user);
  std::set<sp_movie> unseen_set = this->get_all_unwatched (user);
  for (const auto&  itr: seen_set)
  {
    average += user.get_ranked()[itr];
  }
  average /= (double)seen_set.size();

  std::vector<double> pref (this->data.begin()->second.size(), 0);
  for (const auto&  itr: seen_set)
  {
    for (size_t i = 0; i < pref.size(); i++)
    {
      pref[i] += (user.get_ranked()[itr] - average) *
          this->data[itr][i];
    }
  }

  double max = -1; double dist = 0;
  sp_movie recommended;
  for (const auto&  itr: unseen_set)
  {
    dist = vector_distance (pref, this->data[itr]);
    if (max == -1 || max < dist)
    {
      max = dist;
      recommended = itr;
    }
  }
  return recommended;
}

sp_movie RecommenderSystem::recommend_by_cf(const RSUser& user, int k)
{
  std::set<sp_movie> unseen_set = this->get_all_unwatched (user);
  double max_sim = -1; sp_movie ret_movie;
  double prediction = 0;

  for (const auto& unseen_itr : unseen_set)
  { // check all unseen movies
    prediction = this->predict_movie_score (user, unseen_itr, k);
    if (prediction > max_sim)
    {
      max_sim = prediction;
      ret_movie = unseen_itr;
    }
  }
  return ret_movie;
}

double RecommenderSystem::predict_movie_score
      (const RSUser &user, const sp_movie &movie, int k)
{
  std::set<sp_movie> seen_set = this->get_all_watched (user);
  std::vector <std::pair<sp_movie , double>> max_similar_vec{};
  double up = 0; double down = 0;
  double cur_dist = 0;
  for (const auto& seen_itr : seen_set)
  { // go through all seen movies and calculate the distance
    cur_dist = vector_distance(this->data[seen_itr],
                               this->data[movie]);
    max_similar_vec.push_back (std::pair<sp_movie , double>
                                   (seen_itr, cur_dist));
  }
  std::sort(max_similar_vec.begin(), max_similar_vec.end(),comp_by_value);

  auto sorted_itr = max_similar_vec.begin();
  while (sorted_itr != max_similar_vec.end())
  { // get only the k most similar movies
    if (0 < max_similar_vec.size()-k)
    {
      sorted_itr = max_similar_vec.erase(sorted_itr);
    }
    else
    {
      sorted_itr++;
    }
  }
  for (const auto &final_itr : max_similar_vec)
  { // calculate the predicted score for this unseen movie based on k seen
    down += final_itr.second;
    up += final_itr.second * user.get_ranked()[final_itr.first];
  }
  return up/down;
}

sp_movie RecommenderSystem::get_movie(const std::string &name, int year) const
{
  sp_movie movie = std::make_shared<Movie>(name, year);
  const auto ret_movie = this->data.find (movie);
  if (ret_movie == this->data.end())
  {
    return nullptr;
  }
  return ret_movie->first;
}

std::ostream& operator <<(std::ostream& os, const RecommenderSystem& rs)
{
  for(const auto &itr : rs.data)
  {
    std::cout << *(itr.first);
  }
  return os;
}
//testing