import subprocess
import time
import os
import matplotlib.pyplot as plt
import sys

def roda_com_entrada(executavel, arquivo_in):

  with open(arquivo_in) as f:
    start = time.perf_counter()
    proc = subprocess.run([executavel], input=f.read(), text=True, capture_output=True)
    end = time.perf_counter()

    time_e = end-start
    stdout = proc.stdout

    # print('Saída:\n', proc.stdout)
    # print('Stderr:', proc.stderr)
    # print('Tempo total(s): ', (end - start))

    # pega o nome do executável sem ./
    executavel_tratado =  executavel.replace('./', '')
    
    folder = 'outputs-' + executavel_tratado

    # Criar arquivo com a saída do programa
    out = folder + '/' + arquivo_in.split('/')[1].split('.')[0] + '-out.txt'
    with open(out, 'w') as f:
      f.write(proc.stdout)

  
  return (stdout, time_e, out)


# função que gera gráficos para o tempo de execução em função do número de filmes, considerando um número fixo de categorias
def gera_grafico_n(busca):

    busca_tratada = busca.replace('./', '')

    dict_v0 = {'tempo_exec': [], 'n_filmes': [x for x in range(2000, 61000, 1000)], 'quant_filmes': [], 'total_time': []}
    dict_v1 = {'tempo_exec': [], 'n_filmes': [x for x in range(2000, 61000, 1000)], 'quant_filmes': [], 'total_time': []}
    dict_v2 = {'tempo_exec': [], 'n_filmes': [x for x in range(2000, 61000, 1000)], 'quant_filmes': [], 'total_time': []}
    dict_v3 = {'tempo_exec': [], 'n_filmes': [x for x in range(2000, 61000, 1000)], 'quant_filmes': [], 'total_time': []}
    dict_v4 = {'tempo_exec': [], 'n_filmes': [x for x in range(2000, 61000, 1000)], 'quant_filmes': [], 'total_time': []}

    dict_list = [dict_v0, dict_v1, dict_v2, dict_v3, dict_v4]
    
    for n in range(2000, 61000, 1000):
        for v in range(0, 5):

            saida_tempo = roda_com_entrada(busca, f'inputs/input-{v}-{n}-{10}.txt')
            
            dict_list[v]['tempo_exec'].append(saida_tempo[1])
            # print(f'Versão {v} - Tempo de execução')
            # print(dict_list[v]['tempo_exec'])

            with open (saida_tempo[2], 'r') as f:
                # Primeira linha do arquivo de saída
                line = f.readline().replace('\n', '')

                if line != '':
                    dict_list[v]['quant_filmes'].append(line)
                # print(f'Versão {v} - Quantidade de filmes')
                # print(dict_list[v]['quant_filmes'])

                    # última linha do arquivo de saída
                    for line in f:
                        pass

                    line = line.replace('\n', '')
                    dict_list[v]['total_time'].append(line)

                else:
                    dict_list[v]['quant_filmes'].append(0)
                    dict_list[v]['total_time'].append(0)

    # fazer média dos tempos de execução
    lis_tempo = []
    for i in range(0, len(dict_list[0]['tempo_exec'])):
        soma = 0
        for j in range(0, 5):
            soma += dict_list[j]['tempo_exec'][i]
        lis_tempo.append(soma/5)

    # fazer média das quantidades de filmes
    lis_quant_filmes = []
    for i in range(0, len(dict_list[0]['quant_filmes'])):
        soma = 0
        for j in range(0, 5):
            soma += int(dict_list[j]['quant_filmes'][i])
        lis_quant_filmes.append(soma/5)

    # fazer média dos tempos totais de tela
    lis_total_time = []
    for i in range(0, len(dict_list[0]['total_time'])):
        soma = 0
        for j in range(0, 5):
            soma += int(dict_list[j]['total_time'][i])
        lis_total_time.append(soma/5)

    lis_nfilmes = dict_list[0]['n_filmes']

    # PLotando gráficos

    # Tempo de execução X número de filmes
    plt.plot(lis_nfilmes, lis_tempo)
    plt.xlabel('Número de filmes')
    plt.ylabel('Tempo de execução (s)')
    plt.title(f'Tempo de execução X número de filmes - 10 categorias \n {busca_tratada}')

    plt.savefig(f'graficos/{busca_tratada}-tempoxn.png')
    plt.clf()


    # Quantidade de filmes na maratona X número de filmes
    plt.plot(lis_nfilmes, lis_quant_filmes)
    plt.xlabel('Número de filmes')
    plt.ylabel('Quantidade de filmes')
    plt.title(f'Quantidade de filmes X número de filmes - 10 categorias \n {busca_tratada}')

    plt.savefig(f'graficos/{busca_tratada}-nxquant.png')
    plt.clf()

    # Tempo total de tela X número de filmes
    plt.plot(lis_nfilmes, lis_total_time)
    plt.xlabel('Número de filmes')
    plt.ylabel('Tempo total de tela (s)')
    plt.title(f'Tempo total de tela X número de filmes - 10 categorias \n {busca_tratada}')

    plt.savefig(f'graficos/{busca_tratada}-nxtempo.png')
    plt.clf()

    # plt.show()


# função que gera gráficos para o tempo de execução em função do número de categorias, considerando um número fixo de filmes (50000)
def gera_grafico_m(busca):
    busca_tratada = busca.replace('./', '')
    dict_v0 = {'tempo_exec': [], 'm_categorias': [x for x in range(5, 31)], 'quant_filmes': [], 'total_time': []}
    dict_v1 = {'tempo_exec': [], 'm_categorias': [x for x in range(5, 31)], 'quant_filmes': [], 'total_time': []}
    dict_v2 = {'tempo_exec': [], 'm_categorias': [x for x in range(5, 31)], 'quant_filmes': [], 'total_time': []}
    dict_v3 = {'tempo_exec': [], 'm_categorias': [x for x in range(5, 31)], 'quant_filmes': [], 'total_time': []}
    dict_v4 = {'tempo_exec': [], 'm_categorias': [x for x in range(5, 31)], 'quant_filmes': [], 'total_time': []}
    

    dict_list = [dict_v0, dict_v1, dict_v2, dict_v3, dict_v4]

    for m in range(5, 31):
        for v in range(0, 5):
            # print("Categorias: ", dict_list[v]['m_categorias'])
            saida_tempo = roda_com_entrada(busca, f'inputs/input-{v}-50000-{m}.txt')
            
            dict_list[v]['tempo_exec'].append(saida_tempo[1])
            print(f'Versão {v} - Tempo de execução')
            print(dict_list[v]['tempo_exec'])

            with open (saida_tempo[2], 'r') as f:
                # Primeira linha do arquivo de saída
                line = f.readline().replace('\n', '')
                
                # Se a linha não for vazia:
                if line != '':
                    dict_list[v]['quant_filmes'].append(line)
                
                    # print(f'Versão {v} - Quantidade de filmes')
                    # print(dict_list[v]['quant_filmes'])

                    # última linha do arquivo de saída
                    for line in f:
                        pass

                    line = line.replace('\n', '')
                    dict_list[v]['total_time'].append(line)

                else:
                    dict_list[v]['quant_filmes'].append(0)
                    dict_list[v]['total_time'].append(0)

    # fazer média dos tempos de execução
    lis_tempo = []
    for i in range(0, len(dict_list[0]['tempo_exec'])):
        soma = 0
        for j in range(0, 5):
            soma += dict_list[j]['tempo_exec'][i]
        lis_tempo.append(soma/5)

    print("\nTempo de execução:")
    print(lis_tempo)


    # fazer média das quantidades de filmes
    lis_quant_filmes = []
    for i in range(0, len(dict_list[0]['quant_filmes'])):
        soma = 0
        for j in range(0, 5):
            soma += int(dict_list[j]['quant_filmes'][i])
        lis_quant_filmes.append(soma/5)


    # fazer média dos tempos totais de tela
    lis_total_time = []
    for i in range(0, len(dict_list[0]['total_time'])):
        soma = 0
        for j in range(0, 5):
            soma += int(dict_list[j]['total_time'][i])
        lis_total_time.append(soma/5)


    # PLotando gráficos
    lis_mcategorias = dict_list[0]['m_categorias']

    # Tempo de execução X número de categorias
    plt.plot(lis_mcategorias, lis_tempo)
    plt.xlabel('Número de categorias')
    plt.ylabel('Tempo de execução (s)')
    plt.title(f'Tempo de execução X número de categorias - 50000 filmes \n {busca_tratada}')

    plt.savefig(f'graficos/{busca_tratada}-tempoxm.png')
    plt.clf() # limpa o gráfico

    # Quantidade de filmes na maratona X número de categorias
    plt.plot(dict_v1['m_categorias'], lis_quant_filmes)
    plt.xlabel('Número de categorias')
    plt.ylabel('Quantidade de filmes')
    plt.title(f'Quantidade de filmes na maratona X número de categorias - 50000 filmes \n {busca_tratada}')

    plt.savefig(f'graficos/{busca_tratada}-mxquant.png')
    plt.clf() 

    # Tempo total de tela X número de categorias
    plt.plot(dict_v1['m_categorias'], lis_total_time)
    plt.xlabel('Número de categorias')
    plt.ylabel('Tempo total de tela (s)')
    plt.title(f'Tempo total de tela X número de categorias - 50000 filmes \n {busca_tratada}')

    plt.savefig(f'graficos/{busca_tratada}-mxtempo.png')
    plt.clf()






# ___________________________________________________________GRAFICO 3D - PROX ENTREGA_____________________________________________________________     

# def tam_tempo(busca, in_folder):
#   lis_tempo = [] # Tempo de execução
#   lis_nfilmes = [] # Número de filmes
#   lis_mcategorias = [] # Número de categorias
#   lis_quant_filmes = [] # Quantidade de filmes na maratona
#   lis_total_time = [] # Tempo total da maratona (tempo de tela)
#   lis_inputs = [] # Nome do arquivo de entrada


#   for file in sorted(os.listdir(in_folder)): # Percorre todos os arquivos da pasta input
#     print('-------------------------------------')
#     # print('\n'+file+'n')
#     saida_tempo = roda_com_entrada(busca, in_folder + '/' + file) # Roda o executável com o arquivo de entrada

#     lis_tempo.append(saida_tempo[1])

#     lis_inputs.append(file)
#     print(f'File names: {file}')

#     with open (in_folder + '/' + file, 'r') as f:
#         line = f.readline().split(" ")
#         lis_nfilmes.append(line[0])

#         # tirar o \n do final da string
#         line[1] = line[1].replace('\n', '')
#         lis_mcategorias.append(line[1])

#         print(f'Nº de filmes: {line[0]}')
#         print(f'Nº de categorias: {line[1]}')

    
#     # Quantidade de filmes na maratona
#     with open (saida_tempo[2], 'r') as f:
#         # Primeira linha do arquivo de saída
#         line = f.readline().replace('\n', '')        
#         lis_quant_filmes.append(line)

#         # última linha do arquivo de saída
#         for line in f:
#             pass
#         line = line.replace('\n', '')
#         lis_total_time.append(line)

    
#     lis_inputs.append(file)

#   return (lis_tempo, lis_nfilmes, lis_mcategorias, lis_quant_filmes, lis_total_time, lis_inputs)

#_______________________________________________________________________________________________________________________________________



# Para cada versão, chamar a função tam_tempo() com o executável e a pasta de entrada

heuristica = sys.argv[1]

n_fixog = gera_grafico_n(heuristica)  
m_fixog = gera_grafico_m(heuristica)


