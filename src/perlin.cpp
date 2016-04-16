// Code from http://www.flipcode.com/archives/Perlin_Noise_Class.shtml
// This is only in PolyVox for the purpose of the examples. It was not written
// by
// the PolyVox authors and cannot be assumed to be under the same license as
// PolyVox.

/* coherent noise function over 1, 2 or 3 dimensions */
/* (copyright Ken Perlin) */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "perlin.h"

#define B SAMPLE_SIZE
#define BM (SAMPLE_SIZE - 1)

#define N 0x1000
#define NP 12 /* 2^N */
#define NM 0xfff

#define s_curve(t) (t * t * (3.0f - 2.0f * t))
#define lerp(t, a, b) (a + t * (b - a))

#define setup(i, b0, b1, r0, r1) \
    t = vec[i] + N;              \
    b0 = ((int)t) & BM;          \
    b1 = (b0 + 1) & BM;          \
    r0 = t - (int)t;             \
    r1 = r0 - 1.0f;

float Perlin::noise1(float arg) {
    int bx0, bx1;
    float rx0, rx1, sx, t, u, v, vec[1];

    vec[0] = arg;

    if (start_) {
        srand(seed_);
        start_ = false;
        init();
    }

    setup(0, bx0, bx1, rx0, rx1);

    sx = s_curve(rx0);

    u = rx0 * g1_[p_[bx0]];
    v = rx1 * g1_[p_[bx1]];

    return lerp(sx, u, v);
}

float Perlin::noise2(float vec[2]) {
    int bx0, bx1, by0, by1, b00, b10, b01, b11;
    float rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
    int i, j;

    if (start_) {
        srand(seed_);
        start_ = false;
        init();
    }

    setup(0, bx0, bx1, rx0, rx1);
    setup(1, by0, by1, ry0, ry1);

    i = p_[bx0];
    j = p_[bx1];

    b00 = p_[i + by0];
    b10 = p_[j + by0];
    b01 = p_[i + by1];
    b11 = p_[j + by1];

    sx = s_curve(rx0);
    sy = s_curve(ry0);

#define at2(rx, ry) (rx * q[0] + ry * q[1])

    q = g2_[b00];
    u = at2(rx0, ry0);
    q = g2_[b10];
    v = at2(rx1, ry0);
    a = lerp(sx, u, v);

    q = g2_[b01];
    u = at2(rx0, ry1);
    q = g2_[b11];
    v = at2(rx1, ry1);
    b = lerp(sx, u, v);

    return lerp(sy, a, b);
}

float Perlin::noise3(float vec[3]) {
    int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
    float rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
    int i, j;

    if (start_) {
        srand(seed_);
        start_ = false;
        init();
    }

    setup(0, bx0, bx1, rx0, rx1);
    setup(1, by0, by1, ry0, ry1);
    setup(2, bz0, bz1, rz0, rz1);

    i = p_[bx0];
    j = p_[bx1];

    b00 = p_[i + by0];
    b10 = p_[j + by0];
    b01 = p_[i + by1];
    b11 = p_[j + by1];

    t = s_curve(rx0);
    sy = s_curve(ry0);
    sz = s_curve(rz0);

#define at3(rx, ry, rz) (rx * q[0] + ry * q[1] + rz * q[2])

    q = g3_[b00 + bz0];
    u = at3(rx0, ry0, rz0);
    q = g3_[b10 + bz0];
    v = at3(rx1, ry0, rz0);
    a = lerp(t, u, v);

    q = g3_[b01 + bz0];
    u = at3(rx0, ry1, rz0);
    q = g3_[b11 + bz0];
    v = at3(rx1, ry1, rz0);
    b = lerp(t, u, v);

    c = lerp(sy, a, b);

    q = g3_[b00 + bz1];
    u = at3(rx0, ry0, rz1);
    q = g3_[b10 + bz1];
    v = at3(rx1, ry0, rz1);
    a = lerp(t, u, v);

    q = g3_[b01 + bz1];
    u = at3(rx0, ry1, rz1);
    q = g3_[b11 + bz1];
    v = at3(rx1, ry1, rz1);
    b = lerp(t, u, v);

    d = lerp(sy, a, b);

    return lerp(sz, c, d);
}

void Perlin::normalize2(float v[2]) {
    float s;

    s = (float)sqrt(v[0] * v[0] + v[1] * v[1]);
    s = 1.0f / s;
    v[0] = v[0] * s;
    v[1] = v[1] * s;
}

void Perlin::normalize3(float v[3]) {
    float s;

    s = (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    s = 1.0f / s;

    v[0] = v[0] * s;
    v[1] = v[1] * s;
    v[2] = v[2] * s;
}

void Perlin::init(void) {
    int i, j, k;

    for (i = 0; i < B; i++) {
        p_[i] = i;
        g1_[i] = (float)((rand() % (B + B)) - B) / B;
        for (j = 0; j < 2; j++)
            g2_[i][j] = (float)((rand() % (B + B)) - B) / B;
        normalize2(g2_[i]);
        for (j = 0; j < 3; j++)
            g3_[i][j] = (float)((rand() % (B + B)) - B) / B;
        normalize3(g3_[i]);
    }

    while (--i) {
        k = p_[i];
        p_[i] = p_[j = rand() % B];
        p_[j] = k;
    }

    for (i = 0; i < B + 2; i++) {
        p_[B + i] = p_[i];
        g1_[B + i] = g1_[i];
        for (j = 0; j < 2; j++)
            g2_[B + i][j] = g2_[i][j];
        for (j = 0; j < 3; j++)
            g3_[B + i][j] = g3_[i][j];
    }
}

float Perlin::perlin_noise_2D(float vec[2]) {
    int terms = octaves_;
    float result = 0.0f;
    float amp = amp_;

    vec[0] *= freq_;
    vec[1] *= freq_;

    for (int i = 0; i < terms; i++) {
        result += noise2(vec) * amp;
        vec[0] *= 2.0f;
        vec[1] *= 2.0f;
        amp *= 0.5f;
    }

    return result;
}

float Perlin::perlin_noise_3D(float vec[3]) {
    int terms = octaves_;
    float result = 0.0f;
    float amp = amp_;

    vec[0] *= freq_;
    vec[1] *= freq_;
    vec[2] *= freq_;

    for (int i = 0; i < terms; i++) {
        result += noise3(vec) * amp;
        vec[0] *= 2.0f;
        vec[1] *= 2.0f;
        vec[2] *= 2.0f;
        amp *= 0.5f;
    }

    return result;
}

Perlin::Perlin(int octaves, float freq, float amp, int seed) {
    octaves_ = octaves;
    freq_ = freq;
    amp_ = amp;
    seed_ = seed;
    start_ = true;
}
