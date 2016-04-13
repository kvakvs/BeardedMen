#pragma once

#include <map>

#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/MarchingCubesSurfaceExtractor.h"
#include "PolyVox/Mesh.h"
namespace pv = PolyVox;

#include "gfx/loader.h"
#include "gfx/qb_file.h"
#include "vector.h"
#include "world_pager.h"

#include "game/world.h"
#include "game/entity.h"

namespace bm {

class GameWidget : public GLVersion_Widget {
    Q_OBJECT
  public:
    static constexpr float WALL_HEIGHT = 1.0f;
    static constexpr float CELL_SIZE = 1.0f;

    GameWidget(QWidget* parent) : GLVersion_Widget(parent) {}

    // Returns pointer for temporary use and modification, do not store permanently
    Model *load_model(ModelId register_as,
                    const char *file,
                    ShaderPtr shad);
    const Model *find_model(ModelId id) const;

    // QT override
    virtual void keyPressEvent( QKeyEvent* event ) override {
        (this->*keyboard_handler_)(event);
    }

  signals:
    void SIG_cursor_changed(const QPoint &xz, int depth) const;

  protected:
    // World volume
    //std::unique_ptr<WorldPager>     vol_;
    //std::unique_ptr<SlabVolume>     vol_slice_;
    std::unique_ptr<bm::RawVolume>  volume_;
    std::unique_ptr<World> world_;

    ModelMap models_;

    Vec3i cursor_pos_ = Vec3i(2,0,0);

    // Ground shader and mesh
    ShaderPtr  terrain_shader_;
    std::unique_ptr<Model> terrain_;

    ShaderPtr  rgb_vox_shader_;

    virtual void initialize() override;
    virtual void render_frame() override;

    class TerrainIsQuadNeeded {
       public:
        bool operator()(VoxelType back, VoxelType front,
                        VoxelType& materialToUse) {
            if (is_solid(back) && is_air(front)) {
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
        return Vec3f((float)i.getX() - CELL_SIZE * 0.5f,
                     -(float)i.getY() - WALL_HEIGHT * 0.5f,
                     (float)i.getZ() - CELL_SIZE * 0.5f);
    }

    void render_overlay_xyz();

    void on_cursor_changed();
};

}  // namespace bm
