//
// Created by Frank Gao on 3/31/21.
//


#include <cstring>
#include <cstdlib>
#include <iostream>

/**
 * Object representation of a constraint.
 */
class Constraint {

public:
    std::string var1; //First Variable of the constraint
    std::string var2; //Second Variable of the constraint
    char opera; //Equality operator of the Variables


    /**
     * Constructor for making a default {@code Constraint}.
     */
    Constraint() {
        var1 = 'A';
        var2 = 'A';
        opera = '=';
    }

    /**
     * Constructor for making a new {@code Constraint}.
     */
    Constraint(const char* In) {
        var1 = In[0];
        opera = In[2];
        var2 = In[4];
    }

    /**
     * Checks to see if a constraint works on a given pair of values
     * @param Value_One value of the first Variable
     * @param Value_Two value of the second Variable
     * @return true if the pair of values works under the constraint
     */
    bool valid(int Value_One, int Value_Two) {
        switch(opera) {
            case '<':
                return Value_One < Value_Two;
            case '>':
                return Value_One > Value_Two;
            case '!':
                return Value_One != Value_Two;
            case '=':
                return Value_One == Value_Two;
        }
        return false;
    }

    std::string toString() {
        std::cout << var1 << opera << var2;
    }