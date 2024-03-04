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
class ModelAnimation : public ::ModelAnimation {
 public:
    constexpr ModelAnimation(const ::ModelAnimation& model) {
        set(model);
    }

    constexpr ModelAnimation(const ModelAnimation&) = delete;

    constexpr ModelAnimation(ModelAnimation&& other) {
        set(other);

        other.boneCount = 0;
        other.frameCount = 0;
        other.bones = nullptr;
        other.framePoses = nullptr;
    }

    ~ModelAnimation() {
        Unload();
    }

    /**
     * Load model animations from file
     */
    static std::vector<ModelAnimation> Load(const std::filesystem::path& fileName) {
        int count = 0;
        auto modelAnimations = RayUniquePtr<::ModelAnimation>(::LoadModelAnimations(fileName.c_str(), &count));
        return std::vector<ModelAnimation>(modelAnimations.get(), std::next(modelAnimations.get(), count));
    }

    GETTERSETTER(int, BoneCount, boneCount)
    SPAN_GETTER(::BoneInfo, Bones, bones, boneCount)
    GETTERSETTER(int, FrameCount, frameCount)
    GETTER(::Transform**, FramePoses, framePoses)
    /// @TODO: make framePoses as mdspan

    constexpr ModelAnimation& operator=(const ::ModelAnimation& model) {
        set(model);
        return *this;
    }

    constexpr ModelAnimation& operator=(const ModelAnimation&) = delete;
    ModelAnimation& operator=(ModelAnimation&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other);

        other.boneCount = 0;
        other.frameCount = 0;
        other.bones = nullptr;
        other.framePoses = nullptr;

        return *this;
    }

    /**
     * Unload animation data
     */
    void Unload() {
        ::UnloadModelAnimation(*this);
    }

    /**
     * Update model animation pose
     */
    ModelAnimation& Update(const ::Model& model, int frame) {
        ::UpdateModelAnimation(model, *this, frame);
        return *this;
    }

    /**
     * Check model animation skeleton match
     */
    [[nodiscard]] bool IsValid(const ::Model& model) const {
        return ::IsModelAnimationValid(model, *this);
    }

 protected:
    constexpr void set(const ::ModelAnimation& model) {
        boneCount = model.boneCount;
        frameCount = model.frameCount;
        bones = model.bones;
        framePoses = model.framePoses;
    }
};
}  // namespace raylib

using RModelAnimation = raylib::ModelAnimation;

#endif  // RAYLIB_CPP_INCLUDE_MODELANIMATION_HPP_
