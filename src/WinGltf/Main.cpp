#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ���� glTF �Ļ����ṹ
class Accessor {
public:
    int bufferView;
    int componentType;
    int count;
    std::string type;
    std::vector<float> max;
    std::vector<float> min;

    // �� JSON ������ C++ ��
    friend void from_json(const json& j, Accessor& a) {
        j.at("bufferView").get_to(a.bufferView);
        j.at("componentType").get_to(a.componentType);
        j.at("count").get_to(a.count);
        j.at("type").get_to(a.type);
        j.at("max").get_to(a.max);
        j.at("min").get_to(a.min);
    }
};

class Buffer {
public:
    std::string uri;
    int byteLength;

    // �� JSON ������ C++ ��
    friend void from_json(const json& j, Buffer& b) {
        j.at("uri").get_to(b.uri);
        j.at("byteLength").get_to(b.byteLength);
    }
};

class Node {
public:
    int mesh;

    // �� JSON ������ C++ ��
    friend void from_json(const json& j, Node& n) {
        j.at("mesh").get_to(n.mesh);
    }
};

class Scene {
public:
    std::vector<int> nodes;

    // �� JSON ������ C++ ��
    friend void from_json(const json& j, Scene& s) {
        j.at("nodes").get_to(s.nodes);
    }
};

class GLTFAsset {
public:
    std::vector<Buffer> buffers;
    std::vector<Accessor> accessors;
    std::vector<Node> nodes;
    std::vector<Scene> scenes;

    // �� JSON ������ C++ ��
    friend void from_json(const json& j, GLTFAsset& asset) {
        j.at("buffers").get_to(asset.buffers);
        j.at("accessors").get_to(asset.accessors);
        j.at("nodes").get_to(asset.nodes);
        j.at("scenes").get_to(asset.scenes);
    }
};


int main()
{  
    // ����� glTF �ļ����ݣ���Ϊ JSON �ַ�����
    std::string gltf_json = R"({
        "asset": {
            "version": "2.0"
        },
        "buffers": [
            {
                "uri": "data.bin",
                "byteLength": 1234
            }
        ],
        "accessors": [
            {
                "bufferView": 0,
                "componentType": 5126,
                "count": 3,
                "type": "VEC3",
                "max": [1.0, 1.0, 1.0],
                "min": [-1.0, -1.0, -1.0]
            }
        ],
        "nodes": [
            {
                "mesh": 0
            }
        ],
        "scenes": [
            {
                "nodes": [0]
            }
        ]
    })";

    // ���� JSON �ַ���
    json j = json::parse(gltf_json);

    // �� JSON ����ӳ�䵽 GLTFAsset ��
    GLTFAsset asset = j.get<GLTFAsset>();

    // ��ӡ�����������
    std::cout << "Buffers count: " << asset.buffers.size() << std::endl;
    std::cout << "Accessors count: " << asset.accessors.size() << std::endl;
    std::cout << "Nodes count: " << asset.nodes.size() << std::endl;
    std::cout << "Scenes count: " << asset.scenes.size() << std::endl;

    // ���ĳ���ڵ�� mesh
    if (!asset.nodes.empty()) {
        std::cout << "First node mesh: " << asset.nodes[0].mesh << std::endl;
    }

    return 0;
}