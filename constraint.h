#ifndef AIHW2_CONSTRAINT_H
#define AIHW2_CONSTRAINT_H
#include <string>
using namespace std;
class constraint {
private:
    char var1;
    char var2;
    char operat;

public:
     //Constructor for making a default
    constraint() {
        var1 = 'A';
        var2 = 'A';
        operat = '=';
    }

    /**
     * Constructor for making a new {@code Constraint}.
     */
    constraint(string input) {
        var1 = input.charAt(0);
        var2 = input.charAt(4);
        operat = input.charAt(2);
    }

    /**
     * Checks to see if a constraint works on a given pair of values
     * @param val1 value of the first Variable
     * @param val2 value of the second Variable
     * @return true if the pair of values works under the constraint
     */
bool valid(int val1, int val2) {
        switch (operator) {
                case '<':
                return val1 < val2;
                case '>':
                return val1 > val2;
                case '!':
                return val1 != val2;
                case '=':
                return val1 == val2;
        }
            return false;
    }

string toString() {
        return string.format("%c %c %c", var1, operat, var2);
    }

};


#endif //AIHW2_CONSTRAINT_H
