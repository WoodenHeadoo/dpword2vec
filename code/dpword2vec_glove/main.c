#include <stdio.h>
#include <stdlib.h>
#include "TrainModel.h"

/*
TrainingModel

Inputs:
vec_dim: dimension of vectors
iters: number of iterations for training
init_eta: initial learning rate
x_max: parameter of GloVe
adagrad: whether use the AdaGrad algorithm for training (1: Yes, 0: No)
show_loss: whether show the values of the loss function (1: Yes, 0: No)
words_pats_pairs.txt: co-occurrence pairs and counts
words_weight.txt: weights for dp-word pairs

Outputs:
"vecs.txt": the i-th line represents the vector of the i-th word or design pattern in the vocabulary
*/

int main(void)
{
	TrainingModel(100,100,0.05,30,0,1);
	
	system("PAUSE");
	return 0;
}
