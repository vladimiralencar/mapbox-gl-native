#include "async_renderer_frontend.hpp"
#include <mbgl/renderer/renderer.hpp>

namespace mbgl {

AsyncRendererFrontend::AsyncRendererFrontend(std::unique_ptr<Renderer> renderer_, View& view_)
    : renderer(std::move(renderer_))
    , view(view_)
    , asyncInvalidate([this] {
        if (renderer && updateParameters) {
            renderer->render(view, *updateParameters);
        }
    }) {
}

AsyncRendererFrontend::~AsyncRendererFrontend() {
    reset();
}

void AsyncRendererFrontend::reset() {
    if (!renderer) return;
    renderer.reset();
}

void AsyncRendererFrontend::update(std::shared_ptr<UpdateParameters> updateParameters_) {
    updateParameters = updateParameters_;
    asyncInvalidate.send();
}

std::vector<Feature> AsyncRendererFrontend::queryRenderedFeatures(std::shared_ptr<RenderedQueryParameters> params) const {
    if (!renderer) return {};
    return renderer->queryRenderedFeatures(*params);
}

std::vector<Feature> AsyncRendererFrontend::querySourceFeatures(std::shared_ptr<SourceQueryParameters> params) const {
    if (!renderer) return {};
    return renderer->querySourceFeatures(*params);
}

void AsyncRendererFrontend::setRenderer(std::unique_ptr<Renderer> renderer_, RendererObserver& observer_) {
    renderer = std::move(renderer_);
    renderer->setObserver(&observer_);
}

void AsyncRendererFrontend::onLowMemory() {
    if (!renderer) return;
    renderer->onLowMemory();
}

void AsyncRendererFrontend::dumpDebugLogs() {
    if (!renderer) return;
    renderer->dumpDebugLogs();
}

} // namespace mbgl
