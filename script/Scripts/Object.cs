using System;

namespace Scripts;

[NativeEngineType]
public class Object
{
    protected IntPtr _nativeOwner = IntPtr.Zero;

    public Object(IntPtr nativeOwner)
    {
        _nativeOwner = nativeOwner;
    }
}

