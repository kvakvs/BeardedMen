#pragma once

#include <QMatrix4x4>

#include "vector.h"
#include <PolyVox/RawVolume.h>
#include "PolyVox/MaterialDensityPair.h"
#include "PolyVox/CubicSurfaceExtractor.h"
namespace pv = PolyVox;

#include <cstdio>
#include <stdint.h>
#include <memory>

namespace nrdf {

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

    QMatrix4x4 matrix_;

    float scale_;
    Vec3f offset_;

    bool removed_;

    // Call this when size_ is known and it's time to allocate voxels_
    // Result: creates voxels_
    void create_voxels() {
        pv::Region reg(Vec3i(0, 0, 0),
                       Vec3i(size_.getX(), size_.getY(),
                                         size_.getZ()));
        voxels_ = std::make_unique<VoxVolume>(reg);
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
        QBVolume *volData = volumes_[i];
        Vec3i unit_v(1,1,1);
        auto mesh = pv::extractCubicMesh(
                    volData->voxels_.get(),
                    pv::Region(Vec3i(0,0,0), volData->size_ - unit_v)
                    );
        auto decodedMesh = pv::decodeMesh(mesh);
        return decodedMesh;
    }

private:
    // read headers
    void read(FILE *f);
    // continue reading if compressed
    void read_compressed(FILE *f, QBVolume *p);
    void read_uncompressed(FILE *f, QBVolume *vol);
};

} // ns
