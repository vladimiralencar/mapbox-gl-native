#pragma once

#include <mbgl/text/glyph.hpp>
#include <mbgl/text/glyph_manager_observer.hpp>
#include <mbgl/text/glyph_range.hpp>
#include <mbgl/util/noncopyable.hpp>
#include <mbgl/util/font_stack.hpp>
#include <mbgl/util/immutable.hpp>

#include <string>
#include <unordered_map>

namespace mbgl {

class FileSource;
class AsyncRequest;
class Response;

class GlyphRequestor {
public:
    virtual ~GlyphRequestor() = default;
    virtual void onGlyphsAvailable(GlyphMap) = 0;
};

class GlyphManager : public util::noncopyable {
public:
    GlyphManager(FileSource&);
    ~GlyphManager();

    // Workers send a `getGlyphs` message to the main thread once they have determined
    // their `GlyphDependencies`. If all glyphs are already locally available, GlyphManager
    // will provide them to the requestor immediately. Otherwise, it makes a request on the
    // FileSource is made for each range neeed, and notifies the observer when all are
    // complete.
    void getGlyphs(GlyphRequestor&, GlyphDependencies);
    void removeRequestor(GlyphRequestor&);

    void setURL(const std::string& url) {
        glyphURL = url;
    }

    void setObserver(GlyphManagerObserver*);

private:
    FileSource& fileSource;
    std::string glyphURL;

    struct GlyphRequest {
        bool parsed = false;
        std::unique_ptr<AsyncRequest> req;
        std::unordered_map<GlyphRequestor*, std::shared_ptr<GlyphDependencies>> requestors;
    };

    struct Entry {
        std::map<GlyphRange, GlyphRequest> ranges;
        std::map<GlyphID, Immutable<Glyph>> glyphs;
    };

    std::unordered_map<FontStack, Entry, FontStackHash> entries;

    GlyphRequest& requestRange(Entry&, const FontStack&, const GlyphRange&);
    void processResponse(const Response&, const FontStack&, const GlyphRange&);
    void notify(GlyphRequestor&, const GlyphDependencies&);

    GlyphManagerObserver* observer = nullptr;
};

} // namespace mbgl
