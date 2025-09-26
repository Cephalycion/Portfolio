/******************************************************************************/
/*!
\file   E_Object.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (95%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\author Marc Alviz Evangelista (5%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\brief
  This is the implementation file for an object in the world that can be updated
  and rendered to the screen. Also includes physics-enabled objects.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Object.h"
#include "E_Camera.h"
#include "E_Asset.h"

namespace Engine {

	using namespace Physics;

	Object::Object()
		: transparency(1.0f)
		, spritePtr(SpriteBuilder::CreateSpriteSquare())
	{
	}
	Object::Object(const Object& other)
		: transform(other.transform)
		, spritePtr(other.spritePtr)
		, transparency(other.transparency)
		, colorTint(other.colorTint)
	{
	}
	Object::Object(Object&& other) noexcept
		: transform(other.transform)
		, spritePtr(other.spritePtr)
		, transparency(other.transparency)
		, colorTint(other.colorTint)
	{
	}
	Object::Object(const Transform& transform, const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr, f32 transparency, const Color& colorTint)
		: transform(transform)
		, spritePtr(SpriteBuilder::CreateSprite(meshPtr, texturePtr))
		, transparency(transparency)
		, colorTint(colorTint)
	{
	}

	Object::Object(const Transform& transform, const sPtr<Sprite>& spritePtr, f32 transparency, const Color& colorTint)
		: transform(transform)
		, spritePtr(spritePtr)
		, transparency(transparency)
		, colorTint(colorTint)
	{
	}
	Object::~Object()
	{
	}

	void Object::SetPosition(const Vec2& newPosition)
	{
		transform.SetPosition(newPosition);
	}
	void Object::AddPosition(const Vec2& offset)
	{
		transform.AddPosition(offset);
	}
	void Object::SetRotation(f32 newRotation)
	{
		transform.SetRotation(newRotation);
	}
	void Object::AddRotation(f32 offset, bool clockwise)
	{
		transform.AddRotation(offset, clockwise);
	}
	void Object::SetScale(const Vec2& newScale)
	{
		transform.SetScale(newScale);
	}
	void Object::AddScale(const Vec2& offset)
	{
		transform.AddScale(offset);
	}
	void Object::SetTransform(const Transform& newTransform)
	{
		transform = newTransform;
	}

	const Vec2& Object::GetPosition() const
	{
		return transform.GetPosition();
	}
	f32 Object::GetRotation() const
	{
		return transform.GetRotation();
	}
	const Vec2& Object::GetScale() const
	{
		return transform.GetScale();
	}

	void Object::SetSprite(const sPtr<Sprite>& newSpritePtr)
	{
		spritePtr = newSpritePtr;
	}
	sPtr<Sprite> Object::GetSprite() const
	{
		return spritePtr;
	}

	void Object::SetMesh(const sPtr<Mesh>& newMeshPtr)
	{
		spritePtr->SetMesh(newMeshPtr);
	}
	void Object::SetTexture(const sPtr<Texture>& newTexturePtr)
	{
		spritePtr->SetTexture(newTexturePtr);
	}
	void Object::SetTransparency(f32 newTransparency)
	{
		transparency = newTransparency;
	}
	f32 Object::GetTransparency() const
	{
		return transparency;
	}
	void Object::SetColorTint(const Color& newColorTint)
	{
		colorTint = newColorTint;
	}

	sPtr<Texture> Object::GetTexture() const
	{
		return spritePtr->GetTexture();
	}
	const Color& Object::GetColor() const
	{
		return colorTint;
	}

	void Object::Update()
	{
		spritePtr->Update(C_frametime);
	}
	void Object::Render()
	{
		spritePtr->Render(transform, transparency, colorTint);
	}

	Transform& Object::Debug_GetTransform()
	{
		return transform;
	}


	PhysicsObject::PhysicsObject()
		: Object()
		, collider(new OBBCollider(&transform))
		, physicsComp(PhysicsManager::MakePhysicsComp(true, this, &transform, false))
	{
		physicsComp->SetCollider(collider);
	}
	PhysicsObject::PhysicsObject(const PhysicsObject& other)
		: Object(other)
		, collider(other.collider)
		, physicsComp(other.physicsComp)
	{
		physicsComp->SetCollider(collider);
	}
	PhysicsObject::PhysicsObject(PhysicsObject&& other) noexcept
		: Object(other)
		, collider(other.collider)
		, physicsComp(other.physicsComp)
	{
		physicsComp->SetCollider(collider);
	}
	PhysicsObject::PhysicsObject(bool registerToSystem, bool isDynamic, const Transform& transform, const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr, f32 transparency, const Color& colorTint)
		: Object(transform, meshPtr, texturePtr, transparency, colorTint)
		, collider(new OBBCollider(&this->transform))
		, physicsComp(PhysicsManager::MakePhysicsComp(registerToSystem, this, &this->transform, isDynamic))
	{
		physicsComp->SetCollider(collider);
	}

	PhysicsObject::PhysicsObject(bool registerToSystem, bool isDynamic, const Transform& transform, const sPtr<Sprite>& spritePtr, f32 transparency, const Color& colorTint)
		: Object(transform, spritePtr, transparency, colorTint)
		, collider(new OBBCollider(&this->transform))
		, physicsComp(PhysicsManager::MakePhysicsComp(registerToSystem, this, &this->transform, isDynamic))
	{
		physicsComp->SetCollider(collider);
	}

	wPtr<const Physics::PhysicsComponent> PhysicsObject::GetPhysicsCompWeak() const
	{
		return physicsComp;
	}
	wPtr<Physics::PhysicsComponent> PhysicsObject::GetPhysicsCompWeak()
	{
		return physicsComp;
	}
	void PhysicsObject::RemoveFromPhysicsSystem() const
	{
		Singleton<C_TYPE_PHYSICSMANAGER>::Instance()->UpdateComp(physicsComp, Physics::Events::COMP_UPDATE_DEAD);
	}

	bool PhysicsObject::GetIsDynamic() const
	{
		return physicsComp->GetIsDynamic();
	}
	void PhysicsObject::SetIsDynamic(bool isDynamic)
	{
		physicsComp->SetIsDynamic(isDynamic);
	}
	void PhysicsObject::SetGravityEnabled(bool isGravityEnabled)
	{
		physicsComp->SetGravityEnabled(isGravityEnabled);
	}
	void PhysicsObject::SetLockRotation(bool lockRotation)
	{
		physicsComp->SetLockRotation(lockRotation);
	}

	void PhysicsObject::SetLayerMask(const std::initializer_list<Physics::MASK_LAYER>& layers)
	{
		physicsComp->SetLayerMask(layers);
	}
	void PhysicsObject::UnsetLayerMask(const std::initializer_list<Physics::MASK_LAYER>& layers)
	{
		physicsComp->UnsetLayerMask(layers);
	}

	const Vec2& PhysicsObject::GetVelocity() const
	{
		return physicsComp->GetVelocity();
	}
	void PhysicsObject::SetVelocity(const Vec2& newVelocity)
	{
		physicsComp->SetVelocity(newVelocity);
	}
	f32 PhysicsObject::GetMass() const
	{
		return physicsComp->GetMass();
	}
	void PhysicsObject::SetMass(f32 newMass)
	{
		physicsComp->SetMass(newMass);
	}
	f32 PhysicsObject::GetRestitutionCoeff() const
	{
		return physicsComp->GetRestitutionCoeff();
	}
	void PhysicsObject::SetRestitutionCoeff(f32 newRestiCoeff)
	{
		physicsComp->SetRestitutionCoeff(newRestiCoeff);
	}
	f32 PhysicsObject::GetFrictionCoeff() const
	{
		return physicsComp->GetFrictionCoeff();
	}
	void PhysicsObject::SetFrictionCoeff(f32 newFrictionCoeff)
	{
		physicsComp->SetFrictionCoeff(newFrictionCoeff);
	}
	f32 PhysicsObject::GetAngVelocity() const
	{
		return physicsComp->GetAngVelocity();
	}
	void PhysicsObject::SetAngVelocity(f32 newAngVelocity)
	{
		physicsComp->SetAngVelocity(newAngVelocity);
	}

	void PhysicsObject::ApplyForce(const Vec2& force, const Vec2& relativePoint, f32 dt)
	{
		physicsComp->ApplyForce(force, relativePoint, dt);
	}
	void PhysicsObject::ApplyImpulse(const Vec2& impulse, const Vec2& relativePoint)
	{
		physicsComp->ApplyImpulse(impulse, relativePoint);
	}

	void PhysicsObject::OnCollision(PhysicsObject& other, Physics::PhysicsCollisionData& collisionData)
	{
		if (onCollisionCallback)
			onCollisionCallback(*this,other, collisionData);
	}
	void PhysicsObject::SetOnCollisionCallback(const std::function<OnCollisionCallbackSig>& func)
	{
		onCollisionCallback = func;
	}

	void PhysicsObject::OnSettle()
	{
		if (onSettleCallback)
			onSettleCallback(*this);
	}
	void PhysicsObject::SetOnSettleCallback(const std::function<OnSettleCallbackSig>& func)
	{
		onSettleCallback = func;
	}

	sPtr<Object> ObjectBuilder::CreateSquare(const Transform& transform, const sPtr<Sprite> spritePtr, f32 transparency, const Color& colorTint)
	{
		return sPtr<Object>(new Object(
			transform,
			spritePtr,
			transparency,
			colorTint
		));
	}

	sPtr<PhysicsObject> ObjectBuilder::CreatePhysicsSquare(bool registerToSystem, bool isDynamic, const Transform& transform, const sPtr<Sprite> spritePtr, f32 transparency, const Color& colorTint)
	{
		return sPtr<PhysicsObject>(new PhysicsObject(
			registerToSystem,
			isDynamic,
			transform,
			spritePtr,
			transparency,
			colorTint
		));
	}


}
