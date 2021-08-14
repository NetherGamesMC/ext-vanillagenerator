#include <lib/objects/math/Math.h>
#include <lib/objects/constants/BlockList.h>
#include <lib/objects/math/Facing.h>
#include <lib/objects/constants/Logic.h>
#include "CaveCarver.h"

const int_fast32_t CaveCarver::chunkRadius = 8;
const int_fast32_t CaveCarver::density = 16;
const int_fast32_t CaveCarver::topY = 128;
const int_fast32_t CaveCarver::bottomY = 8;
const int_fast32_t CaveCarver::caveLiquidAltitude = 10;

void CaveCarver::Generate(ChunkManager &manager, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ, Chunk *chunk) {
  rand.SetSeed(random.GetSeed());

  int_fast64_t j = rand.NextLong();
  int_fast64_t k = rand.NextLong();
  for (int currChunkX = chunkX - chunkRadius; currChunkX <= chunkX + chunkRadius; ++currChunkX) {
    for (int currChunkZ = chunkZ - chunkRadius; currChunkZ <= chunkZ + chunkRadius; ++currChunkZ) {
      int_fast64_t j1 = currChunkX * j;
      int_fast64_t k1 = currChunkZ * k;

      rand.SetSeed(j1 ^ k1 ^ random.GetSeed());

      recursiveGenerate(currChunkX, currChunkZ, chunkX, chunkZ, chunk, true);
    }
  }
}

void CaveCarver::recursiveGenerate(int_fast32_t chunkX, int_fast32_t chunkZ, int_fast32_t originalChunkX, int_fast32_t originalChunkZ, Chunk *chunk, bool addRooms) {
  auto numAttempts = static_cast<int_fast32_t>(rand.NextInt(rand.NextInt(rand.NextInt(15) + 1) + 1));

  if (rand.NextInt(100) > density) {
    numAttempts = 0;
  }

  for (int_fast32_t i = 0; i < numAttempts; ++i) {
    double caveStartX = chunkX * 16 + static_cast<int_fast32_t>(rand.NextInt(16));
    double caveStartY = static_cast<int_fast32_t>(rand.NextInt(topY - bottomY)) + bottomY;
    double caveStartZ = chunkZ * 16 + static_cast<int_fast32_t>(rand.NextInt(16));

    int_fast32_t numAddTunnelCalls = 1;

    if (addRooms && rand.NextInt(4) == 0) {
      addRoom(rand.NextLong(), originalChunkX, originalChunkZ, chunk, caveStartX, caveStartY, caveStartZ);
      numAddTunnelCalls += static_cast<int_fast32_t>(rand.NextInt(4));
    }

    for (int j = 0; j < numAddTunnelCalls; ++j) {
      float yaw = rand.NextFloat() * ((float) M_PI * 2);
      float pitch = (rand.NextFloat() - 0.5F) * 2.0F / 8.0F;
      float width = rand.NextFloat() * 2.0F + rand.NextFloat();

      // Chance of wider caves.
      // Although not actually related to adding rooms, I perform an addRoom check here
      // to avoid the chance of really large caves when generating surface caves.
      if (addRooms && rand.NextInt(10) == 0) {
        width *= rand.NextFloat() * rand.NextFloat() * 3.0F + 1.0F;
      }

      addTunnel(rand.NextLong(), originalChunkX, originalChunkZ, chunk, caveStartX, caveStartY, caveStartZ, width, yaw, pitch, 0, 0, 1.0);
    }
  }
}

void CaveCarver::addRoom(int_fast64_t seed, int_fast32_t originChunkX, int_fast32_t originChunkZ, Chunk *chunk, double caveStartX, double caveStartY, double caveStartZ) {
  addTunnel(seed, originChunkX, originChunkZ, chunk, caveStartX, caveStartY, caveStartZ, 1.0F + rand.NextFloat() * 6.0F, 0.0F, 0.0F, -1, -1, 0.5);
}

void CaveCarver::addTunnel(int_fast64_t seed, int_fast32_t originChunkX, int_fast32_t originChunkZ, Chunk *chunk, double caveStartX, double caveStartY, double caveStartZ, float width, float yaw, float pitch, int_fast32_t startCounter, int_fast32_t endCounter, double heightModifier) {
  Random random = Random(seed);

  // Center block of the origin chunk
  double originBlockX = (originChunkX * 16 + 8);
  double originBlockZ = (originChunkZ * 16 + 8);

  // Variables to slightly change the yaw/pitch for each iteration in the while loop below.
  float yawModifier = 0.0;
  float pitchModifier = 0.0;

  // Raw calls to addTunnel from recursiveGenerate give startCounter and endCounter a value of 0.
  // Calls from addRoom make them both -1.

  // This appears to be called regardless of where addTunnel was called from.
  if (endCounter <= 0) {
    int i = chunkRadius * 16 - 16;
    endCounter = i - static_cast<int_fast32_t>(random.NextInt(i / 4));
  }

  // Whether or not this function call was made from addRoom.
  bool comesFromRoom = false;

  // Only called if the function call came from addRoom.
  // If this call came from addRoom, startCounter is set to halfway to endCounter.
  // If this is a raw call from recursiveGenerate, startCounter will be zero.
  if (startCounter == -1) {
    startCounter = endCounter / 2;
    comesFromRoom = true;
  }

  auto randomCounterValue = static_cast<int_fast32_t>(random.NextInt(endCounter / 2) + endCounter / 4);

  // Loops one block at a time to the endCounter (about 6-7 chunks away on average).
  // startCounter starts at either zero or endCounter / 2.
  while (startCounter < endCounter) {
    // Appears to change how wide caves are. Value will be between 1.5 and 1.5 + width.
    // Note that caves will become wider toward the middle, and close off on the ends.
    double xzOffset = 1.5 + (double) (sin((float) startCounter * (float) M_PI / (float) endCounter) * width);

    // Appears to just be a linear modifier for the cave height
    double yOffset = xzOffset * heightModifier;

    float pitchXZ = cos(pitch);
    float pitchY = sin(pitch);
    caveStartX += cos(yaw) * pitchXZ;
    caveStartY += pitchY;
    caveStartZ += sin(yaw) * pitchXZ;

    bool flag = random.NextInt(6) == 0;
    if (flag) {
      pitch = pitch * 0.92F;
    } else {
      pitch = pitch * 0.7F;
    }

    pitch = pitch + pitchModifier * 0.1F;
    yaw += yawModifier * 0.1F;

    pitchModifier = pitchModifier * 0.9F;
    yawModifier = yawModifier * 0.75F;

    pitchModifier = pitchModifier + (random.NextFloat() - random.NextFloat()) * random.NextFloat() * 2.0F;
    yawModifier = yawModifier + (random.NextFloat() - random.NextFloat()) * random.NextFloat() * 4.0F;

    if (!comesFromRoom && startCounter == randomCounterValue && width > 1.0F && endCounter > 0) {
      addTunnel(random.NextLong(), originChunkX, originChunkZ, chunk, caveStartX, caveStartY, caveStartZ, random.NextFloat() * 0.5F + 0.5F, yaw - ((float) M_PI / 2.0F), pitch / 3.0F, startCounter, endCounter, 1.0);
      addTunnel(random.NextLong(), originChunkX, originChunkZ, chunk, caveStartX, caveStartY, caveStartZ, random.NextFloat() * 0.5F + 0.5F, yaw + ((float) M_PI / 2.0F), pitch / 3.0F, startCounter, endCounter, 1.0);
      return;
    }

    if (comesFromRoom || random.NextInt(4) != 0) {
      double caveStartXOffsetFromCenter = caveStartX - originBlockX; // Number of blocks from current caveStartX to center of origin chunk
      double caveStartZOffsetFromCenter = caveStartZ - originBlockZ; // Number of blocks from current caveStartZ to center of origin chunk
      double distanceToEnd = endCounter - startCounter;
      double d7 = width + 2.0F + 16.0F;

      // I think this prevents caves from generating too far from the origin chunk
      if (caveStartXOffsetFromCenter * caveStartXOffsetFromCenter + caveStartZOffsetFromCenter * caveStartZOffsetFromCenter - distanceToEnd * distanceToEnd > d7 * d7) {
        return;
      }

      // Only continue if cave start is close enough to origin
      if (caveStartX >= originBlockX - 16.0 - xzOffset * 2.0 && caveStartZ >= originBlockZ - 16.0 - xzOffset * 2.0 && caveStartX <= originBlockX + 16.0 + xzOffset * 2.0 && caveStartZ <= originBlockZ + 16.0 + xzOffset * 2.0) {
        auto minX = static_cast<int_fast8_t>(Math::Floor(caveStartX - xzOffset) - originChunkX * 16 - 1);
        auto minY = static_cast<int_fast16_t>(Math::Floor(caveStartY - yOffset) - 1);
        auto minZ = static_cast<int_fast8_t>(Math::Floor(caveStartZ - xzOffset) - originChunkZ * 16 - 1);
        auto maxX = static_cast<int_fast8_t>(Math::Floor(caveStartX + xzOffset) - originChunkX * 16 + 1);
        auto maxY = static_cast<int_fast16_t>(Math::Floor(caveStartY + yOffset) + 1);
        auto maxZ = static_cast<int_fast8_t>(Math::Floor(caveStartZ + xzOffset) - originChunkZ * 16 + 1);

        if (minX < 0)   minX = 0;
        if (maxX > 16)  maxX = 16;
        if (minY < 1)   minY = 1;
        if (maxY > 248) maxY = 248;
        if (minZ < 0)   minZ = 0;
        if (maxZ > 16)  maxZ = 16;

        for (int_fast8_t currX = minX; currX < maxX; ++currX) {
          // Distance along the x-axis from the center (caveStart) of this ellipsoid.
          // You can think of this value as (x/a), where 'a' is the length of the ellipsoid's semi-axis in the x direction.
          double xAxisDist = ((double) (currX + originChunkX * 16) + 0.5 - caveStartX) / xzOffset;

          for (int_fast8_t currZ = minZ; currZ < maxZ; ++currZ) {
            // Distance along the z-axis from the center (caveStart) of this ellipsoid.
            // You can think of this value as (z/b), where b is the length of the ellipsoid's semi-axis in the z direction (same as 'a' in this case).
            double zAxisDist = ((double) (currZ + originChunkZ * 16) + 0.5 - caveStartZ) / xzOffset;

            // Only operate on points within ellipse on XZ axis. Avoids unnecessary computation along y axis
            if (xAxisDist * xAxisDist + zAxisDist * zAxisDist < 1.0) {
              for (int_fast16_t currY = maxY; currY > minY; --currY) {
                // Distance along the y-axis from the center (caveStart) of this ellipsoid.
                // You can think of this value as (y/c), where c is the length of the ellipsoid's semi-axis in the y direction.
                double yAxisDist = ((double) (currY - 1) + 0.5 - caveStartY) / yOffset;

                // Only operate on points within the ellipsoid.
                // This conditional is validating the current coordinate against the equation of the ellipsoid, that is,
                // (x/a)^2 + (z/b)^2 + (y/c)^2 <= 1.
                if (yAxisDist > -0.7 && xAxisDist * xAxisDist + yAxisDist * yAxisDist + zAxisDist * zAxisDist < 1.0) {
                  digBlock(chunk, currX, currY, currZ);
                }
              }
            }
          }
        }

        if (comesFromRoom) {
          break;
        }
      }
    }
    startCounter++;
  }
}

void CaveCarver::digBlock(Chunk *chunk, int_fast8_t currX, int_fast16_t currY, int_fast8_t currZ) {
  if (canReplaceBlock(chunk, currX, currY, currZ)) {
    if ((currY - 1) < caveLiquidAltitude) {
      chunk->SetFullBlock(currX, currY, currZ, STILL_LAVA.GetFullId());
    } else {
      chunk->SetFullBlock(currX, currY, currZ, AIR.GetFullId());

      auto healY = static_cast<int_fast16_t>(currY - 1);

      const MinecraftBlock &block = MinecraftBlock(chunk->GetFullBlock(currX, healY, currZ));
      if (block == DIRT && chunk->GetHighestBlockAt(currX, currZ) == healY) {
        chunk->SetFullBlock(currX, healY, currZ, GRASS.GetFullId());
      }
    }
  }
}

bool CaveCarver::canReplaceBlock(Chunk *chunk, int_fast8_t currX, int_fast16_t currY, int_fast8_t currZ) {
  const MinecraftBlock &block = MinecraftBlock(chunk->GetFullBlock(currX, currY, currZ));
  const MinecraftBlock &blockAbove = MinecraftBlock(chunk->GetFullBlock(currX, currY, currZ));

  // Avoid damaging trees and digging out under trees.
  if (block.GetId() == 17 || block.GetId() == 18 || block.GetId() == 161 || block.GetId() == 162 || blockAbove.GetId() == 17 || blockAbove == 162) {
    return false;
  }

  if (currY > caveLiquidAltitude) {
    Vector3 unsafeCopy = Vector3(static_cast<int_fast16_t>(currX), currY, static_cast<int_fast16_t>(currZ));
    for (int facing : Facing::ALL) {
      const Vector3 &facingSide = unsafeCopy.GetSide(facing);

      if ((facingSide.GetFloorX() >= 0 && facingSide.GetFloorX() <= 15) && (facingSide.GetFloorZ() >= 0 && facingSide.GetFloorZ() <= 15)) {
        const MinecraftBlock &blockFace = MinecraftBlock(chunk->GetFullBlock(static_cast<int_fast8_t>(facingSide.GetFloorX()), static_cast<int_fast16_t>(facingSide.GetFloorY()), static_cast<int_fast8_t>(facingSide.GetFloorZ())));
        if (IS_LIQUID(blockFace.GetId())) {
          return false;
        }
      }
    }
  }

  if (block == STONE || block == DIRT || block == GRASS || block == HARDENED_CLAY || block == STAINED_CLAY || block == SANDSTONE || block == RED_SANDSTONE || block == MYCELIUM || block == SNOW_LAYER) {
    return true;
  }

  return (block == SAND || block == GRAVEL) && !(IS_LIQUID(blockAbove.GetId()));
}
