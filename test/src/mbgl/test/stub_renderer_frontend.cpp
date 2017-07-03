#include <mbgl/test/stub_renderer_frontend.hpp>

#include <mbgl/map/view.hpp>
#include <mbgl/renderer/renderer.hpp>

namespace mbgl {

StubRendererFrontend::StubRendererFrontend(std::unique_ptr<Renderer> renderer_, InvalidateCallback invalidate)
        : renderer(std::move(renderer_))
        , asyncInvalidate([this, invalidate=std::move(invalidate)]() {
                invalidate(*this);
            }) {
}

StubRendererFrontend::StubRendererFrontend(std::unique_ptr<Renderer> renderer_, View& view)
        : renderer(std::move(renderer_))
        , asyncInvalidate([&]() {
                this->render(view);
            }) {
}

StubRendererFrontend::~StubRendererFrontend() {
    reset();
}

void StubRendererFrontend::reset() {
    if (renderer) {
        renderer.reset();
    }
}

void StubRendererFrontend::setObserver(RendererObserver& observer) {
    if (!renderer) return;
    renderer->setObserver(&observer);
}

void StubRendererFrontend::update(std::shared_ptr<UpdateParameters> params) {
    updateParameters = std::move(params);
    asyncInvalidate.send();
}

void StubRendererFrontend::render(View& view) {
    if (!updateParameters || !renderer) return;
    renderer->render(view, *updateParameters);
}

std::vector<Feature> StubRendererFrontend::queryRenderedFeatures(std::shared_ptr<RenderedQueryParameters> params) const {
    if (!renderer) return {};
    return renderer->queryRenderedFeatures(*params);
}

std::vector<Feature> StubRendererFrontend::querySourceFeatures(std::shared_ptr<SourceQueryParameters> params) const {
    if (!renderer) return {};
    return renderer->querySourceFeatures(*params);
}

} // namespace mbgl

