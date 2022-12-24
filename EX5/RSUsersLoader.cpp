#include "RSUsersLoader.h"


#define FILE_ERR "The file not valid"
#define RUN_MSG "The rate is not valid"
#define MAX_RANK 10
#define MIN_RANK 1
#define NON_RANK "NA"

std::vector<RSUser> RSUsersLoader::create_users_from_file
    (const std::string &users_file_path,
     std::unique_ptr<RecommenderSystem> rs)
noexcept (false)
{
  std::fstream rsl_file;
  rsl_file.open (users_file_path);
  if (!rsl_file)
    {
      throw std::runtime_error (FILE_ERR);
    }

  std::shared_ptr<RecommenderSystem> new_rs = std::move (rs);
  std::vector<RSUser> res_pointer;
  std::string line, temp_name;
  getline (rsl_file, line);

  std::vector<sp_movie> input_movies = insert_movies (line, new_rs);
  rank_map user_map (0, sp_movie_hash, sp_movie_equal);

  while (getline (rsl_file, line))
    {
      std::stringstream linestream (line);
      linestream >> temp_name;
      user_map = insert_users (input_movies, line);
      res_pointer.emplace_back (RSUser (temp_name, user_map,
                                        new_rs));
    }
  return res_pointer;
}

std::vector<sp_movie> RSUsersLoader::insert_movies (const std::string &line,
                                     std::shared_ptr<RecommenderSystem> &rs)
{
  std::vector<sp_movie> input_movies;
  std::stringstream linestream (line);
  std::string temp_name, two_words;
  int temp_year;

  while (linestream >> two_words)
    {
      std::stringstream words (two_words);
      std::getline (words, temp_name, '-');
      words >> temp_year;
      input_movies.emplace_back (rs->get_movie(temp_name,
                                               temp_year));
    }
  return input_movies;
}

rank_map RSUsersLoader::insert_users (const std::vector<sp_movie>&
    input_movies, const std::string &line)
{
  std::string temp_name, temp_rate;
  std::stringstream linestream (line);
  linestream >> temp_name;
  rank_map user_map (0, sp_movie_hash, sp_movie_equal);
  for (const auto & input_movie : input_movies)
    {
      linestream >> temp_rate;
      if (!(temp_rate.compare (NON_RANK)))
        {
          continue;
        }
      else
        {
          double rank = std::stod (temp_rate);
          if(rank > MAX_RANK || rank < MIN_RANK)
            {
              throw std::runtime_error(RUN_MSG);
            }
          user_map.insert (std::pair<sp_movie, double> (input_movie,
                                                        rank));
        }
    }
  return user_map;
}
