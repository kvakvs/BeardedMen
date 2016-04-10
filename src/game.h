#pragma once

#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/MarchingCubesSurfaceExtractor.h"
#include "PolyVox/Mesh.h"
namespace pv = PolyVox;

#include "base_app.h"
#include "qb_file.h"
#include "vector.h"
#include "world.h"

namespace nrdf {

class GameWidget : public BaseWidget {
   public:
    GameWidget(QWidget* parent) : BaseWidget(parent) {}

   protected:
    std::unique_ptr<WorldPager> vol_;
    QElapsedTimer qtimer_;

    // DORF!
    std::unique_ptr<QBFile> model1_;
    OpenGLMeshData          model1_mesh_;

    // CURSOR!
    std::unique_ptr<QBFile> cursor_;
    OpenGLMeshData          cursor_mesh_;
    Vec3i cursor_pos_ = Vec3i(2,0,0);

    // Ground shader and mesh
    ShaderPtr      terrain_shader_;
    OpenGLMeshData terrain_mesh_;

    ShaderPtr rgb_vox_shader_;

    void initializeExample() override;

    // A function for mesh generation
    template <typename VoxT>
    class IsQuadNeeded {
       public:
        bool operator()(VoxT back, VoxT front, VoxT& materialToUse) {
            if (back.getDensity() > 0 && front.getDensity() == 0) {
                materialToUse = static_cast<VoxT>(back);
                return true;
            } else {
                return false;
            }
        }
    };

    virtual void renderOneFrame() override;

    // QT override
    virtual void keyPressEvent( QKeyEvent* event ) override;
    void render_model(OpenGLMeshData& mesh, ShaderPtr shad);
    // Reposition camera on cursor
    void follow_cursor();
};

}  // ns nrdf
