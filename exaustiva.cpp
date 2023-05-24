#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Implementação da busca exaustiva

struct movie{
    int id;
    int start_time;
    int end_time;
    int time;
    int category;
};

// Função para comparar filmes por hora de término
bool compareMovies(movie a, movie b) {
    return a.end_time < b.end_time;
}

// Função de busca exaustiva para determinar filmes que podem ser assistidos
vector<movie> findMaxMovies(vector<movie>& movies, vector<int>& max_movies, vector<int>& used_movies, int category, int last_end_time){
    // Condição de parada: todas categorias foram verificadas
    if(category > max_movies.size()){
        return vector<movie>();
    }

    
}


int main(){

    // LEITURA DOS INPUTS
    // n = quantidade de filmes, m = quantidade de categorias
    int n, m;
    cin >> n >> m;

    // Quantidade máxima de filmes assistidos de cada categoria
    vector<int> max_movies(m); 

    for(int i = 0; i < m; i++){
        cin >> max_movies[i];
    }

    vector<movie> movies(n); // Vetor de filmes

    for(int i = 0; i < n; i++){

        cin >> movies[i].start_time >> movies[i].end_time >> movies[i].category;
        
        // Se o horário de fim for 0, considerar como 24
        if(movies[i].end_time == 0){
            movies[i].end_time = 24;
        }

        // Calcular duração do filme
        movies[i].time = movies[i].end_time - movies[i].start_time;
        movies[i].id = i + 1;
    }

    // Ordenar filmes por horário de término
    sort(movies.begin(), movies.end(), compareMovies);

    // Vetor para marcar filmes assistidos
    vector<int> used_movies(n+1, 0);



    return 0;
}