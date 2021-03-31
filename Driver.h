//
// Created by Frank Gao on 3/31/21.
//

#ifndef CSP_SOLVER_DRIVER_H
#define CSP_SOLVER_DRIVER_H

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

/**
 * Main driver to test the classes
 */
public class Driver {

    int iteration; //Keeps track of which iteration the program is on

    /**
     * Perform pre-search setup.
     * @param vars variables from .var file
     * @param cons constraints from .con file
     * @param useCEP flag for using forward checking
     */
public void go(ArrayList<Variable> vars, ArrayList<Constraint> cons, boolean useCEP) {
        // Have variables keep track of how many constraints they belong to
        // for least constraining variable
        for (Constraint c : cons) {
            for (Variable v : vars) {
                if (c.var1 == v.var || c.var2 == v.var) {
                    v.numConstraints++;
                }
            }
        }
        iteration = 1;
        solve(new State(vars, cons, useCEP));
    }

    /**
     * Follows the Backtracking Search Algorithm to solve input.
     * @param currState the current variable state
     * @return true if the state is valid
     */
public boolean solve(State currState) {
        // Check if current assignments satisfy all constraints
        if (currState.isSolved()) {
            // Limit to 30 iterations
            if (iteration <= 30) {
                // Print solution
                System.out.printf("%d. %s%n", iteration, currState);
                return true;
            } else {
                return false;
            }
        }

        // Pick most constrained, most constraining, and earliest alphabetical variable
        currState.selectNextVar();

        // Check if forward checking failed
        if (currState.failedFC()) {
            // Limit to 30 iterations
            if (iteration <= 30) {
                // Print failure
                System.out.printf("%d. %s%n", iteration, currState);
                iteration++;
            }
            return false;
        }

        // Try to set all values in order from least constraining to most constraining
        for (int val : currState.getOrderedVals()) {
            // Copy current state to make changes
            State nextState = currState.copyOf();

            // Set the chosen variable to val
            nextState.setVar(val);

            // Check consistency with current constraints
            if (nextState.consistent()) {
                if (solve(nextState)) {
                    return true;
                }
            } else if (iteration <= 30) { // Limit to 30 iterations
                // Print failure
                System.out.printf("%d. %s%n", iteration, nextState);
                iteration++;
            } else {
                return false;
            }
        }

        // None of the values worked, so current assignments are wrong
        return false;
    }

    /**
     * Parses the arguments from the command line.
     * @param args the commandline arguments
     */
public static void main(String[] args) {
        //Check if the number of arguments is valid
        if (args.length != 3) {
            System.err.println("Incorrect number of arguments.");
            System.exit(1);
        }

        String varFile = args[0]; //Retrieves the filename of the .var file
        String conFile = args[1]; //Retrieves the filename of the .con file

        ArrayList<Variable> vars = new ArrayList<>(); //Stores all of the information in the .var file
        ArrayList<Constraint> cons = new ArrayList<>(); //Stores all of the information in the .con file
        boolean useCEP; //True if a Consistence-Enforcing Procedure is going to be used


        try {
            //Get the information from the .var file
            Scanner varin = new Scanner(new File(varFile));
            while(varin.hasNextLine()) {
                vars.add(new Variable(varin.nextLine().trim()));
            }
        }
        catch (FileNotFoundException e) {
            System.err.printf("Could not find file: %s%n", varFile);
            System.exit(1);
        }
        try {
            //Get the information from the .con file
            Scanner conin = new Scanner(new File(conFile));
            while(conin.hasNextLine()) {
                cons.add(new Constraint(conin.nextLine().trim()));
            }
        }
        catch (FileNotFoundException e) {
            System.err.printf("Could not find file: %s%n", conFile);
            System.exit(1);
        }

        //Check if the third argument is valid
        if (!args[2].equals("none") && !args[2].equals("fc")) {
            System.err.printf("Invalid flag: %s%n", args[2]);
            System.exit(1);
        }
        useCEP = args[2].equals("fc");

        //Run the main program
        new Driver().go(vars, cons, useCEP);
    }
}



#endif //CSP_SOLVER_DRIVER_H
