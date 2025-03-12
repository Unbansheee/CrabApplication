#pragma once
#include "Nodes/NodeCamera3D.h"

class NodeEditorCamera3D : public NodeCamera3D
{
public:
    CRAB_CLASS(NodeEditorCamera3D, NodeCamera3D)
    BEGIN_PROPERTIES
        ADD_PROPERTY("Mouse Sensitivity", LookSensitivity)
        ADD_PROPERTY("Move Speed", MoveSpeed)
        ADD_PROPERTY("Yaw", Yaw)
        ADD_PROPERTY("Pitch", Pitch)
    END_PROPERTIES

    void HandleMouseMovement(Vector2 movement);
    void HandleKeyboardMovement(Vector3 movement);
    
    float LookSensitivity = 0.5f;
    float MoveSpeed = 0.3f;
    float Yaw;
    float Pitch;
    mutable Vector3 Forward;
    
    
    Matrix4 GetViewMatrix() const override;
    
};
