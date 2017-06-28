#include "qrenderer_frontend.hpp"

#include <mbgl/map/backend_scope.hpp>
#include <mbgl/renderer/renderer.hpp>

QRendererFrontend::QRendererFrontend(std::unique_ptr<mbgl::Renderer> renderer_, mbgl::View& view_)
    : renderer(std::move(renderer_))
    , view(view_) {
}

QRendererFrontend::~QRendererFrontend() {
    reset();
}

void QRendererFrontend::reset() {
    if (renderer) {
        renderer.reset();
    }
}

void QRendererFrontend::update(std::shared_ptr<mbgl::UpdateParameters> updateParameters_) {
    updateParameters = updateParameters_;
    emit updated();
}

std::vector<mbgl::Feature> QRendererFrontend::queryRenderedFeatures(mbgl::ScreenLineString geometry, mbgl::RenderedQueryOptions options) const {
    if (!renderer) return {};
    return renderer->queryRenderedFeatures(geometry, options);
}

std::vector<mbgl::Feature> QRendererFrontend::querySourceFeatures(std::string sourceId, mbgl::SourceQueryOptions options) const {
    if (!renderer) return {};
    return renderer->querySourceFeatures(sourceId, options);
}

void QRendererFrontend::setObserver(mbgl::RendererObserver& observer_) {
    if (!renderer) return;
    renderer->setObserver(&observer_);
}

void QRendererFrontend::render() {
    if (!renderer || !updateParameters) return;
    renderer->render(view, *updateParameters);
}

void QRendererFrontend::onLowMemory() {
    if (!renderer) return;
    renderer->onLowMemory();
}
