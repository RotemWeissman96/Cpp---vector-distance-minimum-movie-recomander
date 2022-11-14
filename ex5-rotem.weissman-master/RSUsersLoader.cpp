//
// Created by rotem on 01/06/2022.
//
#include "RSUsersLoader.h"
#define SCORE_OUT "score must be 1-10"
#define KOMA '-'
#define OPEN_FILE "Could not open file"
#define NOT_WATCHED "NA"
#define SCORE_MAX 10

std::pair<std::string, int> split_title(const std::string& name_year)
/**
 * split a string containing the movie name and year into a pair of name and
 * year
 * @param name_year
 * @return <name, year>
 */
{
  bool still_name = true;
  std::string s_year;
  std::string name;
  for (auto i : name_year)
  {
    if (still_name)
    {
      if (i == KOMA)
      {
        still_name = false;
      }
      else
      {
        name += i;
      }
    }
    else
    {
      s_year += i;
    }
  }
  int year = std::stoi (s_year);
  std::pair<std::string, int> ret(name, year);
  return ret;
}

std::vector<RSUser> RSUsersLoader::create_users_from_file(const std::string&
      users_file_path, std::unique_ptr<RecommenderSystem> rs) noexcept(false)
{
  std::vector<RSUser> users;
  std::string movie_name;
  std::pair<std::string, int> movie_name_year;
  std::ifstream in_file (users_file_path);
  std::string name; std::string s_score; std::string line;
  std::vector<sp_movie> movies; double score;
  if (!in_file.good())
  {
    in_file.close();
    throw std::runtime_error(OPEN_FILE);
  }
  std::shared_ptr<RecommenderSystem> sp_rs = std::move (rs);
  std::getline (in_file, line);
  std::istringstream str_strm(line);
  while(str_strm.good())
  {
    str_strm >> movie_name;
    movie_name_year = split_title (movie_name);
    movies.push_back (sp_rs->get_movie (movie_name_year.first,
                                     movie_name_year.second));
  }
  while (std::getline (in_file, line))
  {
    std::istringstream str_strm2(line);
    rank_map pref_map(1, sp_movie_hash, sp_movie_equal);;
    str_strm2 >> name;
    for (size_t i = 0; i < movies.size(); i++)
    {
      str_strm2 >> s_score;

      if (s_score == NOT_WATCHED)
      {
        pref_map.insert (std::pair<sp_movie , double>(movies[i], -1));
      }
      else
      {
        score = std::stod(s_score);
        if ( score < 1 || score > SCORE_MAX)
        {
          throw std::out_of_range (SCORE_OUT);
        }
        sp_movie next_movie = movies[i];
        pref_map[next_movie] = score;
      }
    }
    users.push_back (RSUser(name, pref_map, sp_rs));
  }
  return users;
}
//testing