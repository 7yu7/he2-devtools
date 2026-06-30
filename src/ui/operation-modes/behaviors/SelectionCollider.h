#pragma once

#include <debug-rendering/DebugRenderable.h>
#include <debug-rendering/DebugRenderer.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include "Selection.h"
#include "ForwardDeclarations.h"

template<typename OpModeContext>
class SelectionColliderBehavior : public OperationModeBehavior, public devtools::debug_rendering::DebugRenderable {
public:
    using ObjectType = typename SelectionBehaviorTraits<OpModeContext>::ObjectType;

private:
    OpModeContext& context;

public:
    static constexpr unsigned int id = 14;
    virtual unsigned int GetId() override { return id; }

    SelectionColliderBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode) : OperationModeBehavior{ allocator, operationMode } , DebugRenderable{} , context{ operationMode.GetContext() } {}

    virtual void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) override
    {
        auto* selectionBehavior = operationMode.template GetBehavior<SelectionBehavior<OpModeContext>>();

        if (!selectionBehavior)
            return;

        auto* focusedChunk = context.GetFocusedChunk();

        if (!focusedChunk)
            return;

        for (auto* objectData : selectionBehavior->GetSelection())
        {
            auto* gameObject = focusedChunk->GetGameObject(objectData);

            if (!gameObject)
                continue;

            auto* visual = gameObject->GetComponent<hh::gfx::GOCVisual>();

            bool invisible = (!visual || visual == gameObject->GetComponent<hh::gfx::GOCVisualDebugDraw>()) && !gameObject->GetComponent<app::gfx::GOCVisualGeometryInstance>();

            if (!invisible)
                continue;

            for (auto* goc : gameObject->components)
            {
				if (goc->pStaticClass == hh::physics::GOCSphereCollider::GetClass()) {
					auto* cGoc = static_cast<hh::physics::GOCSphereCollider*>(goc);
                    ctx.DrawSphere(cGoc->GetWorldTransform(), cGoc->radius, { 0, 255, 255, 255 });
				}
				else if (goc->pStaticClass == hh::physics::GOCBoxCollider::GetClass()) {
					auto* cGoc = static_cast<hh::physics::GOCBoxCollider*>(goc);
                    ctx.DrawOBB(cGoc->GetWorldTransform(), cGoc->dimensions, { 0, 255, 255, 255 });
				}
				else if (goc->pStaticClass == hh::physics::GOCCapsuleCollider::GetClass()) {
					auto* cGoc = static_cast<hh::physics::GOCCapsuleCollider*>(goc);
                    ctx.DrawCapsule(cGoc->GetWorldTransform(), cGoc->radius, cGoc->halfHeight / 2, { 0, 255, 255, 255 });
				}
				else if (goc->pStaticClass == hh::physics::GOCCylinderCollider::GetClass()) {
					auto* cGoc = static_cast<hh::physics::GOCCylinderCollider*>(goc);
                    ctx.DrawCylinder(cGoc->GetWorldTransform(), cGoc->radius, cGoc->halfHeight, { 0, 255, 255, 255 });
				}
            }
        }
    }
};