using System;
using System.Runtime.InteropServices;

namespace Scripts;

[StructLayout(LayoutKind.Sequential)]
public struct Vector2
{
    public float X { get; set; } 
    public float Y { get; set; } 
    
    public override string ToString()
    {
        return "{" + $"{X}, {Y}" + "}";
    }
}


[StructLayout(LayoutKind.Sequential)]
public struct Vector2U
{
    public uint X;
    public uint Y;
    
    public override string ToString()
    {
        return "{" + $"{X}, {Y}" + "}";
    }
}

[StructLayout(LayoutKind.Sequential)]
public struct Vector2I
{
    public int X { get; set; } 
    public int Y { get; set; } 
    
    public override string ToString()
    {
        return "{" + $"{X}, {Y}" + "}";
    }
}

[StructLayout(LayoutKind.Sequential)]
public struct Vector3
{
    public Vector3(float x, float y, float z)
    {
        X = x;
        Y = y;
        Z = z;
    }
    
    public float X { get; set; } 
    public float Y { get; set; } 
    public float Z { get; set; }

    public override string ToString()
    {
        return "{" + $"{X}, {Y}, {Z}" + "}";
    }
}

[StructLayout(LayoutKind.Sequential)]
public struct Vector4
{
    public float X { get; set; } 
    public float Y { get; set; } 
    public float Z { get; set; } 
    public float W { get; set; } 
    
    public override string ToString()
    {
        return "{" + $"{X}, {Y}, {Z}, {W}" + "}";
    }
}

[StructLayout(LayoutKind.Sequential)]
public struct Quat
{
    public float W { get; set; } 
    public float X { get; set; } 
    public float Y { get; set; } 
    public float Z { get; set; } 
    
    public override string ToString()
    {
        return "{" + $"{W}, {X}, {Y}, {Z}" + "}";
    }
}

[StructLayout(LayoutKind.Sequential)]
public struct Transform
{
    public Vector3 Position { get; set; } 
    public Quat Orientation { get; set; } 
    public Vector3 Scale { get; set; }


    public override string ToString()
    {
        return "{\n" + $"    Position: {Position}\n    Orientation: {Orientation}\n    Scale: {Scale}" + "\n}";
    }
}



