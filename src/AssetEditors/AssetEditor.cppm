//
// Created by Vinnie on 28/03/2025.
//

module;
#include "ReflectionMacros.h"


export module Editor.AssetEditor;
import Engine.Node;
import Engine.Resource;

export class AssetEditor : public Node {
    CRAB_CLASS(AssetEditor, Node)

public:
    void SetContext(const std::shared_ptr<Resource>& ctx) {
        Context = ctx;
    }

protected:
    std::shared_ptr<Resource> Context;
};
