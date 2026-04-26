class IGame {
public:
    
    virtual void onInit() = 0;
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onRender() = 0;
    virtual void onShutdown() = 0;
    virtual ~IGame() = default;
};