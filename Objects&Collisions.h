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
    float invMass;

    float restitution;

    float velocityX;
    float velocityY;

    float gravity;

    const char* shape;

    float forceX;
    float forceY;

    Object(float mass_, float restitution_, float velocityX_, float velocityY_, float gravity_, const char* shape_) {
        mass = mass_;

        if (mass != 0) {
            invMass = 1 / mass;
        }
        else {
            invMass = 0; // infinite mass
        }

        restitution = restitution_;

        velocityX = velocityX_;
        velocityY = velocityY_;

        gravity = gravity_;

        shape = shape_; // rect or circle

        forceX = 0.0f;
        forceY = 0.0f;
    }

    virtual float GetCenterX() = 0;
    virtual float GetCenterY() = 0;

    virtual void Move(float dx, float dy) = 0;

    void Update(float dt, const char* method) {
        forceY += gravity;

        if (method == "euler") { // symplectic euler
            velocityX += invMass * forceX * dt;
            velocityY += invMass * forceY * dt;

            Move(velocityX * dt, velocityY * dt);
        }
        else if (method == "rk4") { // runge kutta 4
            float k1_vX = invMass * forceX; // initial derivatives
            float k1_vY = invMass * forceY;
            float k1_posX = velocityX;
            float k1_posY = velocityY;

            float halfDt = dt / 2.0f;

            float temp_vX = velocityX + k1_vX * halfDt; // temporary values at midpoint
            float temp_vY = velocityY + k1_vY * halfDt;
            float temp_posX = GetCenterX() + k1_posX * halfDt;
            float temp_posY = GetCenterY() + k1_posY * halfDt;

            float temp_forceX = forceX; // forces stay the same
            float temp_forceY = forceY;

            float k2_vX = invMass * temp_forceX; // derivatives at midpoint
            float k2_vY = invMass * temp_forceY;
            float k2_posX = temp_vX;
            float k2_posY = temp_vY;

            temp_vX = velocityX + k2_vX * halfDt;
            temp_vY = velocityY + k2_vY * halfDt;
            temp_posX = GetCenterX() + k2_posX * halfDt;
            temp_posY = GetCenterY() + k2_posY * halfDt;

            temp_forceX = forceX;
            temp_forceY = forceY;

            float k3_vX = invMass * temp_forceX; // refined derivatives at midpoint
            float k3_vY = invMass * temp_forceY;
            float k3_posX = temp_vX;
            float k3_posY = temp_vY;

            temp_vX = velocityX + k3_vX * dt;
            temp_vY = velocityY + k3_vY * dt;
            temp_posX = GetCenterX() + k3_posX * dt;
            temp_posY = GetCenterY() + k3_posY * dt;

            float k4_vX = invMass * forceX; // derivatives at endpoint
            float k4_vY = invMass * forceY;
            float k4_posX = temp_vX;
            float k4_posY = temp_vY;

            Move(dt / 6.0f * (k1_posX + 2 * k2_posX + 2 * k3_posX + k4_posX), dt / 6.0f * (k1_posY + 2 * k2_posY + 2 * k3_posY + k4_posY));

            velocityX += dt / 6.0f * (k1_vX + 2 * k2_vX + 2 * k3_vX + k4_vX);
            velocityY += dt / 6.0f * (k1_vY + 2 * k2_vY + 2 * k3_vY + k4_vY);
        }

        forceX = 0.0f;
        forceY = 0.0f;
    };
};

struct Rect : Object {
    float minX;
    float minY;

    float maxX;
    float maxY;

    Rect(float mass_, float restitution_, float velocityX_, float velocityY_, float gravity_,
        float minX_, float minY_, float maxX_, float maxY_) : Object(mass_, restitution_, velocityX_, velocityY_, gravity_, "rect") {
        minX = minX_;
        minY = minY_;

        maxX = maxX_;
        maxY = maxY_;
    }

    float GetCenterX() override {
        return (minX + maxX) / 2;
    }

    float GetCenterY() override {
        return (minY + maxY) / 2;
    }

    void Move(float dx, float dy) override {
        minX += dx;
        maxX += dx;

        minY += dy;
        maxY += dy;
    }
};

struct Circle : Object {
    float radius;

    float posX;
    float posY;

    Circle(float mass_, float restitution_, float velocityX_, float velocityY_, float gravity_,
        float radius_, float posX_, float posY_) : Object(mass_, restitution_, velocityX_, velocityY_, gravity_, "circle") {
        radius = radius_;

        posX = posX_;
        posY = posY_;
    }

    float GetCenterX() override {
        return posX;
    }

    float GetCenterY() override {
        return posY;
    }

    void Move(float dx, float dy) override {
        posX += dx;
        posY += dy;
    }
};

struct Collision {
    Object* o1;
    Object* o2;

    float normalX;
    float normalY;

    float penetration;

    Collision(Object* o1_, Object* o2_) {
        o1 = o1_;
        o2 = o2_;

        normalX = 0;
        normalY = 0;

        penetration = 0;
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

                col->penetration = overlapX;

                return true;
            }
            else {
                col->normalX = 0;
                col->normalY = FindSign(dispY);

                col->penetration = overlapY;

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
        float dispMagnitude = FindSqrt(dispSquared);

        col->normalX = dispX / dispMagnitude;
        col->normalY = dispY / dispMagnitude;

        col->penetration = c1->radius + c2->radius - dispMagnitude;
    }
    else {
        col->normalX = 1;
        col->normalY = 0;

        col->penetration = FindMin(c1->radius, c2->radius);
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
            float normalMagnitude = FindSqrt(normalSquared);

            col->normalX = -1 * normalX / normalMagnitude;
            col->normalY = -1 * normalY / normalMagnitude;

            col->penetration = c2->radius - normalMagnitude;
        }
        else {
            col->normalX = 1;
            col->normalY = 0;

            col->penetration = c2->radius;
        }
    }
    else {
        float normalMagnitude = FindSqrt(normalSquared);

        col->normalX = normalX / normalMagnitude;
        col->normalY = normalY / normalMagnitude;

        col->penetration = c2->radius - normalMagnitude;
    }

    return true;
}

void ResolveCollision(Collision* col) {
    Object* o1 = col->o1;
    Object* o2 = col->o2;

    float tempInvMass1 = o1->invMass;
    float tempInvMass2 = o2->invMass;

    if (o1->invMass == 0 && o2->invMass == 0) { // two infinite mass objects collide
        if (o1->velocityX != 0 || o1->velocityY != 0) {
            tempInvMass1 = 1.0f;
        }
        else if (o2->velocityX != 0 || o2->velocityY != 0) {
            tempInvMass2 = 1.0f;
        }
        else {
            return;
        }
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

    float impulseMagnitude = (-1 * (1 + minRestitution) * normalVelocity) / (tempInvMass1 + tempInvMass2);

    o1->velocityX -= tempInvMass1 * impulseMagnitude * normalX;
    o1->velocityY -= tempInvMass1 * impulseMagnitude * normalY;

    o2->velocityX += tempInvMass2 * impulseMagnitude * normalX;
    o2->velocityY += tempInvMass2 * impulseMagnitude * normalY;
}

void CorrectPositions(Collision* col) {
    Object* o1 = col->o1;
    Object* o2 = col->o2;

    float tempInvMass1 = o1->invMass;
    float tempInvMass2 = o2->invMass;

    if (o1->invMass == 0 && o2->invMass == 0) { // two infinite mass objects collide
        if (o1->velocityX != 0 || o1->velocityY != 0) {
            tempInvMass1 = 1.0f;
        }
        else if (o2->velocityX != 0 || o2->velocityY != 0) {
            tempInvMass2 = 1.0f;
        }
        else {
            return;
        }
    }

    float correctionFactor = 0.2f;
    float correctionThreshold = 0.01f;

    float correctionMagnitude = correctionFactor * FindMax(col->penetration - correctionThreshold, 0.0f) / (tempInvMass1 + tempInvMass2);

    float correctionX = correctionMagnitude * col->normalX;
    float correctionY = correctionMagnitude * col->normalY;

    o1->Move(-1 * tempInvMass1 * correctionX, -1 * tempInvMass1 * correctionY);
    o2->Move(tempInvMass2 * correctionX, tempInvMass2 * correctionY);
}