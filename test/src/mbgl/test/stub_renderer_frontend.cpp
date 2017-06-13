#include <mbgl/test/stub_renderer_frontend.hpp>

#include <mbgl/renderer/renderer.hpp>
#include <mbgl/map/backend_scope.hpp>

namespace mbgl {

StubRendererFrontend::StubRendererFrontend(Backend& backend_, FileSource& fileSource,
                                           Scheduler& scheduler, InvalidateCallback invalidate,
                                           float pixelRatio_, MapMode mapMode,
                                           GLContextMode contextMode, const optional<std::string> programCacheDir)
    : backend(backend_)
    , asyncInvalidate([this, invalidate=std::move(invalidate)]() {
                invalidate(*this);
            }) {
    renderer = std::make_unique<Renderer>(pixelRatio_, scheduler, fileSource, mapMode, contextMode,
                                          programCacheDir);
}

StubRendererFrontend::StubRendererFrontend(Backend& backend_, View& view, FileSource& fileSource,
                                           Scheduler& scheduler, float pixelRatio_, MapMode mapMode,
                                           GLContextMode contextMode,
                                           const optional<std::string> programCacheDir)
        : backend(backend_)
        , asyncInvalidate([&]() {
                BackendScope guard { backend };
                this->render(view);
            }) {
    renderer = std::make_unique<Renderer>(pixelRatio_, scheduler, fileSource, mapMode, contextMode,
                                          programCacheDir);
}

StubRendererFrontend::~StubRendererFrontend() {
    reset();
}

void StubRendererFrontend::reset() {
    if (renderer) {
        BackendScope guard { backend };
        renderer.reset();
    }
}

void StubRendererFrontend::setObserver(RendererObserver& observer) {
    renderer->setObserver(&observer);
}

void StubRendererFrontend::update(std::shared_ptr<UpdateParameters> params) {
    updateParameters = std::move(params);
    asyncInvalidate.send();
}

void StubRendererFrontend::render(View& view) {
    assert(BackendScope::exists());
    if (!updateParameters || !renderer) return;

    renderer->render(backend, view, *updateParameters);
}

std::vector<Feature> StubRendererFrontend::queryRenderedFeatures(std::shared_ptr<RenderedQueryParameters> params) const {
    return renderer->queryRenderedFeatures(*params);
}

std::vector<Feature> StubRendererFrontend::querySourceFeatures(std::shared_ptr<SourceQueryParameters> params) const {
    return renderer->querySourceFeatures(*params);
}

} // namespace mbgl

