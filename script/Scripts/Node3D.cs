using Generated;

namespace Scripts;

[NativeEngineType]
public class Node3D : Node
{
    public Node3D(IntPtr nativeOwner) : base(nativeOwner)
    {
    }

    [NativeBind("SetPosition", typeof(void), typeof(IntPtr), typeof(Vector3))]
    private static SetPositionDelegate? NativeSetPosition;
    
    [NativeBind("SetScale", typeof(void), typeof(IntPtr), typeof(Vector3))]
    private static SetScaleDelegate? NativeSetScale;
    
    [NativeBind("SetOrientation", typeof(void), typeof(IntPtr), typeof(Quat))]
    private static SetOrientationDelegate? NativeSetOrientation;
    
    [NativeBind("SetGlobalPosition", typeof(void), typeof(IntPtr), typeof(Vector3))]
    private static SetGlobalPositionDelegate? NativeSetGlobalPosition;
    
    [NativeBind("SetGlobalScale", typeof(void), typeof(IntPtr), typeof(Vector3))]
    private static SetGlobalScaleDelegate? NativeSetGlobalScale;
    
    [NativeBind("SetGlobalOrientation", typeof(void), typeof(IntPtr), typeof(Quat))]
    private static SetGlobalOrientationDelegate? NativeSetGlobalOrientation;

    [NativeBind("GetPosition", typeof(Vector3), typeof(IntPtr))]
    private static GetPositionDelegate? NativeGetPosition;
    
    [NativeBind("GetScale", typeof(Vector3), typeof(IntPtr))]
    private static GetScaleDelegate? NativeGetScale;
    
    [NativeBind("GetOrientation", typeof(Quat), typeof(IntPtr))]
    private static GetOrientationDelegate? NativeGetOrientation;
    
    [NativeBind("GetGlobalPosition", typeof(Vector3), typeof(IntPtr))]
    private static GetGlobalPositionDelegate? NativeGetGlobalPosition;
    
    [NativeBind("GetGlobalScale", typeof(Vector3), typeof(IntPtr))]
    private static GetGlobalScaleDelegate? NativeGetGlobalScale;
    
    [NativeBind("GetGlobalOrientation", typeof(Quat), typeof(IntPtr))]
    private static GetGlobalOrientationDelegate? NativeGetGlobalOrientation;
    
    public Vector3 Position
    {
        get { return NativeGetPosition.Invoke(_nativeOwner); }
        set { NativeSetPosition.Invoke(_nativeOwner, value) ;} 
    }
    public Vector3 Scale
    {
        get { return NativeGetScale.Invoke(_nativeOwner); }
        set { NativeSetScale.Invoke(_nativeOwner, value) ;} 
    }
    public Quat Orientation
    {
        get { return NativeGetOrientation.Invoke(_nativeOwner); }
        set { NativeSetOrientation.Invoke(_nativeOwner, value) ;} 
    }
    public Vector3 GlobalPosition
    {
        get { return NativeGetGlobalPosition.Invoke(_nativeOwner); }
        set { NativeSetGlobalPosition.Invoke(_nativeOwner, value) ;} 
    }
    public Vector3 GlobalScale
    {
        get { return NativeGetGlobalScale.Invoke(_nativeOwner); }
        set { NativeSetGlobalScale.Invoke(_nativeOwner, value) ;} 
    }
    public Quat GlobalOrientation
    {
        get { return NativeGetGlobalOrientation.Invoke(_nativeOwner); }
        set { NativeSetGlobalOrientation.Invoke(_nativeOwner, value) ;} 
    }
}