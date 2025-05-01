namespace Scripts;

public class TestScriptNode : Node3D
{
    public TestScriptNode(IntPtr nativeOwner) : base(nativeOwner) {}

    [SerializeField("Bob Distance")] 
    public float BobDistance = 3;
    
    [SerializeField("Bob Speed")] 
    public float BobSpeed = 1;

    private float accumulatedTime;
    
    protected override void EnterTree()
    {
        base.EnterTree();
    }

    protected override void Update(float dt)
    {
        base.Update(dt);
        accumulatedTime += dt;
        Position = new Vector3(Position.X, Position.Y, MathF.Sin(accumulatedTime * BobSpeed) * BobDistance);
    }
}
