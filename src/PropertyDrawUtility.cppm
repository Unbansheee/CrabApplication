module;

#pragma once

//#include "CrabTypes.cppm"
//#include "Reflection.cppm"
//#include "Transform.cppm"

export module Engine.PropertyDraw;
import std;
import Engine.Reflection;
import Engine.Types;
import Engine.Reflection.ClassDB;
import Engine.Resource.Ref;
import Engine.Transform;
import Engine.Object.Ref;

export struct PropertyDrawUtility
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
    void operator()(PropertyView& prop, ResourceRef& val);
    void operator()(PropertyView& prop, StrongResourceRef& val);
    void operator()(PropertyView& prop, ObjectRef<Object>& val);
    void operator()(PropertyView& prop, auto val) { std::cout << "Invalid variant for drawing: " << prop.name() << "\n"; }
    

    // Optional: Default handler
    template <typename T>
    void operator()(T&&) {
        static_assert(sizeof(T) == 0, "Unhandled type in PropertyVisitor");
    }
};
