Exercise 7 - MPI Collective Communication Comparison

1. Comparison of the Six Implementations

| Exercise | Collective Communication | Array Allocation | Manual Root Summation | Final Result |
|----------|--------------------------|------------------|-----------------------|--------------|
| 1 | MPI_Bcast + MPI_Send/Recv | Full array on every process | Yes | Root only |
| 2 | MPI_Scatter + MPI_Send/Recv | Chunk on every process | Yes | Root only |
| 3 | MPI_Scatter + MPI_Gather | Chunk on every process | Yes | Root only |
| 4 | MPI_Scatter + MPI_Reduce | Chunk on every process | No | Root only |
| 5 | MPI_Scatter + MPI_Allreduce | Chunk on every process | No | Every process |
| 6 | MPI_Scatter + MPI_Scan | Chunk on every process | No | Prefix result on every process |


2. Performance Comparison

All programs were tested with 2, 4, and 8 MPI processes on the AWS EC2 instance.

| Exercise | 2 Processes | 4 Processes | 8 Processes |
|----------|-------------|-------------|-------------|
| 1 | 1.67 s | 2.00 s | 2.63 s |
| 2 | 1.66 s | 1.99 s | 2.63 s |
| 3 | 1.66 s | 1.99 s | 2.63 s |
| 4 | 1.66 s | 2.00 s | 2.64 s |
| 5 | 1.66 s | 2.00 s | 2.64 s |
| 6 | 1.66 s | 2.00 s | 2.63 s |

Observation:

The 2-process runs had the lowest measured execution times. The 4-process and 8-process runs took longer. This is mainly because the EC2 instance has limited CPU resources and the tests with 4 and 8 processes were run using --oversubscribe. This can introduce CPU contention and additional process and communication overhead.

The differences between the six implementations were very small, so the results do not provide a definitive performance ranking of the MPI collective operations.


3. When Would You Choose MPI_Scan Over MPI_Allreduce?

MPI_Allreduce should be used when every process needs the same final combined result. For example, if every process calculates a local sum and all processes need the overall sum, MPI_Allreduce provides the total to every process.

MPI_Scan should be used when processes need a prefix or cumulative result. For example, it can be used to calculate the global offset for each process when distributing data.

In this exercise, MPI_Scan produces a different prefix sum for each process. Each process receives the sum of its own local value and all values from lower-ranked processes.

For example:

Rank 0 -> prefix sum of Rank 0
Rank 1 -> Rank 0 + Rank 1
Rank 2 -> Rank 0 + Rank 1 + Rank 2
Rank 3 -> Rank 0 + Rank 1 + Rank 2 + Rank 3

Therefore, MPI_Scan is useful when each process needs information about the cumulative results before or including its own rank, while MPI_Allreduce is useful when every process needs the same final global result.
