/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 David Williams and Matthew Williams

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#include "base_app.h"
#include "perlin.h"

#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/MarchingCubesSurfaceExtractor.h"
#include "PolyVox/MaterialDensityPair.h"
#include "PolyVox/Mesh.h"
#include "PolyVox/PagedVolume.h"

#include <QApplication>

namespace pv = PolyVox;

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
class MyPager : public PagedVolume::Pager {
   public:
    /// Constructor
    MyPager() : PagedVolume::Pager() {}

    /// Destructor
    virtual ~MyPager() {}

    virtual void pageIn(const pv::Region& region, PagedVolume::Chunk* pChunk) {
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

                    // Voxel position within a chunk always start from zero. So
                    // if a chunk represents region (4, 8, 12) to (11, 19, 15)
                    // then the valid chunk voxels are from (0, 0, 0) to (7, 11,
                    // 3). Hence we subtract the lower corner position of the
                    // region from the volume space position in order to get the
                    // chunk space position.
                    pChunk->setVoxel(x - region.getLowerX(),
                                     y - region.getLowerY(),
                                     z - region.getLowerZ(), voxel);
                }
            }
        }

//        VoxelType corner_vox;
//        corner_vox.setMaterial(1);
//        corner_vox.setDensity(VoxelType::getMaxDensity());
//        pChunk->setVoxel(0, 0, 0, corner_vox);  // x axis, dark green
//        pChunk->setVoxel(1, 0, 0, corner_vox);
//        pChunk->setVoxel(2, 0, 0, corner_vox);
//        corner_vox.setMaterial(2);
//        pChunk->setVoxel(0, 1, 0, corner_vox);  // y axis, light green
//        pChunk->setVoxel(0, 2, 0, corner_vox);
//        corner_vox.setMaterial(3);
//        pChunk->setVoxel(0, 0, 1, corner_vox);  // z axis, yellow green
//        pChunk->setVoxel(0, 0, 2, corner_vox);
    }

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
                         PagedVolume::Chunk* /*pChunk*/) {
        std::cout << "warning unloading region: " << region.getLowerCorner()
                  << " -> " << region.getUpperCorner() << std::endl;
    }
};

class PagingWidget : public BaseWidget {
   public:
    PagingWidget(QWidget* parent) : BaseWidget(parent) {}

   protected:
    void initializeExample() override {
        MyPager* pager = new MyPager();
        PagedVolume volData(pager, 8 * 1024 * 1024, 64);

        // Just some tests of memory usage, etc.
        std::cout << "Memory usage: "
                  << (volData.calculateSizeInBytes() / 1024.0 / 1024.0) << "MB"
                  << std::endl;

        pv::Region reg(pv::Vector3DInt32(0, 0, 0),
                       pv::Vector3DInt32(world_sz_x, world_sz_y, world_sz_z));
        std::cout << "Prefetching region: " << reg.getLowerCorner() << " -> "
                  << reg.getUpperCorner() << std::endl;
        volData.prefetch(reg);

        std::cout << "Flushing entire volume" << std::endl;
        volData.flushAll();

        //
        // Extract the surface
        //
        pv::Region reg2(pv::Vector3DInt32(0, 0, 0),
                        pv::Vector3DInt32(view_sz_x, view_sz_y, view_sz_z));

        VoxelType empty_vox;
        empty_vox.setMaterial(0);
        empty_vox.setDensity(VoxelType::getMinDensity());

        auto mesh = pv::extractCubicMesh(&volData, reg2,
                                         IsQuadNeeded<VoxelType>(empty_vox));
//         auto mesh = pv::extractCubicMesh(&volData, reg2);
//        auto mesh = pv::extractMarchingCubesMesh(&volData, reg2);
        std::cout << "#vertices: " << mesh.getNoOfVertices() << std::endl;

        auto decodedMesh = pv::decodeMesh(mesh);

        // Pass the surface to the OpenGL window
        //
        addMesh(decodedMesh);
//        addMesh(decodedMesh,
//                pv::Vector3DInt32(0.f, 0.f, 0.f),
//                QVector3D(1.f, -1.f, 1.f));

        setCameraTransform(
            QVector3D(0.0f, 10.0f, 0.0f), PI / 4.0, 0);
    }

    template <typename VoxT>
    class IsQuadNeeded {
        VoxT v_;

       public:
        explicit IsQuadNeeded(VoxT& v) : v_(v) {}
        bool operator()(VoxT back, VoxT front, VoxT& materialToUse) {
            if (back.getDensity() > 0 && front.getDensity() == 0) {
                materialToUse = static_cast<VoxelType>(back);
                return true;
            } else {
                return false;
            }
        }
    };
};

int main(int argc, char* argv[]) {
    // Create and show the Qt OpenGL window
    QApplication app(argc, argv);

    QGLFormat gl_fmt;
    gl_fmt.setVersion(3, 2);
    gl_fmt.setProfile(QGLFormat::CoreProfile);
    gl_fmt.setSampleBuffers(true);
    QGLFormat::setDefaultFormat(gl_fmt);

    PagingWidget openGLWidget(0);
    openGLWidget.show();

    // Run the message pump.
    return app.exec();
}
