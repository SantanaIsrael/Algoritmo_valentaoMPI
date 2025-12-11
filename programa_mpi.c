#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#define TAG_ELEICAO 10
#define TAG_OK 20
#define TAG_COORDENADOR 30
#define TAG_PING 100
#define TAG_PONG 101

void iniciar_eleicao(int, int);
void virar_mestre(int, int);

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int meu_rank, total_processos;
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &total_processos);

    int mestre_atual = 0; // Assumir que o rank zero é o bam bam bam
    int rank_vitima = 2;  // Quem fecha o paletó?
    int sou_mestre = (meu_rank == mestre_atual) ? 1 : 0;
    int em_eleicao = 0, programa_rodando = 1;

    // Pra verificar o tempo
    double tempo_inicio_simulacao = MPI_Wtime();
    double ultimo_ping_enviado = MPI_Wtime();
    double inicio_minha_eleicao = 0.0;

    const double INTERVALO_PING = 1.0;               // Enviar ping a cada 1 segundo pra ver se ta vivo o carinha
    const double TEMPO_LIMITE_RESPOSTA_MESTRE = 2.5; // Se não responder em 2.5 segundos foi pro beleléu
    const double TEMPO_LIMITE_RESP_OK = 2.0;         // Tempo esperando pra responder se é maior no caso

    // Variável para saber quando foi a última vez que o mestre deu sinal de vida
    double ultima_resposta_mestre = MPI_Wtime();

    // if (meu_rank == 0)
    // {
    //     printf("Digite o Rank do processo que deve falhar (ex: %d): ", total_processos);
    //     fflush(stdout);
    //     scanf("%d", &rank_vitima);
    // }

    // Envia pra todo mundo quem vai morrer
    MPI_Bcast(&rank_vitima, 1, MPI_INT, 0, MPI_COMM_WORLD);

    while (programa_rodando)
    {
        double agora = MPI_Wtime();

        if (meu_rank == rank_vitima)
        {
            if (agora - tempo_inicio_simulacao > 5.0 && agora - tempo_inicio_simulacao < 5.1)
            {
                printf("--------- [Rank %d] Ai! vou morrer! ---------\n", meu_rank);
                sleep(10);
                printf("--------- [Rank %d] VOLTEI! Galera vou querer meu posto! ---------\n", meu_rank);
                inicio_minha_eleicao = MPI_Wtime();
                em_eleicao = 1;
                iniciar_eleicao(meu_rank, total_processos);
            }
        }

        int flag = 0;
        MPI_Status status;

        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        if (flag)
        {
            int buffer;
            MPI_Recv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            switch (status.MPI_TAG)
            {
            case TAG_PING:
                // Alguém tá qurendo saber se tô vivo da Silva Chavier
                MPI_Send(&meu_rank, 1, MPI_INT, status.MPI_SOURCE, TAG_PONG, MPI_COMM_WORLD);
                break;

            case TAG_PONG:
                // O cara ta vivo, vou atualizar a resposta do homi
                if (status.MPI_SOURCE == mestre_atual)
                {
                    ultima_resposta_mestre = MPI_Wtime();
                }
                break;

            case TAG_ELEICAO:
                // Alguém menor quer ser líder. Mando ele sair fora que eu vou tentar ser o CHEFE
                MPI_Send(&meu_rank, 1, MPI_INT, status.MPI_SOURCE, TAG_OK, MPI_COMM_WORLD);

                // Se eu não sou mestre e não estou em eleição, inicio a minha
                if (!sou_mestre && !em_eleicao)
                {
                    em_eleicao = 1;
                    inicio_minha_eleicao = MPI_Wtime();
                    iniciar_eleicao(meu_rank, total_processos);
                }
                break;

            case TAG_OK:
                // Alguém maior respondeu. Desisto.
                if (em_eleicao)
                {
                    em_eleicao = 0;
                    printf("[Rank %d] Infelizmente recebi OK do Rank %d. Aguardando novo bocó.\n", meu_rank, status.MPI_SOURCE);
                }
                break;

            case TAG_COORDENADOR:
                mestre_atual = buffer;
                sou_mestre = (meu_rank == mestre_atual);
                em_eleicao = 0; // Paro minha eleição pois perdi para alguém maior
                ultima_resposta_mestre = MPI_Wtime();
                printf("[Rank %d] Novo bam bam bam é rank %d\n", meu_rank, mestre_atual);
                break;
            }
        }

        if (agora - tempo_inicio_simulacao > 20.0)
        {
            programa_rodando = 0;
        }

        if (!sou_mestre && !em_eleicao){
            // Ver se o homi ta vivo
            if (agora - ultimo_ping_enviado > INTERVALO_PING)
            {
                int buffer = 0;
                MPI_Send(&buffer, 1, MPI_INT, mestre_atual, TAG_PING, MPI_COMM_WORLD);
                ultimo_ping_enviado = agora;
            }

            //O chefão morreu?
            if (agora - ultima_resposta_mestre > TEMPO_LIMITE_RESPOSTA_MESTRE)
            {
                printf("[Rank %d] Limite do tempo! O bocó do mestre %d morreu.\n", meu_rank, mestre_atual);
                iniciar_eleicao(meu_rank, total_processos);
                em_eleicao = 1;
                inicio_minha_eleicao = agora;
            }
        }

        // Se ESTOU em eleição: Conto tempo esperando OK
        if (em_eleicao)
        {
            if (agora - inicio_minha_eleicao > TEMPO_LIMITE_RESP_OK)
            {
                // Silêncio total dos maiores. GANHEI.
                printf("[Rank %d] Fim do tempo de resposta, ninguem maior respondeu. EU SOU O CARA!\n", meu_rank);
                virar_mestre(total_processos, meu_rank);

                mestre_atual = meu_rank;
                sou_mestre = 1;
                em_eleicao = 0;
            }
        }
    }

    printf("[Rank %d] Finalizando processo.\n", meu_rank);
    MPI_Finalize();
    return 0;
}

void iniciar_eleicao(int meu_rank, int total_processos)
{
    if (meu_rank < total_processos - 1)
    {
        printf("[Rank %d] Iniciando eleicao...\n", meu_rank);
        int buffer = 0;
        for (int i = meu_rank + 1; i < total_processos; i++)
        {
            MPI_Send(&buffer, 1, MPI_INT, i, TAG_ELEICAO, MPI_COMM_WORLD);
        }
    }
}

void virar_mestre(int total_processos, int meu_rank)
{
    for (int i = 0; i < total_processos; i++)
    {
        if (i != meu_rank)
        {
            MPI_Send(&meu_rank, 1, MPI_INT, i, TAG_COORDENADOR, MPI_COMM_WORLD);
        }
    }
}