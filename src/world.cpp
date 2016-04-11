#include "world.h"
#include "perlin.h"

namespace bm {

void WorldPager::pageIn(const PolyVox::Region &region,
                         PagedVolume::Chunk *pChunk) {
    Perlin perlin(2, 2, 1, 1234);
    float noise_val = 0.f;

    for (int x = region.getLowerX(); x <= region.getUpperX(); x++) {
        for (int y = region.getLowerY(); y <= region.getUpperY(); y++) {
            for (int z = region.getLowerZ(); z <= region.getUpperZ(); z++) {
                float noise_val = perlin.get_3d(x / 255.f, y / 255.f,
                                                z / (float)(VIEWSZ_Z));
                noise_val = (noise_val + 1.0f) * 0.5f;
                auto voxel = get_perlin_voxel(noise_val, x, y, z);
                // auto voxel = get_solid_block_voxel(noise_val, x, y, z);

                pChunk->setVoxel(x - region.getLowerX(),
                                 y - region.getLowerY(),
                                 z - region.getLowerZ(), voxel);
            }
        }
    }
}

VoxelType WorldPager::get_solid_block_voxel(float perlinVal,
                                            int x, int y, int z) {
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

VoxelType WorldPager::get_perlin_voxel(float perlinVal, int x, int y, int z) {
    VoxelType voxel;
    if (y < perlinVal * WORLDSZ_Y / 4) {
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

void WorldPager::pageOut(const PolyVox::Region &region,
                         PagedVolume::Chunk *) {
    std::cout << "warning unloading region: " << region.getLowerCorner()
              << " -> " << region.getUpperCorner() << std::endl;
}

} // namespace bm
