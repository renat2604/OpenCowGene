/* OpenCowGene Math library (ocgmath.c)
 * Description: Math library for OpenCowGene needs
 * License: BSD-3-Clause
 * Author: Nikita (nst) Stavtsev
 */

#include "ocgmath.h"

#include <math.h>

/* VECTOR INIT FUNCTIONS */
inline Vector2 Vector2Zero(void) {
	return (Vector2){ 0.0f, 0.0f };
}
inline Vector2 Vector2One(void) {
	return (Vector2){ 1.0f, 1.0f };
}

/* VECTOR ADD FUNCTIONS */
inline Vector2 Vector2Add(Vector2 v1, Vector2 v2) {
	return (Vector2){ v1.x + v2.x, v1.y + v2.y };
}
inline Vector2 Vector2AddValue(Vector2 v, float add) {
	return (Vector2){ v.x + add, v.y + add };
}

/* VECTOR SUBTRACT FUNCTIONS */
inline Vector2 Vector2Sub(Vector2 v1, Vector2 v2) {
	return (Vector2){ v1.x - v2.x, v1.y - v2.y };
}

inline Vector2 Vector2SubVal(Vector2 v, float sub) {
	return (Vector2){ v.x - sub, v.y - sub };
}

/* VECTOR LENGTH FUNCTIONS */
inline float Vector2Len(Vector2 v) {
	return sqrtf((v.x*v.x) + (v.y*v.y));
}

inline float Vector2LenSqr(Vector2 v) {
	return (v.x * v.x) + (v.y * v.y);
}

/* VECTOR PRODUCTS FUNCTIONS */
inline float Vector2DotProduct(Vector2 v1, Vector2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

/* VECTOR DISTANCE FUNCTIONS */
inline float Vector2Distance(Vector2 v1, Vector2 v2) {
	return sqrtf(
		(v1.x - v2.x)*(v1.x - v2.x) 
		+ (v1.y - v2.y)*(v1.y - v2.y)
	);
}

inline float Vector2DistanceSqr(Vector2 v1, Vector2 v2) {
	return (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y);
}

/* VECTOR ANGLE FUNCTIONs */
/* VECTOR MULTIPLY FUNCTION */
Vector2 Vector2Scale(Vector2 v, float scale) {
	return (Vector2){ v.x * scale, v.y * scale };
}

/* VECTOR NEGATE FUNCTION */
Vector2 Vector2Negate(Vector2 v) {
	return (Vector2){ -v.x, -v.y };
}

/* VECTOR NORMILIZE FUNCION */
Vector2 Vector2Normailize(Vector2 v) {
	Vector2 result = { 0 };
	float length = sqrtf((v.x * v.x) + (v.y * v.y));

	if (length > 0) {
		float ilength = 1.0f / length;
		result.x = v.x * ilength;
		result.y = v.y * ilength;
	}

	return result;
}
