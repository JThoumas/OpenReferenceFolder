#include "thumbnail_cache.h"
#include "framework/core/log.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <algorithm>
#include <condition_variable>

namespace orf {

// Supported image extensions
static bool isImageFile(const std::filesystem::path& path) {
    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".png"  || ext == ".jpg" || ext == ".jpeg"
        || ext == ".bmp"  || ext == ".tga" || ext == ".gif"
        || ext == ".webp" || ext == ".psd";
}

static GLuint uploadTexture(const uint8_t* pixels, int w, int h) {
    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return tex;
}

ThumbnailCache::~ThumbnailCache() { shutdown(); }

void ThumbnailCache::init() {
    // Create a 1x1 grey placeholder texture
    uint8_t grey[4] = {60, 60, 60, 255};
    m_placeholderTex = uploadTexture(grey, 1, 1);

    m_running = true;
    for (int i = 0; i < THREAD_COUNT; ++i)
        m_workers.emplace_back([this]() { workerLoop(); });

    LOG_INFO("ThumbnailCache: " << THREAD_COUNT << " worker threads started");
}

void ThumbnailCache::shutdown() {
    if (!m_running) return;
    
    m_running = false;
    m_jobCond.notify_all();
    
    for (auto& t : m_workers) if (t.joinable()) t.join();
    m_workers.clear();

    // Free GPU textures
    for (auto& [path, tex] : m_textures)
        if (tex && tex != m_placeholderTex) glDeleteTextures(1, &tex);
    if (m_placeholderTex) glDeleteTextures(1, &m_placeholderTex);
    m_textures.clear();
    m_pending.clear();
}

GLuint ThumbnailCache::request(const std::filesystem::path& path) {
    if (!isImageFile(path)) return 0;

    std::string key = path.string();

    // Already have a GPU texture
    auto it = m_textures.find(key);
    if (it != m_textures.end()) return it->second;

    // Already queued or being decoded
    if (m_pending.count(key)) return m_placeholderTex;

    // New request — queue it
    m_pending[key] = true;
    {
        std::lock_guard<std::mutex> lock(m_jobMutex);
        m_jobQueue.push(key);
    }
    m_jobCond.notify_one();

    return m_placeholderTex;
}

void ThumbnailCache::uploadPending() {
    // Drain the result queue — max 4 uploads per frame to avoid stalls
    int uploaded = 0;
    while (uploaded < 4) {
        ThumbnailResult result;
        {
            std::lock_guard<std::mutex> lock(m_resultMutex);
            if (m_resultQueue.empty()) break;
            result = std::move(m_resultQueue.front());
            m_resultQueue.pop();
        }
        if (result.pixels.empty()) {
            // Decode failed — store placeholder so we don't retry forever
            m_textures[result.path] = m_placeholderTex;
        } else {
            m_textures[result.path] = uploadTexture(
                result.pixels.data(), result.width, result.height);
        }
        m_pending.erase(result.path);
        ++uploaded;
    }
}

void ThumbnailCache::workerLoop() {
    while (m_running) {
        std::string path;
        {
            std::unique_lock<std::mutex> lock(m_jobMutex);
            m_jobCond.wait(lock, [this] { return !m_running || !m_jobQueue.empty(); });
            
            if (!m_running && m_jobQueue.empty()) return;
            
            path = std::move(m_jobQueue.front());
            m_jobQueue.pop();
        }
        decodeImage(path);
    }
}

void ThumbnailCache::decodeImage(const std::string& path) {
    int w, h, channels;
    // Force RGBA output regardless of source format
    uint8_t* data = stbi_load(path.c_str(), &w, &h, &channels, 4);

    ThumbnailResult result;
    result.path = path;

    if (!data) {
        LOG_WARN("ThumbnailCache: failed to decode " << path);
        // Empty pixels signals failure to uploadPending()
    } else {
        // Nearest-neighbour downsample to THUMB_SIZE x THUMB_SIZE
        result.width  = THUMB_SIZE;
        result.height = THUMB_SIZE;
        result.pixels.resize(THUMB_SIZE * THUMB_SIZE * 4);

        float xScale = (float)w / THUMB_SIZE;
        float yScale = (float)h / THUMB_SIZE;

        for (int ty = 0; ty < THUMB_SIZE; ++ty) {
            for (int tx = 0; tx < THUMB_SIZE; ++tx) {
                int sx = std::min((int)(tx * xScale), w - 1);
                int sy = std::min((int)(ty * yScale), h - 1);
                uint8_t* src = data + (sy * w + sx) * 4;
                uint8_t* dst = result.pixels.data() + (ty * THUMB_SIZE + tx) * 4;
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = src[3];
            }
        }
        stbi_image_free(data);
    }

    std::lock_guard<std::mutex> lock(m_resultMutex);
    m_resultQueue.push(std::move(result));
}

} // namespace orf
