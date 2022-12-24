#include "RecommenderSystem.h"

#define NORM_CAL(NUM1, NUM2, NUM3) (NUM1 / (sqrt (NUM2) * sqrt (NUM3)))

RecommenderSystem::RecommenderSystem () : _rs_data (sp_movie_order)
{}

sp_movie
RecommenderSystem::recommend_by_content (const RSUser &user)
{
  std::vector<double> avg_vec = get_avg_vec (user);
  std::vector<double> favorites;
  favorites.resize (user.get_ranks ().size ());
  favorites = get_favorites (user, _rs_data, avg_vec);
  return get_movie_according_to_norm (user, favorites, _rs_data);
}

sp_movie
RecommenderSystem::get_movie_according_to_norm (const RSUser &user,
                             std::vector<double> favorites, data &rs_data)
{
  double norm1 = 0, norm2 = 0, norm3 = 0, max_match = -__DBL_MAX__;
  sp_movie best_match = nullptr;
  for (const auto &temp_movie: rs_data)
    {
      if (user.get_ranks ().find (temp_movie.first) ==
          user.get_ranks ().end ())
        {
          auto get_unwatched_freq = rs_data[temp_movie.first];
          for (long unsigned int i = 0; i < favorites.size (); i++)
            {
              norm1 += favorites[i] * get_unwatched_freq[i];
              norm2 += favorites[i] * favorites[i];
              norm3 += get_unwatched_freq[i] * get_unwatched_freq[i];
            }
          if (norm2 == 0)
            {
              return temp_movie.first;
            }
          else if (NORM_CAL(norm1, norm2, norm3) > max_match)
            {
              max_match = NORM_CAL(norm1, norm2, norm3);
              best_match = temp_movie.first;
            }
        }
    }
  return best_match;
}

std::vector<double>
RecommenderSystem::get_favorites (const RSUser &user, data &rs_data,
                                  const std::vector<double> &avg_vec)
{
  std::vector<double> func_favorites, temp;
  func_favorites.resize (rs_data.begin ()->second.size ());
  int index = 0;
  for (const auto &temp_movie: user.get_ranks ())
    {
      for (long unsigned int i = 0; i < rs_data.begin ()->second.size (); i++)
        {
          func_favorites[i] +=
              avg_vec[index] * rs_data[temp_movie.first][i];

        }
      index++;
    }
  return func_favorites;
}

std::vector<double> RecommenderSystem::get_avg_vec (const RSUser &user)
{
  double sum = 0;
  int divided = 0, index = 0;
  std::vector<double> new_vec;
  new_vec.resize (user.get_ranks ().size ());
  for (const auto &loop: user.get_ranks ())
    {
      sum += loop.second;
      divided++;
    }
  sum = sum / divided;

  for (const auto &loop: user.get_ranks ())
    {
      new_vec[index] = loop.second - sum;
      index ++;
    }
  return new_vec;
}

double
RecommenderSystem::predict_movie_score (const RSUser &user, const
sp_movie &movie, int k)
{
  best_mov_map best_match_movies (sp_pair_equal);
  double predict_num;

  for (const auto
        &un_seen_movie: user.get_ranks ())
    {
      if (un_seen_movie.second != 0)
        {
          predict_num = get_movie_predict (un_seen_movie.first,
                                           movie, _rs_data);
          best_match_movies
          [std::make_pair (predict_num, un_seen_movie.first)] =
              un_seen_movie.second;
        }
    }
  double upper_sum = 0, down_sum = 0;

  for (auto iter = best_match_movies.rbegin ();
       iter != best_match_movies.rend (); ++iter)
    {
      if (k == 0)
        {
          break;
        }
      k--;
      down_sum += iter->first.first;
      upper_sum += iter->first.first * user.get_ranks ()[iter->first.second];
    }
  return upper_sum / down_sum;
}

double RecommenderSystem::get_movie_predict (const sp_movie &check_movie,
                          const sp_movie &predict_movie, data &rs_data)
{
  auto norm_movie1 = rs_data[check_movie];
  auto norm_movie2 = rs_data[predict_movie];

  double norm1 = 0, norm2 = 0, norm3 = 0;
  for (long unsigned int i = 0; i < norm_movie1.size (); i++)
    {
      norm1 += norm_movie1[i] * norm_movie2[i];
      norm2 += norm_movie1[i] * norm_movie1[i];
      norm3 += norm_movie2[i] * norm_movie2[i];
    }
  return NORM_CAL(norm1, norm2, norm3);
}

sp_movie RecommenderSystem::add_movie (const std::string &name,
                                       int year, const std::vector<double>
                                       &features)
{
  sp_movie mov_p = std::make_shared<Movie> (Movie (name, year));
  _rs_data.insert (std::pair<sp_movie, std::vector<double>>
                       (mov_p, features));

  return mov_p;
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{

  sp_movie ret_pointer = std::make_shared<Movie> (Movie (name, year));
  auto temp = _rs_data.find (ret_pointer);
  if (temp == _rs_data.end ())
    {
      return nullptr;
    }
  return temp->first;
}

sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  sp_movie best_recommend_movie = nullptr;
  double max_prediction = 0, temp;
  for (const auto &un_seen_movie: _rs_data)
    {
      if(user.get_ranks ().find (un_seen_movie.first) ==
             user.get_ranks ().end ())
        {
          temp = predict_movie_score (user, un_seen_movie.first, k);
          if (temp > max_prediction)
            {
              best_recommend_movie = un_seen_movie.first;
              max_prediction = temp;
            }
        }
    }
  return best_recommend_movie;
}

std::ostream &operator<< (std::ostream &s, const RecommenderSystem &rs)
{
  for (const auto &it: rs._rs_data)
    {
      s << *(it.first);
    }
  return s;
}