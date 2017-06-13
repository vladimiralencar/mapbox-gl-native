
#pragma once

#include <mbgl/map/backend.hpp>
#include <mbgl/map/mode.hpp>
#include <mbgl/renderer/renderer_frontend.hpp>
#include <mbgl/util/async_task.hpp>

#include <functional>
#include <memory>

namespace mbgl {

class StubRendererFrontend : public RendererFrontend {
public:
    // Calls the provided callback when it's time to render
    using InvalidateCallback = std::function<void (StubRendererFrontend&)>;
    StubRendererFrontend(Backend&, FileSource&, Scheduler&, InvalidateCallback, float pixelRatio_,
                         MapMode, GLContextMode = GLContextMode::Unique,
                         const optional<std::string> programCacheDir = {});

    // Will render async with a default backend scope
    StubRendererFrontend(Backend&, View&, FileSource&, Scheduler&, float pixelRatio_, MapMode,
                         GLContextMode = GLContextMode::Unique,
                         const optional<std::string> programCacheDir = {});

    ~StubRendererFrontend() override;

    void reset() override;

    void setObserver(RendererObserver&) override;

    void update(std::shared_ptr<UpdateParameters>) override;
    void render(View& view);

    std::vector<Feature> queryRenderedFeatures(std::shared_ptr<RenderedQueryParameters>) const override;

    std::vector<Feature> querySourceFeatures(std::shared_ptr<SourceQueryParameters>) const override;

private:
    Backend& backend;
    std::unique_ptr<Renderer> renderer;
    std::shared_ptr<UpdateParameters> updateParameters;
    util::AsyncTask asyncInvalidate;
};

} // namespace mbgl
