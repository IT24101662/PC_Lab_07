#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000000

int main(int argc, char *argv[])
{
    int rank, size;
    int *data;
    long long local_sum = 0;
    long long total_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0)
    {
        if (rank == 0)
            printf("Number of processes must evenly divide %d\n", N);

        MPI_Finalize();
        return 1;
    }

    data = malloc(N * sizeof(int));

    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
            data[i] = i + 1;
    }

    MPI_Bcast(data, N, MPI_INT, 0, MPI_COMM_WORLD);

    int chunk_size = N / size;
    int start = rank * chunk_size;
    int end = start + chunk_size;

    for (int i = start; i < end; i++)
        local_sum += data[i];

    if (rank == 0)
    {
        total_sum = local_sum;

        for (int source = 1; source < size; source++)
        {
            long long received_sum;
            MPI_Recv(&received_sum, 1, MPI_LONG_LONG,
                     source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            total_sum += received_sum;
        }

        printf("Total sum = %lld\n", total_sum);
    }
    else
    {
        MPI_Send(&local_sum, 1, MPI_LONG_LONG,
                 0, 0, MPI_COMM_WORLD);
    }

    free(data);

    MPI_Finalize();

    return 0;
}
