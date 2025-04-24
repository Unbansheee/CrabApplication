

export module Editor.AssetEditorRegistry;
import std;
import Engine.Reflection.Class;
import Engine.Resource;
import Engine.Node;
import Engine.Assert;
import Engine.Reflection.ClassDB;
import Engine.StringID;

export class AssetEditorRegistry {
    static inline std::vector<std::pair<const ClassType*, const ClassType*>> resourceToEditorMap;

public:
    static void RegisterEditorType(const ClassType* resourceClass, const ClassType* editorType);
    static std::unique_ptr<Node> CreateEditorFor(std::shared_ptr<Resource> resource);
};

void AssetEditorRegistry::RegisterEditorType(const ClassType* resourceClass, const ClassType* editorType) {
    resourceToEditorMap.emplace_back(resourceClass, editorType);
}

std::pair<const ClassType*, const ClassType*> GetPairFor(const ClassType* c, const std::vector<std::pair<const ClassType*, const ClassType*>>& from) {
    for (auto& i : from) {
        if (i.first == c) return i;
    }
    return {nullptr, nullptr};
}

std::unique_ptr<Node> AssetEditorRegistry::CreateEditorFor(std::shared_ptr<Resource> resource) {
    auto resClass = &resource->GetStaticClassFromThis();

    const ClassType* out = nullptr;
    while (out == nullptr && resClass != nullptr) {
        auto e = GetPairFor(resClass, resourceToEditorMap);

        out = e.second;
        if (out) break;

        resClass = ClassDB::Get().GetClassByName(resClass->Parent);
    }

    if (!out) return {};

    //Assert::Check(out != nullptr, "No Editor Found", std::string("No editor registered for class: ") + resClass->Name.string());
    auto instance = Node::NewNode(*out, "Asset Editor");
    return std::move(instance);
}

export template<typename EditorType, typename Resource>
struct AssetEditorAutoRegister {
    AssetEditorAutoRegister() {
        AssetEditorRegistry::RegisterEditorType(&Resource::GetStaticClass(), &EditorType::GetStaticClass());
    }
};