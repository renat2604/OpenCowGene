#ifndef OCG_MATH_H
#define OCG_MATH_H

typedef struct Vector2 Vector2;

struct Vector2 {
    float x;
    float y;
};

// Vector with components value 0.0f
Vector2 Vector2Zero(void);

// Vector with components value 1.0f
Vector2 Vector2One(void);

// Add two vectors (v1 + v2)
Vector2 Vector2Add(Vector2, Vector2);

// Add vector and float value
Vector2 Vector2AddVal(Vector2, float);

// Subtract two vectors (v1 - v2)
Vector2 Vector2Sub(Vector2, Vector2);

// Subract vector by float value
Vector2 Vector2SubVal(Vector2, float); 

// Calculate vector length
float Vector2Len(Vector2);

// Calculate vector square length
float Vector2LenSqr(Vector2);

// Calculate two vectors dot product
float Vector2DotProduct(Vector2, Vector2);

// Calculate distance between two vectors
float Vector2Distance(Vector2, Vector2);

// Calculate square distance between two vectors
float Vector2DistanceSqr(Vector2, Vector2);

// Calclulate angle between two vectors
// float Vector2Angle(Vector2, Vector2);

// Scale vector by value
Vector2 Vector2Scale(Vector2, float);

// Negate vector
Vector2 Vector2Negate(Vector2);

// Divide vector by vector
Vector2 Vector2Divide(Vector2, Vector2);

// Normailize vector
Vector2 Vector2Normailize(Vector2);

#endif
