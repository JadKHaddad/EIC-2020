void tiled_matmul(int M, int N, int K,
                  const float *A, const float *B, float *C,
                  int ts_l2_M, int ts_l2_N, int ts_l2_K,
                  int ts_l1_M, int ts_l1_N, int ts_l1_K) {
        // tiling for l2 cache
        for (int iii = 0; iii < M; iii += ts_l2_M)
          for (int jjj = 0; jjj < N; jjj += ts_l2_N)
            for (int kkk = 0; kkk < K; kkk += ts_l2_K)

              // tiling for l1 cache
              for (int ii = iii; ii < iii + ts_l2_M; ii += ts_l1_M)
                for (int jj = jjj; jj < jjj + ts_l2_N; jj += ts_l1_N)
                  for (int kk = kkk; kk < kkk + ts_l2_K; kk += ts_l1_K)

                    // processing one l1 tile
                    for (int i = ii; i < ii + ts_l1_M; ++i)
                      for (int j = jj; j < jj + ts_l1_N; ++j)
                        for (int k = kk; k < kk + ts_l1_K; ++k)
                          C[i * N + j] += A[i * K + k] * B[k * N + j];
}