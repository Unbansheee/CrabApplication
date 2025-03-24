module node_editor_camera_3d;
import Engine.Types;


void NodeEditorCamera3D::HandleMouseMovement(Vector2 movement)
{
    movement *= LookSensitivity;
    Pitch = glm::clamp(Pitch - movement.y, -89.9f, 89.9f);
    auto startYaw = Yaw;
    startYaw += movement.x;
    if (startYaw > 360.f)
    {
        startYaw -= 360.f;
    }
    else if (startYaw < 0.f)
    {
        startYaw += 360.f;
    }
    Yaw = startYaw;
}

void NodeEditorCamera3D::HandleKeyboardMovement(Vector3 movement)
{
    Vector3 offset{0,0,0};
    offset += Forward * movement.x;
    offset += glm::cross(Forward, Vector3{0, 0, 1}) * -movement.y;
    offset += Vector3{0,0,1} * movement.z;

    SetGlobalPosition(GetGlobalPosition() + offset * MoveSpeed);
}

Matrix4 NodeEditorCamera3D::GetViewMatrix() const
{
    glm::vec3 direction;

    direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    direction.z = sin(glm::radians(Pitch));
    direction.y = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Forward = glm::normalize(direction);

    return glm::lookAt(GetGlobalPosition(), GetGlobalPosition() + Forward, {0, 0, 1});
}
