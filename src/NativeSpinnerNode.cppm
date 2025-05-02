//
// Created by Vinnie on 2/05/2025.
//


module;
#include "ReflectionMacros.h"

export module NativeSpinnerNode;
import Engine.Node.Node3D;

export class NativeSpinnerNode : public Node3D {
    CRAB_CLASS(NativeSpinnerNode, Node3D)
    CLASS_FLAG(EditorVisible)

    BEGIN_PROPERTIES
        ADD_PROPERTY("Speed", Speed);
    END_PROPERTIES

    void Update(float dt) override {
        Super::Update(dt);

        auto orientation = GetOrientation();
        orientation = glm::rotate(orientation, Speed * dt, {1, 0, 0});
        SetOrientation(orientation);
    }

    float Speed = 1.0f;

};