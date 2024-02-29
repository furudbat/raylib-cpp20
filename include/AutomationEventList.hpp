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

struct AutomationEventListOptions {

};

/**
 * AutomationEventList management functions
 */
class AutomationEventList : public ::AutomationEventList {
 public:
    inline static constexpr int DefaultCapacity = 16384;

    [[deprecated("Use AutomationEventList(automationEventList)")]]
    explicit constexpr AutomationEventList(uint32_t _capacity,
                                 uint32_t _count = 0,
                                 AutomationEvent *_events = nullptr) : ::AutomationEventList{_capacity, _count, _events} {
        // Nothing.
    }

    explicit constexpr AutomationEventList(uint32_t _capacity,
                                           std::span<AutomationEvent> _events) : ::AutomationEventList{_capacity, static_cast<unsigned int>(_events.size()), _events.data()} {
        // Nothing.
    }

    constexpr AutomationEventList(const ::AutomationEventList& automationEventList = {
            .capacity = DefaultCapacity,
            .count = 0,
            .events = nullptr,
    }) {
        set(automationEventList);
    }

    explicit AutomationEventList(const std::filesystem::path& fileName) RAYLIB_CPP_THROWS {
        Load(fileName.c_str());
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

    GETTERSETTER(unsigned int, Capacity, capacity)
    GETTERSETTER(unsigned int, Count, count)
    GETTERSETTER(AutomationEvent*, Events, events)

    constexpr AutomationEventList& operator=(const ::AutomationEventList& other) {
        set(other);
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
            RAYLIB_CPP_RETURN_EXPECTED_OR_THROW(RaylibError("Failed to load automation event list"));
        }
    }

    /**
     * Update audio stream buffers with data
     */
    void Unload() {
        #if RAYLIB_VERSION_MAJOR >= 5
        #if RAYLIB_VERSION_MINOR == 0
        ::UnloadAutomationEventList(this);
        #elif RAYLIB_VERSION_MINOR >= 1
        ::UnloadAutomationEventList(*this);
        #endif
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
    constexpr void set(const ::AutomationEventList& other) {
        capacity = other.capacity;
        count = other.count;
        events = other.events;
    }
};
}  // namespace raylib

using RAutomationEventList = raylib::AutomationEventList;

#endif  // RAYLIB_CPP_INCLUDE_AUTOMATIONEVENTLIST_HPP_
