
#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H

#include "RSUser.h"
#include "map"
#include <cmath>
#include "fstream"
#include "sstream"
#include "Movie.h"

typedef std::map<sp_movie , std::vector<double>, order_func> data;

typedef std::map<std::pair<double, sp_movie>, double, pair_func> best_mov_map;

class RecommenderSystem {
 private:

  /**
 *
 * @param user the current user.
 * @return the vector with the average ranks. after calculating it.
 */
  static std::vector<double> get_avg_vec (const RSUser &user);

  /**
 *
 * @param check_movie the movie that we check.
 * @param predict_movie the movie to compare with.
 * @param rs_data the data of the recommended system.
 * @return the prediction of specific movie.
 */
  static double get_movie_predict (const sp_movie &check_movie,
                            const sp_movie &predict_movie, data &rs_data);

  /**
 *
 * @param user the current user.
 * @param favor the favorites vector according to characters.
 * @param rs_data the data of the recommended system.
 * @return the right movie with the best similarity.
 */
  static sp_movie
  get_movie_according_to_norm (const RSUser &user, std::vector<double> favor,
                               data &rs_data);

  /**
 *
 * @param user the current user.
 * @param rs_data the data of the recommended system.
 * @param avg_vec the average vector according to the ranked movies.
 * @return vector of character similarity.
 */
  static std::vector<double>
  get_favorites (const RSUser &user, data &rs_data, const std::vector<double> &
  avg_vec);

  data _rs_data;

 public:

  explicit RecommenderSystem ();
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie add_movie (const std::string &name, int year, const
  std::vector<double> &features);

  /**
   * a function that calculates the movie with highest score based
   * on movie features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_content (const RSUser &user);

  /**
   * a function that calculates the movie with highest predicted score based
   * on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);

  /**
   * Predict a user rating for a movie given argument using item cf procedure
   * with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;

  friend std::ostream &
  operator<< (std::ostream &s, const RecommenderSystem &rs);

};

#endif //RECOMMENDERSYSTEM_H
