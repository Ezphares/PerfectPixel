AARectangleColliderMask:
  HalfSize: [2, 2]
  Center: [0, 0]
ColliderComponent:
  MaskType: 1
  EventTag: ""
SpriteComponent:
  FrameTimeAccumulator: 0
  DrawHint: 0
  FPS: 1
  CurrentFrame: 0
  SpriteResource: "ball.spr"
  Size: [4, 4]
  SpriteData:
    Image: ~
  Offset: [-2, -2]
PhysicsComponent:
  Bounciness: 1
  Mass: 0
  ActiveForces:
    []
  SimulationType: 2
TransformComponent:
  Position: [0, 0, 0]
  Velocity: [0, 0, 0]
BallComponent:
  Score1: 0
  Score2: 0
  DeltaXPrev: 0
