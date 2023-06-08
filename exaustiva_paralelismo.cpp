#include <algorithm>
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Implementação da busca exaustiva, agora adicionando paralelismo

struct Movie {
    int id;
    int start_time;
    int end_time;
    int duration;
    int category;
};

// Função para comparar filmes por hora de término
bool compareMovies(Movie& a, Movie& b) { return a.start_time < b.start_time; }

bool canWatch(Movie& a, vector<Movie>& playlist, vector<int>& max_movies) {
    int count = 1;

    // Se o filme não conflita com os filmes já assistidos
    for (auto& b : playlist) {
        if (!(a.start_time < b.start_time && a.end_time <= b.end_time)) {
            return false;
        }

        if (a.id == b.id) {
            return false;
        }
    }

    // Se a quantidade de filmes da categoria do filme é menor que a quantidade máxima
    if (max_movies[a.category - 1] == 0) {
        return false;
    }

    return true;
}

// Função para calcular o score da playlist
// Implementação usando opm parallel for reduction
int scorePlaylist(vector<Movie>& playlist) {
    int score = 0;
    
    #pragma omp parallel for reduction(+:score)
    for (int i = 0; i < playlist.size(); i++) {
        score += playlist[i].duration;
    }
    return score;
}

// Função de busca exaustiva para determinar filmes que podem ser assistidos
void findMovies(vector<Movie> movies, vector<int> max_movies, vector<Movie> playlist,
    int& best_score, vector<Movie>& best_playlist) {
    int score = 0;
    vector<Movie> movies_temp = movies;

    // Condição de parada:
    if (movies.empty()) {
        score = scorePlaylist(playlist);
        #pragma omp critical
        {
            if (score > best_score) {
                best_score = score;
                best_playlist = playlist;
            }
        }
        return;
    }

    #pragma omp parallel for
    for (int i = 0; i < movies.size(); i++) {
        vector<Movie> playlist_copy = playlist;
        vector<Movie> movies_temp_copy = movies_temp;
        
        Movie current_movie = movies_temp[i];
        movies_temp_copy.erase(movies_temp_copy.begin() + i);

        bool adicionado = false;

        if (canWatch(current_movie, playlist_copy, max_movies)) {
            if (current_movie.end_time > current_movie.start_time) {
                max_movies[current_movie.category - 1] -= 1; // Diminuir a quantidade máxima de filmes da categoria do filme assistido
                playlist_copy.push_back(current_movie);
                adicionado = true;
            }
        }
        findMovies(movies_temp_copy, max_movies, playlist_copy, best_score, best_playlist);

        // Desfazendo as alterações
        if (adicionado) {
            max_movies[current_movie.category - 1] += 1; // Aumentar a quantidade máxima de filmes da categoria do filme assistido
        }
    }
}

int main() {
    // LEITURA DOS INPUTS
    // n = quantidade de filmes, m = quantidade de categorias
    int n, m;
    cin >> n >> m;

    // Quantidade máxima de filmes assistidos de cada categoria
    vector<int> max_movies(m);

    for (int i = 0; i < m; i++) {
        cin >> max_movies[i];
    }

    vector<Movie> movies(n); // Vetor de filmes

    for (int i = 0; i < n; i++) {

        cin >> movies[i].start_time >> movies[i].end_time >> movies[i].category;

        // Se o horário de fim for 0, considerar como 24
        if (movies[i].end_time == 0) {
            movies[i].end_time = 24;
        }

        // Calcular duração do filme
        movies[i].duration = movies[i].end_time - movies[i].start_time;
        movies[i].id = i + 1;
    }

    // Ordenar filmes por horário de término
    sort(movies.begin(), movies.end(), compareMovies);

    // Vetor vazio para marcar filmes assistidos
    vector<Movie> playlist;
    vector<Movie> best_playlist;

    int best_score = 0; // Melhor score

    // Chamada da função de busca exaustiva
    findMovies(movies, max_movies, playlist, best_score, best_playlist);

    // imprimir total de filmes
    cout << best_playlist.size() << "\n";

    // imprimir o score        
    cout << best_score << "\n";

    return 0;
}
