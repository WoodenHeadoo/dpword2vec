#if !defined(TRAINMODEL_H)
#define TRAINMODEL_H

void InitData(char * filename_pairs, char * filename_weight, double xmax);
void ReleaseData();
void WriteData(char * filename_in_vec, char * filename_out_vec);
void GradientDescent(double eta, int idx, int adagrad);
double OnlineLoss();
void TrainingModel(int vec_dim, int iters, double init_eta, double x_max, int adagrad, int show_loss);

#endif
