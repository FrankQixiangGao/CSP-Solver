//
// Created by Frank Gao on 3/31/21.
//

#ifndef CSP_SOLVER_CONSTRAINT_H
#define CSP_SOLVER_CONSTRAINT_H
#include "strings.h"

/**
 * Object representation of a constraint.
 */
class Constraint {


private:
    char var1; //First Variable of the constraint
    char var2; //Second Variable of the constraint
    char opera; //Equality operator of the Variables

public:
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
        switch (opera) {
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
    };


#endif //CSP_SOLVER_CONSTRAINT_H
