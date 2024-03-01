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
class AutomationEventList : public ::AutomationEventList {
 public:
    inline static constexpr int DefaultCapacity = 16384;

    [[deprecated("Use AutomationEventList(automationEventList)")]]
    explicit constexpr AutomationEventList(uint32_t _capacity,
                                 uint32_t _count = 0,
                                 owner<AutomationEvent*> _events = nullptr) : ::AutomationEventList{_capacity, _count, _events} {
        // Nothing.
    }

    constexpr AutomationEventList(const ::AutomationEventList& automationEventList) = delete;
    constexpr AutomationEventList(::AutomationEventList&& automationEventList = {
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
        set(other);

        other.capacity = 0;
        other.count = 0;
        other.events = nullptr;
    }
    ~AutomationEventList() {
        Unload();
    }

    explicit AutomationEventList(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        Load(fileName.c_str());
    }

    GETTERSETTER(unsigned int, Capacity, capacity)
    GETTERSETTER(unsigned int, Count, count)
    GETTERSETTER(AutomationEvent*, Events, events)

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
        set(other);

        other.capacity = 0;
        other.count = 0;
        other.events = nullptr;

        return *this;
    }

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
                ::UnloadAutomationEventList(this);
            #elif RAYLIB_VERSION_MINOR >= 1
                ::UnloadAutomationEventList(*this);
            #endif
        #else
            ::UnloadAutomationEventList(*this);
        #endif
    }

    bool IsReady() {
        return events != nullptr;
    }

    bool Export(const std::filesystem::path& fileName) {
        return ::ExportAutomationEventList(*this, fileName.c_str());
    }

    void Set() {
        ::SetAutomationEventList(this);
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
        if (index < 0 || std::cmp_greater_equal(index, count)) {
            return;
        }

        Set();
        ::PlayAutomationEvent(events[index]);
    }

 protected:
    constexpr void set(const ::AutomationEventList& other) noexcept {
        capacity = other.capacity;
        count = other.count;
        events = other.events;
    }
};
}  // namespace raylib

using RAutomationEventList = raylib::AutomationEventList;

#endif  // RAYLIB_CPP_INCLUDE_AUTOMATIONEVENTLIST_HPP_
