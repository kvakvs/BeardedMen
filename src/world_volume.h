#pragma once

#include "PolyVox/MaterialDensityPair.h"
#include "PolyVox/PagedVolume.h"
#include "PolyVox/RawVolume.h"
namespace pv = PolyVox;

#include "tab/t_block.h"

namespace bm {

//template <typename Type, Type AIR_VALUE>
class MyVoxelType {
public:
    using DensityType = uint8_t;
    using MaterialType = BlockType;
    using Quantity = uint8_t;
    static constexpr int QTTY_BITS = 4;

    MyVoxelType(): material_(BlockType::AIR) {}
    MyVoxelType(BlockType m): material_(m) {}
    MyVoxelType(BlockType m, uint32_t): material_(m) {}

    bool operator==(const MyVoxelType& rhs) const {
        return material_ == rhs.material_;
    }

    bool operator!=(const MyVoxelType& rhs) const {
        return !(*this == rhs);
    }

    uint32_t getDensity() const {
        return (uint32_t)(
                    material_ == BlockType::AIR //|| is_ramp_
                    );
        // We render ramps as empty blocks and add model to decorate
    }
    BlockType getMaterial() const { return material_; }

    void setDensity(uint32_t) {}
    void setMaterial(BlockType m) { material_ = m; }

    static uint32_t getMaxDensity() { return 1; }
    static uint32_t getMinDensity() { return 0; }

private:
    // 16 bits
    BlockType material_;

    //--- 8 bits ------
    // For liquids - How many units of liquid are in this block. And
    // 2^QTTY_BITS-1 being the max possible amount
    Quantity qty_;
    // using 4 bits only so far, so 4 free

    //--- 8 bits ------
    bool is_ramp_: 1;
    // 7 bits free
};

//using VoxelType = pv::MaterialDensityPair88;
//using VoxelType   = pv::MaterialDensityPair<uint8_t, 8, 4>;
using VoxelType   = MyVoxelType;
using PagedVolume = pv::PagedVolume<VoxelType>;
using RawVolume   = pv::RawVolume<VoxelType>;
using VolumeType  = RawVolume;

inline bool is_solid(VoxelType v) { return v.getMaterial() != BlockType::AIR; }
inline bool is_air(VoxelType v) { return v.getMaterial() == BlockType::AIR; }
inline bool is_rock(VoxelType v) { return v.getMaterial() == BlockType::Rock; }

const int WORLDSZ_X = 256;  // map width
const int WORLDSZ_Y = 32;   // depth
const int WORLDSZ_Z = 256;  // map height

// Visible slab of volume, height=2 so that we see the ground and the walls
// We actually fetch 1 more row then zero it to get good mesh generation
const int VIEWSZ_X = 32;
const int VIEWSZ_Y = 6;
const int VIEWSZ_Z = 32;

/**
 * Is a voxel volume
 * Generates data using Perlin noise.
 */
class WorldPager : public PagedVolume::Pager {
   public:
    WorldPager() : PagedVolume::Pager() {}
    virtual ~WorldPager() {}

    virtual void pageIn(const pv::Region& region,
                        PagedVolume::Chunk* pChunk) override;

    virtual void pageOut(const pv::Region& region,
                         PagedVolume::Chunk* /*pChunk*/) override;
};

class SlabVolume: public pv::PagedVolume<VoxelType> {
public:
    //using Sampler = SlabSampler<pv::PagedVolume<VoxelType>>;

    SlabVolume(Pager *p, uint32_t mem_limit, uint16_t chunk_sz):
        PagedVolume<VoxelType>(p, mem_limit, chunk_sz)
    {
    }
};

namespace populate {
void populate_voxels(const PolyVox::Region &region,
                     bm::RawVolume &vol);

VoxelType get_perlin_voxel(float perlinVal, int x, int y, int z);

} // ns populate

} // namespace bm
