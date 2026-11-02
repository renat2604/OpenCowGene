/* OpenCowGene Math library (ndarray.c)
 * License: BSD-3-Clause
 * Author: Nikita (nst) Stavtsev
 */
#include "ocgmath.h"

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* FloatNDA Implementations */
int FloatNDAInit(FloatNDA *arr, size_t ndim, const size_t *shape) {
	if (!arr || !shape || ndim == 0 || ndim > NDARRAY_MAX_DIMS)
		return -1;
	
	FloatNDA tmp = { 0 };
	tmp.ndim = ndim;
	tmp.owns_data = true;

	size_t stride = 1;

	for (size_t i = ndim; i > 0; i--) {
		if (shape[i] == 0)
			return -1;
		if (stride > SIZE_MAX / shape[i])
			return -1;
		tmp.shape[i] = shape[i];
		tmp.strides[i] = stride;
		stride *= shape[i];
	}

	tmp.size = stride;

	if (tmp.size > SIZE_MAX / sizeof(float))
		return -1;

	tmp.data = calloc(tmp.size, sizeof(float));

	if (!tmp.data)
		return -1;

	*arr = tmp;
	return 0;
}

void FloatNDAFree(FloatNDA *arr) {
	if (!arr)
		return;
	if (arr->owns_data)
		free(arr->data);

	*arr = (FloatNDA){0};
}

int FloatNDAAdd(FloatNDA *res, const FloatNDA *a, const FloatNDA *b) {
    if (a->size != b->size || a->size != res->size)
        return -1;

    for (size_t i = 0; i < a->size; ++i)
        res->data[i] = a->data[i] + b->data[i];

    return 0;
}

void FloatNDAFill(FloatNDA *arr, float val) {
	for (size_t i = 0; i < arr->size; ++i) {
		arr->data[i] = val;
	}
}

int FloatNDASet(FloatNDA *arr, const size_t *indices, float val) {
	if (!arr || !arr->data || !indices)
		return -1;

	size_t offset = 0;
	for (size_t i = 0; i < arr->ndim; i++) {
		if (indices[i] >= arr->shape[i])
			return -1;

		offset += indices[i] * arr->strides[i];
	}

	arr->data[offset] = val;
	return 0;
}

int FloatNDAGet(const FloatNDA *arr, const size_t *indices, float *out) {
	if (!arr || !arr->data || !indices || !out)
		return -1;
	
	size_t offset = 0;
	for (size_t i = 0; i < arr->ndim; i++) {
		if (indices[i] >= arr->shape[i])
			return -1;
		offset += indices[i] * arr->strides[i];
	}

	*out = arr->data[offset];

	return 0;
}
