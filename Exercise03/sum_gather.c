#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000000

int main(int argc, char *argv[])
{
    int rank, size;
    int *data = NULL;
    int *local_chunk;
    int chunk_size;
    long long local_sum = 0;
    long long *all_sums = NULL;
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

    chunk_size = N / size;

    local_chunk = malloc(chunk_size * sizeof(int));

    if (rank == 0)
    {
        data = malloc(N * sizeof(int));
        all_sums = malloc(size * sizeof(long long));

        for (int i = 0; i < N; i++)
            data[i] = i + 1;
    }

    MPI_Scatter(data, chunk_size, MPI_INT,
                local_chunk, chunk_size, MPI_INT,
                0, MPI_COMM_WORLD);

    for (int i = 0; i < chunk_size; i++)
        local_sum += local_chunk[i];

    MPI_Gather(&local_sum, 1, MPI_LONG_LONG,
               all_sums, 1, MPI_LONG_LONG,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int i = 0; i < size; i++)
            total_sum += all_sums[i];

        printf("Total sum = %lld\n", total_sum);

        free(data);
        free(all_sums);
    }

    free(local_chunk);

    MPI_Finalize();

    return 0;
}
