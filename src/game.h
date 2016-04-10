#pragma once

#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/MarchingCubesSurfaceExtractor.h"
#include "PolyVox/Mesh.h"
namespace pv = PolyVox;

#include "base_app.h"
#include "qb_file.h"
#include "vector.h"
#include "world.h"

namespace bm {

class GameWidget : public BaseWidget {
  public:
    GameWidget(QWidget* parent) : BaseWidget(parent) {}

    Model load_model(const char *register_as,
                     const char *file,
                     ShaderPtr shad);

  protected:
    // World volume (pageable)
    std::unique_ptr<WorldPager> vol_;
    QElapsedTimer qtimer_;

    MeshMap raw_meshes_;

    Model dorf_;
    Model cursor_;
    Vec3i cursor_pos_ = Vec3i(2,0,0);

    // Ground shader and mesh
    ShaderPtr  terrain_shader_;
    Model      terrain_;

    ShaderPtr  rgb_vox_shader_;

    virtual void initialize_game() override;
    virtual void renderOneFrame() override;

    // QT override
    virtual void keyPressEvent( QKeyEvent* event ) override;

private:
    // Reposition camera on cursor
    void follow_cursor();
    // Take a slice of the world with 1 extra voxel around data. Generate new
    // model and update 'terrain_'
    void update_terrain_model();
};

}  // namespace bm
