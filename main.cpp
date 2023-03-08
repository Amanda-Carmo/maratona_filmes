#include <iostream>

struct movie{
  int id;
  int time;
  int category;
};

// Greedy heuristic
// marathon of movies - max number of watched movies in a day, respecting the time and categories
// input: number of movies, number of categories

bool compare_time (item a,item b){
  return a.time > b.time;
}

int main(){
    // first line of input: number of movies and categories
    int M; // number of movies
    int C; // number of categories
    
    // second line of input: number of movies per category
    int LC; // limit of movies per category
    
    // read number of movies and categories
    std::cin >> M >> C;
    std::vector<movie> movies;

    // read movies
    for(int i = 0; i < M; i++){
        int start, end, category;

        std::cin >> start >> end >> category;
        movies.push_back({i, end - start, category});

        
    }
}