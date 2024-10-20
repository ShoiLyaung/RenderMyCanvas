#include "pch.hpp"
////
////#include "SceneSerializer.hpp"
////#include "Utils/Utils.hpp"
////#include "Primitive/PrimitiveFactory.hpp"
////#include "yaml-cpp/node/impl.h"
////
////namespace YAML {
////    template<>
////    struct convert<RMC::glm::vec2>
////    {
////        static Node encode(const RMC::glm::vec2& rhs)
////        {
////            Node node;
////            node.push_back(rhs.x);
////            node.push_back(rhs.y);
////            node.SetStyle(EmitterStyle::Flow);
////            return node;
////        }
////
////        static bool decode(const Node& node, RMC::glm::vec2& rhs)
////        {
////            if (!node.IsSequence() || node.size() != 2)
////                return false;
////
////            rhs.x = node[0].as<float>();
////            rhs.y = node[1].as<float>();
////            return true;
////        }
////    };
////
////    template<>
////    struct convert<RMC::glm::vec3>
////    {
////        static Node encode(const RMC::glm::vec3& rhs)
////        {
////            Node node;
////            node.push_back(rhs.x);
////            node.push_back(rhs.y);
////            node.push_back(rhs.z);
////            node.SetStyle(EmitterStyle::Flow);
////            return node;
////        }
////
////        static bool decode(const Node& node, RMC::glm::vec3& rhs)
////        {
////            if (!node.IsSequence() || node.size() != 3)
////                return false;
////
////            rhs.x = node[0].as<float>();
////            rhs.y = node[1].as<float>();
////            rhs.z = node[2].as<float>();
////            return true;
////        }
////    };
////
////    template<>
////    struct convert<RMC::glm::vec4>
////    {
////        static Node encode(const RMC::glm::vec4& rhs)
////        {
////            Node node;
////            node.push_back(rhs.x);
////            node.push_back(rhs.y);
////            node.push_back(rhs.z);
////            node.push_back(rhs.w);
////            node.SetStyle(EmitterStyle::Flow);
////            return node;
////        }
////
////        static bool decode(const Node& node, RMC::glm::vec4& rhs)
////        {
////            if (!node.IsSequence() || node.size() != 4)
////                return false;
////
////            rhs.x = node[0].as<float>();
////            rhs.y = node[1].as<float>();
////            rhs.z = node[2].as<float>();
////            rhs.w = node[3].as<float>();
////            return true;
////        }
////    };
////
////    template<>
////    struct convert<glm::quat>
////    {
////        static Node encode(const glm::quat& rhs)
////        {
////            Node node;
////            node.push_back(rhs.x);
////            node.push_back(rhs.y);
////            node.push_back(rhs.z);
////            node.push_back(rhs.w);
////            node.SetStyle(EmitterStyle::Flow);
////            return node;
////        }
////
////        static bool decode(const Node& node, RMC::glm::quat& rhs)
////        {
////            if (!node.IsSequence() || node.size() != 4)
////                return false;
////
////            rhs.x = node[0].as<float>();
////            rhs.y = node[1].as<float>();
////            rhs.z = node[2].as<float>();
////            rhs.w = node[3].as<float>();
////            return true;
////        }
////    };
////
////    template<>
////    struct convert<RMC::glm::mat2>
////    {
////        static Node encode(const RMC::glm::mat2& rhs)
////        {
////            Node node;
////            node.push_back(rhs[0][0]);
////            node.push_back(rhs[0][1]);
////            node.push_back(rhs[1][0]);
////            node.push_back(rhs[1][1]);
////            node.SetStyle(EmitterStyle::Flow);
////            return node;
////        }
////        static bool decode(const Node& node, RMC::glm::mat2& rhs)
////        {
////            if (!node.IsSequence() || node.size() != 4)
////                return false;
////            rhs[0][0] = node[0].as<float>();
////            rhs[0][1] = node[1].as<float>();
////            rhs[1][0] = node[2].as<float>();
////            rhs[1][1] = node[3].as<float>();
////            return true;
////        }
////    };
////
////    template<>
////    struct convert<RMC::glm::mat4>
////    {
////        static Node encode(const RMC::glm::mat4& rhs)
////        {
////            Node node;
////            for (glm::length_t r = 0; r < 4; ++r) {
////                for (glm::length_t c = 0ULL; c < 4ULL; ++c) {
////                    node.push_back(rhs[r][c]);
////                }
////            }
////            node.SetStyle(EmitterStyle::Flow);
////            return node;
////        }
////        static bool decode(const Node& node, RMC::glm::mat4& rhs)
////        {
////            if (!node.IsSequence() || node.size() != 16)
////                return false;
////            for (glm::length_t r = 0; r < 4; ++r) {
////                for (glm::length_t c = 0; c < 4; ++c) {
////                    rhs[r][c] = node[r * 4ULL + c].as<float>();
////                }
////            }
////            return true;
////        }
////    };
////}
////
////namespace RMC::scene {
////    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
////    {
////        out << YAML::Flow;
////        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
////        return out;
////    }
////
////    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
////    {
////        out << YAML::Flow;
////        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
////        return out;
////    }
////
////    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
////    {
////        out << YAML::Flow;
////        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
////        return out;
////    }
////
////    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& v)
////    {
////        out << YAML::Flow;
////        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
////        return out;
////    }
////
////    SceneSerializer::SceneSerializer(Scene3D* scene) : m_Scene(*scene) { }
////
////    static void SerializePrimitive(YAML::Emitter& out, const std::shared_ptr<::RMC::primitive::BasePrimitive>& primitive)
////    {
////        // Primitive {
////        out << YAML::BeginMap;
////        out << YAML::Key << "Primitive" << YAML::Value << "12345";  // ID
////
////        auto tag = primitive->GetComponent<::RMC::component::Tag>();
////        if (tag != nullptr) {
////            out << YAML::Key << "Tag";
////            out << YAML::BeginMap;
////            out << YAML::Key << "Name" << YAML::Value << tag->toString();
////            out << YAML::EndMap;
////        }
////
////        auto transform = primitive->GetComponent<::RMC::component::Transform>();
////        if (transform != nullptr) {
////            out << YAML::Key << "Transform";
////            out << YAML::BeginMap;
////            out << YAML::Key << "Position" << YAML::Value << transform->position;
////            out << YAML::Key << "Rotation" << YAML::Value << transform->rotation;
////            out << YAML::Key << "Scale" << YAML::Value << transform->scale;
////            out << YAML::EndMap;
////        }
////
////        auto material = primitive->GetComponent<::RMC::component::Material>();
////        if (material != nullptr) {
////            out << YAML::Key << "Material";
////            out << YAML::BeginMap;
////            out << YAML::Key << "Albedo" << YAML::Value << material->Albedo;
////            out << YAML::Key << "Roughness" << YAML::Value << material->Roughness;
////            out << YAML::Key << "Metallic" << YAML::Value << material->Metallic;
////            out << YAML::Key << "EmissionColor" << YAML::Value << material->EmissionColor;
////            out << YAML::Key << "EmissionPower" << YAML::Value << material->EmissionPower;
////            out << YAML::EndMap;
////        }
////
////        out << YAML::EndMap;
////        // } Primitive
////    }
////
////    void SceneSerializer::Serialize(const String & filePath)
////    {
////        YAML::Emitter out;
////
////        out << YAML::BeginMap; // Scene {
////        out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";
////        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
////
////        for (auto& primitive : m_Scene.PrimitiveList)
////        {
////            SerializePrimitive(out, primitive);
////        }
////
////        out << YAML::EndSeq;
////
////        out << YAML::EndMap; // } Scene
////
////        std::ofstream fout(filePath);
////        fout << out.c_str();
////    }
////
////    void SceneSerializer::SerializeRuntime(const String & filePath)
////    {
////    }
////
////    bool SceneSerializer::Deserialize(const String & filePath)
////    {
////        std::ifstream ifs(filePath);
////        std::stringstream strStream;
////        strStream << ifs.rdbuf();
////
////        YAML::Node data = YAML::Load(strStream.str());
////        if (!data["Scene"]) return false;
////
////        String sceneName = data["Scene"].as<String>();
////        auto entities = data["Entities"];
////
////        if (entities)
////        {
////            for (auto primitive : entities) {
////                ////uint64_t uuid = primitive["Eneity"].as<uint64_t>();  // ToDo
////                String name{};
////                auto tag = primitive["Tag"];
////                if (tag) {
////                    name = tag["Name"].as<String>();
////                }
////
////                auto deserializedPrimitive = m_Scene.PrimitiveFactory.Create(name);
////
////                auto yaml_transform = primitive["Transform"];
////                if (yaml_transform) {
////                    auto trans = deserializedPrimitive->GetComponent<::RMC::component::Transform>();
////                    trans->position = yaml_transform["Position"].as<glm::vec3>();
////                    trans->rotation = yaml_transform["Rotation"].as<glm::quat>();
////                    trans->scale = yaml_transform["Scale"].as<glm::vec3>();
////                }
////
////                auto yaml_material = primitive["Material"];
////                if (yaml_material) {
////                    deserializedPrimitive->AddComponent<::RMC::component::Material>();
////                    auto material = deserializedPrimitive->GetComponent<::RMC::component::Material>();
////                    material->Albedo = yaml_material["Albedo"].as<glm::vec4>();
////                    material->Roughness = yaml_material["Roughness"].as<float>();
////                    material->Metallic = yaml_material["Metallic"].as<float>();
////                    material->EmissionColor = yaml_material["EmissionColor"].as<glm::vec3>();
////                    material->EmissionPower = yaml_material["EmissionPower"].as<float>();
////                }
////
////                m_Scene.PrimitiveList.emplace_back(deserializedPrimitive);
////            }
////        }
////
////        return true;
////    }
////
////    bool SceneSerializer::DeserializeRuntime(const String & filePath)
////    {
////        return false;
////    }
////} // namespace RMC::scene