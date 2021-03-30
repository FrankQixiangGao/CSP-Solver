# main.py
import sys
import copy
import utils as csp_utils
import constraint as con

##################
# HELPER FUNCTIONS
##################

# function willl get the remaining variables not yet assigned
def get_remaining_variables(assignment, domains):
    return list(filter(lambda variable: variable not in list(assignment.keys()), list(domains.keys())))


# generic function to return either the min or max of the values in the list, returning None to indicate ties
def get_max_min_value_and_detect_ties(values, get_compare_key, maxSearch=True):
    target_value = values[0]
    target_value_key = get_compare_key(target_value)
    potential_target_value_key = 0
    tie_detected = False

    for value in values:
        potential_target_value_key = get_compare_key(value)

        if maxSearch:
            if potential_target_value_key > target_value_key:
                target_value = value
                target_value_key = potential_target_value_key
                tie_detected = False
        elif potential_target_value_key < target_value_key:
            target_value = value
            target_value_key = potential_target_value_key
            tie_detected = False

        if target_value != value and potential_target_value_key == target_value_key:
            tie_detected = True

    return target_value if not tie_detected else None


#####################################
# FUNCTIONS FOR GETTING NEXT VARIABLE
#####################################
def get_most_constrained_variable(domains, remaining_variables):
    return get_max_min_value_and_detect_ties(remaining_variables, lambda value: len(domains[value]), False)


def get_most_constraining_variable(conditions, remaining_variables):
    return get_max_min_value_and_detect_ties(remaining_variables, lambda value: len(list(
        filter(lambda condition_variable: condition_variable in remaining_variables, list(conditions[value].keys())))),
                                             True)


def get_first_in_alphabet_variable(domains, remaining_variables):
    sorted_remaining_variables = sorted(remaining_variables)
    if len(sorted_remaining_variables) > 0:
        return sorted_remaining_variables[0]
    else:
        return None


# function will return the next variable to be chosen for assignment
def get_next_chosen_variable(assignment, domains, conditions):
    remaining_variables = get_remaining_variables(assignment, domains)
    if len(remaining_variables) == 0:
        return None

    variable_to_choose = get_most_constrained_variable(domains, remaining_variables)

    if variable_to_choose == None:
        variable_to_choose = get_most_constraining_variable(conditions, remaining_variables)

    if variable_to_choose == None:
        variable_to_choose = get_first_in_alphabet_variable(domains, remaining_variables)

    return variable_to_choose


##################################
# FUNCTIONS FOR GETTING NEXT VALUE
##################################

def get_ordered_least_constraining_values(variable, assignment, domains, conditions):
    return None


def get_ordered_smallest_values(variable, domains):
    if len(domains[variable]) > 0:
        return sorted(domains[variable])
    else:
        return None


# function will return the next value to be chosen for the next assigned variable
def get_ordered_next_chosen_values(variable, assignment, domains, conditions):
    ordered_values_to_choose = get_ordered_least_constraining_values(variable, assignment, domains, conditions)

    if (ordered_values_to_choose == None):
        ordered_values_to_choose = get_ordered_smallest_values(variable, domains)

    return ordered_values_to_choose


#############################
# FUNCTIONS FOR BACK TRACKING
#############################




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
    remaining_variables = get_remaining_variables(assignment, domains)
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
    next_chosen_variable = get_next_chosen_variable(assignment, domains, conditions)
    if next_chosen_variable == None:
        print(assignment)
        print("Solution Found!")
        return True

    # gets the next ordered values to be chosen
    ordered_next_chosen_values = get_ordered_next_chosen_values(next_chosen_variable, assignment, domains, conditions)
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