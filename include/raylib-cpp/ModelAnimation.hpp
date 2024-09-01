#ifndef RAYLIB_CPP_INCLUDE_MODELANIMATION_HPP_
#define RAYLIB_CPP_INCLUDE_MODELANIMATION_HPP_


#include "raylib.hpp"
#include "raylib-cpp-utils.hpp"
#include "Mesh.hpp"

#include <vector>
#include <string>
#include <filesystem>
#if defined(__has_include) && __has_include(<version>)
#include <version>
#ifdef __cpp_lib_mdspan
#include <mdspan>
#include <bit>
#endif
#endif

namespace raylib {
/**
 * Model animation
 */
class ModelAnimation {
 public:
    constexpr ModelAnimation(const ::ModelAnimation&) = delete;
    constexpr ModelAnimation(::ModelAnimation&& model) noexcept {
        set(model);

        model.boneCount = 0;
        model.frameCount = 0;
        model.bones = nullptr;
        model.framePoses = nullptr;
    }

    constexpr ModelAnimation(const ModelAnimation&) = delete;
    constexpr ModelAnimation(ModelAnimation&& other) noexcept {
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
    constexpr ModelAnimation& operator=(::ModelAnimation&& model) noexcept {
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
    GETTER(::Transform**, FramePosesC, m_data.framePoses)
#ifdef __cpp_lib_mdspan
private:
    using TransformExtents = std::extents<std::size_t, std::dynamic_extent, std::dynamic_extent>;
    using TransformMdspan = std::mdspan<::Transform, TransformExtents>;
    using ConstTransformMdspan = std::mdspan<const ::Transform, TransformExtents>;
public:
    [[nodiscard]] constexpr TransformMdspan GetFramePose(size_t index) {
        assert(std::cmp_less(index, m_data.frameCount));
        assert(m_data.frameCount >= 0);
        assert(m_data.boneCount >= 0);
        return {std::bit_cast<Transform*>(m_data.framePoses), static_cast<size_t>(m_data.frameCount), static_cast<size_t>(m_data.boneCount)};
    }
    [[nodiscard]] constexpr ConstTransformMdspan GetFramePose(size_t index) const {
        assert(std::cmp_less(index, m_data.frameCount));
        assert(m_data.frameCount >= 0);
        assert(m_data.boneCount >= 0);
        return {std::bit_cast<const Transform*>(m_data.framePoses), static_cast<size_t>(m_data.frameCount), static_cast<size_t>(m_data.boneCount)};
    }
#endif

    [[nodiscard]] constexpr ::BoneInfo& GetBone(size_t index) {
        assert(std::cmp_less(index, m_data.boneCount));
        return m_data.bones[index];
    }
    [[nodiscard]] constexpr const ::BoneInfo& GetBone(size_t index) const {
        assert(std::cmp_less(index, m_data.boneCount));
        return m_data.bones[index];
    }

    [[nodiscard]] constexpr ::Transform* GetFramePoseC(size_t index) {
        assert(std::cmp_less(index, m_data.frameCount));
        return m_data.framePoses[index];
    }
    [[nodiscard]] constexpr const ::Transform* GetFramePoseC(size_t index) const {
        assert(std::cmp_less(index, m_data.frameCount));
        return m_data.framePoses[index];
    }

    [[nodiscard]] constexpr std::span<::Transform> GetFramePose(size_t index) {
        assert(std::cmp_less(index, m_data.frameCount));
        assert(m_data.boneCount >= 0);
        return {m_data.framePoses[index], static_cast<size_t>(m_data.boneCount)};
    }
    [[nodiscard]] constexpr std::span<const ::Transform> GetFramePose(size_t index) const {
        assert(std::cmp_less(index, m_data.frameCount));
        assert(m_data.boneCount >= 0);
        return {m_data.framePoses[index], static_cast<size_t>(m_data.boneCount)};
    }

    /**
     * Load model animations from file
     */
    [[nodiscard]] static std::vector<ModelAnimation> Load(czstring fileName) {
        int count = 0;
        auto modelAnimations = RayUniquePtr<::ModelAnimation>(::LoadModelAnimations(fileName, &count));
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
    [[nodiscard]] static std::vector<ModelAnimation> Load(const std::filesystem::path& fileName) {
        return Load(fileName.c_str());
    }

    /**
     * Unload animation data
     */
    void Unload() noexcept {
        ::UnloadModelAnimation(m_data);
    }

    /**
     * Check model animation skeleton match
     */
    [[nodiscard]] bool IsValid(const ::Model& model) const {
        return ::IsModelAnimationValid(model, m_data);
    }
    /*
    [[nodiscard]] bool IsValid(const raylib::Model& model) const {
        return IsValid(model.c_raylib());
    }
    */

 protected:
    constexpr void set(const ::ModelAnimation& model) noexcept {
        m_data.boneCount = model.boneCount;
        m_data.frameCount = model.frameCount;
        m_data.bones = model.bones;
        m_data.framePoses = model.framePoses;

        // Duplicate the name. TextCopy() uses the null terminator, which we ignore here.
        for (size_t i = 0; i < 32; i++) {
            m_data.name[i] = model.name[i];
        }
    }

    ::ModelAnimation m_data;
};
}  // namespace raylib

using RModelAnimation = raylib::ModelAnimation;

#endif  // RAYLIB_CPP_INCLUDE_MODELANIMATION_HPP_
