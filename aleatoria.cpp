#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>
#include <map>

using namespace std;

struct movie{
    int id;
    int start_time;
    int end_time;
    int time;
    int category;
};

struct schedule{
    int start_time;
    vector<movie> movies;
    int total_movies;
};


bool compare_movies(movie a, movie b){
    return a.start_time < b.start_time;
}


vector<movie> choose_movies(vector<movie> movies, vector<int> max_movies, vector<schedule> schedules){
    // Gerador de números aleatórios
    default_random_engine generator; 
    // Semente para gerar números aleatórios
    generator.seed(10); 

    vector<movie> result;
    vector<int> current_movies(max_movies.size(), 0); // Quantidade atual de filmes assistidos de cada categoria
    int current_time = 0; // Hora atual

    // ordenar os filmes por horário de início
    sort(movies.begin(), movies.end(), compare_movies);

    // Percorrer schedules
    for(int s = 0; s < schedules.size(); s++){
        // sortear números aleatórios entre 0 e 1
        uniform_real_distribution<double> distribution(0.0, 1.0);
        
        // cout << "Horário atual0: " << current_time << endl;
        // para cada horário que um filme se inicia, next_movie é o filme com menor duração
        movie next_movie = {-1, -1, -1, -1, -1}; // Filme com menor duração para o horário atual

        // Aleatorização: colocar na maratona filme com mesmo horário de início, porém aleatório

        // Percorrer filmes 
        for(int i = 0; i < movies.size(); i++){
            // Se o filme tem o mesmo horário de início que o horário atual e o horário de fim é maior que o horário atual e o horário de início é menor que o horário de fim
            if(movies[i].start_time == current_time && movies[i].end_time > current_time && (movies[i].start_time < movies[i].end_time)){
                
                // 25% de chance de adicionar o filme na maratona de forma aleatoria, respeitando tempo                
                if(distribution(generator) > 0.75){ 

                    // ver quantos filmes com o mesmo horaio de inicio ja foram adicionados
                    int n_movies = schedules[s].total_movies;                                    
                                                          
                    // pegar um filme aleatório que começa no horário atual e que ainda não foi assistido
                    uniform_int_distribution<int> distribution(0, n_movies-1);
                    int p = distribution(generator);

                    // Se o filme aleatório não foi assistido ainda
                    if(next_movie.id == -1){
                        // Se a categoria do filme não atingiu o limite de filmes assistidos
                        if(current_movies[movies[p].category-1] < max_movies[movies[p].category-1]){
                            next_movie = movies[p];

                        }                        
                    }                             
                }
            }        
        }


        // Percorrer filmes com horário de início igual ou maior ao horário atual
        // pegar filme com menor duração que ainda não foi assistido e que pertence a este horario
        for(int i = 0; i < movies.size(); i++){
            if(movies[i].start_time == current_time && movies[i].end_time > current_time && (movies[i].start_time < movies[i].end_time)){
                // Se o filme não foi assistido ou se ele tem menor duração que o filme atual
                if(next_movie.id == -1 || movies[i].time < next_movie.time){
                    // Se a categoria do filme não atingiu o limite de filmes assistidos
                    if(current_movies[movies[i].category-1] < max_movies[movies[i].category-1]){
                        next_movie = movies[i];
                    }                
                }
            }       
        }

        // Se o próximo filme for válido, adicionar ele ao resultado
        if(next_movie.id != -1 ){

            result.push_back(next_movie);

            // Atualizar a quantidade de filmes assistidos da categoria do filme
            current_movies[next_movie.category-1]++;

            // Atualizar o horário atual
            current_time = next_movie.end_time-1;

            // output para debug
            // mostrar o filme escolhido
            // cout << "Filme escolhido: " << next_movie.id << " " << next_movie.start_time << " " << next_movie.end_time << " " << next_movie.time << " " << next_movie.category << endl;
            // cout << "Horário atual: " << current_time << endl;
        }
    }

    return result;
}

int main(){
    int n, m;
    cin >> n >> m;
    int start_times;

    vector<int> max_movies(m);
    for(int i = 0; i < m; i++){
        cin >> max_movies[i];
    }

    vector<movie> movies(n);
    for(int i = 0; i < n; i++){

        cin >> movies[i].start_time >> movies[i].end_time >> movies[i].category;
        if(movies[i].end_time == 0){
            movies[i].end_time = 24;
        }

        movies[i].time = movies[i].end_time - movies[i].start_time;
        movies[i].id = i + 1;

    }

    // vetor de horários de início
    vector<schedule> schedules;

    vector<movie> result = choose_movies(movies, max_movies, schedules);

    // map para cada horário de início
    map<int, vector<movie>> movies_by_start_time;

    // Adicionar cada filme ao map
    for (const auto& m : movies) {
        movies_by_start_time[m.start_time].push_back(m);
    }


    // Para cada horário de início
    for (const auto& [start_time, movies] : movies_by_start_time) {
        schedules.push_back({start_time, movies, static_cast<int>(movies.size())});
    }

    
    // Testando o output
    // for (const auto& s : schedules) {
    //     cout << "Schedule starting at " << s.start_time << ":" << endl;
    //     cout << "    Total movies: " << s.total_movies << endl;
    //     for (const auto& m : s.movies) {
    //         cout << "    " << "ID: " << m.id << " - " << " Categoria " << m.category << endl;
    //     }
    // }

    cout << endl;

    // Imprimir resultado
    cout << "Quantidade de filmes assistidos: "  << result.size() << endl;
    for(int i = 0; i < result.size(); i++){
        cout << result[i].id << " ";
    }
    cout << endl;

    // Imprimir tempo total
    int total_time = 0;
    for(int i = 0; i < result.size(); i++){
        total_time += result[i].time;
        cout << "Id: " << result[i].id << " " << "Duração: "<< result[i].time << " " << "Categoria: " << result[i].category << endl;
    }
    
    cout << endl;

    cout << "Quantidade de filmes por categoria:";
    cout << endl;

    // imprimir quantidade de filmes assistidos de cada categoria
    for(int i = 0; i < max_movies.size(); i++){
        int count = 0;
        for(int j = 0; j < result.size(); j++){
            if(result[j].category == i+1){
                count++;
            }
        }
        
        cout << count << " ";
    }

    cout << endl;
    cout << endl;

    cout << "Total de horas na maratona: " << total_time << endl;

    return 0;
}

// g++ gulosa.cpp -o gulosa
// ./gulosa < "input2.txt" > "out_marathon2.txt"
