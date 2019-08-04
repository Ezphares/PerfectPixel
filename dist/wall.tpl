AARectangleColliderMask:
  HalfSize: [80, 2]
  Center: [0, 0]
ColliderComponent:
  MaskType: 1
  EventTag: ""
SpriteComponent:
  FrameTimeAccumulator: 0
  DrawHint: 0
  FPS: 1
  CurrentFrame: 0
  SpriteResource: "wall.spr"
  Size: [160, 4]
  SpriteData:
    Image: ~
  Offset: [-80, -2]
PhysicsComponent:
  Bounciness: 0
  Mass: .inf
  ActiveForces:
    []
  SimulationType: 1
TransformComponent:
  Position: [0, 0, 0]
  Velocity: [0, 0, 0]