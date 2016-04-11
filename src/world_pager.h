#pragma once

#include "PolyVox/MaterialDensityPair.h"
#include "PolyVox/PagedVolume.h"
#include "PolyVox/RawVolume.h"
namespace pv = PolyVox;

namespace bm {

//using VoxelType = pv::MaterialDensityPair88;
using VoxelType   = pv::MaterialDensityPair<uint8_t, 8, 8>;
using PagedVolume = pv::PagedVolume<VoxelType>;
using RawVolume   = pv::RawVolume<VoxelType>;

const int WORLDSZ_X = 256;  // map width
const int WORLDSZ_Y = 32;   // depth
const int WORLDSZ_Z = 256;  // map height

// Visible slab of volume, height=2 so that we see the ground and the walls
// We actually fetch 1 more row then zero it to get good mesh generation
const int VIEWSZ_X = 32;
const int VIEWSZ_Y = 2;
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
