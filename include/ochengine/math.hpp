#pragma once

static float FindSquare(float x) {
    return x * x;
}

static float FindDot(float x1, float y1, float x2, float y2) {
    return x1 * x2 + y1 * y2;
}

static float FindMin(float x, float y) {
    return (x < y) ? x : y;
}

static float FindMax(float x, float y) {
    return (x > y) ? x : y;
}

static float FindClamp(float x, float minX, float maxX) {
    return FindMax(minX, FindMin(x, maxX));
}

static float FindSign(float x) {
    return (x >= 0) ? 1.0f : -1.0f;
}

static float FindAbs(float x) {
    return (x >= 0) ? x : -x;
}

static float FindSqrt(float x)
{
    if (x == 0 || x == 1) {
        return x;
    }

    float y = x;
    float z = (y + (x / y)) / 2;

    while (FindAbs(y - z) >= 0.0000001) { // 7 digits
        y = z;
        z = (y + (x / y)) / 2;
    }

    return z;
}

