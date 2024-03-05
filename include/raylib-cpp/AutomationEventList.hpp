#ifndef RAYLIB_CPP_INCLUDE_AUTOMATIONEVENTLIST_HPP_
#define RAYLIB_CPP_INCLUDE_AUTOMATIONEVENTLIST_HPP_

#include <cstdint>
#include <filesystem>
#include <utility>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#ifdef __cpp_exceptions
#include "./RaylibException.hpp"
#endif
#include "./RaylibError.hpp"

namespace raylib {

/**
 * AutomationEventList management functions
 */
class AutomationEventList {
 public:
    inline static constexpr int DefaultCapacity = 16384;

    [[deprecated("Use AutomationEventList(automationEventList)")]]
    explicit constexpr AutomationEventList(uint32_t _capacity,
                                 uint32_t _count = 0,
                                 owner<AutomationEvent*> _events = nullptr) : m_data{_capacity, _count, _events} {
        // Nothing.
    }

    constexpr AutomationEventList(owner<const ::AutomationEventList&> automationEventList) = delete;
    constexpr AutomationEventList(owner<::AutomationEventList&&> automationEventList = {
            .capacity = DefaultCapacity,
            .count = 0,
            .events = nullptr,
    }) {
        set(automationEventList);

        automationEventList.capacity = 0;
        automationEventList.count = 0;
        automationEventList.events = nullptr;
    }

    constexpr AutomationEventList(const AutomationEventList&) = delete;
    constexpr AutomationEventList(AutomationEventList&& other) {
        set(other.m_data);

        other.m_data.capacity = 0;
        other.m_data.count = 0;
        other.m_data.events = nullptr;
    }
    ~AutomationEventList() {
        Unload();
    }

    //explicit operator ::AutomationEventList() const {
    //    return m_data;
    //}
    [[nodiscard]] ::AutomationEventList c_raylib() const & {
        return m_data;
    }

    explicit AutomationEventList(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        Load(fileName.c_str());
    }

    constexpr AutomationEventList& operator=(const ::AutomationEventList& other) = delete;
    constexpr AutomationEventList& operator=(::AutomationEventList&& other) {
        set(other);

        other.capacity = 0;
        other.count = 0;
        other.events = nullptr;

        return *this;
    }

    constexpr AutomationEventList& operator=(const AutomationEventList&) = delete;
    AutomationEventList& operator=(AutomationEventList&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other.m_data);

        other.m_data.capacity = 0;
        other.m_data.count = 0;
        other.m_data.events = nullptr;

        return *this;
    }

    GETTERSETTER(unsigned int, Capacity, m_data.capacity)
    GETTERSETTER(unsigned int, Count, m_data.count)
    SPAN_GETTER(AutomationEvent, Events, m_data.events, m_data.capacity)

    /**
     * Load audio stream (to stream raw audio pcm data)
     *
     * @throws raylib::RaylibException Throws if the AutomationEventList failed to load.
     */
    RAYLIB_CPP_EXPECTED_RESULT(void) Load(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        Unload();
        set(::LoadAutomationEventList(fileName.c_str()));
        if (!IsReady()) {
            RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(RaylibError("Failed to load automation event list"));
        }
        RAYLIB_CPP_RETURN_EXPECTED();
    }

    /**
     * Update audio stream buffers with data
     */
    void Unload() {
        if (!IsReady()) {
            return;
        }

        // The function signature of UnloadAutomationEventList() changes from raylib 5.0.
        #if RAYLIB_VERSION_MAJOR == 5
            #if RAYLIB_VERSION_MINOR == 0
                ::UnloadAutomationEventList(&m_data);
            #elif RAYLIB_VERSION_MINOR >= 1
                ::UnloadAutomationEventList(m_data);
            #endif
        #else
            ::UnloadAutomationEventList(m_data);
        #endif
    }

    bool IsReady() {
        return m_data.events != nullptr;
    }

    bool Export(const std::filesystem::path& fileName) {
        return ::ExportAutomationEventList(m_data, fileName.c_str());
    }

    void Set() {
        ::SetAutomationEventList(&m_data);
    }

    void SetBaseFrame(int frame) {
        Set();
        ::SetAutomationEventBaseFrame(frame);
    }

    void StartRecording() {
        Set();
        ::StartAutomationEventRecording();
    }

    void StopRecording() {
        Set();
        ::StopAutomationEventRecording();
    }

    void Play(int index) {
        if (index < 0 || std::cmp_greater_equal(index, m_data.count)) {
            return;
        }

        Set();
        ::PlayAutomationEvent(m_data.events[index]);
    }

 protected:
    constexpr void set(const ::AutomationEventList& other) noexcept {
        m_data.capacity = other.capacity;
        m_data.count = other.count;
        m_data.events = other.events;
    }

    ::AutomationEventList m_data;
};
}  // namespace raylib

using RAutomationEventList = raylib::AutomationEventList;

#endif  // RAYLIB_CPP_INCLUDE_AUTOMATIONEVENTLIST_HPP_
