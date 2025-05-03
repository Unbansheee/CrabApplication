using System.Numerics;
using CrabEngine;

namespace CrabApplication;

public class BobberNode : CrabEngine.Node3D
{
    public BobberNode(IntPtr nativeOwner) : base(nativeOwner) {}

    [SerializeField("Bob Distance")] 
    public float BobDistance = 3;
    
    [SerializeField("Bob Speed")] 
    public float BobSpeed = 1;
    

    private float _accumulatedTime;
    
    protected override void EnterTree()
    {
        base.EnterTree();
    }

    protected override void Update(float dt)
    {
        base.Update(dt);
        _accumulatedTime += dt;
        Position = new Vector3(Position.X, Position.Y, MathF.Sin(_accumulatedTime * BobSpeed) * BobDistance);
        Console.WriteLine("Bobbing Hehe");

    }
}