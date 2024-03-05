#ifndef RAYLIB_CPP_INCLUDE_MODELANIMATION_HPP_
#define RAYLIB_CPP_INCLUDE_MODELANIMATION_HPP_

#include <vector>
#include <string>
#include <filesystem>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./Mesh.hpp"

namespace raylib {
/**
 * Model animation
 */
class ModelAnimation {
 public:
    constexpr ModelAnimation(owner<const ::ModelAnimation&>) = delete;
    constexpr ModelAnimation(owner<::ModelAnimation&&> model) {
        set(model);

        model.boneCount = 0;
        model.frameCount = 0;
        model.bones = nullptr;
        model.framePoses = nullptr;
    }

    constexpr ModelAnimation(const ModelAnimation&) = delete;
    constexpr ModelAnimation(ModelAnimation&& other) {
        set(other.m_data);

        other.m_data.boneCount = 0;
        other.m_data.frameCount = 0;
        other.m_data.bones = nullptr;
        other.m_data.framePoses = nullptr;
    }

    ~ModelAnimation() {
        Unload();
    }

    constexpr ModelAnimation& operator=(const ::ModelAnimation& model) = delete;
    constexpr ModelAnimation& operator=(::ModelAnimation&& model) {
        set(model);
        return *this;
    }

    constexpr ModelAnimation& operator=(const ModelAnimation&) = delete;
    ModelAnimation& operator=(ModelAnimation&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other.m_data);

        other.m_data.boneCount = 0;
        other.m_data.frameCount = 0;
        other.m_data.bones = nullptr;
        other.m_data.framePoses = nullptr;

        return *this;
    }

    explicit operator ::ModelAnimation() const {
        return m_data;
    }
    [[nodiscard]] ::ModelAnimation c_raylib() const & {
        return m_data;
    }

    GETTER(int, BoneCount, m_data.boneCount)
    SPAN_GETTER(::BoneInfo, Bones, m_data.bones, m_data.boneCount)
    GETTER(int, FrameCount, m_data.frameCount)
    GETTER(::Transform**, FramePoses, m_data.framePoses)
    /// @TODO: make framePoses as mdspan

    /**
     * Load model animations from file
     */
    static std::vector<ModelAnimation> Load(const std::filesystem::path& fileName) {
        int count = 0;
        auto modelAnimations = RayUniquePtr<::ModelAnimation>(::LoadModelAnimations(fileName.c_str(), &count));
        std::vector<ModelAnimation> ret;
        ret.reserve(static_cast<size_t>(count));
        for (auto& model : std::span{modelAnimations.get(), static_cast<size_t>(count)}) {
            ret.emplace_back(std::move(model));
            model.bones = nullptr;
            model.framePoses = nullptr;
            model.boneCount = 0;
            model.frameCount = 0;
        }
        return ret;
    }

    /**
     * Unload animation data
     */
    void Unload() {
        ::UnloadModelAnimation(m_data);
    }

    /**
     * Update model animation pose
     */
    ModelAnimation& Update(const ::Model& model, int frame) {
        ::UpdateModelAnimation(model, m_data, frame);
        return *this;
    }
    ModelAnimation& Update(const raylib::Model& model, int frame) {
        return Update(model.c_raylib(), frame);
    }

    /**
     * Check model animation skeleton match
     */
    [[nodiscard]] bool IsValid(const ::Model& model) const {
        return ::IsModelAnimationValid(model, m_data);
    }
    [[nodiscard]] bool IsValid(const raylib::Model& model) const {
        return IsValid(model.c_raylib());
    }

 protected:
    constexpr void set(const ::ModelAnimation& model) {
        m_data.boneCount = model.boneCount;
        m_data.frameCount = model.frameCount;
        m_data.bones = model.bones;
        m_data.framePoses = model.framePoses;
    }

    ::ModelAnimation m_data;
};
}  // namespace raylib

using RModelAnimation = raylib::ModelAnimation;

#endif  // RAYLIB_CPP_INCLUDE_MODELANIMATION_HPP_
