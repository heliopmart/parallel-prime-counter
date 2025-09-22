# Trabalho de SO: Manipulação de Threads

- GIT: https://github.com/heliopmart/parallel-prime-counter.git

## Como Executar

Na /root da pasta execute: 
```
    // Obrigatório std vesão >= 20 para tipagem AUTO
    g++ main.cpp threads.cpp serial.cpp  matrix.cpp -o trabalho_threads.exe -std=c++20
    ./trabalho_threads.exe
```

## Arquivos Obrigatórios
```
/root
    /matrix.h
    /matrix.cpp
    /threads.h
    /threads.cpp
    /default.h
    /serial.h
    /serial.cpp
    /main.cpp
    /trabalho_threads.exe
```

## Configurações do Sistema

- CPU: Intel Core i5-2500 @ 3.30GHz
- Núcleos Físicos / Lógicos: 4 / 4 (Este é um ponto crucial: não há Hyper-Threading, então o número de processadores reais e virtuais é o mesmo ).
- Memória RAM: 16 GB
- Sistema Operacional: Windows 10 Pro
- IDE: VSCODE
- Compilador: 
```
    g++.exe (Rev8, Built by MSYS2 project) 15.2.0
```

- OBS: Programas rodando em paralelo: 
    - Gerenciador de Tarefas
    - Navegador Edge com 5 abas abertas 
    - VSCODE ( usar o powershel para executar o programa )

## Configurações da Matrix 

- Tamanho real: 8000x8000
- Seed: 123456789
- Valor de ref de primos: 4.252;198
- Tamanho em memoria: 8000 * 8000 * 4 = 256.000.000 para int 4 bits

## Single Thread

- Tempo de execução: 57748 ms
- Valor calculado: 4252198

## Multi Threads

```
Tamanho do Chunk ( Submatriz ) = 1, 10, 50, 100, 250, 500, 1000, 2500
Parâmetro Variável (Nº de Threads): 2, 4, 6, 8, 10, 16, 32

OBS: Justificativa da escolha: Testamos abaixo da capacidade (<4), na capacidade exata (=4) e acima da capacidade (>4) do processador para observar o efeito da sobrecarga de threads.
OBS: Tempo de criação de tarefas é o tempo gasto para criar as submatrizes e add ao semaforo

Para o teste A usaremos Chunk fixo (100 [melhor resultado]) e Nº Threads variados
Para o teste B usaremos Nº Threads fixo (16 [melhor resultado]) e Chunk variados
```
# Experimento A: Threads Variadas

## Threads Variadas - 1º

```
Nº de Threads: 2
Chunk: 100

-------

Tempo: 41505 ms
Resultado: 4252198
Speedup: 1.39135x
Eficiência: 69.5675%
Tempo de criação de tarefas: 0 ms 
```

## Threads Variadas - 2º

```
Nº de Threads: 4
Chunk: 100

-------

Tempo: 22603 ms
Resultado: 4252198
Speedup: 2.55488x
Eficiência: 63.8721%
Tempo de criação de tarefas: 32 ms 
```
## Threads Variadas - 3º
```
Nº de Threads: 6
Chunk: 100

-------

Tempo: 19822 ms
Resultado: 4252198
Speedup: 2.91333x
Eficiência: 48.5555%
Tempo de criação de tarefas: 33 ms 
```
## Threads Variadas - 4º
```
Nº de Threads: 8
Chunk: 100

-------

Tempo: 18205 ms
Resultado: 4252198
Speedup: 3.1721x
Eficiência: 39.6512%
Tempo de criação de tarefas: 84 ms 
```
## Threads Variadas - 5º
```
Nº de Threads: 10
Chunk: 100

-------

Tempo: 18081 ms
Resultado: 4252198
Speedup: 3.19385x
Eficiência: 31.9385%
Tempo de criação de tarefas: 35 ms 
```
## Threads Variadas - 6º
```
Nº de Threads: 16
Chunk: 100

-------

Tempo: 16469 ms
Resultado: 4252198
Speedup: 3.50647x
Eficiência: 21.9154%
Tempo de criação de tarefas: 109 ms 
```
## Threads Variadas - 7º
```
Nº de Threads: 32
Chunk: 100

-------

Tempo: 19099 ms (Convergiu)
Resultado: 4252198
Speedup: 3.02361x
Eficiência: 9.44879%
Tempo de criação de tarefas: 175 ms 
```

------------- FIM ----------------

# Experimento B: Chunks Variados

## Chunks Variados - 1º
```
Nº de Threads: 16
Chunk: 1

-------

Tempo: 17847 ms
Resultado: 4252198
Speedup: 3.23573x
Eficiência: 20.2233%
Tempo de criação de tarefas: 86 ms 
```
## Chunks Variados - 2º
```
Nº de Threads: 16
Chunk: 10

-------

Tempo: 18470 ms
Resultado: 4252198
Speedup: 3.12658x
Eficiência: 19.5411%
Tempo de criação de tarefas: 0 ms 
```
## Chunks Variados - 3º
```
Nº de Threads: 16
Chunk: 50

-------

Tempo: 17938 ms
Resultado: 4252198
Speedup: 3.21931x
Eficiência: 20.1207%
Tempo de criação de tarefas: 94 ms 
```
## Chunks Variados - 4º
```
Nº de Threads: 16
Chunk: 100

-------

Tempo: 16785 ms
Resultado: 4252198
Speedup: 3.44045x
Eficiência: 21.5028%
Tempo de criação de tarefas: 71 ms 
```
## Chunks Variados - 5º
```
Nº de Threads: 16
Chunk: 250

-------

Tempo: 18523 ms
Resultado: 4252198
Speedup: 3.11764x
Eficiência: 19.4852%
Tempo de criação de tarefas: 88 ms 
```
## Chunks Variados - 6º
```
Nº de Threads: 16
Chunk: 500

-------

Tempo: 18017 ms
Resultado: 4252198
Speedup: 3.2052x
Eficiência: 20.0325%
Tempo de criação de tarefas: 89 ms 
```
## Chunks Variados - 7º
```
Nº de Threads: 16
Chunk: 1000

-------

Tempo: 18207 ms
Resultado: 4252198
Speedup: 3.17175x
Eficiência: 19.8234%
Tempo de criação de tarefas: 80 ms 
```
## Chunks Variados - 8º
```
Nº de Threads: 16
Chunk: 2500

-------

Tempo: 25347 ms (Convergiu)
Resultado: 4252198
Speedup: 2.2783x
Eficiência: 14.2394%
Tempo de criação de tarefas: 35 ms 
```