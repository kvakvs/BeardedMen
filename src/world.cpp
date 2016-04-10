#include "world.h"
#include "perlin.h"

namespace nrdf {

void WorldPager::pageIn(const PolyVox::Region &region,
                         PagedVolume::Chunk *pChunk) {
    Perlin perlin(2, 2, 1, 1234);
    float noise_val = 0.f;

    for (int x = region.getLowerX(); x <= region.getUpperX(); x++) {
        for (int y = region.getLowerY(); y <= region.getUpperY(); y++) {
            for (int z = region.getLowerZ(); z <= region.getUpperZ(); z++) {
                float noise_val = perlin.get_3d(x / 255.f, y / 255.f,
                                                z / (float)(view_sz_z));
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

void WorldPager::pageOut(const PolyVox::Region &region,
                          PagedVolume::Chunk *) {
    std::cout << "warning unloading region: " << region.getLowerCorner()
              << " -> " << region.getUpperCorner() << std::endl;
}

} // ns nrdf
