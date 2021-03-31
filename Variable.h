//
// Created by Frank Gao on 3/31/21.
//
#ifndef AIHW2_VARIABLE_H
#define AIHW2_VARIABLE_H

#include <array>


/**
 * Object representation of a variable.
 */
class Variable implements Comparable<Variable> {

    char var; //Letter which represents the Variable
    ArrayList<Integer> values; //Valid integer values that the Variable can be
    int numConstraints; //Total constraints that the Variable is in

/**
 * Constructor for making a default {@code Variable}.
 */
public Variable() {
    var = 'A';
    values = new ArrayList<>();
    numConstraints = 0;
}

/**
 * Constructor for making a new {@code Variable}.
 */
public Variable(String input) {
    var = input.charAt(0);
    values = new ArrayList<>();
    String[] temp = input.substring(3).split(" ");
    for(String s:temp) {
        values.add(Integer.parseInt(s));
    }
    }

/**
 * Determines which Variable is the most constrained, most constraining, and first alphabetically
 * @param other Variable to compare against
 * @return positive if current Variable is better than other
 */
public int compareTo(Variable other) {
    if (values.size() == other.values.size()) {
        if (numConstraints == other.numConstraints) {
            return var - other.var;
        }
        return other.numConstraints - numConstraints;
    }
    return values.size() - other.values.size();
}

/**
 * Creates a copy of the given Variable so values don't get lost.
 * @return copy of a given Variable
 */
public Variable copyOf() {
    Variable copy = new Variable();
    copy.var = var;
    copy.values.addAll(values);
    copy.numConstraints = numConstraints;
    return copy;
}

@Override
public String toString() {
    return String.format("%c: %s", var, values.toString());
}
}\
