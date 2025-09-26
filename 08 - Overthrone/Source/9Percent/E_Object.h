/******************************************************************************/
/*!
\file   E_Object.h
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
  This is an interface file for an object in the world that can be updated
  and rendered to the screen. Also includes physics-enabled objects.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Asset.h"
#include "E_Transform.h"
#include "G_Physics.h"
#include "E_Sprite.h"
#include <string>
#include <functional>

namespace Engine {

	using namespace Graphics;

	class Object : public std::enable_shared_from_this<Object>
	{
	protected:
		Transform transform;

		sPtr<Sprite> spritePtr;

		f32 transparency;
		Color colorTint;

	public:
		Object();
		Object(const Object& other);
		Object(Object&& other) noexcept;
		Object(const Transform& transform, const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr, f32 transparency, const Color& colorTint);
		Object(const Transform& transform, const sPtr<Sprite>& spritePtr, f32 transparency, const Color& colorTint);
		virtual ~Object();

		void SetPosition(const Vec2& newPosition);
		void AddPosition(const Vec2& offset);
		void SetRotation(f32 newRotation);
		void AddRotation(f32 offset, bool clockwise = true);
		void SetScale(const Vec2& newScale);
		void AddScale(const Vec2& offset);
		void SetTransform(const Transform& newTransform);

		const Vec2& GetPosition() const;
		f32 GetRotation() const;
		const Vec2& GetScale() const;

		void SetSprite(const sPtr<Sprite>& newSpritePtr);
		sPtr<Sprite> GetSprite() const;
		void SetMesh(const sPtr<Mesh>& newMeshPtr);
		void SetTexture(const sPtr<Texture>& newTexturePtr);
		void SetTransparency(f32 newTransparency);
		f32 GetTransparency() const;
		void SetColorTint(const Color& newColorTint);
		sPtr<Texture> GetTexture() const;
		const Color& GetColor() const;

		virtual void Update();
		virtual void Render();

		/* DEBUG */
		Transform& Debug_GetTransform();
	};

	class PhysicsObject : public Object
	{
	public:
		sPtr<Physics::ColliderBase> collider;
		sPtr<Physics::PhysicsComponent> physicsComp;

		// Callback signature
		using OnCollisionCallbackSig = void(PhysicsObject& thisObj, PhysicsObject& otherObj, Physics::PhysicsCollisionData& collisionData);
		using OnSettleCallbackSig = void(PhysicsObject& thisObj);

		std::function<OnCollisionCallbackSig> onCollisionCallback;
		std::function<OnSettleCallbackSig> onSettleCallback;

	public:
		PhysicsObject();
		PhysicsObject(const PhysicsObject& other);
		PhysicsObject(PhysicsObject&& other) noexcept;
		PhysicsObject(bool registerToSystem, bool isDynamic, const Transform& transform, const sPtr<Mesh>& meshPtr, const sPtr<Texture>& texturePtr, f32 transparency, const Color& colorTint);
		PhysicsObject(bool registerToSystem, bool isDynamic, const Transform& transform, const sPtr<Sprite>& spritePtr, f32 transparency, const Color& colorTint);

		// Change the type of collider
		template <typename ColliderType>
		void SetCollider();
		// Be careful with this! It's important that no sPtr is alive when this PhysicsObject dies.
		wPtr<const Physics::PhysicsComponent> GetPhysicsCompWeak() const;
		wPtr<Physics::PhysicsComponent> GetPhysicsCompWeak();
		// Remove this object from the list of objects handled by the physics system.
		void RemoveFromPhysicsSystem() const;

		bool GetIsDynamic() const;
		virtual void SetIsDynamic(bool isDynamic);
		void SetGravityEnabled(bool isGravityEnabled);
		void SetLockRotation(bool lockRotation);

		void SetLayerMask(const std::initializer_list<Physics::MASK_LAYER>& layers);
		void UnsetLayerMask(const std::initializer_list<Physics::MASK_LAYER>& layers);

		const Vec2& GetVelocity() const;
		void SetVelocity(const Vec2& newVelocity);
		f32 GetMass() const;
		void SetMass(f32 newMass);
		f32 GetRestitutionCoeff() const;
		void SetRestitutionCoeff(f32 newRestiCoeff);
		f32 GetFrictionCoeff() const;
		void SetFrictionCoeff(f32 newFrictionCoeff);
		f32 GetAngVelocity() const;
		void SetAngVelocity(f32 newAngVelocity);

		// Apply a constant force at a point relative to this object's position.
		// Call this before physics update.
		void ApplyForce(const Vec2& force, const Vec2& relativePoint = Vec2Zero(), f32 dt = C_frametime);
		// Apply an impulse at a point relative to this object's position.
		// Call this before physics update.
		void ApplyImpulse(const Vec2& impulse, const Vec2& relativePoint = Vec2Zero());

		// Collision callback
		// Limitation: Called each frame if a collision continues multiple frames.
		//			   Will try to see if there's a way to only call once per collision.
		void OnCollision(PhysicsObject& other, Physics::PhysicsCollisionData& collisionData);
		void SetOnCollisionCallback(const std::function<OnCollisionCallbackSig>& func);
		// Settle callback
		// Limitation: Called each frame.
		void OnSettle();
		void SetOnSettleCallback(const std::function<OnSettleCallbackSig>& func);

	};


	class ObjectBuilder
	{
	private:
		ObjectBuilder() = delete;

	public:
		static sPtr<Object> CreateSquare(const Transform& transform = Transform(0.0f, 0.0f, 0.0f, 100.0f, 100.0f),
			const sPtr<Sprite> spritePtr = SpriteBuilder::CreateSpriteSquare(), f32 transparency = 1.0f, const Color& colorTint = Color());

		// Create a physics object
		static sPtr<PhysicsObject> CreatePhysicsSquare(bool registerToSystem, bool isDynamic = false, const Transform& transform = Transform(0.0f, 0.0f, 0.0f, 100.0f, 100.0f),
			const sPtr<Sprite> spritePtr = SpriteBuilder::CreateSpriteSquare(), f32 transparency = 1.0f, const Color& colorTint = Color());

		// For creating a class that is a child of class Object.
		// Sacrifices some initialization performance for flexibility.
		template <typename T = Object>
		static sPtr<T> CreateGenericSquare(const Transform& transform = Transform(0.0f, 0.0f, 0.0f, 100.0f, 100.0f),
			const sPtr<Sprite> spritePtr = SpriteBuilder::CreateSpriteSquare(), f32 transparency = 1.0f, const Color& colorTint = Color())
		{
			sPtr<T> obj;
			obj.reset(new T());
			obj->SetSprite(spritePtr);
			obj->SetTransform(transform);
			obj->SetTransparency(transparency);
			obj->SetColorTint(colorTint);
			return obj;
		}

	};

	/* Definitions */
	template <typename ColliderType>
	void PhysicsObject::SetCollider()
	{
		if (std::is_same_v<decltype(*collider), ColliderType>)
			return;
		collider.reset(new ColliderType(&transform));
		physicsComp->SetCollider(collider);
	}
}
