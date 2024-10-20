#pragma once
#include "BaseDrawAlgorithm.hpp"

namespace RMC::algorithm {
    template <typename Artist, typename CTrans>
    RMC_REQUIRE(IsArtist<Artist, glm::vec2>&& IsCoordTransformable<CTrans, glm::vec2>)
        class DigitalDifferencialAnalyzer : public BaseDrawAlgorithm<Artist, CTrans>
    {
    public:
        using MyBase = BaseDrawAlgorithm<Artist, CTrans>;
        using MyBase::SetArtist;
        using MyBase::SetCTrans;
        using MyBase::GetArtist;
        using MyBase::GetCTrans;

    public:
        DigitalDifferencialAnalyzer(
            std::shared_ptr<Artist> artist = nullptr,
            std::shared_ptr<CTrans> ctrans = nullptr
        ) : MyBase(artist, ctrans), m_start({ 0.0F }), m_end({ 0.0F }) {}

        void SetParams(std::vector<std::any> initList)
        {
            auto it = initList.begin();
            m_start = m_ctrans->Transform(std::any_cast<glm::vec2>(*it));
            ++it;
            m_end = m_ctrans->Transform(std::any_cast<glm::vec2>(*it));
        }

        virtual void Draw() override final
        {
            float k = (m_end.y - m_start.y) / (m_end.x - m_start.x);
            float y = m_start.y;
            for (float x = m_start.x; x <= m_end.x; ++x) {
                m_artist->PutPoint(m_ctrans->InverseTransform(glm::vec2{ x, y }));
                y += k;
            }
        }

    private:
        using MyBase::m_artist;
        using MyBase::m_ctrans;

        glm::vec2 m_start;
        glm::vec2 m_end;
    };
} // namespace RMC::algorithm