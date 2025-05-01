//
// Created by Vinnie on 2/05/2025.
//

module Editor.AssetEditorRegistry;
import Engine.Resource;
import Engine.Reflection.Class;
import Engine.Reflection.ClassDB;
import Engine.Node;

void AssetEditorRegistry::RegisterEditorType(const ClassType* resourceClass, const ClassType* editorType) {
    GetMappings().emplace_back(resourceClass, editorType);
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
        auto e = GetPairFor(resClass, GetMappings());

        out = e.second;
        if (out) break;

        resClass = ClassDB::Get().GetClassByName(resClass->Parent);
    }

    if (!out) return {};

    //Assert::Check(out != nullptr, "No Editor Found", std::string("No editor registered for class: ") + resClass->Name.string());
    auto instance = Node::NewNode(*out, "Asset Editor");
    return std::move(instance);
}