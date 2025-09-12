#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

class physicalSphericalObject {

public:
    

    physicalObject(float radius, float x, float y, float z, float mass, float bounce = 0.5f, float friction = 0.5f)
        : radius(radius), x(x), y(y), z(z), vx(0), vy(0), vz(vz), ax(0), ay(0), az(az), mass(mass), restitution(restitution), friction(friction) {}

    //Get för alla variabler
    float getRadius() const { return radius; }
    float getPosition() const { return std::make_tuple(x, y, z); }
    float getVelocity() const { return std::make_tuple(vx, vy, vz); }
    float getAcceleration() const { return std::make_tuple(ax, ay, az); }
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

        // reseta acceleration efter ny tick
        ax = 0;
        ay = 0;
        az = 0;
    };

    float getTotalEnergy() const {
        float kineticEnergy = 0.5f * mass * (vx * vx + vy * vy + vz * vz);
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
        float dontDestroyTheWorldVariableForDamageCalculationIfSomethingSomehowGainsTooMuchEnergyAndWeDontKnowHowBecauseThatIsEasierThanFindingTheProblem = 500.0f; // Variabel för att undvika extrema skador

        Energy = self.getTotalEnergy(); // Använder total energi som en proxy för skada
        if (Energy > 1000) {
            Energy = dontDestroyTheWorldVariableForDamageCalculationIfSomethingSomehowGainsTooMuchEnergyAndWeDontKnowHowBecauseThatIsEasierThanFindingTheProblem; // Begränsar skadan för att undvika extrema värden
        };

        damage = static_cast<int>(Energy / reductionConstant); // Enkel formel för skada

        return damage;
    };

    void hitObject(int objectHardness) { // sakta ner ner objektet baserat på dess hårdhet
        int damage = getDamage();
        if (damage > objectHardness) {
            
        } else {
            

    }

private:
    float radius; // Radius of the spherical object
    float x, y, z; // Position
    float vx, vy, vz; // Velocity
    float ax, ay, vz; // Acceleration
    float mass; // Mass
    float bounce; // Coefficient of restitution (bounciness)
    float friction; // Coefficient of friction
    
};