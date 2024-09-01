static float FindSquare(float x) {
    return x * x;
}

static float FindDot(float x1, float y1, float x2, float y2) {
    return x1 * x2 + y1 * y2;
}

static float FindMin(float x, float y) {
    return (x < y) ? x : y;
}

static float FindMax(float x, float y) {
    return (x > y) ? x : y;
}

static float FindClamp(float x, float minX, float maxX) {
    return FindMax(minX, FindMin(x, maxX));
}

static float FindSign(float x) {
    return (x >= 0) ? 1.0f : -1.0f;
}

static float FindAbs(float x) {
    return (x >= 0) ? x : -x;
}

static float FindSqrt(float x)
{
    if (x == 0 || x == 1) {
        return x;
    }

    float y = x;
    float z = (y + (x / y)) / 2;

    while (FindAbs(y - z) >= 0.0000001) { // 7 digits
        y = z;
        z = (y + (x / y)) / 2;
    }

    return z;
}

struct Object {
    float mass;
    float restitution;

    float velocityX;
    float velocityY;

    int shape;
    float invMass;

    float forceX;
    float forceY;

    Object(float mass_, float restitution_, float velocityX_, float velocityY_, int shape_) {
        mass = mass_;
        restitution = restitution_;

        velocityX = velocityX_;
        velocityY = velocityY_;

        if (mass != 0) {
            invMass = 1 / mass;
        }
        else {
            invMass = 0; // infinite mass
        }

        shape = shape_; // 1 for rect, 2 for circle

        forceX = 0.0f;
        forceY = 0.0f;
    }

    virtual void Update(float dt) = 0;
};

struct Rect : Object {
    float minX;
    float minY;

    float maxX;
    float maxY;

    Rect(float mass_, float restitution_, float velocityX_, float velocityY_,
        float minX_, float minY_, float maxX_, float maxY_) : Object(mass_, restitution_, velocityX_, velocityY_, 1) {
        minX = minX_;
        minY = minY_;

        maxX = maxX_;
        maxY = maxY_;
    }

    float GetCenterX() {
        return (minX + maxX) / 2;
    }

    float GetCenterY() {
        return (minY + maxY) / 2;
    }

    void Update(float dt) override {
        velocityX += invMass * forceX * dt;
        velocityY += invMass * forceY * dt;

        minX += velocityX * dt;
        maxX += velocityX * dt;
        
        minY += velocityY * dt;
        maxY += velocityY * dt;

        forceX = 0.0f;
        forceY = 0.0f;
    }
};

struct Circle : Object {
    float radius;

    float posX;
    float posY;

    Circle(float mass_, float restitution_, float velocityX_, float velocityY_,
        float radius_, float posX_, float posY_) : Object(mass_, restitution_, velocityX_, velocityY_, 2) {
        radius = radius_;

        posX = posX_;
        posY = posY_;
    }

    void Update(float dt) override {
        velocityX += invMass * forceX * dt;
        velocityY += invMass * forceY * dt;

        posX += velocityX * dt;
        posY += velocityY * dt;

        forceX = 0.0f;
        forceY = 0.0f;
    }
};

struct Collision {
    Object* o1;
    Object* o2;

    float normalX;
    float normalY;

    Collision(Object* o1_, Object* o2_) {
        o1 = o1_;
        o2 = o2_;
        
        normalX = 0;
        normalY = 0;
    }
};

bool CheckRectRectCol(Collision* col) {
    Rect* r1 = static_cast<Rect*>(col->o1);
    Rect* r2 = static_cast<Rect*>(col->o2);

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

                return true;
            }
            else {
                col->normalX = 0;
                col->normalY = FindSign(dispY);

                return true;
            }
        }
    }

    return false;
}

bool CheckCircleCircleCol(Collision* col) {
    Circle* c1 = static_cast<Circle*>(col->o1);
    Circle* c2 = static_cast<Circle*>(col->o2);

    float dispX = c2->posX - c1->posX;
    float dispY = c2->posY - c1->posY;

    float rSquared = FindSquare(c1->radius + c2->radius);
    float dispSquared = FindSquare(dispX) + FindSquare(dispY);

    if (dispSquared > rSquared) {
        return false;
    }

    if (dispSquared != 0) {
        col->normalX = dispX / FindSqrt(dispSquared);
        col->normalY = dispY / FindSqrt(dispSquared);
    }
    else {
        col->normalX = 1;
        col->normalY = 0;
    }

    return true;
}

bool CheckRectCircleCol(Collision* col) {
    Rect* r1 = static_cast<Rect*>(col->o1);
    Circle* c2 = static_cast<Circle*>(col->o2);

    float dispX = c2->posX - r1->GetCenterX();
    float dispY = c2->posY - r1->GetCenterY();

    float r1EdgeX = r1->maxX - r1->minX;
    float r1EdgeY = r1->maxY - r1->minY;

    float closestX = FindClamp(dispX, -1 * r1EdgeX / 2, r1EdgeX / 2);
    float closestY = FindClamp(dispY, -1 * r1EdgeY / 2, r1EdgeY / 2);

    bool inside = false;

    if (dispX == closestX && dispY == closestY) {
        if (FindAbs(dispX) > FindAbs(dispY)) {
            closestX = FindSign(closestX) * r1EdgeX / 2;
        }
        else {
            closestY = FindSign(closestY) * r1EdgeY / 2;
        }

        inside = true;
    }

    float normalX = dispX - closestX;
    float normalY = dispY - closestY;

    float radius = c2->radius;
    float normalSquared = FindSquare(normalX) + FindSquare(normalY);

    if (normalSquared > FindSquare(radius) && !inside) {
        return false;
    }

    if (inside) {
        if (normalSquared != 0) {
            col->normalX = -1 * normalX / FindSqrt(normalSquared);
            col->normalY = -1 * normalY / FindSqrt(normalSquared);
        }
        else {
            col->normalX = 1;
            col->normalY = 0;
        }
    }
    else {
        col->normalX = normalX / FindSqrt(normalSquared);
        col->normalY = normalY / FindSqrt(normalSquared);
    }

    return true;
}

void ResolveCollision(Collision* col) {
    Object* o1 = col->o1;
    Object* o2 = col->o2;

    if (o1->invMass == 0 && o2->invMass == 0) {
        return;
    }

    float relVelocityX = o2->velocityX - o1->velocityX;
    float relVelocityY = o2->velocityY - o1->velocityY;

    float normalX = col->normalX;
    float normalY = col->normalY;

    float normalVelocity = FindDot(relVelocityX, relVelocityY, normalX, normalY);

    if (normalVelocity > 0) {
        return;
    }

    float minRestitution = FindMin(o1->restitution, o2->restitution);

    float impulse = (-1 * (1 + minRestitution) * normalVelocity) / (o1->invMass + o2->invMass);

    o1->velocityX -= o1->invMass * impulse * normalX;
    o1->velocityY -= o1->invMass * impulse * normalY;

    o2->velocityX += o2->invMass * impulse * normalX;
    o2->velocityY += o2->invMass * impulse * normalY;
}