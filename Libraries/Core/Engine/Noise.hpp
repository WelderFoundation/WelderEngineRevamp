// MIT Licensed (see LICENSE.md).

namespace Zero
{
float Noise(int x, int y);
float SmoothNoise(int x, int y);
float InterpolatedNoise(float x, float y);
float PerlinNoise(float x, float y);
float PerlinNoise(float persistence, float frequency, float limit, float x, float y);

float Noise(int x);
float SmoothNoise(int x);
float InterpolatedNoise(float x);
float PerlinNoise(float x);
} // namespace Zero
