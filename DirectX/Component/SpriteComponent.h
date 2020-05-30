#pragma once

#include "Component.h"
#include "ISprite.h"
#include "../Math/Math.h"
#include <memory>
#include <string>

class Sprite;
class SpriteManager;
class Transform2D;

class SpriteComponent : public Component, public ISprite, public std::enable_shared_from_this<SpriteComponent> {
public:
    SpriteComponent();
    virtual ~SpriteComponent();
    virtual void finalize() override;
    virtual void onSetActive(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

    virtual void update() override;
    virtual void setSprite(const std::string& fileName) override;
    virtual const std::shared_ptr<Transform2D>& transform() const override;
    virtual void setColor(const Vector3& color) override;
    virtual void setColor(float r, float g, float b) override;
    virtual void setAlpha(float alpha) override;
    virtual const Vector4& getColor() const override;
    virtual void setUV(float l, float t, float r, float b) override;
    virtual const Vector4& getUV() const override;
    virtual const Vector2& getTextureSize() const override;
    virtual void setActive(bool value) override;
    virtual bool getActive() const override;
    virtual bool isDead() const override;
    virtual const Texture& texture() const override;
    virtual const Shader& shader() const override;
    virtual const std::string& fileName() const override;
    virtual int getDrawOrder() const override;

    virtual void draw(const Matrix4& proj) const;

    static void setSpriteManager(SpriteManager* manager);

private:
    void addToManager();

protected:
    std::shared_ptr<Sprite> mSprite;
    int mDrawOrder;

    static SpriteManager* mSpriteManager;
};

