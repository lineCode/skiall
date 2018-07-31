//
// Copyright (c) 2002-2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// Surface.h: Defines the egl::Surface class, representing a drawing surface
// such as the client area of a window, including any back buffers.
// Implements EGLSurface and related functionality. [EGL 1.4] section 2.2 page 3.

#ifndef LIBANGLE_SURFACE_H_
#define LIBANGLE_SURFACE_H_

#include <EGL/egl.h>

#include "common/PackedEnums.h"
#include "common/angleutils.h"
#include "libANGLE/AttributeMap.h"
#include "libANGLE/Debug.h"
#include "libANGLE/Error.h"
#include "libANGLE/FramebufferAttachment.h"
#include "libANGLE/RefCountObject.h"
#include "libANGLE/formatutils.h"
#include "libANGLE/renderer/SurfaceImpl.h"

namespace gl
{
class Context;
class Framebuffer;
class Texture;
}

namespace rx
{
class EGLImplFactory;
}

namespace egl
{
class Display;
struct Config;

struct SurfaceState final : private angle::NonCopyable
{
    SurfaceState(const egl::Config *configIn, const AttributeMap &attributesIn);

    EGLLabelKHR label;
    const egl::Config *config;
    AttributeMap attributes;
};

class Surface : public LabeledObject, public gl::FramebufferAttachmentObject
{
  public:
    rx::SurfaceImpl *getImplementation() const { return mImplementation; }

    void setLabel(EGLLabelKHR label) override;
    EGLLabelKHR getLabel() const override;

    EGLint getType() const;

    Error initialize(const Display *display);
    Error swap(const gl::Context *context);
    Error swapWithDamage(const gl::Context *context, EGLint *rects, EGLint n_rects);
    Error postSubBuffer(const gl::Context *context,
                        EGLint x,
                        EGLint y,
                        EGLint width,
                        EGLint height);
    Error setPresentationTime(EGLnsecsANDROID time);
    Error querySurfacePointerANGLE(EGLint attribute, void **value);
    Error bindTexImage(const gl::Context *context, gl::Texture *texture, EGLint buffer);
    Error releaseTexImage(const gl::Context *context, EGLint buffer);

    Error getSyncValues(EGLuint64KHR *ust, EGLuint64KHR *msc, EGLuint64KHR *sbc);

    EGLint isPostSubBufferSupported() const;

    void setSwapInterval(EGLint interval);
    Error setIsCurrent(const gl::Context *context, bool isCurrent);
    Error onDestroy(const Display *display);

    void setMipmapLevel(EGLint level);
    void setMultisampleResolve(EGLenum resolve);
    void setSwapBehavior(EGLenum behavior);

    void setFixedWidth(EGLint width);
    void setFixedHeight(EGLint height);

    gl::Framebuffer *createDefaultFramebuffer(const gl::Context *context);

    const Config *getConfig() const;

    // width and height can change with client window resizing
    EGLint getWidth() const;
    EGLint getHeight() const;
    EGLint getPixelAspectRatio() const;
    EGLenum getRenderBuffer() const;
    EGLenum getSwapBehavior() const;
    TextureFormat getTextureFormat() const;
    EGLenum getTextureTarget() const;
    bool getLargestPbuffer() const;
    EGLenum getGLColorspace() const;
    EGLenum getVGAlphaFormat() const;
    EGLenum getVGColorspace() const;
    bool getMipmapTexture() const;
    EGLint getMipmapLevel() const;
    EGLint getHorizontalResolution() const;
    EGLint getVerticalResolution() const;
    EGLenum getMultisampleResolve() const;

    gl::Texture *getBoundTexture() const { return mTexture; }

    EGLint isFixedSize() const;

    // FramebufferAttachmentObject implementation
    gl::Extents getAttachmentSize(const gl::ImageIndex &imageIndex) const override;
    gl::Format getAttachmentFormat(GLenum binding, const gl::ImageIndex &imageIndex) const override;
    GLsizei getAttachmentSamples(const gl::ImageIndex &imageIndex) const override;

    void onAttach(const gl::Context *context) override {}
    void onDetach(const gl::Context *context) override {}
    GLuint getId() const override;

    bool flexibleSurfaceCompatibilityRequested() const
    {
        return mFlexibleSurfaceCompatibilityRequested;
    }
    EGLint getOrientation() const { return mOrientation; }

    bool directComposition() const { return mDirectComposition; }

    gl::InitState initState(const gl::ImageIndex &imageIndex) const override;
    void setInitState(const gl::ImageIndex &imageIndex, gl::InitState initState) override;

    bool isRobustResourceInitEnabled() const { return mRobustResourceInitialization; }

    const gl::Format &getBindTexImageFormat() const { return mColorFormat; }

  protected:
    Surface(EGLint surfaceType,
            const egl::Config *config,
            const AttributeMap &attributes,
            EGLenum buftype = EGL_NONE);
    ~Surface() override;
    rx::FramebufferAttachmentObjectImpl *getAttachmentImpl() const override;

    gl::Framebuffer *createDefaultFramebuffer(const Display *display);

    // ANGLE-only method, used internally
    friend class gl::Texture;
    Error releaseTexImageFromTexture(const gl::Context *context);

    SurfaceState mState;
    rx::SurfaceImpl *mImplementation;
    int mRefCount;
    bool mDestroyed;

    EGLint mType;
    EGLenum mBuftype;

    bool mPostSubBufferRequested;
    bool mFlexibleSurfaceCompatibilityRequested;

    bool mLargestPbuffer;
    EGLenum mGLColorspace;
    EGLenum mVGAlphaFormat;
    EGLenum mVGColorspace;
    bool mMipmapTexture;
    EGLint mMipmapLevel;
    EGLint mHorizontalResolution;
    EGLint mVerticalResolution;
    EGLenum mMultisampleResolve;

    bool mFixedSize;
    size_t mFixedWidth;
    size_t mFixedHeight;

    bool mDirectComposition;

    bool mRobustResourceInitialization;

    TextureFormat mTextureFormat;
    EGLenum mTextureTarget;

    EGLint mPixelAspectRatio;  // Display aspect ratio
    EGLenum mRenderBuffer;     // Render buffer
    EGLenum mSwapBehavior;     // Buffer swap behavior

    EGLint mOrientation;

    // We don't use a binding pointer here. We don't ever want to own an orphaned texture. If a
    // Texture is deleted the Surface is unbound in onDestroy.
    gl::Texture *mTexture;

    gl::Format mColorFormat;
    gl::Format mDSFormat;

  private:
    Error destroyImpl(const Display *display);

    void postSwap(const gl::Context *context);
    Error releaseRef(const Display *display);

    gl::InitState mInitState;
};

class WindowSurface final : public Surface
{
  public:
    WindowSurface(rx::EGLImplFactory *implFactory,
                  const Config *config,
                  EGLNativeWindowType window,
                  const AttributeMap &attribs);
    ~WindowSurface() override;
};

class PbufferSurface final : public Surface
{
  public:
    PbufferSurface(rx::EGLImplFactory *implFactory,
                   const Config *config,
                   const AttributeMap &attribs);
    PbufferSurface(rx::EGLImplFactory *implFactory,
                   const Config *config,
                   EGLenum buftype,
                   EGLClientBuffer clientBuffer,
                   const AttributeMap &attribs);

  protected:
    ~PbufferSurface() override;
};

class PixmapSurface final : public Surface
{
  public:
    PixmapSurface(rx::EGLImplFactory *implFactory,
                  const Config *config,
                  NativePixmapType nativePixmap,
                  const AttributeMap &attribs);

  protected:
    ~PixmapSurface() override;
};

class SurfaceDeleter final
{
  public:
    SurfaceDeleter(const Display *display);
    ~SurfaceDeleter();
    void operator()(Surface *surface);

  private:
    const Display *mDisplay;
};

using SurfacePointer = angle::UniqueObjectPointerBase<Surface, SurfaceDeleter>;

}  // namespace egl

#endif  // LIBANGLE_SURFACE_H_
