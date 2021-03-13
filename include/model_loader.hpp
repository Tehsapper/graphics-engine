#pragma once

#include <util/filesystem.hpp>
#include <shader_types.hpp>
#include <assimp/scene.h>
#include <util/tree.hpp>
#include <glm/glm.hpp>
#include <stdexcept>
#include <memory>
#include <core/vertex.hpp>

namespace glm {
    glm::vec3 convert3f(const aiVector3D &aivec) noexcept;
    glm::vec2 convert2f(const aiVector3D &aivec) noexcept;
    glm::mat4 convert(const aiMatrix4x4& aimat) noexcept;
}

namespace LimitlessEngine {
    class AbstractModel;
    class AbstractMesh;
    class Material;

    struct VertexBoneWeight;
    struct Animation;
    struct Bone;

    struct model_loader_error : public std::runtime_error {
        explicit model_loader_error(const char* error) noexcept : runtime_error(error) {}
    };

    class ModelLoader {
    private:
        std::vector<std::shared_ptr<AbstractMesh>> loadMeshes(const aiScene* scene, const fs::path& path, std::vector<Bone>& bones, std::unordered_map<std::string, uint32_t>& bone_map);

        template<typename T, typename T1>
        std::shared_ptr<AbstractMesh> loadMesh(aiMesh* mesh, const fs::path& path, std::vector<Bone>& bones, std::unordered_map<std::string, uint32_t>& bone_map);
    protected:
        uint32_t unnamed_material_index {0};
        uint32_t unnamed_mesh_index {0};

        std::vector<VertexBoneWeight> loadBoneWeights(aiMesh* mesh, std::vector<Bone>& bones, std::unordered_map<std::string, uint32_t>& bone_map) const;
        std::vector<Animation> loadAnimations(const aiScene* scene, std::vector<Bone>& bones, std::unordered_map<std::string, uint32_t>& bone_map) const;
        Tree<uint32_t> loadAnimationTree(const aiScene* scene, std::vector<Bone>& bones, std::unordered_map<std::string, uint32_t>& bone_map) const;
        std::shared_ptr<Material> loadMaterial(aiMaterial* mat, const fs::path& path, const ModelShaders& model_shaders);
        std::vector<std::shared_ptr<Material>>
        loadMaterials(const aiScene* scene, const fs::path& path, ModelShader model_shader);

        template<typename T>
        std::vector<T> loadVertices(aiMesh* mesh) const noexcept {
	        std::vector<T> vertices;
	        vertices.reserve(mesh->mNumVertices);

	        for (uint32_t j = 0; j < mesh->mNumVertices; ++j) {
		        auto vertex = glm::convert3f(mesh->mVertices[j]);
		        auto normal = glm::convert3f(mesh->mNormals[j]);
		        auto tangent = mesh->mTangents ? glm::convert3f(mesh->mTangents[j]) : glm::vec3{0.f};
		        auto uv = glm::convert2f(mesh->mTextureCoords[0][j]);

		        if constexpr (std::is_same<T, VertexNormalTangent>::value) {
			        vertices.emplace_back(T{vertex, normal, tangent, uv});
		        }

		        if constexpr (std::is_same<T, VertexPackedNormalTangent>::value) {
			        auto packed_normal = pack(normal);
			        auto packed_tangent = pack(tangent);
			        auto packed_uv = glm::packHalf2x16(uv);

			        vertices.emplace_back(T{vertex, packed_normal, packed_tangent, packed_uv});
		        }
	        }

	        return vertices;
        }

        template<typename T>
        std::vector<T> loadIndices(aiMesh* mesh) const noexcept {
        	std::vector<T> indices;
		    indices.reserve(mesh->mNumFaces * 3);

		    for (uint32_t k = 0; k < mesh->mNumFaces; ++k) {
			    auto face = mesh->mFaces[k];
			    indices.emplace_back(face.mIndices[0]);
			    indices.emplace_back(face.mIndices[1]);
			    indices.emplace_back(face.mIndices[2]);
		    }

	        return indices;
        }

    public:
        std::shared_ptr<AbstractModel> loadModel(const fs::path& path, bool flip_uv = false);
    };
}
