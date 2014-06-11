#include <iostream>

template <typename T, int N, int M, int P>
void fill (T (&A)[N][M][P])
{
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < M; ++j)
      for (int k = 0; k < P; ++k)
        A[i][j][k] = (T)((i+1)*(j+1)*(k+1));
}

template <typename T, int N, int M, int P>
void print (const T (&A)[N][M][P])
{
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < M; ++j)
      for (int k = 0; k < P; ++k)
        std::cout << A[i][j][k] << " ";
    std::cout << std::endl;
  }
}

template <typename T, int M, int P>
void print (const T (&A)[M][P])
{
  for (int j = 0; j < M; ++j)
  {
    for (int k = 0; k < P; ++k)
      std::cout << A[j][k] << " ";
    std::cout << std::endl;
  }
}


int main ()
{
  typedef float T;
  const int N = 5;
  const int M = 4;
  const int P = 3;

  T A[N][M][P] = {0};
  fill (A);

  std::cout << "A:" << std::endl;
  print (A);
  std::cout << std::endl;

  std::cout << "A[0]:" << std::endl;
  print (A[0]);
  std::cout << std::endl;

  std::cout << "B = A[0]:" << std::endl;
  const T (&B)[M][P] = A[0];
  print (B);
  std::cout << std::endl;

  std::cout << "C = &B:" << std::endl;
  const T (*C)[M][P];
  C = &B;
  print (*C);
  std::cout << std::endl;

  std::cout << "C = &A[1]:" << std::endl;
  C = &A[1];
  print (*C);

  return 0;
}
