// Potrzebne operacje na macierzach


#ifndef MATRIX_H_
#define MATRIX_H_


void mx1x2_tim_mx2x1(float * t_mx1, float * t_mx2, float * t_res);
void mx1x2_tim_mx2x2(float * t_mx1, float * t_mx2, float * t_res);


void mx2x1_tim_mx1x1(float * t_mx1, float * t_mx2, float * t_res);
void matrix_2x1_add_2x1(float * in1, float * in2, float * out);
void matrix_2x1_mul_1x2(float * in1, float * in2, float * out);

void matrix_2x2_mul_2x1(float * in1, float * in2, float * out);
void matrix_2x2_mul_2x2(float * in1, float * in2, float * out);
void matrix_2x2_add_2x2(float * in1, float * in2, float * out);
void matrix_2x2_sub_2x2(float * in1, float * in2, float * out);
void matrix_2x2_trans(float * in, float * out);


#endif /* MATRIX_H_ */



