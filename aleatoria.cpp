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


    // Percorrendo horários possíveis de início - das 0h às 23h
    for(current_time = 0; current_time <= 23; current_time++){
        int total_hours = 0;

        // sortear números aleatórios entre 0 e 1
        uniform_real_distribution<double> distribution(0.0, 1.0);
        
        // para cada horário que um filme se inicia, next_movie é o filme com menor duração
        movie next_movie = {-1, -1, -1, -1, -1}; // Filme com menor duração para o horário atual
        // cout << "Início: "<< schedules[current_time].start_time << " " << schedules[current_time].total_movies << endl;
        
        vector<movie> lista_movies = schedules[current_time].movies;
        
        // Aleatorização: colocar na maratona filme com mesmo horário de início, porém aleatório

        // 25% de chance de adicionar o filme na maratona de forma aleatoria, respeitando tempo  
        if(distribution(generator) > 0.75){ 


            int n_movies = schedules[current_time].total_movies;                                    
                                                
            // pegar um filme aleatório que começa no horário atual e que ainda não foi assistido
            uniform_int_distribution<int> distribution(0, n_movies-1);
            int p = distribution(generator);
            
            // Se o filme aleatório não foi assistido ainda
            if(next_movie.id == -1){
                
                // enquantoo limite de filmes na categoria for atingido, sortear outro filme
                while (current_movies[lista_movies[p].category-1] >= max_movies[lista_movies[p].category-1] || (lista_movies[p].end_time <= lista_movies[p].start_time || lista_movies[p].end_time <= current_time)){
                    // garantir que o filme sorteado não seja o mesmo
                    
                    lista_movies.erase(lista_movies.begin() + p);
                    // n_movies = lista_movies.size();

                    // Se não houver mais filmes para sortear, sair do loop
                    if(lista_movies.size() == 0){
                        break;
                    }

                    // Sortear outro filme
                    if (lista_movies.size() == 1){
                        p = 0;
                    }
                    else{
                        distribution = uniform_int_distribution<int>(0, (lista_movies.size())-1);
                        p = distribution(generator);
                    }                               
                }                            
                
                if (current_movies[lista_movies[p].category-1] < max_movies[lista_movies[p].category-1])
                {
                    next_movie = lista_movies[p];
                }                        
            }                              
                // }        
            
        }

        // Caso contrário, pegar o filme com menor duração que ainda não foi assistido e que pertence a este horário
        else{
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
        }

        // Se o próximo filme for válido, adicionar ele ao resultado
        if(next_movie.id != -1 && total_hours + next_movie.time <= 24 ){

            result.push_back(next_movie);

            // Atualizar a quantidade de filmes assistidos da categoria do filme
            current_movies[next_movie.category-1]++;

            // Atualizar o horário atual
            current_time = next_movie.end_time-1;

            // Atualizar a quantidade de horas assistidas
            total_hours += next_movie.time;

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
    vector<schedule> schedules(24);

    // map com chave sendo o horário de início e valor sendo um vetor de filmes
    map<int, vector<movie>> movies_by_start_time; 

    // Adicionar cada filme ao map
    for (const auto& m : movies) {
        // adiciona filme ao horário de início correspondente
        movies_by_start_time[m.start_time].push_back(m); 
    }

    for (int i = 0; i < 24; i++){
        // checar se o horário de início existe no map
        if(movies_by_start_time.find(i) != movies_by_start_time.end()){
            // se existir, adicionar ao vetor de horários de início
            schedules[i].start_time = i;
            schedules[i].movies = movies_by_start_time[i];
            schedules[i].total_movies = movies_by_start_time[i].size();
        }
        else{
            // se não existir, adicionar um horário de início vazio
            schedules[i].start_time = i;
            schedules[i].total_movies = 0;
        }     
    }


    vector<movie> result = choose_movies(movies, max_movies, schedules);

    // cout << endl;

    // Imprimir quantidade de fimes assistidos
    // cout << "Quantidade de filmes assistidos: "  << result.size() << endl;
    cout << result.size() << endl;

    // Imprimir ids dos filmes assistidos
    for(int i = 0; i < result.size(); i++){
        cout << result[i].id << " ";
    }
    // cout << endl;

    // cout << "Quantidade de filmes por categoria:";
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
    // cout << endl;

    // Imprimir tempo total
    int total_time = 0;
    for(int i = 0; i < result.size(); i++){
        total_time += result[i].time;
        // cout << "Id: " << result[i].id << " " << "Início: " << result[i].start_time << " " << "Fim: " << result[i].end_time << " " << "Duração: "<< result[i].time << " " << "Categoria: " << result[i].category << endl;
        cout << result[i].id << " " << result[i].start_time << " " << result[i].end_time << " " << result[i].time << " " << result[i].category << endl;
        
    }
    // cout << endl;

    cout << total_time << endl;

    return 0;
}

// g++ gulosa.cpp -o gulosa
// ./gulosa < "input2.txt" > "out_marathon2.txt"
