#pragma once

#include "PolyVox/MaterialDensityPair.h"
#include "PolyVox/PagedVolume.h"
#include "PolyVox/RawVolume.h"
namespace pv = PolyVox;

namespace nrdf {

//using VoxelType = pv::MaterialDensityPair88;
using VoxelType = pv::MaterialDensityPair<uint8_t, 8, 8>;
using PagedVolume = pv::PagedVolume<VoxelType>;

const int world_sz_x = 256;
const int world_sz_y = 256;
const int world_sz_z = 32;

const int view_sz_x = 64;
const int view_sz_y = 1;
const int view_sz_z = 64;

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

    static VoxelType get_solid_block_voxel(float perlinVal,
                                           int x,
                                           int y,
                                           int z) {
        VoxelType voxel;
        uint8_t m = (uint32_t)(perlinVal * 255.0) % 5 + 1;
        if (m == 0) {
            voxel.setMaterial(0);
            voxel.setDensity(VoxelType::getMinDensity());
        } else {
            voxel.setMaterial(m);
            voxel.setDensity(VoxelType::getMaxDensity());
        }
        return voxel;
    }

    static VoxelType get_perlin_voxel(float perlinVal, int x, int y, int z) {
        VoxelType voxel;
        if (y > perlinVal * view_sz_y) {
            const int xpos = 50;
            const int zpos = 100;
            if ((x - xpos) * (x - xpos) + (z - zpos) * (z - zpos) < 200) {
                // tunnel
                voxel.setMaterial(1);
                voxel.setDensity(VoxelType::getMinDensity());
            } else {
                // solid
                uint8_t m = (uint32_t)(perlinVal * 50.0) % 5 + 1;
                voxel.setMaterial(m);
                voxel.setDensity(VoxelType::getMaxDensity());
            }
        } else {
            voxel.setMaterial(0);
            voxel.setDensity(VoxelType::getMinDensity());
        }
        return voxel;
    }

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

} // ns nrdf
