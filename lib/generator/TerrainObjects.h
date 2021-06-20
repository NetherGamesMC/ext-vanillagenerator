#ifndef EXT_NOISE_TERRAIN_OBJECTS
#define EXT_NOISE_TERRAIN_OBJECTS

#include <lib/chunk/Chunk.h>
#include <lib/pocketmine/BlockList.h>
#include <lib/pocketmine/Constants.h>
#include <lib/pocketmine/Random.h>
#include <lib/vanilla/Biome.h>

#include <utility>
#include "Misc.h"

class TerrainObjects {
public:
    /**
     * Removes weak blocks like grass, shrub, flower or mushroom directly above the given block, if present.
     * Does not drop an item.
     *
     * @return bool whether a block was removed; false if none was present
     */
    static bool killWeakBlocksAbove(SimpleChunkManager &world, int x, int y, int z);

    virtual bool generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) = 0;
};

class Lake : public TerrainObjects {
public:
    explicit Lake(MinecraftBlock block) : type(block) {
        // NOOP
    }

    bool generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) override;

private:
    constexpr static const int LAKE_MAX_HEIGHT = 8;
    constexpr static const int LAKE_MAX_DIAMETER = 16;

    static bool isLakeBlock(std::vector<int32_t> &lake_map, int x, int y, int z);

    static void setLakeBlock(std::vector<int32_t> &lake_map, int x, int y, int z);

    bool canPlace(std::vector<int32_t> &lake_map, SimpleChunkManager world, int sourceX, int sourceY, int sourceZ);

    MinecraftBlock type;

};

class OreVein : public TerrainObjects {
public:
    explicit OreVein(OreType *oreType) : type(oreType) {}

    bool generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) override;

protected:
    /**
     * The square of the percentage of the radius that is the distance between the given block's
     * center and the center of an orthogonal ellipsoid. A block's center is inside the ellipsoid
     * if and only if its normalizedSquaredCoordinate values add up to less than 1.
     *
     * @param origin the center of the spheroid
     * @param radius the spheroid's radius on this axis
     * @param x the raw coordinate
     *
     * @return float the square of the normalized coordinate
     */
    static double normalizedSquaredCoordinate(double origin, double radius, int x) {
        double squared_normalized_x;
        squared_normalized_x = (x + 0.5 - origin) / radius;
        squared_normalized_x *= squared_normalized_x;

        return squared_normalized_x;
    }

private:
    OreType *type;
};

class DoubleTallPlant : public TerrainObjects {
public:
    explicit DoubleTallPlant(MinecraftBlock subtype) : species(subtype) {}

    bool generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) override;
private:
    MinecraftBlock species;
};

class GenericTree : public TerrainObjects {
public:
    GenericTree(Random &random, BlockTransaction &txn): transaction(txn){
        height = static_cast<int>(random.nextBoundedInt(3) + 4);
    }

    bool generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) override;

protected:

    void setHeight(int blockHeight) {
        height = blockHeight;
    }

    void setOverrides(std::vector<int> overridable){
        overrides = std::move(overridable);
    }

    /**
     * Sets the block data values for this tree's blocks.
     *
     * @param TreeType $type
     */
    void setType(int magicNumber) {
        logType = MinecraftBlock(magicNumber >= 4 ? 162 : 17, magicNumber & 0x3);
        leavesTypes = MinecraftBlock(magicNumber >= 4 ? 161 : 18, magicNumber & 0x3);
    }

private:
    static bool canPlaceOn(MinecraftBlock soil);

    bool canHeightFit(int base_height) const;

    bool cannotGenerateAt(int base_x, int base_y, int base_z, SimpleChunkManager world);

    void replaceIfAirOrLeaves(int x, int y, int z, MinecraftBlock newBlock, SimpleChunkManager world);

    bool canPlace(int base_x, int base_y, int base_z, SimpleChunkManager world);

    int height;
    BlockTransaction &transaction;

    MinecraftBlock logType = OAK_WOOD;
    MinecraftBlock leavesTypes = OAK_LEAVES;

    std::vector<int> overrides = {0, 18, 2, 3, 17, 162, 6, 106};
};

typedef GenericTree (*TreeObject)(Random&, BlockTransaction&);

class TreeDecoration {
public:
    TreeDecoration(TreeObject treeObject, int _weight) : tree(treeObject), weight(_weight) {}

    TreeObject getBlock() const {
        return tree;
    }

    int getWeight() const {
        return weight;
    }

private:
    int weight;
    TreeObject tree;
};

static GenericTree defaultTree(Random &random, BlockTransaction &txn) {
    return GenericTree(random, txn);
}

#endif //EXT_NOISE_TERRAIN_OBJECTS
