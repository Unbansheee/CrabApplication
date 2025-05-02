using System.Numerics;
using CrabEngine;

namespace CrabApplication;

public class SpinnerNode : CrabEngine.Node3D
{
    public SpinnerNode(IntPtr nativeOwner) : base(nativeOwner) {}

    [SerializeField("Spin Speed")]
    public float SpinSpeed = 3;
    
    protected override void EnterTree()
    {
        base.EnterTree();
    }

    protected override void Update(float dt)
    {
        base.Update(dt);
        Orientation *= Quaternion.CreateFromAxisAngle(new Vector3(0, 0, 1), SpinSpeed * dt);
    }
}