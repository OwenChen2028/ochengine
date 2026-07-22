#pragma once

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

    virtual ~Object() = default;

    virtual float GetCenterX() = 0;
    virtual float GetCenterY() = 0;

    virtual void Move(float dx, float dy) = 0;

    void PhysicsUpdate(float dt, const char* method) {
        forceY += mass * gravity;

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

