#pragma once
#include "Utils/Utils.hpp"

namespace RMC::algorithm {
    template<typename T, typename Point>
    concept IsArtist = IsPoint2D<Point> && requires(T a, Point point) {
        { a.PutPoint(point) };
    };

    template<typename T, typename Point>
    concept IsCoordTransformable = IsPoint2D<Point> && requires (T a, Point point) {
        { a.Transform(point) };
        { a.InverseTransform(point) };
    };

    template <typename Artist, typename CTrans>
        ////requires IsArtist<Artist, glm::vec2> && IsCoordTransformable<CTrans, glm::vec2>
    class BaseDrawAlgorithm
    {
    public:
        explicit BaseDrawAlgorithm(
            std::shared_ptr<Artist> artist,
            std::shared_ptr<CTrans> ctrans
        ) : m_artist(artist), m_ctrans(ctrans) {}

        virtual ~BaseDrawAlgorithm() {}

        virtual void Draw() = 0;

        void SetArtist(std::shared_ptr<Artist> artist) { m_artist = artist; }
        void SetCTrans(std::shared_ptr<CTrans> ctrans) { m_ctrans = ctrans; }

        const std::shared_ptr<Artist> GetArtist() const { return m_artist; }
        const std::shared_ptr<CTrans> GetCTrans() const { return m_ctrans; }
        std::shared_ptr<Artist> GetArtist() { return m_artist; }
        std::shared_ptr<CTrans> GetCTrans() { return m_ctrans; }

    protected:
        std::shared_ptr<Artist> m_artist;
        std::shared_ptr<CTrans> m_ctrans;
    };
} // namespace RMC::algorithm