#pragma once

#include <mbgl/map/backend.hpp>
#include <mbgl/map/backend_scope.hpp>
#include <mbgl/map/view.hpp>
#include <mbgl/renderer/renderer_frontend.hpp>
#include <mbgl/util/async_task.hpp>

#include <memory>
#include <vector>

namespace mbgl {

//
class AsyncRendererFrontend : public mbgl::RendererFrontend {
public:
    AsyncRendererFrontend(mbgl::Backend&, mbgl::View&, BackendScope::ScopeType scopeType_ = BackendScope::ScopeType::Explicit);
    ~AsyncRendererFrontend() override;
    
    void update(std::shared_ptr<mbgl::UpdateParameters> updateParameters_) override;
    std::vector<mbgl::Feature> queryRenderedFeatures(std::shared_ptr<mbgl::RenderedQueryParameters> params) const override;
    
    std::vector<mbgl::Feature> querySourceFeatures(std::shared_ptr<mbgl::SourceQueryParameters> params) const override;
    
    void setRenderer(std::unique_ptr<mbgl::Renderer> renderer_, mbgl::RendererObserver& observer_) override;
    
    void onLowMemory() override;
    void dumpDebugLogs() override;
    
private:
    mbgl::Backend& backend;
    mbgl::View& view;
    BackendScope::ScopeType scopeType;
    std::unique_ptr<mbgl::Renderer> renderer;
    std::shared_ptr<mbgl::UpdateParameters> updateParameters;
    mbgl::util::AsyncTask asyncInvalidate;
};
    
} // namespace mbgl
