# main.py
import sys
import copy
import utils as csp_utils
import constraint as con


def assignment_consistent_with_constraints(new_assignment_variable, assignment, conditions):
    assigned_variables_with_condition = list(
        filter(lambda condition_variable: condition_variable in list(assignment.keys()),
               conditions[new_assignment_variable]))

    for assigned_variable in assigned_variables_with_condition:
        if assigned_variable != new_assignment_variable and not con.variable_condition_is_valid(
                assignment[new_assignment_variable], conditions[new_assignment_variable][assigned_variable],
                assignment[assigned_variable]):
            return False
    return True


def get_foward_check_domains(new_assignment_variable, assignment, domains, conditions):
    remaining_variables = con.get_remaining_variables(assignment, domains)
    for remaining_variable in remaining_variables:
        if remaining_variable in list(conditions[new_assignment_variable].keys()):
            domains[remaining_variable] = list(filter(
                lambda domain_value: con.variable_condition_is_valid(assignment[new_assignment_variable],
                                                                 conditions[new_assignment_variable][
                                                                     remaining_variable], domain_value),
                domains[remaining_variable]))


# main recursive backtracking function, can specify constraint enforcing procedure with cef function
def recursive_backtracking(assignment, domains, conditions, cef):
    # gets the next variable to be chosen
    next_chosen_variable = con.get_next_chosen_variable(assignment, domains, conditions)
    if next_chosen_variable == None:
        print(assignment)
        print("Solution Found!")
        return True

    # gets the next ordered values to be chosen
    ordered_next_chosen_values = con.get_ordered_next_chosen_values(next_chosen_variable, assignment, domains, conditions)
    if ordered_next_chosen_values == None:
        return False

    # loops through the value to be chosen in order
    for value in ordered_next_chosen_values:
        # do forward checking if specified
        if cef == "fc":
            new_domains = copy.deepcopy(domains)
            get_foward_check_domains(new_domains)
            for domain_values in new_domains.values():
                if len(domain_values) != 0:
                    if recursive_backtracking(assignment, new_domains, conditions, cef):
                        return True
                else:
                    print(assignment)
                    print("Failure!")

        # perform recursive backtracking
        if cef == "none":
            assignment[next_chosen_variable] = value
            if assignment_consistent_with_constraints(next_chosen_variable, assignment, conditions):
                if recursive_backtracking(assignment, domains, conditions, cef):
                    return True
            else:
                print(assignment)
                print("Failure!")

    assignment.pop(next_chosen_variable)
    return False


#########
# DRIVERS
#########
# get variable domains, conditions, and consistency enforcing procedure from command line arguments
(variable_domains, variable_conditions, consistency_enforcing_procedure) = csp_utils.get_domains_and_conditions(
    sys.argv)


# calls main backtracking funciton
recursive_backtracking({}, copy.deepcopy(variable_domains), variable_conditions, consistency_enforcing_procedure)
