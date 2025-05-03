using CrabEngine;

namespace CrabModule;

public class EntryPoint
{
    public static void Init()
    {
        Console.WriteLine("LOADED A SECOND MODULE");
    }
}

public class CrabNode : Node3D
{
    public CrabNode(IntPtr nativeOwner) : base(nativeOwner)
    {
    }

    protected override void EnterTree()
    {
        base.EnterTree();
        Console.WriteLine("Bro we have a second module");
    }
}

public class AnotherNodeType : Node3D
{
    public AnotherNodeType(IntPtr nativeOwner) : base(nativeOwner)
    {
    }

    protected override void EnterTree()
    {
        base.EnterTree();
        Console.WriteLine("Bro we have a second module");
    }
}