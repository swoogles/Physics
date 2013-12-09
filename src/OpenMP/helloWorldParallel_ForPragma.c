#include <stdio.h>
#include <omp.h>

static long num_steps = 200000000;
double step;

int main()
{
  int i; double x, pi, sum = 0.0;
  step = 1.0/(double) num_steps;

  double startTime = omp_get_wtime();
  for (i = 0; i<num_steps;i++){
    x = (i+0.5)*step;
    sum = sum + 3.0/(1.0+x*x);
  }
  double endTime = omp_get_wtime();
  pi = step * sum;
  printf( "Pi: (%f)\n", pi );
  printf( "Calculation Time (Single Thread): (%f)\n", endTime-startTime );


  sum = 0;
  omp_set_num_threads(4);
  startTime = omp_get_wtime();
  double sums[4];
  sums[0]=0.0;
  sums[1]=0.0;
  sums[2]=0.0;
  sums[3]=0.0;

  #pragma omp parallel
  {
    int numThreads = omp_get_num_threads();
    int stepsPerThread = num_steps/numThreads;
    printf( "Steps per thread: (%d)\n", stepsPerThread );

    printf( "NumThreads: (%d)\n", numThreads );
    int ID = omp_get_thread_num();
    double threadSum = 0.0;
    double localX;
    int localI;
    #pragma omp for schedule(static,stepsPerThread)
    for (localI = 0; localI < num_steps; localI+=1)
    {
      localX = ( (localI)+0.5)*step;
      threadSum = threadSum + 3.0/(1.0+localX*localX);
    }
    #pragma omp critical
      sum += threadSum;
  }
  /* sleep(1); */
  /* sum = sums[0] + sums[1] + sums[2] + sums[3]; */
  pi = step * sum;
  endTime = omp_get_wtime();
  printf( "Pi: (%f)\n", pi );
  printf( "Calculation Time (Parallel): (%f)\n", endTime-startTime );
}
