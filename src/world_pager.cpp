#include "world_pager.h"
#include "perlin.h"

namespace bm {

void WorldPager::pageIn(const PolyVox::Region &region,
                         PagedVolume::Chunk *pChunk) {
}

void WorldPager::pageOut(const PolyVox::Region &region,
                         PagedVolume::Chunk *) {
    std::cout << "warning unloading region: " << region.getLowerCorner()
              << " -> " << region.getUpperCorner() << std::endl;
}

void populate::populate_voxels(const PolyVox::Region &region, RawVolume &vol) {
    Perlin perlin(2, 2, 1, 1234);
    //float noise_val = 0.f;

    for (int x = region.getLowerX(); x <= region.getUpperX(); x++) {
        for (int y = region.getLowerY(); y <= region.getUpperY(); y++) {
            for (int z = region.getLowerZ(); z <= region.getUpperZ(); z++) {
                float noise_val = perlin.get_3d(x / 255.f, y / 255.f,
                                                z / (float)(VIEWSZ_Z));
                noise_val = (noise_val + 1.0f) * 0.5f;
                auto voxel = get_perlin_voxel(noise_val, x, y, z);
                // auto voxel = get_solid_block_voxel(noise_val, x, y, z);

                vol.setVoxel(x - region.getLowerX(),
                             y - region.getLowerY(),
                             z - region.getLowerZ(), voxel);
            }
        }
    }
}

VoxelType populate::get_perlin_voxel(float perlinVal, int x, int y, int z) {
    VoxelType voxel;
    if (y > perlinVal * WORLDSZ_Y) {
        // solid
        uint8_t m = (uint32_t)(perlinVal * 50.0) % 5 + 1;
        voxel.setMaterial(m);
        voxel.setDensity(VoxelType::getMaxDensity());
    } else {
        voxel.setMaterial(0);
        voxel.setDensity(VoxelType::getMinDensity());
    }
    return voxel;
}

} // namespace bm
