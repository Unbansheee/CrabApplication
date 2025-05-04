using System.Numerics;
using CrabEngine;

namespace CrabModule;

public class CameraController : Node3D
{
    private bool WDown;
    private bool SDown;
    private bool ADown;
    private bool DDown;
    private bool QDown;
    private bool EDown;
    
    [SerializeField("MOve Speed")] public float MoveSpeed = 6.0f;
    
    public CameraController(IntPtr nativeOwner) : base(nativeOwner)
    {
    }

    protected override void EnterTree()
    {
        base.EnterTree();
    }

    protected override void Update(float dt)
    {
        base.Update(dt);

        Vector3 outPos = new Vector3();
        if (WDown) outPos.X += 1;
        if (SDown) outPos.X -= 1;
        if (ADown) outPos.Y += 1;
        if (DDown) outPos.Y -= 1;
        Position += outPos * dt * MoveSpeed;
        
        Vector3 outGPos = new Vector3();
        if (QDown) outGPos.Z -= 1;
        if (EDown) outGPos.Z += 1;
        
        GlobalPosition += outGPos * dt * MoveSpeed;
    }

    protected override InputResult HandleInput(InputEvent inputEvent)
    {
        if (inputEvent is { Type: InputEventType.Key })
        {
            bool down = inputEvent.KeyAction is Input.Key.Action.Press or Input.Key.Action.Repeat;
            switch (inputEvent.Key)
            {
                case Input.Key.Code.W:
                    WDown = down;
                    break;
                case Input.Key.Code.S:
                    SDown = down;
                    break;
                case Input.Key.Code.A:
                    ADown = down;
                    break;
                case Input.Key.Code.D:
                    DDown = down;
                    break;
                case Input.Key.Code.Q:
                    QDown = down;
                    break;
                case Input.Key.Code.E:
                    EDown = down;
                    break;
            }
            return InputResult.Handled;
        }
        
        
        if (inputEvent is { Type: InputEventType.Scroll })
        {
            MoveSpeed += inputEvent.ScrollOffset.Y;
            MoveSpeed = (float)Math.Clamp(MoveSpeed, 1, 25);
            return InputResult.Handled;
        }
        
        return base.HandleInput(inputEvent);
    }
}