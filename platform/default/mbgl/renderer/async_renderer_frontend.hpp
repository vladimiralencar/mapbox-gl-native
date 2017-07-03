#pragma once

#include <mbgl/map/view.hpp>
#include <mbgl/renderer/renderer_frontend.hpp>
#include <mbgl/util/async_task.hpp>

#include <memory>
#include <vector>

namespace mbgl {

//
class AsyncRendererFrontend : public mbgl::RendererFrontend {
public:
    AsyncRendererFrontend(std::unique_ptr<Renderer>, mbgl::View&);
    ~AsyncRendererFrontend() override;

    void reset();

    void update(std::shared_ptr<mbgl::UpdateParameters> updateParameters_) override;

    std::vector<mbgl::Feature> queryRenderedFeatures(std::shared_ptr<mbgl::RenderedQueryParameters> params) const override;
    std::vector<mbgl::Feature> querySourceFeatures(std::shared_ptr<mbgl::SourceQueryParameters> params) const override;
    
    void setObserver(mbgl::RendererObserver& observer_) override;
    
private:
    std::unique_ptr<mbgl::Renderer> renderer;
    mbgl::View& view;
    std::shared_ptr<mbgl::UpdateParameters> updateParameters;
    mbgl::util::AsyncTask asyncInvalidate;
};
    
} // namespace mbgl
