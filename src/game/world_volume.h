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

    MyVoxelType(): material_(BlockType::AIR), is_ramp_(false) {}
    MyVoxelType(BlockType m): material_(m), is_ramp_(false) {}
    MyVoxelType(BlockType m, uint32_t): material_(m), is_ramp_(false) {}

    bool operator==(const MyVoxelType& rhs) const {
        return material_ == rhs.material_;
    }

    bool operator!=(const MyVoxelType& rhs) const {
        return !(*this == rhs);
    }

    bool is_ramp() const { return is_ramp_; }
    void set_ramp(bool r) {
        if (is_not_air()) {
            is_ramp_ = r;
        } else {
            is_ramp_ = false;
        }
    }
    bool is_walkable_on() const { return is_not_air() || is_ramp(); }
    bool is_walkable_into() const { return is_air() || is_ramp(); }
    bool is_not_air() const { return getMaterial() != BlockType::AIR; }
    bool is_air() const { return getMaterial() == BlockType::AIR; }
    bool is_rock() const { return getMaterial() == BlockType::Rock; }

    uint32_t getDensity() const {
        return (material_ == BlockType::AIR || is_ramp_)
                ? getMinDensity()
                : getMaxDensity();
        // We render ramps as empty blocks and add model to decorate
    }
    BlockType getMaterial() const { return material_; }

    void setDensity(uint32_t) {}
    void setMaterial(BlockType m) { material_ = m; }

    static constexpr uint32_t getMaxDensity() { return 1; }
    static constexpr uint32_t getMinDensity() { return 0; }

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

//inline bool is_solid(VoxelType v) { return v.getMaterial() != BlockType::AIR; }
//inline bool is_air(VoxelType v) { return v.getMaterial() == BlockType::AIR; }
//inline bool is_rock(VoxelType v) { return v.getMaterial() == BlockType::Rock; }

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

// Allows reading relative positions from a volume
class VoxelReader {
private:
    const VolumeType &vol_;
    Vec3i pos_;

public:
    VoxelReader(const VolumeType* vt, const Vec3i& p): vol_(*vt), pos_(p) {}
    VoxelType read(int dx, int dy, int dz) const {
        vol_.getVoxel(pos_.getX() + dx,
                      pos_.getY() + dy,
                      pos_.getZ() + dz);
    }
};

} // namespace bm
