import os

# Definir as listas de valores para n e m
n_list = list(range(2000, 61000, 1000))
m_list = list(range(4, 31))

# n_list = list(range(2, 61, 10))
# m_list = list(range(5, 15))


# Criar a pasta "inputs" caso ela não exista ainda
if not os.path.exists("inputs"):
    os.mkdir("inputs")

for k in range(5):  # loop externo para gerar 5 inputs com mesmo n e m
    # loop interno para gerar os inputs com os valores de n e m
    for i in n_list: 
        for j in m_list:
            # print(f"Gerando input {k}-{i}-{j}...")
            
            # Gerar o conteúdo do arquivo input.txt com o executável "gerador"            
            os.system(f"./gerador {i} {j}")

            # colocar o conteúdo gerado em um arquivo de texto
            with open(f"inputs/input-{k}-{i}-{j}.txt", "w") as f:
                with open("input.txt", "r") as f_temp:
                    f.write(f_temp.read())
                
            # excluir o arquivo temporário
            os.remove("input.txt")
            

