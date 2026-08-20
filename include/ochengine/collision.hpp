#pragma once

#include "ochengine/math.hpp"
#include "ochengine/shapes.hpp"

struct Collision {
  Object *object1;
  Object *object2;

  float normalX;
  float normalY;

  float penetration;

  Collision(Object *object1_, Object *object2_) {
    object1 = object1_;
    object2 = object2_;

    normalX = 0;
    normalY = 0;

    penetration = 0;
  }
};

inline bool CheckRectRectCol(Collision *col) {
  Rect *r1 = static_cast<Rect *>(col->object1);
  Rect *r2 = static_cast<Rect *>(col->object2);

  float dispX = r2->GetCenterX() - r1->GetCenterX();
  float dispY = r2->GetCenterY() - r1->GetCenterY();

  float r1EdgeX = r1->maxX - r1->minX;
  float r2EdgeX = r2->maxX - r2->minX;

  float overlapX = r1EdgeX / 2 + r2EdgeX / 2 - FindAbs(dispX);

  if (overlapX >= 0) {
    float r1EdgeY = r1->maxY - r1->minY;
    float r2EdgeY = r2->maxY - r2->minY;

    float overlapY = r1EdgeY / 2 + r2EdgeY / 2 - FindAbs(dispY);

    if (overlapY >= 0) {
      if (overlapX < overlapY) {
        col->normalX = FindSign(dispX);
        col->normalY = 0;

        col->penetration = overlapX;

        return true;
      } else {
        col->normalX = 0;
        col->normalY = FindSign(dispY);

        col->penetration = overlapY;

        return true;
      }
    }
  }

  return false;
}

inline bool CheckCircleCircleCol(Collision *col) {
  Circle *c1 = static_cast<Circle *>(col->object1);
  Circle *c2 = static_cast<Circle *>(col->object2);

  float dispX = c2->posX - c1->posX;
  float dispY = c2->posY - c1->posY;

  float rSquared = FindSquare(c1->radius + c2->radius);
  float dispSquared = FindSquare(dispX) + FindSquare(dispY);

  if (dispSquared > rSquared) {
    return false;
  }

  if (dispSquared != 0) {
    float dispMagnitude = FindSqrt(dispSquared);

    col->normalX = dispX / dispMagnitude;
    col->normalY = dispY / dispMagnitude;

    col->penetration = c1->radius + c2->radius - dispMagnitude;
  } else {
    col->normalX = 1;
    col->normalY = 0;

    col->penetration = c1->radius + c2->radius;
  }

  return true;
}

inline bool CheckRectCircleCol(Collision *col) {
  Rect *r1 = static_cast<Rect *>(col->object1);
  Circle *c2 = static_cast<Circle *>(col->object2);

  float dispX = c2->posX - r1->GetCenterX();
  float dispY = c2->posY - r1->GetCenterY();

  float r1EdgeX = r1->maxX - r1->minX;
  float r1EdgeY = r1->maxY - r1->minY;

  float closestX = FindClamp(dispX, -1 * r1EdgeX / 2, r1EdgeX / 2);
  float closestY = FindClamp(dispY, -1 * r1EdgeY / 2, r1EdgeY / 2);

  if (dispX == closestX && dispY == closestY) {
    float distX = r1EdgeX / 2 - FindAbs(dispX);
    float distY = r1EdgeY / 2 - FindAbs(dispY);

    if (distX < distY) {
      col->normalX = FindSign(dispX);
      col->normalY = 0;
      col->penetration = c2->radius + distX;
    } else {
      col->normalX = 0;
      col->normalY = FindSign(dispY);
      col->penetration = c2->radius + distY;
    }

    return true;
  }

  float normalX = dispX - closestX;
  float normalY = dispY - closestY;

  float radius = c2->radius;
  float normalSquared = FindSquare(normalX) + FindSquare(normalY);

  if (normalSquared > FindSquare(radius)) {
    return false;
  }

  float normalMagnitude = FindSqrt(normalSquared);

  col->normalX = normalX / normalMagnitude;
  col->normalY = normalY / normalMagnitude;

  col->penetration = c2->radius - normalMagnitude;

  return true;
}

inline void ResolveCollision(Collision *col) {
  Object *object1 = col->object1;
  Object *object2 = col->object2;

  float tempInvMass1 = object1->invMass;
  float tempInvMass2 = object2->invMass;

  if (object1->invMass == 0 && object2->invMass == 0) { // two infinite mass objects collide
    if (object1->velocityX != 0 || object1->velocityY != 0) {
      tempInvMass1 = 1.0f;
    } else if (object2->velocityX != 0 || object2->velocityY != 0) {
      tempInvMass2 = 1.0f;
    } else {
      return;
    }
  }

  float relVelocityX = object2->velocityX - object1->velocityX;
  float relVelocityY = object2->velocityY - object1->velocityY;

  float normalX = col->normalX;
  float normalY = col->normalY;

  float normalVelocity = FindDot(relVelocityX, relVelocityY, normalX, normalY);

  if (normalVelocity > 0) {
    return;
  }

  float minRestitution = FindMin(object1->restitution, object2->restitution);

  float impulseMagnitude = (-1 * (1 + minRestitution) * normalVelocity) / (tempInvMass1 + tempInvMass2);

  object1->velocityX -= tempInvMass1 * impulseMagnitude * normalX;
  object1->velocityY -= tempInvMass1 * impulseMagnitude * normalY;

  object2->velocityX += tempInvMass2 * impulseMagnitude * normalX;
  object2->velocityY += tempInvMass2 * impulseMagnitude * normalY;
}

inline void CorrectPositions(Collision *col) {
  Object *object1 = col->object1;
  Object *object2 = col->object2;

  float tempInvMass1 = object1->invMass;
  float tempInvMass2 = object2->invMass;

  if (object1->invMass == 0 && object2->invMass == 0) { // two infinite mass objects collide
    if (object1->velocityX != 0 || object1->velocityY != 0) {
      tempInvMass1 = 1.0f;
    } else if (object2->velocityX != 0 || object2->velocityY != 0) {
      tempInvMass2 = 1.0f;
    } else {
      return;
    }
  }

  float correctionFactor = 0.2f;
  float correctionThreshold = 0.01f;

  float correctionMagnitude = correctionFactor * FindMax(col->penetration - correctionThreshold, 0.0f) / (tempInvMass1 + tempInvMass2);

  float correctionX = correctionMagnitude * col->normalX;
  float correctionY = correctionMagnitude * col->normalY;

  object1->Move(-1 * tempInvMass1 * correctionX, -1 * tempInvMass1 * correctionY);
  object2->Move(tempInvMass2 * correctionX, tempInvMass2 * correctionY);
}
