#include <algorithm>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>

using namespace std;

struct Movie {
    int id;
    int start_time;
    int end_time;
    int duration;
    int category;
};

// functor para imprimir
struct Print {
    __host__ __device__
    void operator()(const int& x) {
        printf("%d\n", x);
    }
};

// functor para comparar filmes por hora de término
struct MovieComparison {
    __host__ __device__
    bool operator()(const Movie& a, const Movie& b) {
        return a.start_time < b.start_time;
    }
};

// functor para checar se um filme pode ser assistido
struct MovieDP {
    int n;
    int m;
    const int* start_times;
    const int* end_times;
    const int* categories;
    const int* L;
    int* dp;

    __host__ __device__
    int operator()(const int& index) {
        int i = index / (m + 1);
        int j = index % (m + 1);

        if (i == 0 || j == 0) {
            return 0;
        }

        int max_count = 0;
        for (int k = 0; k < i; k++) {
            
            if (categories[k] == j && end_times[k] <= start_times[i] && dp[(k * (m + 1)) + j - 1] + 1 <= L[j - 1]) {
                max_count = max(max_count, dp[(k * (m + 1)) + j - 1] + 1);
            } else {
                max_count = max(max_count, dp[(k * (m + 1)) + j]);
            }

            printf("i: %d, j: %d, k: %d, max_count: %d\n", i, j, k, max_count);
        }
        dp[(i*(m+1)) + j] = max_count;

        return max_count;
    }
};

int main() {
    // LEITURA DOS INPUTS
    // n = quantidade de filmes, m = quantidade de categorias
    int n, m;
    std::cin >> n >> m;

    // Carregar os dados do arquivo de entrada na memória da GPU
    thrust::host_vector<int> start_times_host(n);
    thrust::host_vector<int> end_times_host(n);
    thrust::host_vector<int> categories_host(n);
    thrust::host_vector<int> L_host(m);

    for (int c = 0; c < m; c++) {
      int max_movies;
      std::cin >> max_movies;
      L_host[c] = max_movies;
    }

    vector<Movie> movies(n); // Vetor de filmes

    for (int i = 0; i < n; i++) {
        int start_time;
        int end_time;
        int category;

        std::cin >> start_time >> end_time >> category;

        // Se o horário de fim for 0, considerar como 24
        if (movies[i].end_time == 0) {
            movies[i].end_time = 24;
        }

        start_times_host[i] = start_time;
        end_times_host[i] = end_time;
        categories_host[i] = category;
    }

    // Ordenar filmes por horário de início
    std::sort(movies.begin(), movies.end(), MovieComparison());

    thrust::device_vector<int> start_times(start_times_host);
    thrust::device_vector<int> end_times(end_times_host);
    thrust::device_vector<int> categories(categories_host);
    thrust::device_vector<int> L(L_host);
    thrust::device_vector<int> dp((n + 1) * (m + 1), 0);

    MovieDP movieDP{
        n, m,
        thrust::raw_pointer_cast(start_times.data()),
        thrust::raw_pointer_cast(end_times.data()),
        thrust::raw_pointer_cast(categories.data()),
        thrust::raw_pointer_cast(L.data()),
        thrust::raw_pointer_cast(dp.data())
    };

    thrust::counting_iterator<int> index_sequence_begin(0);
    thrust::counting_iterator<int> index_sequence_end((n + 1) * (m + 1));

    thrust::transform(index_sequence_begin, index_sequence_end, dp.begin(), movieDP);


    int max_count = *thrust::max_element(dp.begin() + n * (m + 1) + 1, dp.end());


    std::cout << max_count << std::endl;

    return 0;
}
