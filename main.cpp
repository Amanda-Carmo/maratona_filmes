#include <iostream>
#include <vector>
#include <algorithm>

// INPUT FORMAT
// 10 4
// 1 3 1 2 
// 11 13 3
// 14 15 3
// 10 16 2
// 10 14 1
// 11 17 2
// 11 14 3
// 13 15 3
// 14 15 1
// 12 16 4
// 12 13 4

struct movie{
  int id;
  int time;
  int category;
};

// Greedy heuristic
// marathon of movies - max number of watched movies in a day, respecting the time and categories
// input: number of movies, number of categories

// movies are sorted by time - biggest first
bool compare_time (movie a,movie b){
  return a.time > b.time;
}


int main(){

    // first line of input: number of movies and categories
    int M; // number of movies
    int C; // number of categories
    
    // read number of movies and categories
    std::cin >> M >> C;
    
    // second line of input: time and category of each movie
    std::vector<int> categories(C); // vector of categories
    
    // read number of movies per category
    for (int i = 0; i < C; i++){
        std::cin >> categories[i];
    }

    // counter of movies per category
    // create a vector of size C, initialized with all values as 0
    std::vector<int> counter(C, 0);

    // read movies and sort them by time
    std::vector<movie> movies(M);

    for (int i = 0; i < M; i++){
      // calculate time using hour of beggining and end
      int start, end, category_id;

      std::cin >> start >> end >> category_id;

      // considering movies that ends at midnight
      if (end == 0){
        end = 24;
      }

      movies[i].id = i;
      movies[i].time = end - start;
      movies[i].category = category_id;
   
    }

    // // output counter of movies per category
    // for (int i = 0; i < C; i++){
    //   std::cout << counter[i] << " ";
    // }

    // sort movies by time - biggest first
    std::sort(movies.begin(), movies.end(), compare_time);

    // output movies in order
    // for (int i = 0; i < M; i++){
    //   std::cout << movies[i].id << " " << movies[i].time << " " << movies[i].category << std::endl;
    // }

    // creata marathon of movies
    std::vector<movie> marathon;

    // add movies to marathon

    int hours = 0;

    for (int i = 0; i < M; i++){
      // check if number of movies per category exceeded the limit
      // check if passed midnight

      if ((counter[movies[i].category - 1] < categories[movies[i].category - 1]) && (movies[i].time >= 0) && (hours + movies[i].time <= 24)){

        marathon.push_back(movies[i]);

        // update counter of movies per category
        counter[movies[i].category -1 ]++;

        // update hours
        hours += movies[i].time;
      }
    }

    // output counter of movies per category
    for (int i = 0; i < C; i++){
      std::cout << counter[i] << " " << std::endl;
    }

    // output marathon
    for (int i = 0; i < marathon.size(); i++){
      std::cout << marathon[i].id << " " << marathon[i].time << " " << marathon[i].category << std::endl;
    }

}