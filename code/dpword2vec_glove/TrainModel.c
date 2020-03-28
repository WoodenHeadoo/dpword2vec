#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include "TrainModel.h"

int V, Dim, C;
int *inPos, *outPos;
double *weight, *logval;
double *U, *Vec, *GU, *GV;

void InitData(char * filename_pairs, char * filename_weight, double xmax)
{
	int i,j,n,k,Vp;
	double temp;
	double *ww,nn;
	FILE * fp;
	
	V = 0,C = 0;
	fp = fopen(filename_pairs,"r");
	while(fscanf(fp,"%d %d %d",&i,&j,&n) == 3)
	{
		C++;
		if(i+1 > V)
		{
			V = i+1;
		}
		if(j+1 > V)
		{
			V = j+1;
		}
	}
	fclose(fp);
	
	ww = (double*)malloc(V*sizeof(double));
	i = 0;
	fp = fopen(filename_weight,"r");
	fscanf(fp,"%d",&Vp);
	while(fscanf(fp,"%lf",&temp) == 1)
	{
		*(ww+i++) = temp;
	}
	fclose(fp);
	
	inPos = (int*)malloc(2*C*sizeof(int));
	outPos = (int*)malloc(2*C*sizeof(int));
	weight = (double*)malloc(2*C*sizeof(double));
	logval = (double*)malloc(2*C*sizeof(double));
	k = 0;
	fp = fopen(filename_pairs,"r");
	while(fscanf(fp,"%d %d %d",&i,&j,&n) == 3)
	{
		*(inPos+2*k) = i;
		*(inPos+2*k+1) = j;
		*(outPos+2*k) = j;
		*(outPos+2*k+1) = i;
		if(i < Vp && j >= Vp)
		{
			temp = (*(ww+i))*(*(ww+j));
			if(temp < 1.0)
			{
				n = ceil(temp*n);
			}
			else
			{
				n = floor(temp*n);
			}
		}
		temp = log(n);
		*(logval+2*k) = temp;
		*(logval+2*k+1) = temp;
		if(n < xmax)
		{
			temp = pow(n/xmax,0.75);
		}
		else
		{
			temp = 1.0;
		}
		*(weight+2*k) = temp;
		*(weight+2*k+1) = temp;
		k++;
	}
	fclose(fp);
	free(ww);
}

void ReleaseData()
{
	free(inPos);
	free(outPos);
	free(logval);
	free(weight);
}

void WriteData(char * filename_in_vec, char * filename_out_vec)
{
	int i,j;
	FILE * fp;
	fp = fopen(filename_in_vec,"w");
	for(i = 0;i < V;i++)
	{
		for(j = 0;j < Dim-1;j++)
		{
			fprintf(fp,"%lf ",*(U+i*Dim+j));
		}
		fprintf(fp,"%lf\n",*(U+i*Dim+Dim-1));
	}
	fclose(fp);
	fp = fopen(filename_out_vec,"w");
	for(i = 0;i < V;i++)
	{
		for(j = 0;j < Dim-1;j++)
		{
			fprintf(fp,"%lf ",*(Vec+i*Dim+j)+*(U+i*Dim+j));
		}
		fprintf(fp,"%lf\n",*(Vec+i*Dim+Dim-1)+*(U+i*Dim+Dim-1));
	}
	fclose(fp);
}

void GradientDescent(double eta, int idx, int adagrad)
{
	int pos_in,pos_out,i;
	double s,coeff,gradv,gradu,temp;
	pos_in = *(inPos+idx);
	pos_out = *(outPos+idx);
	
	s = 0.0;
	for(i = 0;i < Dim;i++)
	{
		s += (*(Vec+pos_in*Dim+i))*(*(U+pos_out*Dim+i));
	}
	if(adagrad)
	{
		coeff = 2*(*(weight+idx))*(s - *(logval+idx));
		for(i = 0;i < Dim;i++)
		{
			gradv = coeff*(*(U+pos_out*Dim+i));
			gradu = coeff*(*(Vec+pos_in*Dim+i));
			*(GV+pos_in*Dim+i) += gradv*gradv;
			*(GU+pos_out*Dim+i) += gradu*gradu;
			*(Vec+pos_in*Dim+i) -= eta*gradv/sqrt(*(GV+pos_in*Dim+i)+1e-7);
			*(U+pos_out*Dim+i) -= eta*gradu/sqrt(*(GU+pos_out*Dim+i)+1e-7);
		}
	}
	else
	{
		coeff = eta*2*(*(weight+idx))*(s - *(logval+idx));
		for(i = 0;i < Dim;i++)
		{
			temp = *(Vec+pos_in*Dim+i);
			*(Vec+pos_in*Dim+i) -= coeff*(*(U+pos_out*Dim+i));
			*(U+pos_out*Dim+i) -= coeff*temp;
		}
	}
}

double OnlineLoss()
{
	int pos_in,pos_out,d,i;
	double loss,s,temp;
	
	loss = 0.0;
	for(d = 0;d < 2*C;d++)
	{
		pos_in = *(inPos+d);
		pos_out = *(outPos+d);
		s = 0.0;
		for(i = 0;i < Dim;i++)
		{
			s += (*(Vec+pos_in*Dim+i))*(*(U+pos_out*Dim+i));
		}
		temp = s-*(logval+d);
		loss += (*(weight+d))*temp*temp;
	}
	return loss;
}

void TrainingModel(int vec_dim, int iters, double init_eta, double x_max, int adagrad, int show_loss)
{
	int i,d,count,pos,rd,temp;
	double eta,loss;
	FILE * fp;
	int *orders;
	Dim = vec_dim;

	InitData("words_pats_pairs.txt","words_weight.txt",x_max);

	srand((unsigned)time(NULL));
	U = (double*)malloc(V*Dim*sizeof(double));
	Vec = (double*)malloc(V*Dim*sizeof(double));
	if(adagrad)
	{
		GU = (double*)malloc(V*Dim*sizeof(double));
		GV = (double*)malloc(V*Dim*sizeof(double));
	}
	else
	{
		GU = NULL;
		GV = NULL;
	}
	for(i = 0;i < V*Dim;i++)
	{
		*(U+i) = (rand()/(RAND_MAX+0.1)-0.5)/Dim;
		*(Vec+i) = (rand()/(RAND_MAX+0.1)-0.5)/Dim;
		if(adagrad)
		{
			*(GU+i) = 0.0;
			*(GV+i) = 0.0;
		}
	}
	orders = (int*)malloc(2*C*sizeof(int));
	for(i = 0;i < 2*C;i++)
	{
		*(orders+i) = i;
	}

	if(show_loss)
	{
		fp = fopen("loss.txt","w");
	}
	for(count = 0;count < iters;count++)
	{
		printf("%d\n",count+1);
		for(i = 0;i < 2*C-1;i++)
		{
			pos = i+rand()%(2*C-i);
			temp = *(orders+i);
			*(orders+i) = *(orders+pos);
			*(orders+pos) = temp;
		}
		for(d = 0;d < 2*C;d++)
		{
			if(adagrad)
			{
				eta = init_eta;
			}
			else
			{
				eta = init_eta*(1.0-(count*2*C+d)/(double)(iters*2*C));
				if(eta < 0.0001*init_eta)
				{
					eta = 0.0001*init_eta;
				}
			}
			rd = *(orders+d);	
			GradientDescent(eta,rd,adagrad);
		}
		if(show_loss)
		{
			loss = OnlineLoss();
			printf("Online Loss: %lf\n",loss);
			fprintf(fp,"%lf\n",loss);
		}
	}
	if(show_loss)
	{
		fclose(fp);
	}
	WriteData("inner_vecs.txt","vecs.txt");

	ReleaseData();
	free(U);
	free(Vec);
	free(orders);
	if(adagrad)
	{
		free(GU);
		free(GV);
	}
}

