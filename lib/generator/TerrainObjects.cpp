#include "TerrainObjects.h"

bool TerrainObjects::killWeakBlocksAbove(SimpleChunkManager &world, int x, int y, int z) {
    int cur_y = y + 1;
    bool changed = false;

    while (cur_y < Y_MAX) {
        auto block = world.getBlockAt(x, cur_y, z);
        if (!IS_FLOWABLE(block.getId())) {
            break;
        }

        world.setBlockAt(x, cur_y, z, AIR);
        changed = true;
        ++cur_y;
    }

    return changed;
}

bool Lake::generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) {
    double size_x, size_y, size_z, dx, dy, dz;
    bool succeeded = false;
    source_y -= 4;

    std::vector<int32_t> lake_map;
    for (int n = 0; n < random.nextBoundedInt(4) + 4; ++n) {
        size_x = random.nextFloat() * 6.0 + 3;
        size_y = random.nextFloat() * 4.0 + 2;
        size_z = random.nextFloat() * 6.0 + 3;
        dx = random.nextFloat() * (LAKE_MAX_DIAMETER - size_x - 2) + 1 + size_x / 2.0;
        dy = random.nextFloat() * (LAKE_MAX_HEIGHT - size_y - 4) + 2 + size_y / 2.0;
        dz = random.nextFloat() * (LAKE_MAX_DIAMETER - size_z - 2) + 1 + size_z / 2.0;
        for (int x = 1; x < LAKE_MAX_DIAMETER - 1; ++x) {
            for (int z = 1; z < LAKE_MAX_DIAMETER - 1; ++z) {
                for (int y = 1; y < LAKE_MAX_HEIGHT - 1; ++y) {
                    double nx = (x - dx) / (size_x / 2.0); nx *= nx;
                    double ny = (y - dy) / (size_y / 2.0); ny *= ny;
                    double nz = (z - dz) / (size_z / 2.0); nz *= nz;

                    if ((nx + ny + nz) < 1.0) {
                        setLakeBlock(lake_map, x, y, z);
                        succeeded = true;
                    }
                }
            }
        }
    }

    if (!canPlace(lake_map, world, source_x, source_y, source_z)) {
        return succeeded;
    }

    auto chunk = world.getChunk(source_x >> 4, source_z >> 4);
    int biome = chunk->getBiomeArray().get((source_x + 8 + LAKE_MAX_DIAMETER / 2) & 0x0f,
                                           (source_z + 8 + LAKE_MAX_DIAMETER / 2) & 0x0f);
    bool mycel_biome = biome == MUSHROOM_SHORE;

    for (int x = 0; x < LAKE_MAX_DIAMETER; ++x) {
        for (int z = 0; z < LAKE_MAX_DIAMETER; ++z) {
            for (int y = 0; y < LAKE_MAX_DIAMETER; ++y) {
                if (!isLakeBlock(lake_map, x, y, z)) {
                    continue;
                }

                MinecraftBlock replaceType = this->type;
                MinecraftBlock block = world.getBlockAt(source_x + x, source_y + y, source_z + z);
                MinecraftBlock block_above = world.getBlockAt(source_x + x, source_y + y + 1, source_z + z);
                uint8_t block_type = block.getId();
                uint8_t block_above_type = block_above.getId();
                if ((block_type == 3 && (block_above_type == 17 || block_above_type == 162)) || block_type == 17 ||
                    block_type == 162) {
                    continue;
                }

                if (y >= (LAKE_MAX_HEIGHT / 2)) {
                    replaceType = AIR;
                    if (killWeakBlocksAbove(world, source_x + x, source_y + y, source_z + z)) {
                        break;
                    }

                    if ((block_type == 79 || block_type == 174) && this->type.getId() == 9) {
                        replaceType = block;
                    }
                } else if (y == (LAKE_MAX_HEIGHT / 2 - 1)) {
                    if (type.getId() == 9 &&
                        isCold(chunk->getBiomeArray().get(x & 0x0f, z & 0x0f), source_x + x, y, source_z + z)) {
                        type = ICE;
                    }
                }

                world.setBlockAt(source_x + x, source_y + y, source_z + z, replaceType);
            }
        }
    }

    for (int x = 0; x < LAKE_MAX_DIAMETER; ++x) {
        for (int z = 0; z < LAKE_MAX_DIAMETER; ++z) {
            for (int y = LAKE_MAX_HEIGHT / 2; y < (int) LAKE_MAX_HEIGHT; ++y) {
                if (!isLakeBlock(lake_map, x, y, z)) {
                    continue;
                }

                MinecraftBlock block = world.getBlockAt(source_x + x, source_y + y - 1, source_z + z);
                MinecraftBlock block_above = world.getBlockAt(source_x + x, source_y + y, source_z + z);
                if (block.getId() == 3 && IS_TRANSPARENT(block_above.getId()) && GET_LIGHT_LEVEL(block_above.getId()) > 0) {
                    world.setBlockAt(source_x + x, source_y + y - 1, source_z + z, mycel_biome ? MYCELIUM : GRASS);
                }
            }
        }
    }

    return succeeded;
}

bool Lake::canPlace(std::vector<int32_t> &lake_map, SimpleChunkManager world, int sourceX, int sourceY, int sourceZ) {
    for (int x = 0; x < LAKE_MAX_DIAMETER; ++x) {
        for (int z = 0; z < LAKE_MAX_DIAMETER; ++z) {
            for (int y = 0; y < LAKE_MAX_HEIGHT; ++y) {
                if (isLakeBlock(lake_map, x, y, z)
                    || (((x >= (LAKE_MAX_DIAMETER - 1)) || !isLakeBlock(lake_map, x + 1, y, z))
                        && ((x <= 0) || !isLakeBlock(lake_map, x - 1, y, z))
                        && ((z >= (LAKE_MAX_DIAMETER - 1)) || !isLakeBlock(lake_map, x, y, z + 1))
                        && ((z <= 0) || !isLakeBlock(lake_map, x, y, z - 1))
                        && ((z >= (LAKE_MAX_HEIGHT - 1)) || !isLakeBlock(lake_map, x, y + 1, z))
                        && ((z <= 0) || !isLakeBlock(lake_map, x, y - 1, z)))) {
                    continue;
                }
                MinecraftBlock block = world.getBlockAt(sourceX + x, sourceY + y, sourceZ + z);
                if (y >= LAKE_MAX_HEIGHT / 2 && (IS_LIQUID(block.getId()) || block.getId() == 79)) {
                    return false; // there's already some liquids above
                }

                if ((y < LAKE_MAX_HEIGHT / 2) && !(IS_SOLID(block.getId())) && block.getId() != this->type.getId()) {
                    return false; // bottom must be solid and do not overlap with another liquid type
                }
            }
        }
    }

    return true;
}

bool Lake::isLakeBlock(std::vector<int32_t> &lake_map, int x, int y, int z) {
    return std::find(lake_map.begin(), lake_map.end(), static_cast<int>((x * LAKE_MAX_DIAMETER + z) * LAKE_MAX_HEIGHT + y)) != lake_map.end();
}

void Lake::setLakeBlock(std::vector<int32_t> &lake_map, int x, int y, int z) {
    lake_map.emplace_back(static_cast<int>((x * LAKE_MAX_DIAMETER + z) * LAKE_MAX_HEIGHT + y));
}

bool OreVein::generate(SimpleChunkManager world, Random &random, int sourceX, int sourceY, int sourceZ) {
    double amount = type->getAmount();
    float angle = random.nextFloat() * (float) M_PI;
    double dx1 = sourceX + sin(angle) * amount / 8.0F;
    double dx2 = sourceX - sin(angle) * amount / 8.0F;
    double dz1 = sourceZ + cos(angle) * amount / 8.0F;
    double dz2 = sourceZ - cos(angle) * amount / 8.0F;
    double dy1 = sourceY + static_cast<double>(random.nextBoundedInt(3)) - 2;
    double dy2 = sourceY + static_cast<double>(random.nextBoundedInt(3)) - 2;
    bool succeeded = false;
    for (int i = 0; i < amount; i++) {
        double originX = dx1 + (dx2 - dx1) * i / amount;
        double originY = dy1 + (dy2 - dy1) * i / amount;
        double originZ = dz1 + (dz2 - dz1) * i / amount;
        double q = random.nextFloat() * amount / 16.0;
        double radiusH = (sin(static_cast<double>(i) * (float) M_PI / amount) + 1 * q + 1) / 2.0;
        double radiusV = (sin(static_cast<double>(i) * (float) M_PI / amount) + 1 * q + 1) / 2.0;
        for (int x = (int) (originX - radiusH); x <= (int) (originX + radiusH); x++) {

            // scale the center of x to the range [-1, 1] within the circle
            double squaredNormalizedX = normalizedSquaredCoordinate(originX, radiusH, x);
            if (squaredNormalizedX >= 1) {
                continue;
            }
            for (int y = (int) (originY - radiusV); y <= (int) (originY + radiusV); y++) {
                double squaredNormalizedY = normalizedSquaredCoordinate(originY, radiusV, y);
                if (squaredNormalizedX + squaredNormalizedY >= 1) {
                    continue;
                }

                for (int z = (int) (originZ - radiusH); z <= (int) (originZ + radiusH); z++) {
                    double squaredNormalizedZ = normalizedSquaredCoordinate(originZ, radiusH, z);
                    if (squaredNormalizedX + squaredNormalizedY + squaredNormalizedZ < 1 && world.getBlockAt(x, y, z).getId() == type->getTargetType()) {
                        world.setBlockAt(x, y, z, type->getType());
                        succeeded = true;
                    }
                }
            }
        }
    }

    return succeeded;
}

bool DoubleTallPlant::generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) {
    int x, y, z;

    bool placed = false;
    int height = world.getMaxY();
    for (int i = 0; i < 64; ++i) {
        x = source_x + static_cast<int>(random.nextBoundedInt(8) - random.nextBoundedInt(8));
        z = source_z + static_cast<int>(random.nextBoundedInt(8) - random.nextBoundedInt(8));
        y = source_y + static_cast<int>(random.nextBoundedInt(4) - random.nextBoundedInt(4));

        MinecraftBlock block = world.getBlockAt(x, y, z);
        MinecraftBlock top_block = world.getBlockAt(x, y + 1, z);
        if (y < height && block == AIR && top_block == AIR && world.getBlockAt(x, y - 1, z) == GRASS) {
            world.setBlockAt(x, y, z, this->species);
            world.setBlockAt(x, y + 1, z, this->species.makeBlock(0x08, 0b1000));
            placed = true;
        }
    }

    return placed;
}

bool GenericTree::generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) {
    if (cannotGenerateAt(source_x, source_y, source_z, world)) {
        return false;
    }

    // generate the leaves
    for (int y = source_y + height - 3; y <= source_y + height; ++y) {
        int n = y - (source_y + height);
        int radius = (int)(1 - n / 2);
        for (int x = source_x - radius; x <= source_x + radius; ++x) {
            for (int z = source_z - radius; z <= source_z + radius; ++z) {
                if (abs(x - source_x) != radius || abs(z - source_z) != radius || (random.nextBoolean() && n != 0)) {
                    replaceIfAirOrLeaves(x, y, z, leavesTypes, world);
                }
            }
        }
    }

    // generate the trunk
    for (int y = 0; y < height; ++y) {
        replaceIfAirOrLeaves(source_x, source_y + y, source_z, logType, world);
    }

    // block below trunk is always dirt
    transaction.addBlockAt(source_x, source_y - 1, source_z, new MinecraftBlock(3, 0));

    return true;
}

bool GenericTree::canHeightFit(int base_height) const {
    return base_height >= 1 && base_height + height + 1 < Y_MAX;
}

bool GenericTree::canPlace(int base_x, int base_y, int base_z, SimpleChunkManager world) {
    for (int y = base_y; y <= base_y + 1 + height; ++y) {
        // Space requirement
        int radius = 1; // default radius if above first block
        if (y == base_y) {
            radius = 0; // radius at source block y is 0 (only trunk)
        } else if (y >= base_y + 1 + height - 2) {
            radius = 2; // max radius starting at leaves bottom
        }

        // check for block collision on horizontal slices
        for (int x = base_x - radius; x <= base_x + radius; ++x) {
            for (int z = base_z - radius; z <= base_z + radius; ++z) {
                if (y >= 0 && y < world.getMaxY()) {
                    // we can overlap some blocks around
                    if (std::find(overrides.begin(), overrides.end(), world.getBlockAt(x, y, z).getId()) == overrides.end()) {
                        return false;
                    }
                } else { // height out of range
                    return false;
                }
            }
        }
    }

    return true;
}

void GenericTree::replaceIfAirOrLeaves(int x, int y, int z, MinecraftBlock newBlock, SimpleChunkManager world) {
    auto oldBlock = world.getBlockAt(x, y, z).getId();
    if (oldBlock == 0 || oldBlock == 18) {
        transaction.addBlockAt(x, y, z, new MinecraftBlock(newBlock.getId(), newBlock.getMeta()));
    }
}

bool GenericTree::canPlaceOn(MinecraftBlock soil)  {
    int type = soil.getId();

    return type == GRASS.getId() || type == DIRT.getId() || type == FARMLAND.getId();
}

bool GenericTree::cannotGenerateAt(int base_x, int base_y, int base_z, SimpleChunkManager world) {
    return !canHeightFit(base_y)
           || !canPlaceOn(world.getBlockAt(base_x, base_y - 1, base_z))
           || !canPlace(base_x, base_y, base_z, world);
}