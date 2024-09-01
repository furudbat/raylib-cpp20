#ifndef RAYLIB_CPP_INCLUDE_VRSTEREOCONFIG_HPP_
#define RAYLIB_CPP_INCLUDE_VRSTEREOCONFIG_HPP_

#include "raylib.hpp"

namespace raylib {
/**
 * VR stereo config functions for VR simulator
 */
class VrStereoConfig {
public:
    constexpr VrStereoConfig() = default;

    explicit VrStereoConfig(const ::VrDeviceInfo& info) { Load(info); }
    explicit VrStereoConfig(::VrDeviceInfo&& info) noexcept {
        Load(info);
        info = {};
    }

    /**
     * Load VR stereo config for VR simulator device parameters
     */
    void Load(const ::VrDeviceInfo& info) { set(::LoadVrStereoConfig(info)); }

    /**
     * Unload VR stereo config
     */
    ~VrStereoConfig() { Unload(); }

    /**
     * Begin stereo rendering
     */
    VrStereoConfig& BeginMode() {
        ::BeginVrStereoMode(m_data);
        return *this;
    }

    /**
     * End stereo rendering
     */
    VrStereoConfig& EndMode() {
        ::EndVrStereoMode();
        return *this;
    }

    /**
     * Unload VR stereo config
     */
    void Unload() { ::UnloadVrStereoConfig(m_data); }
protected:
    constexpr void set(const ::VrStereoConfig& config) noexcept {
        m_data.projection[0] = config.projection[0];
        m_data.projection[1] = config.projection[1];
        m_data.viewOffset[0] = config.viewOffset[0];
        m_data.viewOffset[1] = config.viewOffset[1];
        m_data.leftLensCenter[0] = config.leftLensCenter[0];
        m_data.leftLensCenter[1] = config.leftLensCenter[1];
        m_data.rightLensCenter[0] = config.rightLensCenter[0];
        m_data.rightLensCenter[1] = config.rightLensCenter[1];
        m_data.leftScreenCenter[0] = config.leftScreenCenter[0];
        m_data.leftScreenCenter[1] = config.leftScreenCenter[1];
        m_data.rightScreenCenter[0] = config.rightScreenCenter[0];
        m_data.rightScreenCenter[1] = config.rightScreenCenter[1];
        m_data.scale[0] = config.scale[0];
        m_data.scale[1] = config.scale[1];
        m_data.scaleIn[0] = config.scaleIn[0];
        m_data.scaleIn[1] = config.scaleIn[1];
    }

    ::VrStereoConfig m_data;
};
} // namespace raylib

using RVrStereoConfig = raylib::VrStereoConfig;

#endif // RAYLIB_CPP_INCLUDE_VRSTEREOCONFIG_HPP_
