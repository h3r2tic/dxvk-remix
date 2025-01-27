#pragma once

#include "dxvk_include.h"
#include "../tracy/Tracy.hpp"
#include "../tracy/TracyVulkan.hpp"

#define ScopedCpuProfileZoneN(name) \
        ZoneScopedN(name)

#define ScopedCpuProfileZone() \
        ScopedCpuProfileZoneN(__FUNCTION__)

#define ScopedGpuProfileZone(ctx, name) \
        ScopedCpuProfileZoneN(name); \
        TracyVkZone(ctx->getDevice()->queues().graphics.tracyCtx, ctx->getCmdBuffer(DxvkCmdBuffer::ExecBuffer), name); \
        __ScopedAnnotation __scopedAnnotation(ctx, name)

#ifdef REMIX_DEVELOPMENT
  // NOTE: Since this uses dynamic strings to write variables to profiler, it can be more expensive than constexpr above, and so is only enabled in REMIX_DEVELOPMENT
  //       even still, it should only be used when absolutely necessary.  Ideally the cost of profiling is minimal for most representative results.
  #define ScopedGpuProfileZoneDynamic(ctx, name) \
          ScopedCpuProfileZone(); \
          ZoneText(name, strlen(name)); \
          TracyVkZoneTransient(ctx->getDevice()->queues().graphics.tracyCtx, TracyConcat(__tracy_gpu_source_location,__LINE__), ctx->getCmdBuffer(DxvkCmdBuffer::ExecBuffer), name, true); \
          __ScopedAnnotation __scopedAnnotation(ctx, name)
#else
  #define ScopedGpuProfileZoneDynamic(ctx, name)
#endif

namespace dxvk {

  class DxvkContext;

  /**
   * A helper class to add annotation/profiler ranges as renderOps into cmd buffer.
   */
  class __ScopedAnnotation {
  public:
    __ScopedAnnotation(Rc<DxvkContext> ctx, const char* name);
    ~__ScopedAnnotation();

  private:
    Rc<DxvkContext> m_ctx;
  };
}