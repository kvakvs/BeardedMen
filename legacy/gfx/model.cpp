#include "gfx/model.h"

namespace bm {

void OpenglMesh::destroy()
{
    if (vert_array_ > 0) {
        gl_->glDeleteVertexArrays(1, &vert_array_);
    }
//    if (indx_buf_) {
//        gl_->glDeleteBuffers(1, &indx_buf_);
//    }
//    if (vert_buf_) {
//        gl_->glDeleteBuffers(1, &vert_buf_);
//    }
    // mark to avoid double destroy
    indx_count_ = 0;
    vert_array_ = indx_buf_ = vert_buf_ = 0;
}

} // namespace bm
