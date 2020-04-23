#include "matrix.h"



void mx1x2_tim_mx2x1(float * t_mx1, float * t_mx2, float * t_res){
	t_res[0] = t_mx1[0] * t_mx2[0] + t_mx1[1] * t_mx2[1];
}

void mx1x2_tim_mx2x2(float * t_mx1, float * t_mx2, float * t_res){
	 t_res[0] = t_mx1[0] * t_mx2[0] + t_mx1[1] * t_mx2[2];
	 t_res[1] = t_mx1[0] * t_mx2[1] + t_mx1[1] * t_mx2[3];
}



void mx2x1_tim_mx1x1(float * t_mx1, float * t_mx2, float * t_res){
	t_res[0] = t_mx1[0] * t_mx2[0];
	t_res[1] = t_mx1[1] * t_mx2[0];
}









void matrix_2x2_mul_2x1(float * in1, float * in2, float * out)
{
   out[0] = in1[0] * in2[0] + in1[1] * in2[1];
   out[1] = in1[2] * in2[0] + in1[3] * in2[1];
}


void matrix_2x1_add_2x1(float * in1, float * in2, float * out)
{
   out[0] = in1[0] + in2[0];
   out[1] = in1[1] + in2[1];
}

void matrix_2x2_mul_2x2(float * in1, float * in2, float * out)
{
   out[0] = in1[0] * in2[0] + in1[1] * in2[2];
   out[1] = in1[0] * in2[1] + in1[1] * in2[3];
   out[2] = in1[2] * in2[0] + in1[3] * in2[2];
   out[3] = in1[2] * in2[1] + in1[3] * in2[3];
}

void matrix_2x2_add_2x2(float * in1, float * in2, float * out)
{
   out[0] = in1[0] + in2[0];
   out[1] = in1[1] + in2[1];
   out[2] = in1[2] + in2[2];
   out[3] = in1[3] + in2[3];
}



void matrix_2x1_mul_1x2(float * in1, float * in2, float * out)
{
   out[0] = in1[0] * in2[0];
   out[1] = in1[0] * in2[1];
   out[2] = in1[1] * in2[0];
   out[3] = in1[1] * in2[1];
}

void matrix_2x2_sub_2x2(float * in1, float * in2, float * out)
{
   out[0] = in1[0] - in2[0];
   out[1] = in1[1] - in2[1];
   out[2] = in1[2] - in2[2];
   out[3] = in1[3] - in2[3];
}

void matrix_2x2_trans(float * in, float * out)
{
   out[0] = in[0];
   out[1] = in[2];
   out[2] = in[1];
   out[3] = in[3];
}
