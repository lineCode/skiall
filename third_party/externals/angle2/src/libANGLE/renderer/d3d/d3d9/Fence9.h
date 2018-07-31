//
// Copyright (c) 2013 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// Fence9.h: Defines the rx::FenceNV9 class which implements rx::FenceNVImpl.

#ifndef LIBANGLE_RENDERER_D3D_D3D9_FENCE9_H_
#define LIBANGLE_RENDERER_D3D_D3D9_FENCE9_H_

#include "libANGLE/renderer/FenceNVImpl.h"
#include "libANGLE/renderer/SyncImpl.h"

namespace rx
{
class Renderer9;

class FenceNV9 : public FenceNVImpl
{
  public:
    explicit FenceNV9(Renderer9 *renderer);
    ~FenceNV9() override;

    gl::Error set(const gl::Context *context, GLenum condition) override;
    gl::Error test(const gl::Context *context, GLboolean *outFinished) override;
    gl::Error finish(const gl::Context *context) override;

  private:
    gl::Error testHelper(bool flushCommandBuffer, GLboolean *outFinished);

    Renderer9 *mRenderer;
    IDirect3DQuery9 *mQuery;
};
}  // namespace rx

#endif // LIBANGLE_RENDERER_D3D_D3D9_FENCE9_H_
