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

    // QT override
    virtual void keyPressEvent( QKeyEvent* event ) override {
        (this->*keyboard_handler_)(event);
    }
  protected:
    // World volume (pageable)
    std::unique_ptr<WorldPager> vol_;

    MeshMap raw_meshes_;

    Model dorf_;
    Vec3i dorf_pos_;
    Model cursor_, cursor_red_;
    Vec3i cursor_pos_ = Vec3i(2,0,0);
    Model grass_[3], wood_;

    // Ground shader and mesh
    ShaderPtr  terrain_shader_;
    Model      terrain_;

    ShaderPtr  rgb_vox_shader_;

    virtual void initialize_game() override;
    virtual void render_frame() override;

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
};

}  // namespace bm
