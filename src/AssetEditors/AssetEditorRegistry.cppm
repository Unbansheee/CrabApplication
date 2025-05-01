

export module Editor.AssetEditorRegistry;
import std;


export class Node;
export class Resource;
export struct ClassType;

export class AssetEditorRegistry {
    static std::vector<std::pair<const ClassType*, const ClassType*>>& GetMappings() {
        static std::vector<std::pair<const ClassType*, const ClassType*>> s_mappings;
        return s_mappings;
    };

public:
    static void RegisterEditorType(const ClassType* resourceClass, const ClassType* editorType);
    static std::unique_ptr<Node> CreateEditorFor(std::shared_ptr<Resource> resource);
};

export template<typename EditorType, typename Resource>
struct AssetEditorAutoRegister {
    AssetEditorAutoRegister() {
        AssetEditorRegistry::RegisterEditorType(&Resource::GetStaticClass(), &EditorType::GetStaticClass());
    }
};