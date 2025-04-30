using System;

namespace Scripts;

using Generated;

[NativeEngineType]
public class Node : Object
{
    public Node(IntPtr nativeOwner) : base(nativeOwner) {}
    
    [NativeBind("Ready", typeof(void), typeof(IntPtr))]
    private static ReadyDelegate? NativeReady; 
    
    [NativeBind("Update", typeof(void), typeof(IntPtr), typeof(float))]
    private static UpdateDelegate? NativeUpdate;
    
    [NativeBind("EnterTree", typeof(void), typeof(IntPtr))]
    private static EnterTreeDelegate? NativeEnterTree;
    
    [NativeBind("ExitTree", typeof(void), typeof(IntPtr))]
    private static ExitTreeDelegate? NativeExitTree;
    
    protected virtual void EnterTree() {  
        NativeEnterTree?.Invoke(_nativeOwner);
    }

    protected virtual void ExitTree()
    {
        NativeExitTree?.Invoke(_nativeOwner);
    }

    protected virtual void Ready()
    {
        NativeReady?.Invoke(_nativeOwner);
    }

    protected virtual void Update(float dt)
    {
        NativeUpdate?.Invoke(_nativeOwner, dt);
    }
}