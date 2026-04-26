#pragma once
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>
#include <filesystem>

namespace orf {

struct ThumbnailResult {
    std::string           path;
    std::vector<uint8_t>  pixels;  // RGBA, always THUMB_SIZE x THUMB_SIZE
    int                   width;
    int                   height;
};

class ThumbnailCache {
public:
    static constexpr int THUMB_SIZE    = 128;
    static constexpr int THREAD_COUNT  = 3;

    ThumbnailCache()  = default;
    ~ThumbnailCache();

    // Must be called from the main thread after GL context is current
    void init();
    void shutdown();

    // Request a thumbnail. Returns 0 (no texture yet) or a valid GLuint.
    // If the path is new, queues an async decode job.
    GLuint request(const std::filesystem::path& path);

    // Call once per frame from the main thread —
    // uploads any finished decode results to GPU textures.
    void uploadPending();

private:
    void workerLoop();
    void decodeImage(const std::string& path);

    // GPU texture cache — main thread only
    std::unordered_map<std::string, GLuint> m_textures;

    // Paths currently being decoded or already queued — main thread only
    // (no lock needed since only main thread reads/writes this set)
    std::unordered_map<std::string, bool> m_pending;

    // Job queue — written by main thread, consumed by workers
    std::queue<std::string>  m_jobQueue;
    std::mutex               m_jobMutex;
    std::condition_variable  m_jobCond;

    // Result queue — written by workers, consumed by main thread
    std::queue<ThumbnailResult> m_resultQueue;
    std::mutex                  m_resultMutex;

    std::vector<std::thread> m_workers;
    std::atomic<bool>        m_running{false};

    // 1x1 grey placeholder shown while a thumbnail loads
    GLuint m_placeholderTex = 0;
};

} // namespace orf
