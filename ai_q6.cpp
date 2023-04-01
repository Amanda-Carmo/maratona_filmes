#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct movie{
    int id;
    int H;
    int F;
    int time;
    int C;
};

bool compare_movies(movie a, movie b){
    return a.F < b.F;
}


int main(){
    // LER N, M
    int n, m;
    cin >> n >> m;

    // Ler max_filmes[M]
    vector<int> max_filmes(m);
    
    for(int i = 0; i < m; i++){
        cin >> max_filmes[i];
        // cout << max_filmes[i] << endl;
        // cout << endl;
    }

    // cout << n << " " << m << endl;

    // CRIAR UMA LISTA DE FILMES VAZIA
    vector<movie> movies(n);

    // PARA I DE 1 ATÉ N
    for(int i = 1; i <= n; i++){
        cin >> movies[i].H >> movies[i].F >> movies[i].C;

        movies[i].id = i;

        movies.push_back(movies[i]);
        // cout << movies[i].C << endl;
    }

    // Ordenar a lista de filmes por ordem crescente de hora de t´ermino F[i]
    sort(movies.begin(), movies.end(), compare_movies);

    // Cria uma matriz dp[N+1][M+1] preenchida com zeros
    // vector< vector< bool > > verified( cols, vector<bool>( rows, false ) );

    vector<vector<int>> dp(n+1, vector<int>(m+1, false));
    

    for(int j = 0; j < m; j++){
        
        dp[0][j] = max_filmes[j];  
        // cout << dp[0][j] << endl;      
    }

    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++){
            if(movies[i].C != j){
                // cout << dp[i][j];
                dp[i][j] = dp[i-1][j];
            }
            else{
                int last = i-1;

                while (last >= 0 && movies[last].F > movies[i].H){
                    last = last -1;
                }

                int max_count = 0;

                for(int k=0; k <= min(dp[last][j-1], (movies[i].F - movies[i].H - 1)); k++){
                    max_count = max(max_count, (dp[last][j-1-k] + k + 1));
                }

                dp[i][j] = max(dp[i-1][j], max_count);
                
            }
        }
    }

    // Imprimir dp[N][M]

    for (int i=0; i < n+1; i++){
        for(int j=0; j < m+1; j++){
        cout << dp[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}