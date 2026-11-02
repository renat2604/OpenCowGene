/* OpenCowGene Math library (ocgmath.h)
 * License: BSD-3-Clause
 * Author: Nikita (nst) Stavtsev
 */

#ifndef OCG_MATH_H
#define OCG_MATH_H

#include <unistd.h>
#include <stdbool.h>

#define NDARRAY_MAX_DIMS 8

typedef struct {
    float *data;
    size_t ndim;
    size_t shape[NDARRAY_MAX_DIMS];
    size_t strides[NDARRAY_MAX_DIMS];
    size_t size;
    bool owns_data;
} FloatNDA;

int FloatNDAInit(FloatNDA *arr, size_t ndim, const size_t *shape);
void FloatNDAFree(FloatNDA *arr);
int FloatNDAAdd(FloatNDA*, const FloatNDA*, const FloatNDA*);
void FloatNDAFill(FloatNDA *arr, float val);
int FloatNDASet(FloatNDA *arr, const size_t *indices, float val);
int FloatNDAGet(const FloatNDA *arr, const size_t *indices, float *out);

#endif
