#include <cmath>

struct Object {
    float mass;
    float elasticity;

    float velocityX;
    float velocityY;

    float inverseMass;

    Object(float mass_, float elasticity_, float velocityX_, float velocityY_) {
        mass = mass_;
        elasticity = elasticity_;

        velocityX = velocityX_;
        velocityY = velocityY_;

        if (mass != 0) {
            inverseMass = 1 / mass;
        }
        else {
            inverseMass = 0;
        }
    }

    virtual int CalcCenterX() = 0;

    virtual int CalcCenterY() = 0;
};

struct Rect : Object {
    float minX;
    float minY;
    float maxX;
    float maxY;

    Rect(float mass_, float elasticity_, float velocityX_, float velocityY_, 
         float minX_, float minY_, float maxX_, float maxY_): Object(mass_, elasticity_, velocityX_, velocityY_) {
        minX = minX_;
        minY = minY_;

        maxX = maxX_;
        maxY = maxY_;
    }

    int CalcCenterX() {
        return (minX + maxX) / 2;
    }

    int CalcCenterY() {
        return (minY + maxY) / 2;
    }
};

bool CheckRectRectCol(Rect* r1, Rect* r2) {
    if ((r1->maxX < r2->minX || r1->minX > r2->maxX) || (r1->maxY < r2->minY || r1->minY > r2->maxY)) {
        return false;
    }
    else {
        return true;
    }
}

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

    int CalcCenterX() {
        return posX;
    }

    int CalcCenterY() {
        return posY;
    }
};

bool CheckCircleCircleCol(Circle* c1, Circle* c2) {
    float rSum = c1->radius + c2->radius;

    float xSum = c1->posX + c2->posX;
    float ySum = c1->posY + c2->posY;
    
    return (rSum * rSum) < (xSum * xSum + ySum * ySum);
}

void ResolveCollision(Object* o1, Object* o2) {
    float relativeVelocityX = o2->velocityX - o1->velocityX;
    float relativeVelocityY = o2->velocityY - o1->velocityY;

    float normalDirectionX = o2->CalcCenterX() - o1->CalcCenterX();
    float normalDirectionY = o2->CalcCenterY() - o1->CalcCenterY();

    float normalDirectionMagnitude = 1 / sqrt(normalDirectionX * normalDirectionX + normalDirectionY * normalDirectionY);

    if (normalDirectionMagnitude != 0) {
        normalDirectionX *= normalDirectionMagnitude;
        normalDirectionY *= normalDirectionMagnitude;
    }

    float normalVelocityMagnitude = relativeVelocityX * normalDirectionX + relativeVelocityY * normalDirectionY;

    if (normalVelocityMagnitude > 0) {
        return;
    }

    float minElasticity = o1->elasticity;
    if (minElasticity > o2->elasticity) {
        minElasticity = o2->elasticity;
    }

    float impulseMagnitude = (-1 * (1 + minElasticity) * normalVelocityMagnitude) / (o1->inverseMass + o2->inverseMass);

    o1->velocityX -= o1->inverseMass * impulseMagnitude * normalDirectionX;
    o1->velocityY -= o1->inverseMass * impulseMagnitude * normalDirectionY;

    o2->velocityX += o2->inverseMass * impulseMagnitude * normalDirectionX;
    o2->velocityY += o2->inverseMass * impulseMagnitude * normalDirectionY;
}

#include <iostream>

int main() {
    Object* o1 = new Circle(1.0f, 1.0f, 2.0f, 0.0f, 5.0f, 0.0f, 0.0f);
    Object* o2 = new Circle(1.0f, 1.0f, -1.0f, 0.0f, 5.0f, 10.0f, 0.0f);

    ResolveCollision(o1, o2);

    std::cout << "Circle 1 Velocity: (" << o1->velocityX << ", " << o1->velocityY << ")\n";
    std::cout << "Circle 2 Velocity: (" << o2->velocityX << ", " << o2->velocityY << ")\n";


    o1 = new Rect(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 2.0f);
    o2 = new Rect(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 3.0f, 2.0f);

    ResolveCollision(o1, o2);

    std::cout << "Rectangle 1 Velocity: (" << o1->velocityX << ", " << o1->velocityY << ")\n";
    std::cout << "Rectangle 2 Velocity: (" << o2->velocityX << ", " << o2->velocityY << ")\n";

    o1 = new Circle(1.0f, 1.0f, 2.0f, 0.0f, 1.0f, 5.0f, 5.0f);
    o2 = new Rect(1.0f, 1.0f, -1.0f, 0.0f, 6.0f, 4.0f, 8.0f, 6.0f);

    ResolveCollision(o1, o2);

    std::cout << "Object 1 Velocity: (" << o1->velocityX << ", " << o1->velocityY << ")\n";
    std::cout << "Object 2 Velocity: (" << o2->velocityX << ", " << o2->velocityY << ")\n";
}