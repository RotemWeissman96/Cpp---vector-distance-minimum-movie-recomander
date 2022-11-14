#include "RecommenderSystemLoader.h"
#include <iostream>
#include <string>
#include <sstream>
#define COMA '-'
#define OPEN_FILE_ERR "Could not open file"
#define SCORE_OUT_OF_RANGE "score must be 1-10"
#define MAX_SCORE 10

std::pair<std::string, int> split_movie_title(const std::string& name_year)
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
  for (const auto i : name_year)
  {
    if (still_name)
    {
      if (i == COMA)
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


std::unique_ptr<RecommenderSystem>
    RecommenderSystemLoader::create_rs_from_movies_file
    (const std::string& movies_file_path) noexcept(false)
{
  std::unique_ptr<RecommenderSystem> rs =std::make_unique<RecommenderSystem>();
  std::ifstream in_file (movies_file_path);
  if (!in_file.good())
  {
    in_file.close();
    throw std::runtime_error(OPEN_FILE_ERR);
  }
  std::string line;
  std::string name_year;
  std::vector<double> feature;
  double score = 0;
  std::pair<std::string, int> split_name_year;
  while (std::getline (in_file, line))
  {
    std::istringstream str_strm(line);
    str_strm >> name_year;
    while(str_strm.good())
    {
      str_strm >> score;
      if ( score < 1 || score > MAX_SCORE)
      {
        throw std::out_of_range (SCORE_OUT_OF_RANGE);
      }
      feature.push_back (score);
    }
    split_name_year = split_movie_title (name_year);
    rs->add_movie (split_name_year.first, split_name_year.second, feature);
    feature.clear();
  }
  return rs;
}
//testing