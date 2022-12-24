#include "RecommenderSystemLoader.h"

#define FILE_ERR "The file not valid"
#define RUN_MSG "The rate is not valid"
#define MAX_RANK 10
#define MIN_RANK 1

std::unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file
(const std::string &movies_file_path)
noexcept (false)
{
  std::fstream rsl_file;
  rsl_file.open (movies_file_path);
  if (!rsl_file)
    {
      throw std::runtime_error (FILE_ERR);
    }
  RecommenderSystem rs;

  std::string line;
  while (getline (rsl_file, line))
    {
      std::vector<double> input_vector;
      std::istringstream linestream (line);
      std::string temp_name;

      getline (linestream, temp_name, '-');
      int temp_year;
      double temp_feature;

      linestream >> temp_year;
      while (linestream >> temp_feature)
        {
          if(temp_feature > MAX_RANK || temp_feature < MIN_RANK)
            {
              throw std::runtime_error(RUN_MSG);
            }
          input_vector.emplace_back (temp_feature);
        }
      rs.add_movie(temp_name, temp_year,input_vector);
    }
  return std::make_unique<RecommenderSystem> (rs);
}
