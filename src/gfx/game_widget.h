#pragma once

#include <map>

#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/MarchingCubesSurfaceExtractor.h"
#include "PolyVox/Mesh.h"
namespace pv = PolyVox;

//#include <selene.h>

#include "gfx/mesh_maker.h"
#include "gfx/qb_file.h"
#include "gfx/ramp.h"

#include "util/vec.h"
#include "util/optional.h"

#include "game/world_volume.h"
#include "game/world.h"
#include "game/co_entity.h"

namespace bm {

// Currently active keyboard map. Changes to allow access to sub-commands.
enum class KeyFSM: int {
    Default,        // regular keyboard movement and cursor operation
    //Orders,
    Designations,
};

class GameWidget : public GLVersion_Widget {
    Q_OBJECT
  public:
    static constexpr float CELL_SIZE = 1.0f;
    static constexpr float WALL_HEIGHT = CELL_SIZE;

    GameWidget(QWidget* parent);

    const Model *find_model(ModelId id) const;

    // QT override
    virtual void keyPressEvent( QKeyEvent* event ) override {
        (this->*keyboard_handler_)(event);
    }

signals:
    void SIG_cursor_changed(const QPoint &xz, int depth) const;
    void SIG_keyboard_fsm_changed(bm::KeyFSM);
    void SIG_battle_log(const QString& msg);

protected:
    // World volume
    std::unique_ptr<bm::RawVolume>  volume_;
    std::unique_ptr<World>          world_;
    //sel::State  lua_;

    ModelMap models_;

    Vec3i           cursor_pos_ = Vec3i(2,0,0);
    // if M was pressed while giving designations, it contains starting corner
    Optional<Vec3i> mark_begin_;

    // Ground shader and mesh
    ShaderPtr              terrain_shader_;
    std::unique_ptr<Model> terrain_;

    ShaderPtr  rgb_vox_shader_;

    // Ramps are calculated at the same time as terrain is rebuilt. Ramps
    // are rendered using instancing (or old style - one call per ramp).
    std::vector<visual::Ramp> ramps_;

protected:
    virtual void initialize() override;
    virtual void render_frame() override;

    void render_model(const Model& m, const Vec3f &pos, float rot_y);
    void render_terrain_model();

    // These two camera control funs make camera always look at cursor
    virtual QVector3D get_camera_focus(QVector3D /*forward*/) override;
    virtual QVector3D get_camera_up(QVector3D /*right*/,
                                    QVector3D /*forward*/) override;
    virtual QVector3D get_cam_forward() const override;

private:
    // Reposition camera on cursor
    void camera_follow_cursor();
    // Take a slice of the world with 1 extra voxel around data. Generate new
    // model and update 'terrain_'
    void update_terrain();
    void update_terrain_model();
    void update_ramps();

    // Change keyboard handler to get different keypress reactions
    typedef void(GameWidget::*KeyboardHandler)(QKeyEvent*);
    KeyboardHandler keyboard_handler_;

    void change_keyboard_fsm(KeyFSM id);
    void fsm_keypress_exploremap(QKeyEvent *event);
    void fsm_keypress_orders(QKeyEvent *event);
    void fsm_keypress_designations(QKeyEvent *event);

    void render_overlay_xyz();

    // Loads RGB-colored QB file model. Rescales to fit into 1x1x1 cell
    Model *load_rgb(ModelId reg, const char *name, bool re_scale = true);
    // Returns pointer for temporary use and modification, do not store permanently
    Model *load_model(ModelId register_as,
                      const char *file,
                      ShaderPtr shad,
                      bool re_scale = true);

    bool keypress_navigate_cursor(QKeyEvent *event);
    void on_cursor_changed();

    void render_orders();
    void render_orders(const ai::OrderMap &order_map);
    void render_debug_routes();
    void render_marked_area();
    void render_animate_objects();
    void render_inanimate_objects();
    void render_ramps();
    //void get_visible_region(Vec3i &a, Vec3i &b);
    pv::Region get_visible_region();
    void create_ramps();
};

}  // namespace bm
