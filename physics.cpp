#include <cmath>

float min(float a, float b) {
    return (a < b) ? a : b;
}

float max(float a, float b) {
    return (a > b) ? a : b;
}

float sign(float x) {
    return (x >= 0) ? 1.0f : -1.0f;
}

float square(float val) {
    return val * val;
}

float dot(float x1, float y1, float x2, float y2) {
    return x1 * x2 + y1 * y2;
}

float clamp(float val, float minVal, float maxVal) {
    return max(minVal, min(val, maxVal));
}

struct Object {
    float mass;
    float elasticity;

    float velocityX;
    float velocityY;

    float invMass;

    Object(float mass_, float elasticity_, float velocityX_, float velocityY_) {
        mass = mass_;
        elasticity = elasticity_;

        velocityX = velocityX_;
        velocityY = velocityY_;

        if (mass != 0) {
            invMass = 1 / mass;
        }
        else {
            invMass = 0;
        }
    }
};

struct Rect : Object {
    float minX;
    float minY;
    
    float maxX;
    float maxY;

    float centerX;
    float centerY;

    Rect(float mass_, float elasticity_, float velocityX_, float velocityY_, 
         float minX_, float minY_, float maxX_, float maxY_): Object(mass_, elasticity_, velocityX_, velocityY_) {
        minX = minX_;
        minY = minY_;

        maxX = maxX_;
        maxY = maxY_;

        centerX = (minX + maxX) / 2;
        centerY = (minY + maxY) / 2;
    }
};

struct Circle : Object {
    float radius;

    float posX;
    float posY;

    Circle(float mass_, float elasticity_, float velocityX_, float velocityY_, 
         float radius_, float posX_, float posY_) : Object(mass_, elasticity_, velocityX_, velocityY_) {
        radius = radius_;
        
        posX = posX_;
        posY = posY_;
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
    }
};

bool CheckRectRectCol(Collision* col) {
    Rect* r1 = static_cast<Rect*>(col->o1);
    Rect* r2 = static_cast<Rect*>(col->o2);

    float dispX = r2->centerX - r1->centerX;
    float dispY = r2->centerY - r1->centerY;

    float r1EdgeX = r1->maxX - r1->minX;
    float r2EdgeX = r2->maxX - r2->minX;

    float overlapX = r1EdgeX / 2 + r2EdgeX / 2 - abs(dispX);

    if (overlapX >= 0) {
        float r1EdgeY = r1->maxY - r1->minY;
        float r2EdgeY = r2->maxY - r2->minY;

        float overlapY = r1EdgeY / 2 + r2EdgeY / 2 - abs(dispY);

        if (overlapY >= 0) {
            if (overlapX < overlapY) {
                col->normalX = sign(dispX);
                col->normalY = 0;

                return true;
            }
            else {
                col->normalX = 0;
                col->normalY = sign(dispY);
                
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

    float rSquared = square(c1->radius + c2->radius);
    float dispSquared = square(dispX) + square(dispY);

    if (dispSquared > rSquared) {
        return false;
    }

    if (dispSquared != 0) {
        col->normalX = dispX / sqrt(dispSquared);
        col->normalY = dispY / sqrt(dispSquared);
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

    float dispX = c2->posX - r1->centerX;
    float dispY = c2->posY - r1->centerY;

    float r1EdgeX = r1->maxX - r1->minX;
    float r1EdgeY = r1->maxY - r1->minY;

    float closestX = clamp(dispX, -1 * r1EdgeX / 2, r1EdgeX / 2);
    float closestY = clamp(dispY, -1 * r1EdgeY / 2, r1EdgeY / 2);

    bool inside = false;

    if (dispX == closestX && dispY == closestY) {
        if (abs(dispX) > abs(dispY)) {
            closestX = sign(closestX) * r1EdgeX / 2;
        }
        else {
            closestY = sign(closestY) * r1EdgeY / 2;
        }

        inside = true;
    }

    float normalX = dispX - closestX;
    float normalY = dispY - closestY;

    float radius = c2->radius;
    float normalSquared = square(normalX) + square(normalY);

    if (normalSquared > square(radius) && !inside) {
        return false;
    }

    if (inside) {
        col->normalX = -1 * normalX;
        col->normalY = -1 * normalY;
    }
    else {
        col->normalX = normalX;
        col->normalY = normalY;
    }

    return true;
}

void ResolveCollision(Collision* col) {
    Object* o1 = col->o1;
    Object* o2 = col->o2;

    float relVelocityX = o2->velocityX - o1->velocityX;
    float relVelocityY = o2->velocityY - o1->velocityY;

    float normalX = col->normalX;
    float normalY = col->normalY;

    float normalVelocity = dot(relVelocityX, relVelocityY, normalX, normalY);

    if (normalVelocity > 0) {
        return;
    }

    float minElasticity = min(o1->elasticity, o2->elasticity);

    float impulse = (-1 * (1 + minElasticity) * normalVelocity) / (o1->invMass + o2->invMass);

    o1->velocityX -= o1->invMass * impulse * normalX;
    o1->velocityY -= o1->invMass * impulse * normalY;

    o2->velocityX += o2->invMass * impulse * normalX;
    o2->velocityY += o2->invMass * impulse * normalY;
}

#include <iostream>

int main() {
    Object* o1 = new Circle(1.0f, 1.0f, 2.0f, 0.0f, 5.0f, 0.0f, 0.0f);
    Object* o2 = new Circle(1.0f, 1.0f, -1.0f, 0.0f, 5.0f, 10.0f, 0.0f);
    Collision* col = new Collision(o1, o2);

    if (CheckCircleCircleCol(col)) {
        std::cout << "Rect-Rect Col Detected" << "\n";
        ResolveCollision(col);
    }

    std::cout << "Circle 1 Velocity: (" << o1->velocityX << ", " << o1->velocityY << ")\n";
    std::cout << "Circle 2 Velocity: (" << o2->velocityX << ", " << o2->velocityY << ")\n";

    delete o1;
    delete o2;
    delete col;

    o1 = new Rect(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 2.0f);
    o2 = new Rect(1.0f, 1.0f, -1.0f, 0.0f, 2.0f, 0.0f, 4.0f, 2.0f);
    col = new Collision(o1, o2);

    if (CheckRectRectCol(col)) {
        std::cout << "Rect-Rect Col Detected" << "\n";
        ResolveCollision(col);
    }

    std::cout << "Rectangle 1 Velocity: (" << o1->velocityX << ", " << o1->velocityY << ")\n";
    std::cout << "Rectangle 2 Velocity: (" << o2->velocityX << ", " << o2->velocityY << ")\n";

    delete o1;
    delete o2;
    delete col;

    o1 = new Rect(1.0f, 1.0f, -1.0f, 0.0f, 6.0f, 4.0f, 8.0f, 6.0f);
    o2 = new Circle(1.0f, 1.0f, 2.0f, 0.0f, 1.0f, 5.0f, 5.0f);
    col = new Collision(o1, o2);

    if (CheckRectCircleCol(col)) {
        std::cout << "Rect-Circle Col Detected" << "\n";
        ResolveCollision(col);
    }
    std::cout << "Object 1 Velocity: (" << o1->velocityX << ", " << o1->velocityY << ")\n";
    std::cout << "Object 2 Velocity: (" << o2->velocityX << ", " << o2->velocityY << ")\n";

    delete o1;
    delete o2;
    delete col;
}