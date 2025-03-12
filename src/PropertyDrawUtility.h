#pragma once
#include <iostream>
#include <string>

#include "CrabTypes.h"
#include "Reflection.h"
#include "Transform.h"

struct PropertyDrawUtility
{
    // Handle all supported types
    void operator()(PropertyView& prop, int& val);
    void operator()(PropertyView& prop, float& val);
    void operator()(PropertyView& prop, bool& val);
    void operator()(PropertyView& prop, std::string& val);
    void operator()(PropertyView& prop, Vector2& val);
    void operator()(PropertyView& prop, Vector3& val);
    void operator()(PropertyView& prop, Vector4& val);
    void operator()(PropertyView& prop, Quat& val);
    void operator()(PropertyView& prop, Transform& val);
    void operator()(PropertyView& prop, ResourceHandle& val);
    void operator()(PropertyView& prop, auto val) { std::cout << "Invalid variant for drawing: " << prop.name() << "\n"; }
    

    // Optional: Default handler
    template <typename T>
    void operator()(T&&) {
        static_assert(sizeof(T) == 0, "Unhandled type in PropertyVisitor");
    }
};
