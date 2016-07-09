#include "game/world_volume.h"
#include "util/perlin.h"

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
    Perlin perlin(2, 2.0, 1.0, 1234);
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
    // Perlin formula gives values in range 0.3...0.8 approx.
    // So in top 3-8 layers we build earth surface
    const float MOUNTAIN_AMPL = 16.0f;
    const float MOUNTAIN_MIN_Y = 1.0f;
    const float PERLIN_DENSITY = 15.0f; // less density - larger soil structures

    if (y >= perlinVal * MOUNTAIN_AMPL + MOUNTAIN_MIN_Y) {
        // solid
        BlockType m = (BlockType)((int)(perlinVal * PERLIN_DENSITY) % 4 + 1);
        voxel.setMaterial(m);
        voxel.setDensity(VoxelType::getMaxDensity());
    } else {
        voxel.setMaterial(BlockType::AIR);
        voxel.setDensity(VoxelType::getMinDensity());
    }
    return voxel;
}

} // namespace bm
