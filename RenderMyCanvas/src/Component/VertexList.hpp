#pragma once
#include "BaseComponent.hpp"
#include "Material.hpp"

namespace RMC::component {
    struct Vertex
    {
        explicit Vertex(
            Ref<const glm::vec3> pos = { 0.0F, 0.0F, 0.0F },
            Ref<const glm::vec3> n = { 0.0F, 0.0F, 1.0F },
            Ref<const Material> m = Material{}
        ) : position(pos), normal(n), material(m) {}

        glm::vec3 position;
        glm::vec3 normal;
        Material material;
    };

    /**
     * @brief Binder to a vertex.
     *        ||Warning|| If the vertex is deleted, the binder will be invalid; The binder will not offer any check.
    */
    struct VertexBinder : public BaseComponent
    {
        virtual std::string WhatAmI() const override final
        {
            return "RMC::component::VertexBinder";
        }

        Ptr<Vertex> vertex_ptr = nullptr;
    };

    class VertexList : public BaseComponent
    {
    public:
        explicit VertexList() = default;
        virtual ~VertexList() override = default;

        virtual std::string WhatAmI() const override final
        {
            return "RMC::component::VertexList";
        }

        constexpr uint64_t size() const { return vertices.size(); }
        constexpr bool empty() const { return vertices.empty(); }

        constexpr const Vertex& operator[](uint64_t index) const { return vertices.at(index); }
        constexpr Vertex& operator[](uint64_t index) { return vertices.at(index); }

        constexpr const Vertex& at(uint64_t index) const { return vertices.at(index); }
        constexpr Vertex& at(uint64_t index) { return vertices.at(index); }

        constexpr void resize(uint64_t _count) { vertices.resize(_count); }
        constexpr void resize(uint64_t _count, const Vertex& _val) { vertices.resize(_count, _val); }

        template<typename... _ValTy>
        constexpr decltype(auto) emplace_back(_ValTy&&... _vals) { return vertices.emplace_back(std::forward<_ValTy>(_vals)...); }

        constexpr void push_back(const Vertex& _val) { vertices.push_back(_val); }
        constexpr void push_back(Vertex&& _val) { vertices.push_back(std::move(_val)); }

        constexpr void pop_back() { vertices.pop_back(); }

        template<typename Iter>
            requires std::is_same_v<Iter, typename std::vector<Vertex>::iterator> || std::is_same_v<Iter, typename std::vector<Vertex>::const_iterator>
        constexpr void erase(Iter _where) { vertices.erase(_where); }
        template<typename Iter>
            requires std::is_same_v<Iter, typename std::vector<Vertex>::iterator> || std::is_same_v<Iter, typename std::vector<Vertex>::const_iterator>
        constexpr void erase(Iter _begin, Iter _end) { vertices.erase(_begin, _end); }

        template<typename Iter>
            requires std::is_same_v<Iter, typename std::vector<Vertex>::iterator> || std::is_same_v<Iter, typename std::vector<Vertex>::const_iterator>
        constexpr void insert(Iter _where, const Vertex& _val) { vertices.insert(_where, _val); }

        template<typename Iter>
            requires std::is_same_v<Iter, typename std::vector<Vertex>::iterator> || std::is_same_v<Iter, typename std::vector<Vertex>::const_iterator>
        constexpr void insert(Iter _where, Vertex&& _val) { vertices.insert(_where, std::move(_val)); }

        template<typename Iter>
            requires std::is_same_v<Iter, typename std::vector<Vertex>::iterator> || std::is_same_v<Iter, typename std::vector<Vertex>::const_iterator>
        constexpr void insert(Iter _where, uint64_t _count, const Vertex& _val) { vertices.insert(_where, _count, _val); }

        constexpr auto cbegin() const { return vertices.cbegin(); }
        constexpr auto begin() const { return vertices.cbegin(); }
        constexpr auto being() { return vertices.begin(); }
        constexpr auto cend()const { return vertices.cend(); }
        constexpr auto end() const { return vertices.cend(); }
        constexpr auto end() { return vertices.end(); }

        constexpr void clear() { vertices.clear(); }

        glm::vec3 GetCentroid() const
        {
            glm::vec3 centroid{ 0.0F };
            for (const auto& vertex : vertices) {
                centroid += vertex.position;
            }
            centroid /= vertices.size();
            return centroid;
        }

        glm::vec4 GetAverageAlbedo() const
        {
            glm::vec4 averageAlbedo{ 0.0F };
            for (const auto& vertex : vertices) {
                averageAlbedo += vertex.material.Albedo;
            }
            averageAlbedo /= float(vertices.size());
            return averageAlbedo;
        }

        glm::vec4 GetFirstAlbedo() const
        {
            return vertices.front().material.Albedo;
        }

        void ReCalculateNormal()
        {
            glm::vec3 centroid = GetCentroid();
            for (auto& vertex : vertices) {
                vertex.normal = glm::normalize(vertex.position - centroid);
            }
        }

        std::vector<Vertex> vertices;
    };
}