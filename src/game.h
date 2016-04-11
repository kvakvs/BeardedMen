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
    Q_OBJECT
  public:
    GameWidget(QWidget* parent) : BaseWidget(parent) {}

    Model load_model(const char *register_as,
                     const char *file,
                     ShaderPtr shad);

    // QT override
    virtual void keyPressEvent( QKeyEvent* event ) override {
        (this->*keyboard_handler_)(event);
    }

  signals:
    void SIG_cursor_changed(const QPoint &xz, int depth) const;

  protected:
    // World volume (pageable)
    std::unique_ptr<WorldPager>     vol_;
    std::unique_ptr<SlabVolume>     vol_slice_;
    //std::unique_ptr<bm::RawVolume>  tmp_volume_;

    MeshMap raw_meshes_;

    Model dorf_;
    Vec3i dorf_pos_;
    Model cursor_, cursor_red_;
    Vec3i cursor_pos_ = Vec3i(2,0,0);
//    Model grass_[3];
    Model wood_;
    Model xyz_;

    // Ground shader and mesh
    ShaderPtr  terrain_shader_;
    std::unique_ptr<Model> terrain_;

    ShaderPtr  rgb_vox_shader_;

    virtual void initialize_game() override;
    virtual void render_frame() override;

    class TerrainIsQuadNeeded {
       public:
        bool operator()(VoxelType back, VoxelType front,
                        VoxelType& materialToUse) {
            if (back.getDensity() > 0
                && front.getDensity() == 0) {
                materialToUse = static_cast<VoxelType>(back);
                return true;
            } else {
                return false;
            }
        }
    };

private:
    // Reposition camera on cursor
    void follow_cursor();
    // Take a slice of the world with 1 extra voxel around data. Generate new
    // model and update 'terrain_'
    void update_terrain_model();

    // Change keyboard handler to get different keypress reactions
    typedef void(GameWidget::*KeyboardHandler)(QKeyEvent*);
    KeyboardHandler keyboard_handler_;

    enum class KeyFSM: int {
        ExploreMap,        // regular keyboard movement and cursor operation
        Orders,
    };

    void change_keyboard_fsm(KeyFSM id);
    void fsm_keypress_exploremap(QKeyEvent *event);
    void fsm_keypress_orders(QKeyEvent *event);
    // Given integer cell position make world pos
    static Vec3f pos_for_cell(const Vec3i &i) {
        return Vec3f((float)i.getX() - 0.5f,
                     -(float)i.getY() + 0.5f,
                     (float)i.getZ() - 0.5f);
    }

    void render_overlay_xyz();

    void on_cursor_changed();
};

}  // namespace bm
