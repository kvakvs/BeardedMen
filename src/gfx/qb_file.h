#pragma once

//#include <QMatrix4x4>
#include <OgreMatrix4.h>

#include "util/vec.h"
#include "gfx/mesh_maker.h"

#include <PolyVox/RawVolume.h>
#include "PolyVox/MaterialDensityPair.h"
#include "PolyVox/CubicSurfaceExtractor.h"
namespace pv = PolyVox;

#include <cstdio>
#include <stdint.h>
#include <memory>

namespace bm {

class QBVolume
{
public:
    std::string name_;

    Vec3i size_;
    Vec3i pos_;

    // RGBA basically
    using VoxelType = pv::MaterialDensityPair<uint32_t, 24, 8>;
    using VoxVolume = pv::RawVolume<VoxelType>;
    //std::unique_ptr<uint32_t[]> m_pColour;
    std::unique_ptr<VoxVolume> voxels_;

    int bone_idx_;

    Ogre::Matrix4 matrix_;

    float scale_;
    Vec3f offset_;

    bool removed_;

    // Call this when size_ is known and it's time to allocate voxels_
    // Result: creates voxels_
    void create_voxels();

    static constexpr bool INVERT_X = false;
    static constexpr bool INVERT_Y = true;
    static constexpr bool INVERT_Z = true;
    void set_vox(int x, int y, int z, VoxelType v) {
        voxels_->setVoxel(1 + (INVERT_X ? (size_.getX() - x) : x),
                          1 + (INVERT_Y ? (size_.getY() - y) : y),
                          1 + (INVERT_Z ? (size_.getZ() - z) : z),
                          v);
    }
    // Return (1,1,1) / size-(2,2,2)
    Vec3f get_downscale() {
        return Vec3f(1.0f / size_.getX(),
                     1.0f / size_.getY(),
                     1.0f / size_.getZ());
    }
};

class QBFile {
private:
    enum: uint32_t {
        CODEFLAG = 2,
        NEXTSLICEFLAG = 6
    };

    // Qubicle binary file information
    char     ver_[4];
    uint32_t color_fmt_;
    uint32_t z_axis_;
    uint32_t compressed_;
    uint32_t vis_mask_encoded_;
    uint32_t num_matrices_;
    std::vector<QBVolume*> volumes_;

public:
    QBFile(const char *filename);

    auto get_mesh_for_volume(int i) {
        QBVolume *qb_vol = volumes_[i];

        Vec3i two(2, 2, 2);
        auto mesh = pv::extractCubicMesh(
                    qb_vol->voxels_.get(),
                    pv::Region(Vec3i(0,0,0),
                               qb_vol->size_ + two)
                    );

        auto decodedMesh = pv::decodeMesh(mesh);
        return decodedMesh;
    }
    void free_voxels_for_volume(int i) {
        volumes_[i]->voxels_.release();
    }
    Vec3f get_downscale(int i) {
        return volumes_[i]->get_downscale();
    }

private:
    // read headers
    void read(FILE *f);
    // continue reading if compressed
    void read_non_compressed(FILE *f, QBVolume *p);
    void read_compressed(FILE *f, QBVolume *vol);
};

uniq::ManualObject create_model_from_qb(Ogre::SceneManager *scenem,
                                        const char *file,
                                        bool re_scale = false)
{
    auto qb_model = std::make_unique<QBFile>(file);
    auto raw_mesh = qb_model->get_mesh_for_volume(0);
    qb_model->free_voxels_for_volume(0);

    return mesh::create_mesh_from_pv(scenem, file, raw_mesh);

    // Rescale to fit in 1x1x1 cube or use typical 1/8=0.125
    // shrink for 8x8x8 model
//    re_scale ? qb_model->get_downscale(0)
//             : Vec3f(.125f, .125f, .125f)
}


} // ns
