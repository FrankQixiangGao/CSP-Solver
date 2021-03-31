def variable_condition_is_valid(val1,  operator, val2):
    if operator == "=":
        return val1 == val2

    if operator == "!":
        return val1 != val2

    if operator == ">":
        return val1 > val2

    if operator == "<":
        return val1 < val2


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
