void syr2k(float alpha, float beta, float A[64][64], float B[64][64], float C[64][64], float D_out[64][64]){
	int i, j, k;
	float buff_A0[64][64], buff_A1[64][64];
	float buff_B0[64][64], buff_B1[64][64];
	float buff_C[64][64];
	float buff_D_out[64][64];
	float tmp1[64][64];
	float tmp2[64][64];

	for (int i = 0; i < 64; i++){
		for (int j = 0; j < 64; j++){
			buff_A0[i][j] = A[i][j];
			buff_A1[i][j] = A[i][j];
			buff_B0[i][j] = B[i][j];
			buff_B1[i][j] = B[i][j];
			buff_C[i][j] = C[i][j];
			buff_D_out[i][j] = 0;
			tmp1[i][j] = 0;
			tmp2[i][j] = 0;
		}
	}

	for (int i = 0; i < 64; i++){
		for (int j = 0; j < 64; j++){
			float tmp_1 = 0;
			for (int k = 0; k < 64; k++){
				tmp_1 += alpha * buff_A0[i][k] * buff_B0[j][k];
			}
			tmp1[i][j] = tmp_1;
		}
	}

	for (int i = 0; i < 64; i++){
		for (int j = 0; j < 64; j++){
			float tmp_2 = 0;
			for (int k = 0; k < 64; k++){
				tmp_2 += alpha * buff_B0[i][k] * buff_A1[j][k];
			}
			tmp2[i][j] = tmp_2;
		}
	}

	for (int i = 0; i < 64; i++){
		for (int j = 0; j < 64; j++){
			buff_D_out[i][j] = tmp1[i][j] + tmp2[i][j] + beta * buff_C[i][j];
		}
	}

	float tmp = (float)(0);
	for(int i = 0; i < 64; i++){
		for(int j = 0; j < 64; j++){
			if (j <= i) tmp = buff_D_out[i][j];
            else tmp = (float)(0);
            D_out[i][j] = tmp;
		}
	}
	/*
	for(int i = 0; i < 64; i++){
		for(int j = 0; j < 64; j++){
			if (j > i) D_out[i][j] = 0;
			else D_out[i][j] = buff_D_out[i][j];
		}
	}
	*/
}

