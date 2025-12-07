#include <mpi.h>
#include <stdio.h>

// #define TAG_ELEICAO 1
// #define TAG_OK 2
// #define TAG_COORDENADOR 3
// #define TAG_HEARTBEAT 4

int main(int argc, char **argv)
{
    // Inicializa o ambiente MPI
    // Os argumentos de linha de comando são passados para o sistema MPI
    MPI_Init(&argc, &argv);

    // Variáveis para armazenar o ID do processo e o número total de processos
    int world_rank;
    int world_size;

    // Pega o número total de processos rodando (o tamanho do time)
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Pega o rank (ID) do processo atual (quem sou eu?)
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int numero_secreto;

    printf("Olá! Eu sou o processo %d de um total de %d processos.\n", world_rank, world_size);

    // --- LÓGICA DO PROCESSO 0 (O ENVIADOR) ---
    if (world_rank == 0)
    {
        numero_secreto = 42;
        printf("Processo 0: Enviando o número %d para o processo 1...\n", numero_secreto);

        // MPI_Send(dados, quantidade, tipo, destino, tag, comunicador)
        MPI_Send(&numero_secreto, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }

    // --- LÓGICA DO PROCESSO 1 (O RECEBEDOR) ---
    else if (world_rank == 1)
    {
        // MPI_Recv(onde_guardar, quantidade, tipo, origem, tag, comunicador, status)
        MPI_Recv(&numero_secreto, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Processo 1: Recebi o número %d do processo 0!\n", numero_secreto);
    }

    // Finaliza o ambiente MPI. Nenhuma função MPI pode ser chamada depois disso.
    MPI_Finalize();

    return 0;
}

// #include <mpi.h>
// #include <stdio.h>

// int main(int argc, char** argv) {
//     MPI_Init(&argc, &argv);

//     int meu_rank;
//     int total_processos;
    
//     MPI_Comm_rank(MPI_COMM_WORLD, &meu_rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &total_processos);

//     // Definimos uma TAG para saber que o assunto é "ELEIÇÃO"
//     int TAG_ELEICAO = 10;
//     int msg_conteudo = 0; // O conteúdo não importa muito, só o aviso

//     // --- CENÁRIO: O Rank 1 percebeu que o Mestre caiu ---
//     if (meu_rank == 1) {
//         printf("Processo 1: O Mestre caiu! Iniciando eleição...\n");

//         // LOOP: Enviar apenas para quem é MAIOR que eu
//         // Começa em (meu_rank + 1) e vai até (total_processos - 1)
//         for (int i = meu_rank + 1; i < total_processos; i++) {
            
//             printf("Processo 1: Cutucando o processo %d (Maior que eu)\n", i);
            
//             // Envia para o destino 'i'
//             MPI_Send(&msg_conteudo, 1, MPI_INT, i, TAG_ELEICAO, MPI_COMM_WORLD);
//         }
//     }

//     // --- CENÁRIO: Ranks maiores recebendo a cutucada ---
//     // Se eu sou maior que 1, eu devo esperar uma mensagem
//     if (meu_rank > 1) {
//         int origem; 
//         MPI_Status status;

//         // Recebe de QUALQUER um (MPI_ANY_SOURCE) que tenha a TAG_ELEICAO
//         MPI_Recv(&msg_conteudo, 1, MPI_INT, MPI_ANY_SOURCE, TAG_ELEICAO, MPI_COMM_WORLD, &status);
        
//         origem = status.MPI_SOURCE;
//         printf("Processo %d: Recebi chamado de eleição do processo %d!\n", meu_rank, origem);
//     }

//     MPI_Finalize();
//     return 0;
// }