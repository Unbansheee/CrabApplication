namespace Scripts;

public class TestScriptNode : Node3D
{
    public TestScriptNode(IntPtr nativeOwner) : base(nativeOwner) {}
    
    protected override void EnterTree()
    {
        base.EnterTree();
        Name = "GREGGY";
        Console.WriteLine(Name);
    }

    protected override void Update(float dt)
    {
        base.Update(dt);
    }
}
