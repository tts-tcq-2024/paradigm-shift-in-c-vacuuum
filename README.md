# paradigm-shift-c

# Programming Paradigms

Electric Vehicles have BMS - Battery Management Systems

[Here is an article that helps to understand the need for BMS](https://circuitdigest.com/article/battery-management-system-bms-for-electric-vehicles)

[Wikipedia gives an idea of the types and topologies](https://en.wikipedia.org/wiki/Battery_management_system)

[This site gives the optimum Charging-temperature limits](https://batteryuniversity.com/learn/article/charging_at_high_and_low_temperatures)

[This abstract suggests a range for the optimum State of Charge](https://www.sciencedirect.com/science/article/pii/S2352484719310911)

[Here is a reference for the maximum charge rate](https://www.electronics-notes.com/articles/electronic_components/battery-technology/li-ion-lithium-ion-charging.php#:~:text=Constant%20current%20charge:%20In%20the%20first%20stage%20of,rate%20of%20a%20maximum%20of%200.8C%20is%20recommended.)

## Possible purpose

- Protect batteries while charging:
at home, in public place, within vehicle / regenerative braking
- Estimate life, inventory and supply chains

## The Starting Point

We will explore the charging phase of Li-ion batteries to start with.

## Issues

- The code here has high complexity in a single function.
- The tests are not complete - they do not cover all the needs of a consumer

## Tasks

1. Reduce the cyclomatic complexity.
1. Separate pure functions from I/O
1. Avoid duplication - functions that do nearly the same thing
1. Complete the tests - cover all conditions.
1. To take effective action, we need to know
the abnormal measure and the breach -
whether high or low. Add this capability.

## The Exploration

How well does our code hold-out in the rapidly evolving EV space?
Can we add new functionality without disturbing the old?

## The Landscape

- Limits may change based on new research
- Technology changes due to obsolescence
- Sensors may be from different vendors with different accuracy
- Predicting the future requires Astrology!

## Keep it Simple

Shorten the Semantic distance

- Procedural to express sequence
- Functional to express relation between input and output
- Object oriented to encapsulate state with actions
- Apect oriented to capture repeating aspects

## Refactoring Approach
  
To implement the warning level feature and prepare for potential future customization (where warnings may be enabled or disabled for individual parameters), the Strategy Pattern is a suitable design choice. The Strategy Pattern allows you to encapsulate different algorithms (checking and warning behavior in this case) and makes it easier to modify or extend the functionality without changing the code's structure. Check- https://refactoring.guru/design-patterns/strategy

### Why the Strategy Pattern?

- Encapsulation of Behavior: The checks for temperature, SoC, and charge rate each have distinct logic for detecting breaches. Encapsulating these checks into separate strategies (classes) makes the code more modular and easier to modify.
- Open for Extension, Closed for Modification: The Strategy Pattern allows us to extend the warning logic without modifying the existing code (just by adding new strategies). This minimizes the impact of future changes, such as adding new parameters or enabling/disabling warnings selectively.
- Reusability: The pattern supports reusability as we can easily create new strategies if new parameters or new types of warnings are needed.
- Tuning Flexibility: Customers might want warnings for specific parameters but not others. The Strategy Pattern can enable or disable warnings for specific strategies without affecting the others.

### Steps undertaken in Refactoring

- Encapsulate Check and Warning Logic: Define separate strategies for each battery parameter (Temperature, SoC, and Charge Rate) that will handle both breach and warning levels.
- Warning Logic: Add a tolerance-based warning calculation for each parameter. The warning logic will be separate from the breach logic but will be encapsulated in the same strategy.
- Extendability for Warnings: Implement a way to easily enable or disable warnings for each strategy in the future.
- Refactor the batteryIsOk Function: Modify the logic to use the strategy objects instead of function pointers.
