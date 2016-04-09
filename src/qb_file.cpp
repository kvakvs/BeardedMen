#include "qb_file.h"

namespace nrdf {

QBFile::QBFile(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        throw "no-qb-file";
    }
    read(f);
    fclose(f);
}

void QBFile::read(FILE* f)
{
    fread(&ver_[0], sizeof(char) * 4, 1, f);
    fread(&color_fmt_, sizeof(uint32_t), 1, f);
    fread(&z_axis_, sizeof(uint32_t), 1, f);
    fread(&compressed_, sizeof(uint32_t), 1, f);
    fread(&vis_mask_encoded_, sizeof(uint32_t), 1, f);
    fread(&num_matrices_, sizeof(uint32_t), 1, f);

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

        fread(&t, sizeof(int), 1, f);   vol->pos_.setX(t);
        fread(&t, sizeof(int), 1, f);   vol->pos_.setY(t);
        fread(&t, sizeof(int), 1, f);   vol->pos_.setZ(t);

        vol->m_boneIndex = -1;
        //pNewMatrix->m_pMesh     = NULL;

        vol->m_scale   = 1.0f;
        vol->m_offsetX = 0.0f;
        vol->m_offsetY = 0.0f;
        vol->m_offsetZ = 0.0f;

        vol->m_removed = false;

        vol->create_voxels();
//        vol->m_pColour = std::make_unique<uint32_t[]>(
//            vol->size_.getX() * vol->size_.getY() * vol->size_.getZ()
//        );

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

    for (uint32_t z = 0; z < vol->size_.getZ(); z++) {
        for (uint32_t y = 0; y < vol->size_.getY(); y++) {
            for (uint32_t x = 0; x < vol->size_.getX(); x++) {
                // TODO: Oh yeah, fread every pixel, sloooow
                // Make at least per-plane or whole data read then parse
                fread(&tmp, sizeof(uint32_t), 1, f);
                v.setMaterial(tmp >> 8);
                v.setDensity(tmp & 0xFF);
                vol->voxels_->setVoxel(x, y, z, v);
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

                    v.setMaterial(data >> 8);
                    v.setDensity(data & 0xFF);
                    vol->voxels_->setVoxel(x, y, z, v);
                    index++;
                }
            } else {
                uint32_t x = index % vol->size_.getX();
                uint32_t y = index / vol->size_.getX();

                v.setMaterial(data >> 8);
                v.setDensity(data & 0xFF);
                vol->voxels_->setVoxel(x, y, z, v);
                index++;
            }
        }

        z++;
    }
}

} // ns nrdf
