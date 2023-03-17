#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct movie{
    int id;
    int start_time;
    int end_time;
    int time;
    int category;
};

bool compare_movies(movie a, movie b){
    return a.start_time < b.start_time;
}

vector<movie> choose_movies(vector<movie> movies, vector<int> max_movies){
    vector<movie> result;
    vector<int> current_movies(max_movies.size(), 0); // Quantidade atual de filmes assistidos de cada categoria
    int current_time = 0; // Hora atual

    // ordenar os filmes por horário de início
    sort(movies.begin(), movies.end(), compare_movies);

    // Escolher o filme com menor duração para cada horário de início disponível
    for(int t = movies.front().start_time; t <= movies.back().end_time; t++){
        // next_movie = filme com menor duração que começa no horário t e tem a menor duração
        movie next_movie = {-1, -1, -1, -1, -1};

        // Percorrer todos os filmes
        for(int i = 0; i < movies.size(); i++){

            // Se o filme começa no horário t e ainda não foi assistido o máximo de filmes da sua categoria
            if(movies[i].start_time == t && movies[i].end_time > current_time && (next_movie.id == -1 || movies[i].time < next_movie.time)){
                // Escolher o próximo filme disponível que começa no horário t e tem a menor duração
                if(current_movies[movies[i].category-1] < max_movies[movies[i].category-1]){
                    next_movie = movies[i];
                }
            }
        }

        // Se o próximo filme for válido, adicionar ele ao resultado
        if(next_movie.id != -1){
            result.push_back(next_movie);

            // Atualizar a quantidade de filmes assistidos da categoria do filme
            current_movies[next_movie.category-1]++;

            // Atualizar o horário atual
            current_time = next_movie.end_time;
        }
    }

    return result;
}

int main(){
    int n, m;
    cin >> n >> m;

    vector<int> max_movies(m);
    for(int i = 0; i < m; i++){
        cin >> max_movies[i];
    }

    vector<movie> movies(n);
    for(int i = 0; i < n; i++){
        cin >> movies[i].start_time >> movies[i].end_time >> movies[i].category;
        movies[i].time = movies[i].end_time - movies[i].start_time;
        movies[i].id = i + 1;
    }

    vector<movie> result = choose_movies(movies, max_movies);

    // Imprimir resultado
    cout << result.size() << endl;
    for(int i = 0; i < result.size(); i++){
        cout << result[i].id << " ";
    }
    cout << endl;

    return 0;
}
