#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

class physicalObject {
public:
    physicalObject(float x, float y, float z, float mass, float bounce = 0.5f, float friction = 0.5f)
        : x(x), y(y), z(z), vx(0), vy(0), vz(0), ax(0), ay(0), az(0), mass(mass), bounce(bounce), friction(friction) {}
    
    //Get för alla variabler
    std::tuple<float,float,float> getPosition() const { return std::make_tuple(x, y, z); }
    std::tuple<float,float,float> getVelocity() const { return std::make_tuple(vx, vy, vz); }
    std::tuple<float,float,float> getAcceleration() const { return std::make_tuple(ax, ay, az); }
    float getMass() const { return mass; }
    float getBounce() const { return bounce; }
    float getFriction() const { return friction; }

    void airResistance(const float dragCoefficient, const float airDensity) {
        //Måste prata med Mikael om hur luft motstånd fungerar och ska räknas ut

        //Drag coefficient = 0.45
    };

    void applyGravity(){
        ay -= 9.81f; // Gravity acceleration in m/s^2
    };

    void disableGravity(){
        ay += 9.81f; // lägger till 9.81 för att neutralisera gravitationen (sätter den inte till 0 direkt för att undvika problem med andra krafter)
    };

    void applyForce(float fx, float fy, float fz) {
        ax += fx / mass; // F = ma vilket betyder att a = F/m
        ay += fy / mass;
        az += fz / mass;
    };

    void update(float deltaTime) {
        // updatera hastigheten baserat på acceleration
        vx += ax * deltaTime;
        vy += ay * deltaTime;
        vz += az * deltaTime;

        // updatera positionen baserat på hastighet
        x += vx * deltaTime;
        y += vy * deltaTime;
        z += vz * deltaTime;
    };

    float getTotalEnergy() const {
        float kineticEnergy = 0.5f * mass * (vx*vx + vy*vy + vz*vz); // 0.5 * m * v^2
        float potentialEnergy = mass * 9.81f * y; // om y är höjden från marken annars får en variabel eller konstant läggas till för marknivån 
        return kineticEnergy + potentialEnergy;
    };

    void applyWind(float windSpeedX, float windSpeedY, float windSpeedZ) {
        // Enkel vindpåverkan som lägger till vindhastigheten till objektets hastighet
        // Viden ska vara mellan 100 och -100 i varje riktning
        vx += windSpeedX / 100; // Delar med 100 för att minska effekten av vinden (kan ändras efter behov)
        vy += windSpeedY / 100;
        vz += windSpeedZ / 100;
    };

    int getDamage() const {
        // Enkel skademodell baserad på hastighet och massa
        int reductionConstant = 10; // Konstant för att justera skalan på skadan
        damage = 0;
        float dontDestroyTheWorldVariableForDamageCalculationIfSomethingSomehowGainsTooMuchEnergyAndWeDontKnowHowItGotThatMuchEnergyBecauseThatIsEasierThanFindingTheProblem = 500.0f; // Variabel för att undvika extrema skador

        Energy = getTotalEnergy(); // Använder total energi som en proxy för skada
        if (Energy > 1000) {
            Energy = dontDestroyTheWorldVariableForDamageCalculationIfSomethingSomehowGainsTooMuchEnergyAndWeDontKnowHowItGotThatMuchEnergyBecauseThatIsEasierThanFindingTheProblem; // Begränsar skadan för att undvika extrema värden
        };

        damage = static_cast<int>(Energy / reductionConstant); // Enkel formel för skada

        return damage;
    };

    void destroySelf() {
        // Placeholder för att markera objektet som förstört
        // I en riktig implementation skulle detta hantera borttagning från scenen eller liknande
        return;
    };

    void hitObject(int objectHardness) { // sakta ner ner objektet baserat på dess hårdhet
        int damage = getDamage();
        if (damage > objectHardness) {
            vx -= (damage - objectHardness)/3.0f;
            vy -= (damage - objectHardness)/3.0f;
            vz -= (damage - objectHardness)/3.0f;
            // Ska också förstöra objektet om skadan överstiger dess hårdhet men det har mer med implemnetation att göra
        } else {
            objectHardness -= damage; // Minska objektets hårdhet
            destroySelf(); // Förstör objektet om skadan överstiger dess hårdhet
    }

private:
    float x, y, z; // Position
    float vx, vy, vz; // Velocity
    float ax, ay, az; // Acceleration
    float mass; // Mass
    float bounce; // Coefficient of restitution (bounciness)
    float friction; // Coefficient of friction

}; // Forward declaration

class physicalSphericalObject : public physicalObject {

public:
    

    physicalObject(float radius, float x, float y, float z, float mass, float bounce = 0.5f, float friction = 0.5f)
        : radius(radius), x(x), y(y), z(z), vx(0), vy(0), vz(0), ax(0), ay(0), az(0), mass(mass), bounce(bounce), friction(friction) {}

    //Get för alla variabler
    float getRadius() const { return radius; }

private:
    float radius; // Radius of the sphere
    
};