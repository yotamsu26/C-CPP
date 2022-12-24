
// don't change those includes
#include "RSUser.h"
#include "RecommenderSystem.h"

#define MAX_RANK 10
#define MIN_RANK 1
#define RUN_MSG "The movie is not valid"

RSUser::RSUser (const std::string &name, rank_map &ranks,
                std::shared_ptr<RecommenderSystem> &RS)
    : _name (name), _ranks (ranks), _rs (RS)
{}

std::string RSUser::get_name () const
{
  return _name;
}

rank_map RSUser::get_ranks () const
{
  return _ranks;
}

void RSUser::add_movie_to_rs (const std::string &name,
                              int year, const std::vector<double> &features,
                              double rate)
{
  _ranks.insert (std::pair<sp_movie, double>
                     (_rs->add_movie (name, year, features), rate));
}

double RSUser::get_prediction_score_for_movie (const std::string &name,
                                               int year, int k) const
{
  auto sp_temp = _rs->get_movie (name, year);
  if (!sp_temp)
    {
      throw std::runtime_error (RUN_MSG);
    }
  return _rs->predict_movie_score (*this,
                                   sp_temp, k);
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return _rs->recommend_by_content (*this);
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return _rs->recommend_by_cf (*this, k);
}

std::ostream &operator<< (std::ostream &s, RSUser &rs)
{
  s << "name: " << rs._name << std::endl;
  s << *(rs._rs);
  s << std::endl;
  return s;
}