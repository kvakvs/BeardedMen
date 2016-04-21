#include "gfx/qb_file.h"

namespace bm {

QBFile::QBFile(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (not f) {
        throw "no-qb-file";
    }
    read(f);
    fclose(f);
}

void QBFile::read(FILE* f)
{
    fread(&ver_[0],     sizeof(char) * 4, 1, f);
    fread(&color_fmt_,  sizeof(uint32_t), 1, f);
    fread(&z_axis_,     sizeof(uint32_t), 1, f);
    fread(&compressed_,       sizeof(uint32_t), 1, f);
    fread(&vis_mask_encoded_, sizeof(uint32_t), 1, f);
    fread(&num_matrices_,     sizeof(uint32_t), 1, f);

    for (uint32_t i = 0; i < num_matrices_; i++) {
        QBVolume *vol = new QBVolume();

        char name_len = 0;
        fread(&name_len, sizeof(char), 1, f);
        vol->name_.resize(name_len);

        fread(const_cast<char *>(vol->name_.data()),
              sizeof(char) * name_len, 1, f);

        uint32_t t;
        fread(&t, sizeof(uint32_t), 1, f);  vol->size_.setX(t);
        fread(&t, sizeof(uint32_t), 1, f);  vol->size_.setY(t);
        fread(&t, sizeof(uint32_t), 1, f);  vol->size_.setZ(t);

        fread(&t, sizeof(int32_t), 1, f);   vol->pos_.setX(t);
        fread(&t, sizeof(int32_t), 1, f);   vol->pos_.setY(t);
        fread(&t, sizeof(int32_t), 1, f);   vol->pos_.setZ(t);

        vol->bone_idx_ = -1;
        vol->scale_   = 1.0f;
        vol->removed_ = false;

        vol->create_voxels();

        if (compressed_ == 0) {
            read_compressed(f, vol);
        } else {
            read_uncompressed(f, vol);
        }

        volumes_.push_back(vol);
    }
}

void QBFile::read_compressed(FILE *f, QBVolume *vol)
{
    uint32_t tmp = 0;
    QBVolume::VoxelType v;
    auto xx = vol->size_.getX();
    auto yy = vol->size_.getY();
    auto zz = vol->size_.getZ();
    size_t pixels = xx * yy * zz;
    auto data = std::make_unique<uint32_t[]>(pixels);
    fread(data.get(), sizeof(uint32_t), pixels, f);
    size_t ptr = 0;

    for (uint32_t z = 0; z < zz; z++) {
        for (int32_t y = yy - 1; y >= 0; y--) {
            for (uint32_t x = 0; x < xx; x++) {
                tmp = data[ptr++];
                v.setMaterial(tmp & 0xFFFFFF);
                v.setDensity(tmp >> 24);
                vol->set_vox(x+1, y+1, z+1, v);
            }
        }
    }
}

void QBFile::read_uncompressed(FILE *f, QBVolume* vol)
{
    uint32_t z = 0;
    QBVolume::VoxelType v;

    while (z < vol->size_.getZ()) {
        uint32_t index = 0;

        while (true) {
            uint32_t data = 0;
            // TODO: Oh yeah, fread every pixel, sloooow
            // Make at least per-plane or whole data read then parse
            fread(&data, sizeof(uint32_t), 1, f) ;

            if (data == NEXTSLICEFLAG) {
                break;
            } else if (data == CODEFLAG) {
                uint32_t count = 0;
                // TODO: This is going to be slooooooo(o)w
                fread(&count, sizeof(uint32_t), 1, f);
                fread(&data, sizeof(uint32_t), 1, f);

                for (uint32_t j = 0; j < count; j++)
                {
                    uint32_t x = index % vol->size_.getX();
                    uint32_t y = index / vol->size_.getX();

                    v.setMaterial(data & 0xFFFFFF);
                    v.setDensity(data >> 24);
                    vol->set_vox(x+1, y+1, z+1, v);
                    index++;
                }
            } else {
                uint32_t x = index % vol->size_.getX();
                uint32_t y = index / vol->size_.getX();

                v.setMaterial(data & 0xFFFFFF);
                v.setDensity(data >> 24);
                vol->set_vox(x, y, z, v);
                index++;
            }
        }

        z++;
    }
}

void QBVolume::create_voxels() {
    // Have a margin around all voxels, hence all sizes+2
    pv::Region reg(Vec3i(0, 0, 0),
                   Vec3i(size_.getX()+2, size_.getY()+2, size_.getZ()+2));
    voxels_ = std::make_unique<VoxVolume>(reg);
}

} // namespace bm
