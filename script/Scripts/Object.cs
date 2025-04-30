using System;

namespace Scripts;

[NativeEngineType]
public class Object
{
    public IntPtr _nativeOwner;

    public Object(IntPtr nativeOwner)
    {
        _nativeOwner = nativeOwner;
    }

    public Object()
    {
        
    }
}

