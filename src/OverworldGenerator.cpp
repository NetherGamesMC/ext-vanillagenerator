#include <iostream>
#include <chrono>

#include <blocks/Blocks.h>
#include <blocks/Chunk.h>
#include <math/Random.h>
#include <noise/PerlinNoise.h>
#include <noise/octaves/Octaves.h>
#include <OverworldGenerator.h>
#include "noise/octaves/SimplexOctaveGenerator.h"

using namespace std;

int main() {
  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;
  using std::chrono::nanoseconds;

  block::Block::RegisterAllBlocks();

  Random rand(32);

  FastNoise2::SimplexOctaves octaves(rand, 8, [&](FastNoise2::OctaveSettings &settings) {
    settings.SetSize<FastNoise2::Pos::Y>(2);
    settings.SetSize<FastNoise2::Pos::X>(2);

    settings.SetScale<FastNoise2::Pos::Y>(2);
    settings.SetScale<FastNoise2::Pos::X>(2);
  });

  auto t3 = high_resolution_clock::now();
  FastNoise2::NoiseArray result = octaves.Fractal(2, 1, 0.5, 2.0);
  auto t4 = high_resolution_clock::now();

  duration<double, std::nano> result_2 = t4 - t3;

  cout << "OK New: " << result_2.count() << "ns\n";

  Random rand2(32);
  SimplexOctaveGenerator generator(rand2, 8, 2, 1, 2);
  generator.SetScale(2);

  auto resultOut = generator.GetFractalBrownianMotion(2, 1, 1, 0.5, 2.0);

  auto t1 = high_resolution_clock::now();
  resultOut = generator.GetFractalBrownianMotion(2, 1, 1, 0.5, 2.0);
  auto t2 = high_resolution_clock::now();

  duration<double, std::nano> result_1 = t2 - t1;

  cout << "OK Old: " << result_1.count() << "ns\n";


  return 0;
}