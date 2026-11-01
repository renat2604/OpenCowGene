/* OpenCowGene Math library (ocgmath.h)
 * License: BSD-3-Clause
 * Author: Nikita (nst) Stavtsev
 */

#ifndef OCG_MATH_H
#define OCG_MATH_H

#include <unistd.h>
#include <stdbool.h>

#define NDARRAY_MAX_DIMS 8

typedef struct Vector2 Vector2;
struct Vector2 {
    float x;
    float y;
};

Vector2 Vector2Zero(void);
Vector2 Vector2One(void);
Vector2 Vector2Add(Vector2, Vector2);
Vector2 Vector2AddVal(Vector2, float);
Vector2 Vector2Sub(Vector2, Vector2);
Vector2 Vector2SubVal(Vector2, float); 
float Vector2Len(Vector2);
float Vector2LenSqr(Vector2);
float Vector2DotProduct(Vector2, Vector2);
float Vector2Distance(Vector2, Vector2);
float Vector2DistanceSqr(Vector2, Vector2);
Vector2 Vector2Scale(Vector2, float);
Vector2 Vector2Negate(Vector2);
Vector2 Vector2Divide(Vector2, Vector2);
Vector2 Vector2Normailize(Vector2);


typedef struct {
    float *data;
    size_t ndim;
    size_t shape[NDARRAY_MAX_DIMS];
    size_t strides[NDARRAY_MAX_DIMS];
    size_t size;
    bool owns_data;
} FloatNDArray;

int FloatNDArrayInit(FloatNDArray *arr, size_t ndim, const size_t *shape);
void FloatNDArrayFree(FloatNDArray *arr);
int FloatNDArrayAdd(FloatNDArray*, const FloatNDArray*, const FloatNDArray*);
void FloatNDArrayFill(FloatNDArray *arr, float val);
int FloatNDArraySet(FloatNDArray *arr, const size_t *indices, float val);
int FloatNDArrayGet(const FloatNDArray *arr, const size_t *indices, float *out);

#endif
