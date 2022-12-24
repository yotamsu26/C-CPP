
#ifndef USERFACTORY_H
#define USERFACTORY_H

#include "RecommenderSystem.h"


class RSUsersLoader
{
private:

  /**
 *
 * @param line the current line.
 * @param rs the recommended system.
 * @return vector of the movies that ranked by the users.
 */
  static std::vector<sp_movie> insert_movies
  (const std::string &line,std::shared_ptr<RecommenderSystem> &rs);


  /**
 *
 * @param input_movies vector of the movies that ranked by the users.
 * @param line the current line.
 * @return rank map of a specific user.
 */
  static rank_map insert_users (const std::vector<sp_movie>& input_movies,
                         const std::string &line);


public:
    RSUsersLoader() = delete;
    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file of the users and their
     * movie ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static std::vector<RSUser> create_users_from_file(const std::string&
    users_file_path, std::unique_ptr<RecommenderSystem> rs)
    noexcept(false);

};


#endif //USERFACTORY_H
