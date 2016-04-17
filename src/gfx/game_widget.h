#pragma once

#include <map>

#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/MarchingCubesSurfaceExtractor.h"
#include "PolyVox/Mesh.h"
namespace pv = PolyVox;

#include <selene.h>

#include "gfx/loader.h"
#include "gfx/qb_file.h"
#include "util/vec.h"
#include "world_volume.h"

#include "game/world.h"
#include "game/co_entity.h"

namespace bm {

// Currently active keyboard map. Changes to allow access to sub-commands.
enum class KeyFSM: int {
    Default,        // regular keyboard movement and cursor operation
    Orders,
    Digging,
};

class GameWidget : public GLVersion_Widget {
    Q_OBJECT
  public:
    static constexpr float WALL_HEIGHT = 1.0f;
    static constexpr float CELL_SIZE = 1.0f;

    GameWidget(QWidget* parent);

    // Returns pointer for temporary use and modification, do not store permanently
    Model *load_model(ModelId register_as,
                      const char *file,
                      ShaderPtr shad,
                      bool re_scale = true);
    const Model *find_model(ModelId id) const;

    // QT override
    virtual void keyPressEvent( QKeyEvent* event ) override {
        (this->*keyboard_handler_)(event);
    }

  signals:
    void SIG_cursor_changed(const QPoint &xz, int depth) const;
    void SIG_keyboard_fsm_changed(bm::KeyFSM);

  protected:
    // World volume
    //std::unique_ptr<WorldPager>     vol_;
    //std::unique_ptr<SlabVolume>     vol_slice_;
    std::unique_ptr<bm::RawVolume>  volume_;
    std::unique_ptr<World> world_;
    sel::State  lua_;

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

    void render_model(const Model& m, const Vec3f &pos, float rot_y);

    // These two camera control funs make camera always look at cursor
    virtual QVector3D get_camera_focus(QVector3D /*forward*/) override {
        auto cur = pos_for_cell(cursor_pos_);
        return QVector3D(cur.getX(), cur.getY(), cur.getZ());
    }
    virtual QVector3D get_camera_up(QVector3D /*right*/,
                                    QVector3D /*forward*/) override {
        return QVector3D(0.0f, 1.0f, 0.0f);
    }

private:
    // Reposition camera on cursor
    void camera_follow_cursor();
    // Take a slice of the world with 1 extra voxel around data. Generate new
    // model and update 'terrain_'
    void update_terrain_model();

    // Change keyboard handler to get different keypress reactions
    typedef void(GameWidget::*KeyboardHandler)(QKeyEvent*);
    KeyboardHandler keyboard_handler_;

    void change_keyboard_fsm(KeyFSM id);
    void fsm_keypress_exploremap(QKeyEvent *event);
    void fsm_keypress_orders(QKeyEvent *event);
    void fsm_keypress_digging(QKeyEvent *event);

    // Given integer cell position make world pos
    static Vec3f pos_for_cell(const Vec3i &i) {
        return Vec3f((float)i.getX() - CELL_SIZE * 0.5f,
                     -(float)i.getY() - WALL_HEIGHT * 0.5f,
                     (float)i.getZ() - CELL_SIZE * 0.5f);
    }

    void render_overlay_xyz();

    void on_cursor_changed();
    void render_orders();
    void render_orders(const ai::OrderMap &order_map);
    void render_debug_routes();
};

}  // namespace bm
